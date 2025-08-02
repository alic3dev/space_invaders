#ifndef __alien_h
#define __alien_h

#include "cexil.h"

#include "game_state.h"
#include "velocity.h"

#define length_alien_frames 2

#define ALIEN_SIZE_WIDTH 8
#define ALIEN_SIZE_HEIGHT 8

#define ALIEN_SPACING_X 8
#define ALIEN_SPACING_Y 6

extern const char alien_frames[length_alien_frames][ALIEN_SIZE_HEIGHT][ALIEN_SIZE_WIDTH];

struct alien {
  struct cexil_sprite sprite;
  struct velocity velocity;
  struct game_state* game_state;
};

void alien_initialize(
  struct alien*,
  struct game_state*,
  unsigned char
);

void alien_frame_set(
  char**,
  unsigned char
);

void alien_poll(struct alien*);

#endif
