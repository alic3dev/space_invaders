#ifndef __GAME_STATE_H
#define __GAME_STATE_H

#include "cexil.h"

#include "alien.h"
#include "player.h"
#include "projectile.h"
#include "velocity.h"

struct game_state {
  struct cexil_text score_text;
  int score;

  int total_score;
  unsigned long long int total_time;

  struct player* player;
  
  struct alien** aliens;
  unsigned short int aliens_count;
  unsigned char aliens_rows;
  unsigned char aliens_columns;
  struct cexil_position aliens_position;
  struct cexil_size aliens_size;
  struct velocity aliens_velocity;
  
  unsigned int health;
  struct cexil_timer timer;
  unsigned int level;

  struct cexil_renderer* renderer;

  struct projectile** projectiles_player;
  struct projectile** projectiles_alien;

  unsigned short int projectiles_player_count;
  unsigned short int projectiles_alien_count;
};

extern const unsigned int game_state_default_health;
extern const unsigned int game_state_default_level;

void game_state_initialize(
  struct game_state*,
  struct cexil_renderer*,
  struct player*
);

void game_state_aliens_populate(struct game_state*);

void game_state_progress_level(struct game_state*);

void game_state_score_text_set(
  struct game_state*
);

void game_state_poll(struct game_state*);

void game_state_alien_remove(
  struct game_state*,
  unsigned short int index_alien
);

void game_state_aliens_remove_all(struct game_state*);

void game_state_projectile_add(
  struct game_state* game_state,
  struct projectile* projectile,
  struct projectile*** projectiles,
  unsigned short int* projectiles_count
);

void game_state_projectile_player_add(
  struct game_state*,
  struct projectile*
);

void game_state_projectile_alien_add(
  struct game_state*,
  struct projectile*
);

void game_state_projectile_remove(
  struct game_state*,
  struct projectile***,
  unsigned short int,
  unsigned short int*
);

void game_state_projectile_player_remove(
  struct game_state*,
  unsigned short int
);

void game_state_projectile_alien_remove(
  struct game_state*,
  unsigned short int
);

void game_state_destroy(struct game_state*);

#endif
