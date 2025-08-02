#include <alien.h>

#include <game_state.h>
#include <projectile.h>

#include <cexil.h>

#include <stdlib.h>

const struct cexil_size size_alien = {
  .width = alien_size_width,
  .height = alien_size_height
};

const char alien_frames[length_alien_frames][alien_size_height][alien_size_width] = {
  {
    {0,0,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1},
    {1,0,0,1,1,0,0,1},
    {1,1,0,1,1,0,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,0,1,0,1,0,1,0},
    {1,0,1,0,1,0,1,0},
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,1,1,0,0,1,1,0},
    {0,1,0,1,1,0,1,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0},
  }, 
};

void alien_initialize(
  struct alien* alien,
  struct game_state* game_state,
  unsigned char index_frame
) {
  cexil_sprite_initialize(
    &alien->sprite,
    (struct cexil_size*) &size_alien
  );

  alien_frame_set(
    alien->sprite.pixels,
    index_frame
  );

  alien->game_state = game_state;
}

void alien_frame_set(
  char** pixels,
  unsigned char index_frame
) {
  for (
    unsigned char y_index = 0;
    y_index < alien_size_height;
    ++y_index
  ) {
    for (
      unsigned char x_index = 0;
      x_index < alien_size_width;
      ++x_index
    ) {
      pixels[y_index][x_index] = (
        alien_frames[index_frame][y_index][x_index]
      );
    }
  }
}

void alien_poll(struct alien* alien) {
  if (rand() % 1000 >= 999) {
    struct projectile* projectile = malloc(sizeof(struct projectile));

    projectile_initialize(
      projectile,
      projectile_alien
    );

    projectile->sprite.position.x = alien->sprite.position.x + (alien->sprite.size.width / 2);
    projectile->sprite.position.y = alien->sprite.position.y + (alien->sprite.size.height);

    game_state_projectile_alien_add(
      alien->game_state,
      projectile
    );
  }
}
