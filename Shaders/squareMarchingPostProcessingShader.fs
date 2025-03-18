#version 430

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 screenSize;

void main() {
    vec2 uv = fragTexCoord;
    vec2 screenUnit = 1 / screenSize;

    vec4 col = texture(texture0, uv);

    vec4 total = vec4(0);
    int div = 0;
    for(int i = -2; i <= 2; i++){
        for(int j = -2; j <= 2; j++){
            vec2 offset = vec2(i, j) * screenUnit;
            total += texture(texture0, uv + offset);

            div++;
        }
    }

    total /= div;
    total = round(total);

    finalColor = vec4(total.rgb, 1.0);
}
