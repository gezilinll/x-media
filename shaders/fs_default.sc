$input v_texcoord0

#include "../thirdparty/bgfx.cmake/bgfx/examples/common/common.sh"
SAMPLER2D(s_texColor0, 0);

void main()
{
   gl_FragColor = texture2D(s_texColor0, v_texcoord0);	 
}