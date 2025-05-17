#ifndef __PLAYER_H
#define __PLAYER_H

#include "cexil.h"

#include "game_state.h"
#include "velocity.h"

extern const unsigned int player_default_health_max;
extern const unsigned int player_default_health;
extern const float player_default_speed;

struct player {
  struct game_state* game_state;
  struct cexil_sprite sprite;
  struct cexil_sprite* sprites_hearts;
  unsigned int health;
  unsigned int health_max;
  struct velocity velocity;
  float speed;
};

void player_initialize(
  struct player*,
  struct game_state*
);

void player_poll(struct player*);

void player_destroy(struct player*);

#endif
