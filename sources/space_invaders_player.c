#include <space_invaders_player.h>

#include <space_invaders_heart.h>
#include <space_invaders_player_input.h>
#include <space_invaders_projectile.h>

#include <cexil_renderer.h>
#include <cexil_sprite.h>

#include <clic3_memory.h>

#include <math_c_vector.h>

void player_initialize(
  struct player* player,
  struct cexil_renderer* cexil_renderer
) {
  player->renderer = (
    cexil_renderer
  );

  velocity_initialize(
    &player->velocity
  );

  player->projectile = (
    0x00
  );

  player->speed = (
    player_default_speed
  );

  struct math_c_vector2_unsigned_int size_sprite_player = {
    .x = (
      0x0c
    ),
    .y = (
      0x08
    )
  };

  cexil_sprite_initialize(
    &player->sprite,
    &size_sprite_player
  );

  for (
    unsigned int index_x = (
      0x00
    );
    (
      index_x <
      size_sprite_player.x
    );
    ++index_x
  ) {
    player->sprite.pixels[
      0x06
    ][
      index_x
    ] = (
      0x01
    );

    player->sprite.pixels[
      0x07
    ][
      index_x
    ] = (
      0x01
    );

    if (
      (
        index_x >=
        (
          size_sprite_player.x /
          0x04
        )
      ) &&
      (
        index_x <=
        (
          size_sprite_player.x -
          size_sprite_player.x /
          0x04
        )
      )
    ) {
      player->sprite.pixels[
        0x05
      ][
        index_x
      ] = (
        0x01
      );
    }
  }

  player_reset(
    player
  );

  cexil_renderer_sprite_add(
    player->renderer,
    &player->sprite
  );

  player->sprites_hearts = (
    clic3_memory_allocate_raw(
      sizeof(
        struct cexil_sprite
      ) *
      player->health_max
    )
  );

  struct math_c_vector2_unsigned_int size_sprite_heart = {
    .x = (
      0x08
    ),
    .y = (
      0x08
    )
  };

  for (
    unsigned char index_health = (
      0x00
    );
    (
      index_health <
      player->health_max
    );
    ++index_health
  ) {
    cexil_sprite_initialize(
      &player->sprites_hearts[
        index_health
      ],
      &size_sprite_heart
    );

    heart_frame_set(
      player->sprites_hearts[
        index_health
      ].pixels,
      0x01
    );

    cexil_renderer_sprite_add(
      player->renderer,
      &player->sprites_hearts[
        index_health
      ]
    );

    player->sprites_hearts[
      index_health
    ].position.x = (
      (
        size_sprite_heart.x +
        0x02
      ) *
      index_health
    );
  }

  player_visibility_set(
    player,
    0x00
  );

  space_invaders_player_input_initialize(
    &player->input
  );
}

void player_visibility_set(
  struct player* player,
  unsigned char visible
) {
  player->sprite.visible = (
    visible
  );

  for (
    unsigned char index_health = (
      0x00
    );
    (
      index_health <
      player->health_max
    );
    ++index_health
  ) {
    player->sprites_hearts[
      index_health
    ].visible = (
      visible
    );
  }
}

void player_reset(
  struct player* player
) {
  player->health = (
    player_default_health
  );

  player->health_max = (
    player_default_health_max
  );

  player_center(
    player
  );

  clic3_memory_free(
    player->projectile
  );

  player->projectile = (
    0x00
  );
}

void player_center(
  struct player* player) {
  player->sprite.position.x = (
    player->renderer->size.x /
    0x02 -
    player->sprite.size.x /
    0x02
  );

  player->sprite.position.y = (
    player->renderer->size.y -
    player->sprite.size.y -
    0x08
  );

  velocity_reset(
    &player->velocity
  );
}

void player_poll(
  struct player* player
) {
  switch(
    player->input.value
  ) {
    case space_invaders_player_input_value_up: {
      clic3_memory_free(
        player->projectile
      );

      player->projectile = (
        clic3_memory_allocate_raw(
          sizeof(
            struct projectile
          )
        )
      );

      projectile_initialize(
        player->projectile,
        projectile_player
      );

      player->projectile->sprite.position.x = (
        player->sprite.position.x +
        player->sprite.size.x /
        0x02
      );

      player->projectile->sprite.position.y = (
        player->sprite.position.y
      );

      break;
    }
    case space_invaders_player_input_value_left: {
      player->velocity.x = -(
        player->speed
      );

      break;
    }
    case space_invaders_player_input_value_right: {
      player->velocity.x = (
        player->speed
      );

      break;
    }
    case space_invaders_player_input_value_down: {
      player->velocity.x = (
        0x00
      );

      player->velocity.y = (
        0x00
      );

      break;
    }
    default: {
      break;
    }
  }

  player->input.value = (
    space_invaders_player_input_value_none
  );

  velocity_advance(
    &player->velocity
  );

  player->sprite.position.x = (
    player->sprite.position.x +
    player->velocity.x_rollover
  );

  player->sprite.position.y = (
    player->sprite.position.y +
    player->velocity.y_rollover
  );

  struct math_c_vector2_unsigned_int position_max = {
    .x = (
      player->renderer->size.x -
      player->sprite.size.x
    ),
    .y = (
      player->renderer->size.y -
      player->sprite.size.y
    )
  };

  if (
    player->sprite.position.x <
    0x00
  ) {
    player->sprite.position.x = (
      0x00
    );

    velocity_reset_x(
      &player->velocity
    );
  } else if (
    player->sprite.position.x >
    position_max.x
  ) {
    player->sprite.position.x = (
      position_max.x
    );

    velocity_reset_x(
      &player->velocity
    );
  }

  if (
    player->sprite.position.y <
    0x00
  ) {
    player->sprite.position.y = (
      0x00
    );

    velocity_reset_y(
      &player->velocity
    );
  } else if (
    player->sprite.position.y >
    position_max.y
  ) {
    player->sprite.position.y = (
      position_max.y
    );

    velocity_reset_y(
      &player->velocity
    );
  }

  player->velocity.x_rollover = (
    0x00
  );

  player->velocity.y_rollover = (
    0x00
  );
}

void player_damage(
  struct player* player,
  unsigned char amount
) {
  for (
    unsigned char index_damage = (
      0x00
    );
    (
      index_damage <
      amount
    );
    ++index_damage
  ) {
    if (
      player->health >
      0x00
    ) {
      player->health = (
        player->health -
        0x01
      );

      heart_frame_set(
        player->sprites_hearts[
          player->health
        ].pixels,
        0x00
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
    unsigned char index_heal = (
      0x00
    );
    (
      index_heal <
      amount
    );
    ++index_heal
  ) {
    if (
      player->health <
      player->health_max
    ) {
      player->health = (
        player->health +
        0x01
      );

      heart_frame_set(
        player->sprites_hearts[
          player->health -
          0x01
        ].pixels,
        0x01
      );
    } else {
      break;
    }
  }
}

void player_destroy(
  struct player* player
) {
  clic3_memory_free(
    player->projectile
  );

  clic3_memory_free_raw(
    player->sprites_hearts
  );

  space_invaders_player_input_destroy(
    &player->input
  );
}
