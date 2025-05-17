#include "game_state.h"

#include <stdlib.h>

#include "cexil.h"

#include "alien.h"
#include "projectile.h"

const unsigned int game_state_default_health = 0;
const unsigned int game_state_default_level = 1;

void game_state_initialize(
  struct game_state* game_state,
  struct cexil_renderer* renderer
) {
  game_state->renderer = renderer;

  game_state->aliens_columns = 20;
  game_state->aliens_rows = 4;

  game_state->aliens_count = game_state->aliens_columns * game_state->aliens_rows;
  game_state->aliens = malloc(
    sizeof(struct alien*) * game_state->aliens_count
  );

  velocity_initialize(&game_state->aliens_velocity);
  game_state->aliens_velocity.x = 1;

  game_state->aliens_size.width = (game_state->aliens_columns * (ALIEN_SIZE_WIDTH + ALIEN_SPACING_X)) - ALIEN_SPACING_X;
  game_state->aliens_size.height = game_state->aliens_rows * (ALIEN_SIZE_WIDTH + ALIEN_SPACING_Y);

  game_state->aliens_position.x = 0;

  for (
    unsigned char y_index = 0;
    y_index < game_state->aliens_rows;
    ++y_index
  ) {
    unsigned char y_offset = y_index * game_state->aliens_columns;

    for (
      unsigned char x_index = 0;
      x_index < game_state->aliens_columns;
      ++x_index
    ) {
      unsigned char index_alien = y_offset + x_index;

      game_state->aliens[index_alien] = malloc(
        sizeof(struct alien)
      );

      alien_initialize(game_state->aliens[index_alien]);

      game_state->aliens[index_alien]->sprite.position.x = 0 + (x_index * (ALIEN_SIZE_WIDTH + ALIEN_SPACING_X));
      game_state->aliens[index_alien]->sprite.position.y = 14 + (y_index * (ALIEN_SIZE_HEIGHT + ALIEN_SPACING_Y));

      cexil_renderer_sprite_add(
        game_state->renderer,
        &game_state->aliens[index_alien]->sprite
      );
    }
  }

  game_state->score = 0;
  game_state->total_score = 0;
  
  cexil_text_initialize(
    &game_state->score_text,
    ""
  );
  
  cexil_renderer_text_add(
    game_state->renderer,
    &game_state->score_text
  );

  game_state_score_text_set(
    game_state
  );
  
  game_state->total_time = 0;
  cexil_timer_start(&game_state->timer);

  game_state->health = game_state_default_health;
  game_state->level = game_state_default_level;

  game_state->projectiles_player_count = 0;
  game_state->projectiles_alien_count = 0;

  game_state->projectiles_player = malloc(
    sizeof(struct projectile*) * game_state->projectiles_player_count
  );
  game_state->projectiles_alien = malloc(
    sizeof(struct projectile*) * game_state->projectiles_alien_count
  );
}

void game_state_progress_level(struct game_state* game_state) {
  game_state->total_score = game_state->total_score + game_state->score;
  // game_state->score = 0;

  cexil_timer_stop(&game_state->timer);
  game_state->total_time = game_state->total_time + cexil_timer_time_total(&game_state->timer);
  cexil_timer_start(&game_state->timer);

  game_state->level = game_state->level + 1; 
}

void game_state_score_text_set(
  struct game_state* game_state
) {
  int score = game_state->score;
  unsigned short int score_length;

  if (score < 0) {
    score_length = 2;
    score = (score * -1);
  } else {
    score_length = 1;
  }

  char* score_char_array = malloc(
    sizeof(char) * score_length
  );

  while (score >= 0) {
    unsigned int score_part = score / 10;

    score_length = (
      score_length + 1
    );

    score_char_array = realloc(
      score_char_array,
      sizeof(char) * score_length
    );

    score_char_array[score_length - 2] = (
      '0' + (score - (score_part * 10))
    );

    score = score_part;

    if (score == 0) {
      break;
    }
  }

  for (
    unsigned short int index_score = 0;
    index_score <= (score_length - 2) / 2;
    ++index_score
  ) {
    char char_score_hold = score_char_array[index_score];

    score_char_array[index_score] = score_char_array[score_length - 2 - index_score];
    score_char_array[score_length - 2 - index_score] = char_score_hold;
  }

  score_char_array[score_length - 1] = '\0';

  cexil_text_text_set(
    &game_state->score_text,
    score_char_array
  );

  game_state->score_text.position.x = (
    game_state->renderer->size.width -
    game_state->score_text.size.width - 1
  );

  free(score_char_array);
}

