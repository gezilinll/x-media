$input v_texcoord0

#include "../thirdparty/bgfx.cmake/bgfx/examples/common/common.sh"
SAMPLER2D(s_texColor0, 0);
SAMPLER2D(s_texColor1, 1);

void main()
{
   vec4 c2 = texture2D(s_texColor0, v_texcoord0);
   vec4 c1 = texture2D(s_texColor1, v_texcoord0);
   vec4 outputColor;
   float a = c1.a + c2.a * (1.0 - c1.a);
    float alphaDivisor = a + step(a, 0.0); // Protect against a divide-by-zero blacking out things in the output

   outputColor.r = (c1.r * c1.a + c2.r * c2.a * (1.0 - c1.a)) / alphaDivisor;
   outputColor.g = (c1.g * c1.a + c2.g * c2.a * (1.0 - c1.a)) / alphaDivisor;
   outputColor.b = (c1.b * c1.a + c2.b * c2.a * (1.0 - c1.a)) / alphaDivisor;
   outputColor.a = a;

   gl_FragColor = outputColor;
}
