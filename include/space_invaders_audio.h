#ifndef __space_invaders_audio_h
#define __space_invaders_audio_h

#include <space_invaders_game_state.h>

#include <CoreAudio/CoreAudio.h>

struct space_invaders_audio_output_io_proc_data {
  struct game_state* game_state;
};

int space_invaders_audio_output_io_proc(
  AudioObjectID,
  const AudioTimeStamp*,
  const AudioBufferList*,
  const AudioTimeStamp*,
  AudioBufferList*,
  const AudioTimeStamp*,
  void*
);

#endif
