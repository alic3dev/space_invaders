#include <space_invaders_velocity.h>

void velocity_initialize(struct velocity* velocity) {
  velocity_reset(velocity);
}

void velocity_reset(struct velocity* velocity) {
  velocity_reset_x(velocity);
  velocity_reset_y(velocity);
}

void velocity_reset_x(struct velocity* velocity) {
  velocity->x = 0.0f;
  velocity->x_buffer = 0.0f;
  velocity->x_rollover = 0;
}

void velocity_reset_y(struct velocity* velocity) {
  velocity->y = 0.0f;
  velocity->y_buffer = 0.0f;
  velocity->y_rollover = 0;
}

void velocity_advance(struct velocity* velocity) {
  velocity->x_buffer = (
    velocity->x_buffer + velocity->x
  );

  velocity->y_buffer = (
    velocity->y_buffer + velocity->y
  );

  while (velocity->x_buffer >= 1.0f) {
    velocity->x_buffer = (
      velocity->x_buffer - 1.0f
    );

    velocity->x_rollover = (
      velocity->x_rollover + 1
    );
  }

  while (velocity->x_buffer <= -1.0f) {
    velocity->x_buffer = (
      velocity->x_buffer + 1.0f
    );

    velocity->x_rollover = (
      velocity->x_rollover - 1
    );
  }

  while (velocity->y_buffer >= 1.0f) {
    velocity->y_buffer = (
      velocity->y_buffer - 1.0f
    );

    velocity->y_rollover = (
      velocity->y_rollover + 1
    );
  }

  while (velocity->y_buffer <= -1.0f) {
    velocity->y_buffer = (
      velocity->y_buffer + 1.0f
    );

    velocity->y_rollover = (
      velocity->y_rollover - 1
    );
  }
}
