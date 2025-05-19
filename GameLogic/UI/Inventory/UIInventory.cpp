#include "UIInventory.h"

#include "raymath.h"
#include "../../../CoreGameLogic/GameManager.h"
#include "../../../CoreGameLogic/InputManager.h"
#include "../../Player/Inventory/InventoryManager.h"

UIInventory::UIInventory(float slotSize, float spacing)
    : slotShader(AssetManager::LoadShader("Shaders/inventorySlot.frag")), slots(1), slotSize(slotSize), spacing(spacing) {

    indexLoc = GetShaderLocation(slotShader, "index");
    timeLoc = GetShaderLocation(slotShader, "time");
    iconLoc = GetShaderLocation(slotShader, "icon");
    highlightLoc = GetShaderLocation(slotShader, "highlight");
    roundnessLoc = GetShaderLocation(slotShader, "roundness");

    whiteTex = AssetManager::LoadTexture("Textures/White.png");
    transparentTex = AssetManager::LoadTexture("Textures/Transparent.png");
    InventoryManager::onItemAdded += [this](IItem* item, int index) {
        HandleOnItemAdded(item, index);
    };
    InventoryManager::onNrOfLockedSlotsChanged += [this](int nrOfLockedSlots) {
        HandleNrOfLockedSlotsChanged(nrOfLockedSlots);
    };
    InventoryManager::onActiveSlotChanged += [this](int index) {
        HandleActiveChanged(index);
    };
    InventoryManager::onItemsSwapped += [this](int i1, int i2) {
        HandleItemsSwapped(i1, i2);
    };

    HandleActiveChanged(activeIndex);
}


void UIInventory::HandleActiveChanged(int index) {
    slots[activeIndex].targetHighlight = 0;
    slots[activeIndex].targetSize = 1;
    activeIndex = index;
    slots[activeIndex].targetHighlight = 1;
    slots[activeIndex].targetSize = 1.3;
}
void UIInventory::HandleItemsSwapped(int i1, int i2) {
    swap(slots[i1].item, slots[i2].item);
}
void UIInventory::HandleOnItemAdded(IItem* item, int index) {
    if(index < 0 || index > slots.size()) return;
    slots[index].item = item;
}
void UIInventory::HandleNrOfLockedSlotsChanged(int nrOfLockedSlots) {
    int newSize = nrOfLockedSlots + 1;
    while(slots.size() < newSize) {
        slots.push_back(UIInventorySlot());
    }
    while(slots.size() > newSize) {
        slots.pop_back();
    }

    int posX = GameManager::GetWindowWidth() / 2 -  (slots.size() * slotSize - (slots.size() - 1) * spacing) / 2;
    int posY = GameManager::GetWindowHeight() - spacing - slotSize;

    rect.x = posX;
    rect.y = posY;

    rect.height = slotSize;
    rect.width = slots.size() * slotSize + (slots.size() - 1) * spacing;
}



