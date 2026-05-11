#include <space_invaders_projectile.h>

#include <space_invaders_velocity.h>

#include <cexil.h>

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
  
  projectile->velocity.x = 0;
  projectile->velocity.y = (
    source == projectile_player
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
    (char*) projectile_frame_player
  );
}

const char projectile_frame_player[projectile_size_height][projectile_size_width] = {
  { 1 },
  { 1 }
};

void projectile_frame_set(
  char** pixels,
  char* frame
) {
  for (
    unsigned char y_index = 0;
    y_index < projectile_size_height;
    ++y_index
  ) {
    const unsigned char y_index_offset = y_index * projectile_size_width;

    for (
      unsigned char x_index = 0;
      x_index < projectile_size_width;
      ++x_index
    ) {
      pixels[y_index][x_index] = frame[y_index_offset + x_index];
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

  projectile->velocity.x_rollover = 0;
  projectile->velocity.y_rollover = 0;
}
