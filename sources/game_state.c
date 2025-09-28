#include <game_state.h>

#include <alien.h>
#include <intro.h>
#include <mode.h>
#include <player.h>
#include <player_input.h>
#include <projectile.h>

#include <cexil.h>

#include <stdlib.h>
#include <stdio.h>

const unsigned int game_state_default_level = 1;

void game_state_initialize_with_mode(
  struct game_state* game_state,
  struct cexil_renderer* renderer,
  struct player* player,
  enum mode mode
) {
  game_state->renderer = renderer;

  intro_initialize(
    &game_state->intro
  );
  
  cexil_renderer_sprite_add(
    game_state->renderer,
    &game_state->intro.sprite
  );

  game_state->intro.sprite.position.x = (
    (game_state->renderer->size.width / 2) - 
    (game_state->intro.sprite.size.width / 2)
  );

  game_state->intro.sprite.position.y = (
    (game_state->renderer->size.height / 2) - 
    (game_state->intro.sprite.size.height / 2)
  );

  game_state->intro.sprite.visible = mode == intro ? 1 : 0;
  
  game_state->player = player;

  game_state->aliens_columns = 10;
  game_state->aliens_rows = 4;
  game_state->aliens_count = 0;
  game_state->aliens = malloc(
    sizeof(struct alien*) * game_state->aliens_count
  );

  velocity_initialize(&game_state->aliens_velocity);

  game_state->score = 0;
  game_state->total_score = 0;
  game_state->total_time = 0;
  game_state->level = game_state_default_level;

  cexil_text_initialize(
    &game_state->text_score,
    ""
  );

  cexil_text_initialize(
    &game_state->text_level,
    ""
  );
  
  cexil_renderer_text_add(
    game_state->renderer,
    &game_state->text_score
  );

  cexil_renderer_text_add(
    game_state->renderer,
    &game_state->text_level
  );

  game_state_text_score_set(
    game_state
  );

  game_state_text_level_set(
    game_state
  );
  
  game_state->text_score.visible = game_state->mode == game;
  game_state->text_level.visible = game_state->mode == game;


  game_state->projectiles_player_count = 0;
  game_state->projectiles_alien_count = 0;

  game_state->projectiles_player = malloc(
    sizeof(struct projectile*) * game_state->projectiles_player_count
  );

  game_state->projectiles_alien = malloc(
    sizeof(struct projectile*) * game_state->projectiles_alien_count
  );

  game_state_mode_set(
    game_state,
    mode
  );
}

void game_state_initialize(
  struct game_state* game_state,
  struct cexil_renderer* renderer,
  struct player* player
) {
  game_state_initialize_with_mode(
    game_state,
    renderer,
    player,
    intro
  );
}

void game_state_mode_set(
  struct game_state* game_state,
  enum mode mode
) {
  if (game_state->mode == mode) {
    return;
  }

  game_state->mode = mode;

  unsigned char elements_game_visible = (
    game_state->mode == game
    ? 1
    : 0
  );

  if (game_state->player->initialized == 1) {
    player_visibility_set(
      game_state->player,
      elements_game_visible
    );
  }

  game_state->text_score.visible = elements_game_visible;
  game_state->text_level.visible = elements_game_visible;

  game_state->intro.sprite.visible = mode == intro ? 1 : 0;

  switch (game_state->mode) {
    case intro:
      cexil_timer_start(&game_state->timer);
      break;
    case menu:
      break;
    case game:
      game_state->total_score = 0;
      game_state->score = 0;

      game_state->total_time = 0;
      cexil_timer_start(&game_state->timer);

      game_state->level = game_state_default_level; 

      player_heal(
        game_state->player,
        player_default_health_max
      );

      game_state_aliens_remove_all(game_state);

      game_state_projectiles_alien_remove_all(game_state);
      game_state_projectiles_player_remove_all(game_state);

      velocity_initialize(&game_state->aliens_velocity);
      game_state_aliens_populate(game_state);

      player_reset(
        game_state,
        game_state->player
      );
      break;
    case game_over:
      break;
    case outro:
      break;
  }
}

