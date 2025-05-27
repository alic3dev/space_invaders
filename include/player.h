#ifndef __PLAYER_H
#define __PLAYER_H

#include "cexil.h"

#include "game_state.h"
#include "velocity.h"

extern const unsigned char player_default_health_max;
extern const unsigned char player_default_health;
extern const float player_default_speed;

struct player {
  struct game_state* game_state;
  struct cexil_sprite sprite;
  struct cexil_sprite* sprites_hearts;
  unsigned char health;
  unsigned char health_max;
  struct velocity velocity;
  float speed;
  unsigned char initialized;
};

void player_initialize(
  struct player*,
  struct game_state*
);

void player_reset(
  struct game_state*,
  struct player*
);

void player_center(
  struct game_state*,
  struct player*
);

void player_visibility_set(
  struct player*,
  unsigned char
);

void player_poll(struct player*);

void player_damage(
  struct player*,
  unsigned char
);

void player_heal(
  struct player*,
  unsigned char
);

void player_destroy(struct player*);

#endif
