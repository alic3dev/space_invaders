#include <space_invaders.h>

#include <space_invaders_game_state.h>
#include <space_invaders_parameters.h>
#include <space_invaders_player.h>

#include <cexil_renderer.h>
#include <cexil_size.h>

#include <interrupt_handler.h>

#include <math_c_maximum.h>
#include <math_c_vector.h>

#include <stdio.h>

int main(
  int length_parameters,
  char** parameters
) {
  unsigned char space_invaders_parameters = (
    space_invaders_parameters_parse(
      (
        length_parameters -
        0x01
      ),
      (
        parameters +
        0x01
      )
    )
  );
  
  if (
    space_invaders_parameters ==
    space_invaders_parameter_error
  ) {
    return (
      0x01
    );
  }

  interrupt_handler_initialize();

  struct math_c_vector2_unsigned_int size_screen;

  cexil_size_set_to_terminal(
    &size_screen
  );

  size_screen.x = (
    size_screen.x -
    0x02
  );

  size_screen.y = (
    size_screen.y -
    0x04
  );

  struct math_c_vector2_unsigned_int size_renderer;

  if (
    space_invaders_parameters &
    space_invaders_parameter_fill_screen
  ) {
    size_renderer.x = (
      size_screen.x
    );

    size_renderer.y = (
      size_screen.y
    );
  } else {
    size_renderer.x = (
      space_invaders_renderer_width_default
    );

    size_renderer.y = (
      space_invaders_renderer_height_default
    );
  }

  if (
    (
      size_screen.x <
      size_renderer.x
    ) ||
    (
      size_screen.y <
      size_renderer.y
    )
  ) {
    fprintf(
      stderr,
      "terminal_size_too_small\n"
      "  required:\n"
      "    width->{%i}\n"
      "    height->{%i}\n"
      "  use->{--fill-screen}.to_disable_size_constraints\n",
      (
        space_invaders_renderer_width_default /
        0x02
      ),
      (
        space_invaders_renderer_height_default /
        0x04
      )
    );

    return (
      0x01
    );
  }

  struct math_c_vector2_unsigned_int size_offset = {
    .x = (
      (
        (
          size_screen.x -
          (
            size_renderer.x -
            0x08
          )
        ) /
        0x02
      ) /
      0x02
    ),
    .y = (
      (
        (
          size_screen.y -
          (
            size_renderer.y -
            0x04
          )
        ) /
        0x02
      ) /
      0x04
    )
  };

  struct cexil_renderer renderer;

  cexil_renderer_initialize(
    &renderer,
    &size_renderer
  );

  cexil_renderer_offset_set(
    &renderer,
    &size_offset
  );

  cexil_renderer_target_frame_rate_set(
    &renderer,
    0x3c
  );

  struct game_state game_state;

  game_state_initialize(
    &game_state,
    &renderer
  );
  
  if (
    space_invaders_parameters &
    space_invaders_parameter_fill_screen
  ) {
    game_state.aliens_columns = (
      math_c_maximum_unsigned_int(
        (
          size_renderer.x /
          (
            alien_size_width +
            alien_spacing_x
          ) -
          0x04
        ),
        0x01
      )
    );
    
    game_state.aliens_rows = (
      math_c_maximum_unsigned_int(
        (
          size_renderer.y /
          (
            alien_size_height +
            alien_spacing_y
          ) /
          0x02
        ),
        0x01
      )
    );
  }

  if (
    space_invaders_parameters &
    space_invaders_parameter_skip_intro
  ) {
    game_state_mode_set(
      &game_state,
      game
    );
  }

  while (
    (
      interrupt_handler_interrupted ==
      0x00
    ) &&
    (
      game_state.mode !=
      outro
    )
  ) {
    cexil_renderer_render_clear(
      &renderer
    );

    game_state_poll(
      &game_state
    );

    if (
      game_state.mode !=
      game_over
    ) {
      cexil_renderer_render(
        &renderer
      );
    }
  }

  cexil_renderer_destroy(
    &renderer
  );

  if (
    game_state.mode !=
    intro
  ) {
    if (
      game_state.mode ==
      game
    ) {
      game_state_totals_set(
        &game_state
      );
    }
  
    printf(
      "game_over\n\n"
      "total_score->{%lli}\n"
      "total_time->{%llu_seconds}\n"
      "level->{%llu}\n"
      "\n",
      game_state.total_score,
      (
        game_state.total_time /
        0x0f4240
      ),
      game_state.level
    );
  }
  
  printf(
    "press_any_key_to_exit\n"
  );

  game_state_destroy(
    &game_state
  );

  return (
    0x00
  );
}
