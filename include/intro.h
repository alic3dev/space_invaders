#ifndef __intro_h
#define __intro_h

#include <cexil.h>

extern const unsigned char size_intro_height;
extern const unsigned char size_intro_width;

struct intro {
  struct cexil_sprite sprite;
  struct cexil_size size_render;
};

void intro_initialize(
  struct intro*
);

#endif
