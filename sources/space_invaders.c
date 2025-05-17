#include "space_invaders.h"

#include <stdio.h>
#include <stdlib.h>

#include "cexil.h"
#include "interrupt_handler.h"

#include "game_state.h"
#include "player.h"
#include "player_input.h"
#include "screen.h"

int main() {
  interrupt_handler_initialize();

  struct cexil_size size_screen;
  cexil_size_set_to_terminal(&size_screen);
  size_screen.width = size_screen.width - 2; // SCREEN_SIZE_COLUMNS * 2; //
  size_screen.height = size_screen.height - 4; // SCREEN_SIZE_ROWS * 4; //

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

  player_input_thread_start();

  while (interrupt_handler_interrupted == 0) {
    cexil_renderer_render_clear(
      &renderer
    );
    
    player_poll(&player);
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
