#include <space_invaders_projectile.h>

#include <space_invaders_velocity.h>

#include <cexil.h>

const char projectile_frame_player[
  projectile_size_total
] = {
  0x01,
  0x01
};

void projectile_initialize(
  struct projectile* projectile,
  enum projectile_source source
) {
  projectile->source = (
    source
  );

  velocity_initialize(
    &projectile->velocity
  );

  projectile->velocity.x = (
    0x00
  );

  projectile->velocity.y = (
    (
      source ==
      projectile_player
    )
    ? -1.25125f
    : 1.25125f
  );

  struct math_c_vector2_unsigned_int projectile_size = {
    .x = (
      projectile_size_width
    ),
    .y = (
      projectile_size_height
    )
  };

  cexil_sprite_initialize(
    &projectile->sprite,
    &projectile_size
  );

  projectile_frame_set(
    projectile->sprite.pixels,
    projectile_frame_player
  );
}

void projectile_frame_set(
  char** pixels,
  const char* frame
) {
  for (
    unsigned char index_y = (
      0x00
    );
    (
      index_y <
      projectile_size_height
    );
    ++index_y
  ) {
    unsigned char offset_y = (
      index_y *
      projectile_size_width
    );

    for (
      unsigned char index_x = (
        0x00
      );
      (
        index_x <
        projectile_size_width
      );
      ++index_x
    ) {
      pixels[
        index_y
      ][
        index_x
      ] = (
        frame[
          offset_y +
          index_x
        ]
      );
    }
  }
}

void projectile_poll(
  struct projectile* projectile
) {
  velocity_advance(
    &projectile->velocity
  );

  projectile->sprite.position.x = (
    projectile->sprite.position.x +
    projectile->velocity.x_rollover
  );

  projectile->sprite.position.y = (
    projectile->sprite.position.y +
    projectile->velocity.y_rollover
  );

  projectile->velocity.x_rollover = (
    0x00
  );

  projectile->velocity.y_rollover = (
    0x00
  );
}
