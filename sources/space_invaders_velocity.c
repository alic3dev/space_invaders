#include <space_invaders_velocity.h>

void velocity_initialize(
  struct velocity* velocity
) {
  velocity_reset(
    velocity
  );
}

void velocity_reset(
  struct velocity* velocity
) {
  velocity_reset_x(
    velocity
  );

  velocity_reset_y(
    velocity
  );
}

void velocity_reset_x(
  struct velocity* velocity
) {
  velocity->x = (
    0x00
  );

  velocity->x_buffer = (
    0x00
  );

  velocity->x_rollover = (
    0x00
  );
}

void velocity_reset_y(
  struct velocity* velocity
) {
  velocity->y = (
    0x00
  );

  velocity->y_buffer = (
    0x00
  );

  velocity->y_rollover = (
    0x00
  );
}

void velocity_advance(
  struct velocity* velocity
) {
  velocity->x_buffer = (
    velocity->x_buffer +
    velocity->x
  );

  velocity->y_buffer = (
    velocity->y_buffer +
    velocity->y
  );

  while (
    velocity->x_buffer >=
    0x01
  ) {
    velocity->x_buffer = (
      velocity->x_buffer -
      0x01
    );

    velocity->x_rollover = (
      velocity->x_rollover +
      0x01
    );
  }

  while (
    velocity->x_buffer <=
    -0x01
  ) {
    velocity->x_buffer = (
      velocity->x_buffer +
      0x01
    );

    velocity->x_rollover = (
      velocity->x_rollover -
      0x01
    );
  }

  while (
    velocity->y_buffer >=
    0x01
  ) {
    velocity->y_buffer = (
      velocity->y_buffer -
      0x01
    );

    velocity->y_rollover = (
      velocity->y_rollover +
      0x01
    );
  }

  while (
    velocity->y_buffer <=
    -0x01
  ) {
    velocity->y_buffer = (
      velocity->y_buffer +
      0x01
    );

    velocity->y_rollover = (
      velocity->y_rollover -
      0x01
    );
  }
}
