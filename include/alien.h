#ifndef __alien_h
#define __alien_h

#include <cexil.h>

#include <game_state.h>
#include <velocity.h>

#define length_alien_frames 10

#define alien_size_width 8
#define alien_size_height 8

#define alien_spacing_x 8
#define alien_spacing_y 6

extern const char alien_frames[length_alien_frames][alien_size_height][alien_size_width];

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
