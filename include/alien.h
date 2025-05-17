#ifndef __ALIEN_H
#define __ALIEN_H

#include "cexil.h"

#include "velocity.h"

#define ALIEN_SIZE_WIDTH 8
#define ALIEN_SIZE_HEIGHT 8

#define ALIEN_SPACING_X 8
#define ALIEN_SPACING_Y 6

extern const char alien_frame[ALIEN_SIZE_HEIGHT][ALIEN_SIZE_WIDTH];

struct alien {
  struct cexil_sprite sprite;
  struct velocity velocity;
};

void alien_initialize(struct alien*);

void alien_frame_set(char**);

void alien_poll(struct alien*);

#endif
