$input v_texcoord0

#include "../thirdparty/bgfx.cmake/bgfx/examples/common/common.sh"
SAMPLER2D(s_texColor, 0);

#define GammaCorrection(color, gamma)								pow(color, 1.0 / gamma)
#define LevelsControlInputRange(color, minInput, maxInput)				min(max(color - minInput, vec3(0.0)) / (maxInput - minInput), vec3(1.0))
#define LevelsControlInput(color, minInput, gamma, maxInput)				GammaCorrection(LevelsControlInputRange(color, minInput, maxInput), gamma)
#define LevelsControlOutputRange(color, minOutput, maxOutput) 			mix(minOutput, maxOutput, color)
#define LevelsControl(color, minInput, gamma, maxInput, minOutput, maxOutput) 	LevelsControlOutputRange(LevelsControlInput(color, minInput, gamma, maxInput), minOutput, maxOutput)

vec4 levelMinimum;
vec4 levelMiddle;
vec4 levelMaximum;
vec4 minOutput;
vec4 maxOutput;

void main()
{
     mediump vec4 textureColor = texture2D(s_texColor, v_texcoord0);
     gl_FragColor = vec4(LevelsControl(textureColor.rgb, vec3(levelMinimum.x, levelMinimum.y, levelMinimum.z),
      vec3(levelMiddle.x, levelMiddle.y, levelMiddle.z),
       vec3(levelMaximum.x, levelMaximum.y, levelMaximum.z),
       vec3(minOutput.x, minOutput.y, minOutput.z),
       vec3(maxOutput.x, maxOutput.y, maxOutput.z)), textureColor.a);
}
