#ifndef __space_invaders_player_input_h
#define __space_invaders_player_input_h

#include <pthread.h>

enum space_invaders_player_input_value {
  space_invaders_player_input_value_none  = 0x00,
  space_invaders_player_input_value_up    = 0x01,
  space_invaders_player_input_value_down  = 0x02,
  space_invaders_player_input_value_left  = 0x03,
  space_invaders_player_input_value_right = 0x04
};

struct space_invaders_player_input {
  unsigned char value;

  unsigned char active;

  pthread_t thread;
};

void space_invaders_player_input_initialize(
  struct space_invaders_player_input*
);

void space_invaders_player_input_destroy(
  struct space_invaders_player_input*
);

void* space_invaders_player_input_thread(
  void*
);

#endif