void game_state_aliens_populate(struct game_state* game_state) {
  game_state_aliens_remove_all(game_state);

  game_state->aliens_count = game_state->aliens_columns * game_state->aliens_rows;
  game_state->aliens = realloc(
    game_state->aliens,
    sizeof(struct alien*) * game_state->aliens_count
  );

  game_state->aliens_velocity.x = 1;

  game_state->aliens_size.width = (game_state->aliens_columns * (alien_size_width + alien_spacing_x)) - alien_spacing_x;
  game_state->aliens_size.height = game_state->aliens_rows * (alien_size_width + alien_spacing_y);

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

      alien_initialize(
        game_state->aliens[index_alien],
        game_state,
        y_index % length_alien_frames
      );

      game_state->aliens[index_alien]->sprite.position.x = 0 + (x_index * (alien_size_width + alien_spacing_x));
      game_state->aliens[index_alien]->sprite.position.y = 14 + (y_index * (alien_size_height + alien_spacing_y));

      cexil_renderer_sprite_add(
        game_state->renderer,
        &game_state->aliens[index_alien]->sprite
      );
    }
  }
}

void game_state_progress_level(struct game_state* game_state) {
  game_state->total_score = game_state->total_score + game_state->score;

  cexil_timer_stop(&game_state->timer);
  game_state->total_time = (
    game_state->total_time +
    cexil_timer_time_total(&game_state->timer)
  );
  cexil_timer_start(&game_state->timer);

  game_state->level = game_state->level + 1; 

  player_heal(
    game_state->player,
    1
  );

  game_state_aliens_remove_all(game_state);

  game_state_projectiles_alien_remove_all(game_state);
  game_state_projectiles_player_remove_all(game_state);

  game_state_aliens_populate(game_state);

  player_center(
    game_state,
    game_state->player
  );

  game_state_text_level_set(
    game_state
  );
}

void game_state_text_score_set(
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
    &game_state->text_score,
    score_char_array
  );

  game_state->text_score.position.x = (
    game_state->renderer->size.width -
    game_state->text_score.size.width - 1
  );

  free(score_char_array);
}

void game_state_text_level_set(
  struct game_state* game_state
) {
  int level = game_state->level;
  unsigned short int level_length = 1;

  char* level_char_array = malloc(
    sizeof(char) * level_length
  );

  while (level >= 0) {
    unsigned int level_part = level / 10;

    level_length = (
      level_length + 1
    );

    level_char_array = realloc(
      level_char_array,
      sizeof(char) * level_length
    );

    level_char_array[level_length - 2] = (
      '0' + (level - (level_part * 10))
    );

    level = level_part;

    if (level == 0) {
      break;
    }
  }

  for (
    unsigned short int index_level = 0;
    index_level <= (level_length - 2) / 2;
    ++index_level
  ) {
    char char_level_hold = level_char_array[index_level];

    level_char_array[index_level] = level_char_array[level_length - 2 - index_level];
    level_char_array[level_length - 2 - index_level] = char_level_hold;
  }

  level_char_array[level_length - 1] = '\0';

  cexil_text_text_set(
    &game_state->text_level,
    level_char_array
  );

  game_state->text_level.position.x = (
    (game_state->renderer->size.width / 2) -
    (game_state->text_level.size.width / 2)
  );

  free(level_char_array);
}

void game_state_poll_intro(
  struct game_state* game_state
) {
  if (game_state->intro.size_render.height < game_state->intro.sprite.size.height) {
    if (cexil_timer_time_elapsed(&game_state->timer) >= 50000) {
      game_state->intro.size_render.height = (
        game_state->intro.size_render.height + 1
      );

      cexil_sprite_render_size_set(
        &game_state->intro.sprite,
        &game_state->intro.size_render
      );

      cexil_timer_start(&game_state->timer);
    }
  } else {
    if (cexil_timer_time_elapsed(&game_state->timer) >= 2500000) {
      game_state_mode_set(
        game_state,
        game
      );
    }
  }
}

