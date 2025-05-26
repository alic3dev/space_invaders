#ifndef __PLAYER_INPUT_H
#define __PLAYER_INPUT_H

#include <pthread.h>

enum player_input_value {
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT
};

extern enum player_input_value player_input;

extern unsigned char user_input_thread_running;

extern pthread_mutex_t player_input_thread_running_mutex;
extern pthread_mutex_t player_input_mutex;

extern pthread_t player_input_thread;

void player_input_thread_start();
void player_input_thread_join();

void player_input_destroy();

void* __player_input_get(void*);

#endif
