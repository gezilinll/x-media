$input v_texcoord0

#include "../thirdparty/bgfx.cmake/bgfx/examples/common/common.sh"
SAMPLER2D(s_texColor, 0);

vec4 brightness;
void main()
{
     lowp vec4 textureColor = texture2D(s_texColor, v_texcoord0);
     gl_FragColor = vec4((textureColor.rgb + vec3(brightness.x)), textureColor.w);
}