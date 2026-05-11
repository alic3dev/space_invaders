#include <space_invaders.h>

#include <space_invaders_game_state.h>
#include <space_invaders_player.h>

#include <cexil.h>
#include <clic3.h>
#include <interrupt_handler.h>

#include <stdio.h>
#include <stdlib.h>

int main(
  int length_parameters,
  char** parameters
) {
  unsigned char skip_intro = (
    0x00
  );

  unsigned char fill_screen = (
    0x00
  );

  for (
    int index_parameter = (
      0x01
    );
    (
      index_parameter <
      length_parameters
    );
    ++index_parameter
  ) {
    switch (
      clic3_char_arrays_within(
        parameters[
          index_parameter
        ],
        0x02,
        "--skip-intro",
        "--fill-screen"
      )
    ) {
      case 0x00: {
        if (
          skip_intro ==
          0x01
        ) {
          fprintf(
            stderr,
            "already_set:parameter->{%s}\n",
            parameters[
              index_parameter
            ]
          );

          return (
            0x01
          );
        }

        skip_intro = (
          0x01
        );

        break;
      }
      case 0x01: {
        if (
          fill_screen ==
          0x01
        ) {
          fprintf(
            stderr,
            "already_set:parameter->{%s}\n",
            parameters[
              index_parameter
            ]
          );

          return (
            0x01
          );
        }

        fill_screen = (
          0x01
        );

        break;
      }
      default: {
        fprintf(
          stderr,
          "unknown:parameter->{%s}\n",
          parameters[
            index_parameter
          ]
        );

        return (
          0x01
        );
      }
    }
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
    fill_screen ==
    0x01
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
    skip_intro ==
    0x01
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

  printf(
    "game_over\n\n"
    "total_score->{%i}\n"
    "total_time->{%llu_seconds}\n"
    "level->{%u}\n"
    "\npress_any_key_to_exit\n",
    game_state.total_score,
    (
      game_state.total_time /
      0x0f4240
    ),
    game_state.level
  );

  game_state_destroy(
    &game_state
  );

  return (
    0x00
  );
}
