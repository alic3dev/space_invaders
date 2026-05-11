#ifndef __space_invaders_alien_h
#define __space_invaders_alien_h

#include <space_invaders_game_state.h>
#include <space_invaders_velocity.h>

#include <cexil.h>

#include <rand_parameters.h>
#include <rand_result.h>
#include <rand_source.h>

#define length_alien_frames 0x0a

#define alien_size_width 0x08
#define alien_size_height 0x08

#define alien_spacing_x 0x08
#define alien_spacing_y 0x06

extern const char alien_frames[
  length_alien_frames
][
  alien_size_height
][
  alien_size_width
];

struct alien {
  struct cexil_sprite sprite;
  struct velocity velocity;
  struct game_state* game_state;

  struct rand_parameters rand_parameters;
  struct rand_result rand_result;
  struct rand_source rand_source;

  unsigned int index_rand;
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

void alien_poll(
  struct alien*
);

#endif