void game_state_poll(
  struct game_state* game_state
) {
  for (
    long int index_projectile_player = 0;
    index_projectile_player < game_state->projectiles_player_count;
    ++index_projectile_player
  ) {
    projectile_poll(game_state->projectiles_player[index_projectile_player]);

    if (game_state->projectiles_player[index_projectile_player]->sprite.position.y <= -PROJECTILE_SIZE_HEIGHT) {
      game_state_projectile_player_remove(
        game_state,
        index_projectile_player
      );

      index_projectile_player = (
        index_projectile_player - 1
      );
    }
  }

  for (
    unsigned int index_projectile_alien = 0;
    index_projectile_alien < game_state->projectiles_alien_count;
    ++index_projectile_alien
  ) {
    projectile_poll(game_state->projectiles_alien[index_projectile_alien]);

    if (game_state->projectiles_alien[index_projectile_alien]->sprite.position.y >= game_state->renderer->size.height) {
      game_state_projectile_alien_remove(
        game_state,
        index_projectile_alien
      );

      index_projectile_alien = (
        index_projectile_alien - 1
      );
    }
  }

  velocity_advance(&game_state->aliens_velocity);

  game_state->aliens_position.x = (
    game_state->aliens_position.x +
    game_state->aliens_velocity.x_rollover
  );

  if (game_state->aliens_velocity.x > 0) {
    if (game_state->aliens_position.x + game_state->aliens_size.width >= game_state->renderer->size.width) {
      game_state->aliens_velocity.x = -game_state->aliens_velocity.x - 0.025;
      game_state->aliens_velocity.y_rollover = 1;

      if (game_state->aliens_position.x + game_state->aliens_size.width > game_state->renderer->size.width) {
        game_state->aliens_position.x = game_state->renderer->size.width - game_state->aliens_size.width;
      }
    }
  } else {
    if (game_state->aliens_position.x <= 0) {
      game_state->aliens_velocity.x = -game_state->aliens_velocity.x + 0.025;
      game_state->aliens_velocity.y_rollover = 1;

      if (game_state->aliens_position.x < 0) {
        game_state->aliens_position.x = 0;
      }
    }
  }

  for (
    long int index_alien = 0;
    index_alien < game_state->aliens_count;
    ++index_alien
  ) {
    alien_poll(game_state->aliens[index_alien]);

    game_state->aliens[index_alien]->sprite.position.x = (
      game_state->aliens[index_alien]->sprite.position.x + game_state->aliens_velocity.x_rollover
    );

    game_state->aliens[index_alien]->sprite.position.y = (
      game_state->aliens[index_alien]->sprite.position.y + game_state->aliens_velocity.y_rollover
    );

    for (
      long int index_projectile_player = 0;
      index_projectile_player < game_state->projectiles_player_count;
      ++index_projectile_player
    ) {
      if (
        cexil_collision_intersects(
          &game_state->aliens[index_alien]->sprite.position,
          &game_state->aliens[index_alien]->sprite.size,
          &game_state->projectiles_player[index_projectile_player]->sprite.position,
          &game_state->projectiles_player[index_projectile_player]->sprite.size
        )
      ) {
        game_state_projectile_player_remove(
          game_state,
          index_projectile_player
        );

        game_state_alien_remove(
          game_state,
          index_alien
        );

        game_state->score = (
          game_state->score + 100
        );

        game_state_score_text_set(
          game_state
        );

        index_projectile_player = (
          index_projectile_player - 1
        );

        index_alien = (
          index_alien - 1
        );

        if (game_state->aliens_count == 0) {
          game_state_progress_level(game_state);
        }

        break;
      }
    }
  }

  game_state->aliens_velocity.x_rollover = 0;
  game_state->aliens_velocity.y_rollover = 0;
}

