#include "UIInventory.h"

#include "../../../CoreGameLogic/GameManager.h"
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
    InventoryManager::onItemAdded += [this](std::pair<IItem*, int> item) {
        HandleOnItemAdded(item.first, item.second);
    };
    InventoryManager::onNrOfLockedSlotsChanged += [this](int nrOfLockedSlots) {
        HandleNrOfLockedSlotsChanged(nrOfLockedSlots);
    };
    InventoryManager::onActiveSlotChanged += [this](int index) {
        HandleActiveChanged(index);
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
void UIInventory::HandleOnItemAdded(IItem* item, int index) {
    if(index < 0 || index > slots.size()) return;
    slots[index].icon = item->GetIcon();
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
    int posX = rect.x, posY = rect.y;

    float time = GetTime();
    float deltaTime = GetFrameTime();

    for(int i = 0; i < slots.size(); i++) {
        slots[i].UpdateValues(deltaTime);

        BeginShaderMode(slotShader);
            SetShaderValue(slotShader, indexLoc, &i, SHADER_UNIFORM_INT);
            SetShaderValue(slotShader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
            if(slots[i].icon != nullopt) SetShaderValueTexture(slotShader, iconLoc, slots[i].icon.value());
            else SetShaderValueTexture(slotShader, iconLoc, transparentTex);

            SetShaderValue(slotShader, highlightLoc, &slots[i].highlight, SHADER_UNIFORM_FLOAT);
            SetShaderValue(slotShader, roundnessLoc, &slots[i].roundness, SHADER_UNIFORM_FLOAT);

            Rectangle renderRect = Rectangle(posX - slotSize * (slots[i].size - 1) / 2,posY - slotSize * (slots[i].size - 1) / 2,slotSize * slots[i].size,slotSize * slots[i].size);

            DrawTexturePro(whiteTex, Rectangle(0,0,1,1), renderRect, Vector2(0,0), 0, WHITE);
        EndShaderMode();

        posX += slotSize + spacing;
    }
}

void UIInventory::OnMouseDown() {

}
void UIInventory::OnMouseUp() {

}
void UIInventory::OnHover() {

}

