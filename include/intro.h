#ifndef __intro_h
#define __intro_h

#include <cexil.h>

#define size_intro_height 18
#define size_intro_width 70

struct intro {
  struct cexil_sprite sprite;
  struct cexil_size size_render;
};

void intro_initialize(
  struct intro*
);

#endif
