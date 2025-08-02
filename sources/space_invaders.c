#include <space_invaders.h>

#include <game_state.h>
#include <player.h>
#include <player_input.h>
#include <screen.h>
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
    length_parameters >= 2 &&
    clic3_char_arrays_equal(
      parameters[1],
      "--skip-intro"
    ) == 1
    ? 1
    : 0
  );

  interrupt_handler_initialize();

  struct cexil_size size_screen;
  cexil_size_set_to_terminal(&size_screen);
  size_screen.width = size_screen.width - 2; // screen_size_columns * 2; //
  size_screen.height = size_screen.height - 4; // screen_size_rows * 4; //

  struct cexil_renderer renderer;

  cexil_renderer_initialize(
    &renderer,
    &size_screen
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

  while (interrupt_handler_interrupted == 0) {
    cexil_renderer_render_clear(
      &renderer
    );

    if (game_state.mode == game) {
      player_poll(&player);
    }

    game_state_poll(&game_state);

    cexil_renderer_render(
      &renderer
    );
  }
  
  cexil_renderer_destroy(
    &renderer
  );

  game_state_destroy(&game_state);

  player_destroy(&player);

  printf("press_any_key_to_exit\n");
  player_input_thread_join();
  player_input_destroy();

  return interrupt_handler_interrupted;
}
