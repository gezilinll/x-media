$input v_texcoord0

#include "../thirdparty/bgfx.cmake/bgfx/examples/common/common.sh"
SAMPLER2D(s_texColor0, 0);
SAMPLER2D(s_texColor1, 1);

void main()
{
  vec4 base = texture2D(s_texColor0, v_texcoord0);
  vec4 overlay = texture2D(s_texColor1, v_texcoord0);

  float r;
  if (overlay.r * base.a + base.r * overlay.a >= overlay.a * base.a) {
     r = overlay.a * base.a + overlay.r * (1.0 - base.a) + base.r * (1.0 - overlay.a);
  } else {
     r = overlay.r + base.r;
  }

  float g;
  if (overlay.g * base.a + base.g * overlay.a >= overlay.a * base.a) {
     g = overlay.a * base.a + overlay.g * (1.0 - base.a) + base.g * (1.0 - overlay.a);
  } else {
     g = overlay.g + base.g;
  }

  float b;
  if (overlay.b * base.a + base.b * overlay.a >= overlay.a * base.a) {
     b = overlay.a * base.a + overlay.b * (1.0 - base.a) + base.b * (1.0 - overlay.a);
  } else {
     b = overlay.b + base.b;
  }

  float a = overlay.a + base.a - overlay.a * base.a;

  gl_FragColor = vec4(r, g, b, a);
}
