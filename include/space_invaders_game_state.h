#ifndef __space_invaders_game_state_h
#define __space_invaders_game_state_h

#include <space_invaders_alien.h>
#include <space_invaders_intro.h>
#include <space_invaders_mode.h>
#include <space_invaders_player.h>
#include <space_invaders_projectile.h>
#include <space_invaders_velocity.h>

#include <cexil.h>

#include <math_c_vector.h>

#define game_state_default_level 0x01

struct game_state {
  enum mode mode;

  struct intro intro;

  struct cexil_text text_score;
  struct cexil_text text_level;

  long long int score;
  long long int total_score;

  unsigned long long int total_time;

  struct player player;

  struct alien** aliens;
  unsigned long long int aliens_count;

  unsigned long long int aliens_rows;
  unsigned long long int aliens_columns;

  struct math_c_vector2_int aliens_position;

  struct math_c_vector2_unsigned_int aliens_size;

  struct velocity aliens_velocity;

  struct cexil_timer timer;
  unsigned long long int level;

  struct cexil_renderer* renderer;

  struct projectile** projectiles_player;
  struct projectile** projectiles_alien;

  unsigned long long int projectiles_player_count;
  unsigned long long int projectiles_alien_count;
};

void game_state_initialize_with_mode(
  struct game_state*,
  struct cexil_renderer*,
  enum mode
);

void game_state_initialize(
  struct game_state*,
  struct cexil_renderer*
);

void game_state_mode_set(
  struct game_state*,
  enum mode
);

void game_state_aliens_populate(
  struct game_state*
);

void game_state_progress_level(
  struct game_state*
);

void game_state_text_score_set(
  struct game_state*
);

void game_state_text_level_set(
  struct game_state*
);

void game_state_poll_intro(
  struct game_state*
);

void game_state_poll_game(
  struct game_state*
);

void game_state_poll_game_over(
  struct game_state*
);

void game_state_poll(
  struct game_state*
);

void game_state_totals_set(
  struct game_state*
);

void game_state_alien_remove(
  struct game_state*,
  unsigned long long int index_alien
);

void game_state_aliens_remove_all(
  struct game_state*
);

void game_state_projectile_add(
  struct game_state* game_state,
  struct projectile* projectile,
  struct projectile*** projectiles,
  unsigned long long int* projectiles_count
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
  unsigned long long int,
  unsigned long long int*
);

void game_state_projectile_player_remove(
  struct game_state*,
  unsigned long long int
);

void game_state_projectile_alien_remove(
  struct game_state*,
  unsigned long long int
);

void game_state_projectiles_remove_all(
  struct game_state*,
  struct projectile***,
  unsigned long long int*
);

void game_state_projectiles_alien_remove_all(
  struct game_state*
);

void game_state_projectiles_player_remove_all(
  struct game_state*
);

void game_state_destroy(
  struct game_state*
);

#endif
