#include "projectile.h"

#include "cexil.h"

#include "velocity.h"

const struct cexil_size projectile_size = {
  width: PROJECTILE_SIZE_WIDTH,
  height: PROJECTILE_SIZE_HEIGHT
};

void projectile_initialize(
  struct projectile* projectile,
  enum projectile_source source
) {
  projectile->source = source;

  velocity_initialize(&projectile->velocity);
  projectile->velocity.x = 0;
  projectile->velocity.y = (
    source == PLAYER
    ? -1.25125f
    : 1.25125f
  );

  cexil_sprite_initialize(
    &projectile->sprite,
    (struct cexil_size*) &projectile_size
  );

  projectile_frame_set(
    projectile->sprite.pixels,
    (char*) projectile_frame_player
  );
}

const char projectile_frame_player[PROJECTILE_SIZE_HEIGHT][PROJECTILE_SIZE_WIDTH] = {
  { 1 },
  { 1 }
};

void projectile_frame_set(
  char** pixels,
  char* frame
) {
  for (
    unsigned char y_index = 0;
    y_index < PROJECTILE_SIZE_HEIGHT;
    ++y_index
  ) {
    const unsigned char y_index_offset = y_index * PROJECTILE_SIZE_WIDTH;

    for (
      unsigned char x_index = 0;
      x_index < PROJECTILE_SIZE_WIDTH;
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
