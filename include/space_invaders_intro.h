#ifndef __space_invaders_intro_h
#define __space_invaders_intro_h

#include <cexil.h>

#include <math_c_vector.h>

#define size_intro_height 0x12
#define size_intro_width 0x46
#define size_intro_total (\
  size_intro_height *\
  size_intro_width\
)

extern const char pixels_intro[
  size_intro_total
];

struct intro {
  struct cexil_sprite sprite;
  struct math_c_vector2_unsigned_int size_render;
};

void intro_initialize(
  struct intro*
);

#endif
