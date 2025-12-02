#include <player.h>

#include <game_state.h>
#include <heart.h>
#include <player_input.h>
#include <projectile.h>

#include <pthread.h>
#include <stdlib.h>

const unsigned char player_default_health_max = 5;
const unsigned char player_default_health = player_default_health_max;
const float player_default_speed = 2.0f;

void player_initialize(
  struct player* player,
  struct game_state* game_state
) {
  player->game_state = game_state;
  velocity_initialize(&player->velocity);
  player->speed = player_default_speed;

  struct cexil_size size_sprite_player = {
    .width = 12,
    .height = 8
  };

  cexil_sprite_initialize(
    &player->sprite,
    &size_sprite_player
  );

  for (
    unsigned int index_x = 0;
    index_x < size_sprite_player.width;
    ++index_x
  ) {
    player->sprite.pixels[6][index_x] = 1;
    player->sprite.pixels[7][index_x] = 1;

    if (
      index_x >= size_sprite_player.width / 4 &&
      index_x <= size_sprite_player.width - (size_sprite_player.width / 4)
    ) {
      player->sprite.pixels[5][index_x] = 1;
    }
  }

  player_reset(
    game_state,
    player
  );

  cexil_renderer_sprite_add(
    game_state->renderer,
    &player->sprite
  );

  player->sprites_hearts = malloc(sizeof(struct cexil_sprite) * player->health_max);
  struct cexil_size size_sprite_heart = {
    .width = 8,
    .height = 8
  };

  for (
    unsigned char index_health = 0;
    index_health < player->health_max;
    ++index_health
  ) {
    cexil_sprite_initialize(
      &player->sprites_hearts[index_health],
      &size_sprite_heart
    );

    heart_frame_set(
      player->sprites_hearts[index_health].pixels,
      1
    );

    cexil_renderer_sprite_add(
      game_state->renderer,
      &player->sprites_hearts[index_health]
    );

    player->sprites_hearts[
      index_health
    ].position.x = (
      (size_sprite_heart.width + 2) * index_health
    );
  }

  player->initialized = 1;
}

void player_visibility_set(
  struct player* player,
  unsigned char visible
) {
  player->sprite.visible = visible;

  for (
    unsigned char index_health = 0;
    index_health < player->health_max;
    ++index_health
  ) {
    player->sprites_hearts[index_health].visible = visible;
  }
}

void player_reset(
  struct game_state* game_state,
  struct player* player
) {
  player->health = player_default_health;
  player->health_max = player_default_health_max;

  player_center(
    game_state,
    player
  );
}

void player_center(
  struct game_state* game_state,
  struct player* player
) {
  player->sprite.position.x = game_state->renderer->size.width / 2 - player->sprite.size.width / 2;
  player->sprite.position.y = game_state->renderer->size.height - player->sprite.size.height - 8;

  velocity_reset(
    &player->velocity
  );
}

void player_poll(
  struct player* player
) {
  pthread_mutex_lock(&player_input_mutex);
  switch(player_input) {
    case up: {
      struct projectile* projectile = malloc(sizeof(struct projectile));

      projectile_initialize(
        projectile,
        projectile_player
      );

      projectile->sprite.position.x = player->sprite.position.x + (player->sprite.size.width / 2);
      projectile->sprite.position.y = player->sprite.position.y;

      game_state_projectile_player_add(
        player->game_state,
        projectile
      );
      break;
    }
    case left: {
      player->velocity.x = -player->speed;
      break;
    }
    case right: {
      player->velocity.x = player->speed;
      break;
    }
    case DOWN: {
      player->velocity.x = 0;
      player->velocity.y = 0;
      break;
    }
    default: {
      break;
    }
  }
  player_input = none;
  pthread_mutex_unlock(&player_input_mutex);

  velocity_advance(&player->velocity);

  player->sprite.position.x = player->sprite.position.x + player->velocity.x_rollover;
  player->sprite.position.y = player->sprite.position.y + player->velocity.y_rollover;

  struct cexil_position position_max = {
    .x = (
      player->game_state->renderer->size.width -
      player->sprite.size.width
    ),
    .y = (
      player->game_state->renderer->size.height -
      player->sprite.size.height
    )
  };

  if (player->sprite.position.x < 0) {
    player->sprite.position.x = 0;

    velocity_reset_x(&player->velocity);
  } else if (player->sprite.position.x > position_max.x) {
    player->sprite.position.x = position_max.x;

    velocity_reset_x(&player->velocity);
  }

  if (player->sprite.position.y < 0) {
    player->sprite.position.y = 0;

    velocity_reset_y(&player->velocity);
  } else if (player->sprite.position.y > position_max.y) {
    player->sprite.position.y = position_max.y;

    velocity_reset_y(&player->velocity);
  }

  player->velocity.x_rollover = 0;
  player->velocity.y_rollover = 0;
}

void player_damage(
  struct player* player,
  unsigned char amount
) {
  for (
    unsigned char index_damage = 0;
    index_damage < amount;
    ++index_damage
  ) {
    if (player->health > 0) {
      player->health = (
        player->health - 1
      );

      heart_frame_set(
        player->sprites_hearts[player->health].pixels,
        0
      );
    } else {
      break;
    }
  }
}

void player_heal(
  struct player* player,
  unsigned char amount
) {
  for (
    unsigned char index_heal = 0;
    index_heal < amount;
    ++index_heal
  ) {
    if (player->health < player->health_max) {
      player->health = (
        player->health + 1
      );

      heart_frame_set(
        player->sprites_hearts[player->health - 1].pixels,
        1
      );
    } else {
      break;
    }
  }
}

void player_destroy(struct player* player) {
  free(player->sprites_hearts);
}