void game_state_poll_game(
  struct game_state* game_state
) {
  for (
    long int index_projectile_player = 0;
    index_projectile_player < game_state->projectiles_player_count;
    ++index_projectile_player
  ) {
    projectile_poll(game_state->projectiles_player[index_projectile_player]);

    if (
      game_state->projectiles_player[index_projectile_player]->sprite.position.y <=
      -projectile_size_height
    ) {
      game_state_projectile_player_remove(
        game_state,
        index_projectile_player
      );

      index_projectile_player = (
        index_projectile_player - 1
      );
      
      continue;
    }

    for (
      long int index_alien = 0;
      index_alien < game_state->aliens_count;
      ++index_alien
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

        game_state_text_score_set(
          game_state
        );

        index_projectile_player = (
          index_projectile_player - 1
        );

        break;
      }
    }
  }

  for (
    unsigned int index_projectile_alien = 0;
    index_projectile_alien < game_state->projectiles_alien_count;
    ++index_projectile_alien
  ) {
    projectile_poll(game_state->projectiles_alien[index_projectile_alien]);

    unsigned char projectile_should_remove = 0;

    if (
      cexil_collision_intersects(
        &game_state->player->sprite.position,
        &game_state->player->sprite.size,
        &game_state->projectiles_alien[index_projectile_alien]->sprite.position,
        &game_state->projectiles_alien[index_projectile_alien]->sprite.size
      ) == 1
    ) {
      projectile_should_remove = 1;

      player_damage(
        game_state->player,
        1
      );

      game_state->score = (
        game_state->score - 150
      );
    } else if (
      game_state->projectiles_alien[index_projectile_alien]->sprite.position.y >= 
      game_state->renderer->size.height
    ) {
      projectile_should_remove = 1;
    }

    if (projectile_should_remove == 1) {
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
      game_state->aliens_velocity.x_rollover = 0;
      game_state->aliens_velocity.y_rollover = 1;

      if (game_state->aliens_position.x + game_state->aliens_size.width > game_state->renderer->size.width) {
        game_state->aliens_position.x = game_state->renderer->size.width - game_state->aliens_size.width;
      }
    }
  } else {
    if (game_state->aliens_position.x <= 0) {
      game_state->aliens_velocity.x = -game_state->aliens_velocity.x + 0.025;
      game_state->aliens_velocity.x_rollover = 0;
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

    if (
      (game_state->aliens[index_alien]->sprite.position.y +
       game_state->aliens[index_alien]->sprite.size.height) >=
      game_state->player->sprite.position.y
    ) {
      game_state_alien_remove(
        game_state,
        index_alien
      );

      player_damage(
        game_state->player,
        1
      );

      game_state->score = (
        game_state->score - 250
      );

      continue;
    }
  }

  game_state->aliens_velocity.x_rollover = 0;
  game_state->aliens_velocity.y_rollover = 0;

  if (game_state->aliens_count == 0) {
    game_state_progress_level(game_state);
  }

  if (
    game_state->player->health == 0
  ) {
    game_state_mode_set(
      game_state,
      game_over
    );
  }
}

void game_state_poll_game_over(struct game_state* game_state) {
  game_state->total_score = game_state->total_score + game_state->score;

  cexil_timer_stop(&game_state->timer);
  game_state->total_time = game_state->total_time + cexil_timer_time_total(&game_state->timer);

  game_state_mode_set(
    game_state,
    outro
  );
}

void game_state_poll(
  struct game_state* game_state
) {
  switch (game_state->mode) {
    case intro:
      game_state_poll_intro(game_state);
      break;
    case game:
      game_state_poll_game(game_state);
      break;
    case game_over:
      game_state_poll_game_over(game_state);
      break;
    default:
      break;
  }
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

void game_state_aliens_remove_all(struct game_state* game_state) {
  if (game_state->aliens_count == 0) {
    return;
  }

  for (
    unsigned short int index_alien = game_state->aliens_count;
    index_alien > 0;
    --index_alien
  ) {
    game_state_alien_remove(
      game_state,
      index_alien - 1
    );
  }
}

void game_state_projectile_add(
  struct game_state* game_state,
  struct projectile* projectile,
  struct projectile*** projectiles,
  unsigned short int* projectiles_count
) {
  *projectiles_count = (
    *projectiles_count + 1
  );

  *projectiles = realloc(
    *projectiles,
    sizeof(struct projectile*) * *projectiles_count
  );

  (*projectiles)[
    *projectiles_count - 1
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
  game_state_projectile_add(
    game_state,
    projectile,
    &game_state->projectiles_alien,
    &game_state->projectiles_alien_count
  );
}

void game_state_projectile_player_add(
  struct game_state* game_state,
  struct projectile* projectile
) {
  game_state_projectile_add(
    game_state,
    projectile,
    &game_state->projectiles_player,
    &game_state->projectiles_player_count
  );
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

void game_state_projectiles_remove_all( 
  struct game_state* game_state,
  struct projectile*** projectiles,
  unsigned short int* projectiles_count 
) {
  for (
    unsigned int index_projectile = *projectiles_count;
    index_projectile > 0;
    --index_projectile
  ) {
    game_state_projectile_remove(
      game_state,
      projectiles,
      index_projectile - 1,
      projectiles_count
    );
  }
}

void game_state_projectiles_alien_remove_all(
  struct game_state* game_state
) {
  game_state_projectiles_remove_all(
    game_state,
    &game_state->projectiles_alien,
    &game_state->projectiles_alien_count
  );
}

void game_state_projectiles_player_remove_all(
  struct game_state* game_state
) {
  game_state_projectiles_remove_all(
    game_state,
    &game_state->projectiles_player,
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
