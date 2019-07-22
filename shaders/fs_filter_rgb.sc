$input v_texcoord0

#include "../thirdparty/bgfx.cmake/bgfx/examples/common/common.sh"
SAMPLER2D(s_texColor, 0);

vec4 redAdjustment;
vec4 greenAdjustment;
vec4 blueAdjustment;
void main()
{
     lowp vec4 textureColor = texture2D(s_texColor, v_texcoord0);
     gl_FragColor = vec4(textureColor.r * redAdjustment.x, textureColor.g * greenAdjustment.x, textureColor.b * blueAdjustment.x, textureColor.a);
}