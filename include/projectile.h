#ifndef __projectile_h
#define __projectile_h

#include <velocity.h>

#include <cexil.h>

#define projectile_size_width 1
#define projectile_size_height 2

enum projectile_source {
  projectile_player,
  projectile_alien
};

extern const struct cexil_size projectile_size;

extern const char projectile_frame_player[projectile_size_height][projectile_size_width];

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