void game_state_alien_remove(
  struct game_state* game_state,
  unsigned short int index_alien
) {
  cexil_renderer_sprite_remove(
    game_state->renderer,
    &game_state->aliens[index_alien]->sprite
  );

  free(game_state->aliens[index_alien]);

  game_state->aliens_count = (game_state->aliens_count - 1);

  for (
    long int index_remaining_alien = index_alien;
    index_remaining_alien < game_state->aliens_count;
    ++index_remaining_alien
  ) {
    game_state->aliens[index_remaining_alien] = (
      game_state->aliens[index_remaining_alien + 1]
    );
  }

  game_state->aliens = realloc(
    game_state->aliens,
    sizeof(struct alien*) * game_state->aliens_count
  );
}

void game_state_projectile_player_add(
  struct game_state* game_state,
  struct projectile* projectile
) {
  game_state->projectiles_player_count = (
    game_state->projectiles_player_count + 1
  );

  game_state->projectiles_player = realloc(
    game_state->projectiles_player,
    sizeof(struct projectile*) * game_state->projectiles_player_count
  );

  game_state->projectiles_player[
    game_state->projectiles_player_count - 1
  ] = projectile;

  cexil_renderer_sprite_add(
    game_state->renderer,
    &projectile->sprite
  );
}

void game_state_projectile_alien_add(
  struct game_state* game_state,
  struct projectile* projectile
) {
  game_state->projectiles_alien_count = (
    game_state->projectiles_alien_count + 1
  );

  game_state->projectiles_alien = realloc(
    game_state->projectiles_alien,
    sizeof(struct projectile*) * game_state->projectiles_alien_count
  );

  game_state->projectiles_alien[
    game_state->projectiles_alien_count - 1
  ] = projectile;
}

void game_state_projectile_remove(
  struct game_state* game_state,
  struct projectile*** projectiles,
  unsigned short int index_projectile,
  unsigned short int* projectiles_count
) {
  cexil_renderer_sprite_remove(
    game_state->renderer,
    &((*projectiles)[index_projectile]->sprite)
  );

  free((*projectiles)[index_projectile]);

  *projectiles_count = (*projectiles_count) - 1;

  for (
    long int index_remaining_projectile = index_projectile;
    index_remaining_projectile < *projectiles_count;
    ++index_remaining_projectile
  ) {
    (*projectiles)[index_remaining_projectile] = (
      (*projectiles)[index_remaining_projectile + 1]
    );
  }

  *projectiles = realloc(
    *projectiles,
    sizeof(struct projectile*) * (*projectiles_count)
  );
}

void game_state_projectile_alien_remove(
  struct game_state* game_state,
  unsigned short int index_projectile
) {
  game_state_projectile_remove(
    game_state,
    &game_state->projectiles_alien,
    index_projectile,
    &game_state->projectiles_alien_count
  );
}

void game_state_projectile_player_remove(
  struct game_state* game_state,
  unsigned short int index_projectile
) {
  game_state_projectile_remove(
    game_state,
    &game_state->projectiles_player,
    index_projectile,
    &game_state->projectiles_player_count
  );
}

void game_state_destroy(struct game_state* game_state) {
  for (
    unsigned int index_projectile_player = 0;
    index_projectile_player < game_state->projectiles_player_count;
    ++index_projectile_player
  ) {
    free(game_state->projectiles_player[index_projectile_player]);
  }

  for (
    unsigned int index_projectile_alien = 0;
    index_projectile_alien < game_state->projectiles_alien_count;
    ++index_projectile_alien
  ) {
    free(game_state->projectiles_alien[index_projectile_alien]);
  }

  for (
    unsigned int index_alien = 0;
    index_alien < game_state->aliens_count;
    ++index_alien
  ) {
    free(game_state->aliens[index_alien]);
  }

  free(game_state->aliens);
  free(game_state->projectiles_player);
  free(game_state->projectiles_alien);
}
