#ifndef __space_invaders_player_h
#define __space_invaders_player_h

#include <space_invaders_game_state.h>
#include <space_invaders_velocity.h>

#include <cexil.h>

#define player_default_health_max 0x05
#define player_default_health player_default_health_max
#define player_default_speed 0x02

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

void player_poll(
  struct player*
);

void player_damage(
  struct player*,
  unsigned char
);

void player_heal(
  struct player*,
  unsigned char
);

void player_destroy(
  struct player*
);

#endif
