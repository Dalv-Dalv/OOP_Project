#version 430

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 screenSize;

void main() {
    vec2 uv = fragTexCoord;
    vec2 screenUnit = 1 / screenSize;

    uv.y = 1 - uv.y;

    vec4 col = texture(texture0, uv);

    //TODO: OPTIMIZE WITH TWO PASSES: ONE HORIZONTAL ONE VERTICAL
    vec4 total = vec4(0);
    int div = 0;
    const int blurSize = 1;
    for(int i = -blurSize; i <= blurSize; i++){
        for(int j = -blurSize; j <= blurSize; j++){
            vec2 offset = vec2(i, j) * screenUnit;
            total += texture(texture0, uv + offset);

            div++;
        }
    }

    total /= div;
    total = round(total);

    finalColor = vec4(total.rgb, 1.0);
}
