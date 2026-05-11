#ifndef __space_invaders_intro_h
#define __space_invaders_intro_h

#include <cexil.h>

#include <math_c_vector.h>

#define size_intro_height 0x12
#define size_intro_width 0x46

struct intro {
  struct cexil_sprite sprite;
  struct math_c_vector2_unsigned_int size_render;
};

void intro_initialize(
  struct intro*
);

#endif
