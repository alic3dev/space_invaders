#include <space_invaders_alien.h>

#include <space_invaders_game_state.h>
#include <space_invaders_projectile.h>

#include <clic3_memory.h>

#include <cexil.h>

#include <rand_initialize.h>
#include <rand_functions.h>

const char alien_frames[
  length_alien_frames
][
  alien_size_height
][
  alien_size_width
] = {
  {
    {0,0,1,1,0,1,1,0},
    {0,1,1,0,0,0,1,1},
    {1,0,0,1,1,0,0,1},
    {1,1,0,1,1,0,1,1},
    {1,0,1,0,0,1,0,1},
    {0,1,1,1,1,1,1,0},
    {0,1,0,1,0,1,0,0},
    {0,1,0,0,0,1,0,0},
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
  {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,0,1,1,0,1,0},
    {0,0,1,0,0,1,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,1,0,1,0},
    {0,0,0,0,0,0,0,0},
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,0,1,0,0,1,0,0},
    {0,1,0,1,1,0,1,0},
    {1,0,0,0,0,0,0,1},
    {1,0,1,0,0,1,0,1},
    {0,1,0,0,0,0,1,0},
    {1,0,0,1,1,0,0,1},
    {0,0,0,0,0,0,0,0},
  },
  {
    {0,0,0,0,1,1,0,0},
    {0,0,1,1,0,0,0,0},
    {1,1,0,0,1,1,1,0},
    {1,0,0,0,1,0,0,0},
    {1,0,1,0,0,1,1,0},
    {1,0,1,0,1,1,0,0},
    {0,1,0,0,1,0,0,0},
    {1,0,1,0,0,0,0,0},
  },
  {
    {0,0,0,0,1,0,1,0},
    {0,1,1,1,0,1,0,0},
    {1,1,0,0,0,0,1,0},
    {0,1,1,0,0,1,0,1},
    {0,1,0,1,0,0,0,1},
    {0,0,1,1,0,0,1,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,0,0,1,0,1},
  },
  {
    {0,1,0,0,0,0,1,1},
    {1,0,1,0,0,1,0,0},
    {0,0,0,1,0,0,1,0},
    {0,0,1,1,0,1,0,1},
    {0,0,0,1,0,1,0,0},
    {0,0,1,1,0,1,0,1},
    {0,1,1,1,0,1,1,0},
    {1,0,1,0,1,0,0,0},
  },
  {
    {1,0,0,1,0,0,0,0},
    {1,1,1,1,1,0,1,1},
    {0,1,0,0,0,1,1,0},
    {0,1,1,0,0,0,1,1},
    {0,1,0,1,0,1,0,0},
    {1,0,0,0,1,1,0,0},
    {0,1,1,1,0,0,1,0},
    {1,1,0,0,1,1,0,1},
  },
  {
    {0,1,0,1,0,1,1,0},
    {1,1,1,0,0,0,1,1},
    {1,0,0,1,0,1,0,1},
    {1,0,1,1,0,1,1,1},
    {0,1,1,0,1,1,0,1},
    {1,0,0,1,1,1,0,0},
    {0,1,0,0,0,1,1,1},
    {1,1,0,1,1,0,1,0},
  },
  {
    {0,0,0,0,0,0,0,0},
    {0,1,0,1,1,0,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0},
    {0,0,1,1,1,1,0,0},
  }
};

void alien_initialize(
  struct alien* alien,
  struct game_state* game_state,
  unsigned char index_frame
) {
  struct math_c_vector2_unsigned_int size_alien = {
    .x = (
      alien_size_width
    ),
    .y = (
      alien_size_height
    )
  };

  cexil_sprite_initialize(
    &alien->sprite,
    &size_alien
  );

  alien_frame_set(
    alien->sprite.pixels,
    index_frame
  );

  alien->game_state = (
    game_state
  );
  
  rand_initialize(
    &alien->rand_parameters,
    &alien->rand_result,
    &alien->rand_source,
    0xff,
    rand_mode_bytes,
    rand_source_type_divisive
  );
  
  alien->index_rand = (
    0x00
  );
}

void alien_frame_set(
  char** pixels,
  unsigned char index_frame
) {
  for (
    unsigned char y_index = (
      0x00
    );
    (
      y_index <
      alien_size_height
    );
    ++y_index
  ) {
    for (
      unsigned char x_index = (
        0x00
      );
      (
        x_index <
        alien_size_width
      );
      ++x_index
    ) {
      pixels[
        y_index
      ][
        x_index
      ] = (
        alien_frames[
          index_frame
        ][
          y_index
        ][
          x_index
        ]
      );
    }
  }
}

void alien_poll(
  struct alien* alien
) {
  if (
    alien->index_rand == 
    0x00
  ) {
    rand_get(
      &alien->rand_source,
      &alien->rand_result,
      &alien->rand_parameters
    );
  }

  if (
    (
      alien->rand_result.bytes[
        alien->index_rand
      ]
    ) >=
    0xfe
  ) {
    static struct projectile* projectile;
    
    projectile = (
      clic3_memory_allocate_raw(
        sizeof(
          struct projectile
        )
      )
    );

    projectile_initialize(
      projectile,
      projectile_alien
    );

    projectile->sprite.position.x = (
      alien->sprite.position.x +
      alien->sprite.size.x /
      0x02
    );
    
    projectile->sprite.position.y = (
      alien->sprite.position.y +
      alien->sprite.size.y
    );

    game_state_projectile_alien_add(
      alien->game_state,
      projectile
    );
  }
  
  alien->index_rand = (
    (
      alien->index_rand +
      0x01
    ) %
    alien->rand_result.length
  );
}
