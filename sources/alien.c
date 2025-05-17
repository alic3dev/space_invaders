#include "alien.h"

#include "cexil.h"

#include "game_state.h"

const struct cexil_size size_alien = { width: ALIEN_SIZE_WIDTH, height: ALIEN_SIZE_HEIGHT };

const char alien_frame[ALIEN_SIZE_HEIGHT][ALIEN_SIZE_WIDTH] = {
  {0,0,1,1,1,1,1,1},
  {0,1,1,1,1,1,1,1},
  {1,0,0,1,1,0,0,1},
  {1,1,0,1,1,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,0,1,0,1,0,1,0},
  {1,0,1,0,1,0,1,0},
};

void alien_initialize(
  struct alien* alien,
  struct game_state* game_state
) {
  cexil_sprite_initialize(
    &alien->sprite,
    (struct cexil_size*) &size_alien
  );

  alien_frame_set(
    alien->sprite.pixels
  );

  alien->game_state = game_state;
}

void alien_frame_set(char** pixels) {
  for (
    unsigned char y_index = 0;
    y_index < ALIEN_SIZE_HEIGHT;
    ++y_index
  ) {
    for (
      unsigned char x_index = 0;
      x_index < ALIEN_SIZE_WIDTH;
      ++x_index
    ) {
      pixels[y_index][x_index] = alien_frame[y_index][x_index];
    }
  }
}

void alien_poll(struct alien* alien) {}
