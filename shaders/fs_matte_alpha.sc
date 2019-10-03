$input v_texcoord0

#include "../thirdparty/bgfx.cmake/bgfx/examples/common/common.sh"
SAMPLER2D(s_texColor0, 0);
SAMPLER2D(s_texColor1, 1);

void main()
{
   vec4 textureColor = texture2D(s_texColor0, v_texcoord0);
   vec4 textureColor2 = texture2D(s_texColor1, v_texcoord0);

   //Averages mask's the RGB values, and scales that value by the mask's alpha
   //
   //The dot product should take fewer cycles than doing an average normally
   //
   //Typical/ideal case, R,G, and B will be the same, and Alpha will be 1.0
   float newAlpha = dot(textureColor2.rgb, vec3(.33333334, .33333334, .33333334)) * textureColor2.a;

   gl_FragColor = vec4(textureColor.xyz, newAlpha);
}
