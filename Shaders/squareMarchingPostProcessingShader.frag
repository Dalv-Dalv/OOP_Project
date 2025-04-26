#version 430

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 screenSize;

void main() {
    vec2 uv = fragTexCoord;
    vec2 screenUnit = 1 / screenSize;

    vec4 col = texture(texture0, uv);

    //TODO: OPTIMIZE WITH TWO PASSES: ONE HORIZONTAL ONE VERTICAL
    float total = 0;
    vec3 avg = vec3(0);
    int div = 0;
    const int blurSize = 2;
    for(int i = -blurSize; i <= blurSize; i++){
        for(int j = -blurSize; j <= blurSize; j++){
            vec2 offset = vec2(i, j) * screenUnit;
            vec3 val = texture(texture0, uv + offset).rgb;
            if(val.r > 0.001){
                if(val.r > avg.r){
                    avg = val;
                }
                total += 1.0;
            }

            div++;
        }
    }

    total /= div;
    total = round(total);

    if(total < 0.001) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    }else{
        finalColor = vec4(avg, 1.0);
    }
    //    finalColor = vec4(total * texture(texture0, uv).rgb, 1.0);
}
