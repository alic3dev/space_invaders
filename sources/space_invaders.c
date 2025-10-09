#include <space_invaders.h>

#include <game_state.h>
#include <player.h>
#include <player_input.h>
#include <cexil.h>
#include <clic3.h>
#include <interrupt_handler.h>

#include <stdio.h>
#include <stdlib.h>

const unsigned short int space_invaders_renderer_width_default = 228;
const unsigned short int space_invaders_renderer_height_default = 128;

int main(
  int length_parameters,
  char** parameters
) {
  unsigned char skip_intro = 0;
  unsigned char fill_screen = 0;

  for (
    int index_parameter = 1;
    index_parameter < length_parameters;
    ++index_parameter
  ) {
    int index_parameter_found = clic3_char_arrays_within(
      parameters[index_parameter],
      2,
      "--skip-intro",
      "--fill-screen"
    );

    switch (index_parameter_found) {
      case 0:
        if (
          skip_intro == 1
        ) {
          fprintf(
            stderr,
            "already_set:parameter->{%s}\n",
            parameters[
              index_parameter
            ]
          );

          return 1;
        }

        skip_intro = 1;
        break;
      case 1:
        if (
          fill_screen == 1
        ) {
          fprintf(
            stderr,
            "already_set:parameter->{%s}\n",
            parameters[
              index_parameter
            ]
          );

          return 1;
        }

        fill_screen = 1;
        break;
      default:
        fprintf(
          stderr,
          "unknown:parameter->{%s}\n",
          parameters[
            index_parameter
          ]
        );

        return 1;
    }
  }

  interrupt_handler_initialize();

  struct cexil_size size_screen;
  cexil_size_set_to_terminal(&size_screen);

  size_screen.width = size_screen.width - 2;
  size_screen.height = size_screen.height - 4;

  struct cexil_size size_renderer;

  if (fill_screen) {
    size_renderer.width = size_screen.width;
    size_renderer.height = size_screen.height;
  } else {
    size_renderer.width = space_invaders_renderer_width_default;
    size_renderer.height = space_invaders_renderer_height_default;
  }

  if (
    size_screen.width < size_renderer.width ||
    size_screen.height < size_renderer.height
  ) {
    fprintf(
      stderr,
      "terminal_size_too_small\n"
      "  required:\n"
      "    width->{%i}\n"
      "    height->{%i}\n"
      "  use->{--fill-screen}.to_disable_size_constraints\n",
      space_invaders_renderer_width_default / 2,
      space_invaders_renderer_height_default / 4
    );

    return 1;
  }

  struct cexil_size size_offset = {
    .width = ((size_screen.width - (size_renderer.width - 8)) / 2) / 2,
    .height = ((size_screen.height - (size_renderer.height - 4)) / 2) / 4
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
    60.0f
  );

  struct game_state game_state;
  struct player player;

  game_state_initialize(
    &game_state,
    &renderer,
    &player
  );

  player_initialize(
    &player,
    &game_state
  );

  player_visibility_set(
    &player,
    0
  );

  if (skip_intro == 1) {
    game_state_mode_set(
      &game_state,
      game
    );
  }

  player_input_thread_start();

  while (
    interrupt_handler_interrupted == 0 &&
    game_state.mode != outro
  ) {
    cexil_renderer_render_clear(
      &renderer
    );

    if (game_state.mode == game) {
      player_poll(&player);
    }

    game_state_poll(&game_state);

    if (
      game_state.mode != game_over
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
    "GAME OVER\n\n"
    "total_score: %i\n"
    "total_time: %llus\n"
    "level: %u\n",
    game_state.total_score,
    game_state.total_time / 1000000,
    game_state.level
  );

  game_state_destroy(&game_state);

  player_destroy(&player);

  printf("press_any_key_to_exit\n");
  player_input_thread_join();
  player_input_destroy();

  return 0;
}
