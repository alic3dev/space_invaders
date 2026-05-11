#include <space_invaders_player_input.h>

#include <clic3.h>

#include <pthread.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void space_invaders_player_input_initialize(
  struct space_invaders_player_input* space_invaders_player_input
) {
  space_invaders_player_input->value = (
    space_invaders_player_input_value_none
  );
  
  space_invaders_player_input->active = (
    0x01
  );
  pthread_create(
    &space_invaders_player_input->thread,
    0x00,
    space_invaders_player_input_thread,
    space_invaders_player_input
  );
}

void space_invaders_player_input_destroy(
  struct space_invaders_player_input* space_invaders_player_input
) {
  space_invaders_player_input->active = (
    0x00
  );

  pthread_join(
    space_invaders_player_input->thread,
    0x00
  );
}

void* space_invaders_player_input_thread(
  void* data
) {
  struct space_invaders_player_input* space_invaders_player_input = (
    data
  );

  struct termios termios_attrs_original;
  struct termios termios_attrs_updated;

  tcgetattr(
    STDIN_FILENO,
    &termios_attrs_original
  );

  tcgetattr(
    STDIN_FILENO,
    &termios_attrs_updated
  );

  termios_attrs_updated.c_lflag &= ~(
    ICANON |
    ECHO
  );

  tcsetattr(
    STDIN_FILENO,
    TCSANOW,
    &termios_attrs_updated
  );

  int player_input_intermediary;
  
  int player_input_intermediary_buffer[
    0x02
  ] = {
    0x00,
    0x00
  };
  
  unsigned char player_input_intermediary_buffer_index = (
    0x00
  );

  do {
    player_input_intermediary = (
      getchar()
    );

    switch(
      player_input_intermediary
    ) {
      case clic3_char_value_ctrl: {
        if (
          player_input_intermediary_buffer_index ==
          0x00
        ) {
          player_input_intermediary_buffer[
            0x00
          ] = (
            player_input_intermediary
          );
          
          player_input_intermediary_buffer_index = (
            0x01
          );
        } else {
          player_input_intermediary_buffer[
            0x00
          ] = (
            0x00
          );
          
          player_input_intermediary_buffer[
            0x01
          ] = (
            0x00
          );
          
          player_input_intermediary_buffer_index = (
            0x00
          );
        }
        
        break;
      }
      case clic3_char_value_square_bracket_opening: {
        if (
          player_input_intermediary_buffer_index ==
          0x01
        ) {
          player_input_intermediary_buffer[
            0x01
          ] = (
            player_input_intermediary
          );
          
          player_input_intermediary_buffer_index = (
            0x02
          );
        } else {
          player_input_intermediary_buffer[
            0x00
          ] = (
            0x00
          );
          
          player_input_intermediary_buffer[
            0x01
          ] = (
            0x00
          );
          
          player_input_intermediary_buffer_index = (
            0x00
          );
        }
        
        break;
      }
      case clic3_char_value_A: {
        if (
          player_input_intermediary_buffer_index ==
          0x02
        ) {
          space_invaders_player_input->value = (
            space_invaders_player_input_value_up
          );
        }

        player_input_intermediary_buffer[
          0x00
        ] = (
          0x00
        );
        
        player_input_intermediary_buffer[
          0x01
        ] = (
          0x00
        );
        
        player_input_intermediary_buffer_index = (
          0x00
        );

        break;
      }
      case clic3_char_value_B: {
        if (
          player_input_intermediary_buffer_index ==
          0x02
        ) {
          space_invaders_player_input->value = (
            space_invaders_player_input_value_down
          );
        }

        player_input_intermediary_buffer[
          0x00
        ] = (
          0x00
        );
        
        player_input_intermediary_buffer[
          0x01
        ] = (
          0x00
        );
        
        player_input_intermediary_buffer_index = (
          0x00
        );

        break;
      }
      case clic3_char_value_C: {
        if (
          player_input_intermediary_buffer_index ==
          0x02
        ) {
          space_invaders_player_input->value = (
            space_invaders_player_input_value_right
          );
        }

        player_input_intermediary_buffer[
          0x00
        ] = (
          0x00
        );
        
        player_input_intermediary_buffer[
          0x01
        ] = (
          0x00
        );
        
        player_input_intermediary_buffer_index = (
          0x00
        );

        break;
      }
      case clic3_char_value_D: {
        if (
          player_input_intermediary_buffer_index ==
          0x02
        ) {
          space_invaders_player_input->value = (
            space_invaders_player_input_value_left
          );
        }

        player_input_intermediary_buffer[
          0x00
        ] = (
          0x00
        );
        
        player_input_intermediary_buffer[
          0x01
        ] = (
          0x00
        );
        
        player_input_intermediary_buffer_index = (
          0x00
        );

        break;
      }
      default: {
        player_input_intermediary_buffer[
          0x00
        ] = (
          0x00
        );
        
        player_input_intermediary_buffer[
          0x01
        ] = (
          0x00
        );
        
        player_input_intermediary_buffer_index = (
          0x00
        );

        break;
      }
    }
  } while (
    space_invaders_player_input->active ==
    0x01
  );

  tcsetattr(
    STDIN_FILENO,
    TCSANOW,
    &termios_attrs_original
  );

  return (
    0x00
  );
}
