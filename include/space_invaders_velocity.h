#ifndef __space_invaders_velocity_h
#define __space_invaders_velocity_h

struct velocity {
  float x;
  float y;

  float x_buffer;
  float y_buffer;
  
  signed int x_rollover;
  signed int y_rollover;
};

void velocity_initialize(
  struct velocity*
);

void velocity_reset(
  struct velocity*
);

void velocity_reset_x(
  struct velocity*
);

void velocity_reset_y(
  struct velocity*
);

void velocity_advance(
  struct velocity*
);

#endif
