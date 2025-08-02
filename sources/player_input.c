#include <player_input.h>

#include <clic3.h>

#include <pthread.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

enum player_input_value player_input = NONE;

unsigned char player_input_thread_running = 0;

pthread_mutex_t player_input_thread_running_mutex;
pthread_mutex_t player_input_mutex;

pthread_t player_input_thread;

void player_input_thread_start() {
  pthread_mutex_init(
    &player_input_mutex,
    (void*)0
  );
  pthread_mutex_init(
    &player_input_thread_running_mutex,
    (void*)0
  );

  pthread_create(
    &player_input_thread,
    (void*)0,
    __player_input_get,
    (void*)0
  );

  pthread_mutex_lock(
    &player_input_thread_running_mutex
  );
  player_input_thread_running = 1;
  pthread_mutex_unlock(
    &player_input_thread_running_mutex
  );
}

void player_input_thread_join() {
  pthread_mutex_lock(
    &player_input_thread_running_mutex
  );
  player_input_thread_running = 0;
  pthread_mutex_unlock(
    &player_input_thread_running_mutex
  );

  pthread_join(
    player_input_thread,
    (void*)0
  );
}

void player_input_destroy() {
  pthread_mutex_destroy(
    &player_input_mutex
  );
  pthread_mutex_destroy(
    &player_input_thread_running_mutex
  );
}

void* __player_input_get(void* _) {
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
    ICANON | ECHO
  );

  tcsetattr(
    STDIN_FILENO,
    TCSANOW,
    &termios_attrs_updated
  );

  int player_input_intermediary;
  int player_input_intermediary_buffer[2] = { 0, 0 };
  unsigned char player_input_intermediary_buffer_index = 0;

  do {
    pthread_mutex_unlock(&player_input_thread_running_mutex);

    player_input_intermediary = getchar();

    switch(player_input_intermediary) {
      case clic3_char_value_ctrl:
        if (player_input_intermediary_buffer_index == 0) {
          player_input_intermediary_buffer[0] = player_input_intermediary;
          player_input_intermediary_buffer_index = 1;
        } else {
          player_input_intermediary_buffer[0] = 0;
          player_input_intermediary_buffer[1] = 0;
          player_input_intermediary_buffer_index = 0;
        }
        break;
      case clic3_char_value_square_bracket_opening:
        if (player_input_intermediary_buffer_index == 1) {
          player_input_intermediary_buffer[1] = player_input_intermediary;
          player_input_intermediary_buffer_index = 2;
        } else {
          player_input_intermediary_buffer[0] = 0;
          player_input_intermediary_buffer[1] = 0;
          player_input_intermediary_buffer_index = 0;
        }
        break;
      case clic3_char_value_A:
        if (player_input_intermediary_buffer_index == 2) {
          pthread_mutex_lock(&player_input_mutex);
          player_input = UP;
          pthread_mutex_unlock(&player_input_mutex);
        }

        player_input_intermediary_buffer[0] = 0;
        player_input_intermediary_buffer[1] = 0;
        player_input_intermediary_buffer_index = 0;
        
        break;
      case clic3_char_value_B:
        if (player_input_intermediary_buffer_index == 2) {
          pthread_mutex_lock(&player_input_mutex);
          player_input = DOWN;
          pthread_mutex_unlock(&player_input_mutex);
        }

        player_input_intermediary_buffer[0] = 0;
        player_input_intermediary_buffer[1] = 0;
        player_input_intermediary_buffer_index = 0;
        
        
        break;
      case clic3_char_value_C:
        if (player_input_intermediary_buffer_index == 2) {
          pthread_mutex_lock(&player_input_mutex);
          player_input = RIGHT;
          pthread_mutex_unlock(&player_input_mutex);
        }

        player_input_intermediary_buffer[0] = 0;
        player_input_intermediary_buffer[1] = 0;
        player_input_intermediary_buffer_index = 0;
        
        break;
      case clic3_char_value_D:
        if (player_input_intermediary_buffer_index == 2) {
          pthread_mutex_lock(&player_input_mutex);
          player_input = LEFT;
          pthread_mutex_unlock(&player_input_mutex);
        }

        player_input_intermediary_buffer[0] = 0;
        player_input_intermediary_buffer[1] = 0;
        player_input_intermediary_buffer_index = 0;
        
        break;
      default: 
        player_input = NONE;
        player_input_intermediary_buffer[0] = 0;
        player_input_intermediary_buffer[1] = 0;
        player_input_intermediary_buffer_index = 0;

        break;
    }

    pthread_mutex_lock(&player_input_thread_running_mutex);
  } while(player_input_thread_running == 1);

  tcsetattr(
    STDIN_FILENO,
    TCSANOW,
    &termios_attrs_original
  );
}