void UIInventory::Draw() {
    Vector2 mousePos = InputManager::GetMousePosition();
    if(!isDragging && isBeingClickedOn && clickIndex >= 0 && Vector2Distance(clickPos, mousePos) > 20 && slots[clickIndex].item != nullptr) {
        isDragging = true;
    }

    int posX = rect.x, posY = rect.y;

    float time = GetTime();
    float deltaTime = GetFrameTime();

    for(int i = 0; i < slots.size(); i++) {
        slots[i].UpdateValues(deltaTime);

        BeginShaderMode(slotShader);
            SetShaderValue(slotShader, indexLoc, &i, SHADER_UNIFORM_INT);
            SetShaderValue(slotShader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
            if(slots[i].item == nullptr || (isDragging && clickIndex == i)) SetShaderValueTexture(slotShader, iconLoc, transparentTex);
            else SetShaderValueTexture(slotShader, iconLoc, slots[i].item->GetIcon());

            SetShaderValue(slotShader, highlightLoc, &slots[i].highlight, SHADER_UNIFORM_FLOAT);
            SetShaderValue(slotShader, roundnessLoc, &slots[i].roundness, SHADER_UNIFORM_FLOAT);

            Rectangle renderRect(posX - slotSize * (slots[i].size - 1) / 2,posY - slotSize * (slots[i].size - 1) / 2,slotSize * slots[i].size,slotSize * slots[i].size);

            DrawTexturePro(whiteTex, Rectangle(0,0,1,1), renderRect, Vector2(0,0), 0, WHITE);
        EndShaderMode();

        posX += slotSize + spacing;
    }

    if(isHoveredOver) {
        isHoveredOver = false;
        slots[prevHoverIndex].targetSize -= 0.125;
        prevHoverIndex = -1;
    }

    if(isBeingClickedOn) {
        InputManager::CaptureMouse();
    }

    if(isDragging) {
        Rectangle drawRect(mousePos.x, mousePos.y, slotSize, slotSize);
        const Texture2D& icon = slots[clickIndex].item->GetIcon();
        DrawTexturePro(icon, Rectangle(0,0,icon.width,icon.height), drawRect, Vector2(drawRect.width / 2, drawRect.height / 2), 0, WHITE);
    }

    if(InputManager::IsMouseUp(MOUSE_LEFT_BUTTON) && isBeingClickedOn) {
        HandleClickOrDrag();
    }
}

int UIInventory::GetSlotIndexFromPos(Vector2 pos) const {
    int i = (pos.x - rect.x) / (slotSize + spacing);
    if(i < 0 || i >= slots.size()) return -1;
    return i;
}

void UIInventory::HandleClickOrDrag() {
    if(!isBeingClickedOn && !isDragging) { ResetClickOrDrag(); return; }

    Vector2 pos = InputManager::GetMousePosition();

    if(!ContainsPoint(pos)) { ResetClickOrDrag(); return; }

    if(isDragging) {
        isDragging = false;

        int i2 = GetSlotIndexFromPos(pos);
        if(i2 < 0) { ResetClickOrDrag(); return; }

        InventoryManager::SwapItems(clickIndex, i2);
    } else {
        InventoryManager::EquipItem(clickIndex);
    }

    ResetClickOrDrag();
}
void UIInventory::ResetClickOrDrag() {
    isBeingClickedOn = false;
    clickPos = Vector2(0,0);
    clickIndex = -1;
    isDragging = false;
}



void UIInventory::OnClicked() {
    if(isBeingClickedOn) return;

    int i = GetSlotIndexFromPos(InputManager::GetMousePosition());
    if(i < 0) return;

    isBeingClickedOn = true;
    clickPos = InputManager::GetMousePosition();
    clickIndex = i;
}
void UIInventory::OnMouseUp() {
    if(!isBeingClickedOn) return;
    HandleClickOrDrag();
}
void UIInventory::OnHover() {
    isHoveredOver = true;

    Vector2 pos = InputManager::GetMousePosition();
    int i = GetSlotIndexFromPos(pos);
    if(i < 0) return;

    if(i != prevHoverIndex) {
        slots[prevHoverIndex].targetSize -= 0.125; // TODO: this might degrade due to floating point imprecisions stacking up
        prevHoverIndex = i;
        slots[prevHoverIndex].targetSize += 0.125;
    }

    if(slots[i].item == nullptr) return;

    if(isBeingClickedOn) return;

    ItemInfo info = slots[i].item->GetInfo();

    Rectangle contextInfoRect(pos.x, pos.y, 170, 100);

    contextInfoRect.width = max(contextInfoRect.width, float(info.itemDescription.length() * 10.6) + 40);
    slots[i].item->AdaptDrawInfoRectangle(contextInfoRect);

    if(contextInfoRect.x + contextInfoRect.width > GameManager::GetWindowWidth()) {
        contextInfoRect.x -= contextInfoRect.x + contextInfoRect.width - GameManager::GetWindowWidth();
    }

    contextInfoRect.y -= contextInfoRect.height;
    DrawRectanglePro(contextInfoRect, {0, 0}, 0, Color(25, 10, 10, 200));
    DrawRectangleLinesEx(contextInfoRect, 3, Color(255, 200, 200, 50));
    DrawText(info.itemName.c_str(), contextInfoRect.x + 10, contextInfoRect.y + 10, 25, WHITE);
    DrawText(info.itemDescription.c_str(), contextInfoRect.x + 30, contextInfoRect.y + 40, 20, WHITE);
    slots[i].item->DrawInfo(contextInfoRect);
}

bool UIInventory::ContainsPoint(Vector2 point) const {
    if (!(point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height)) return false;

    float dist = point.x - rect.x;
    dist = fmod(dist, slotSize + spacing);
    if(dist > slotSize) return false;

    return true;
}


