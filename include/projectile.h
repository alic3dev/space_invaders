#ifndef __PROJECTILE_H
#define __PROJECTILE_H

#include <velocity.h>

#include <cexil.h>

#define PROJECTILE_SIZE_WIDTH 1
#define PROJECTILE_SIZE_HEIGHT 2

enum projectile_source {
  PLAYER,
  ALIEN
};

extern const struct cexil_size projectile_size;

extern const char projectile_frame_player[PROJECTILE_SIZE_HEIGHT][PROJECTILE_SIZE_WIDTH];

struct projectile {
  struct cexil_sprite sprite;
  struct velocity velocity;
  enum projectile_source source;
};

void projectile_initialize(
  struct projectile*,
  enum projectile_source
);

void projectile_frame_set(
  char**,
  char*
);

void projectile_poll(struct projectile*);

#endif
