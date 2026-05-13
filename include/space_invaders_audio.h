#ifndef __space_invaders_audio_h
#define __space_invaders_audio_h

#include <CoreAudio/CoreAudio.h>

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
