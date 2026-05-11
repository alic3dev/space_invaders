#ifndef __space_invaders_projectile_h
#define __space_invaders_projectile_h

#include <space_invaders_velocity.h>

#include <cexil.h>

#include <math_c_vector.h>

#define projectile_size_width 0x01
#define projectile_size_height 0x02

enum projectile_source {
  projectile_player,
  projectile_alien
};

extern const struct math_c_vector2_unsigned_char projectile_size;

extern const char projectile_frame_player[
  projectile_size_height
][
  projectile_size_width
];

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

void projectile_poll(
  struct projectile*
);

#endif
