#ifndef __space_invaders_audio_h
#define __space_invaders_audio_h

#include <cer0_synthesizer.h>

#include <CoreAudio/CoreAudio.h>

#define space_invaders_audio_output_io_proc_data_length_synthesizers 0x0a
#define space_invaders_audio_output_io_proc_data_index_synthesizer_lazer 0x00
#define space_invaders_audio_output_io_proc_data_index_synthesizer_explosion 0x05

struct space_invaders_audio_output_io_proc_data {
  struct cer0_synthesizer synthesizers[
    space_invaders_audio_output_io_proc_data_length_synthesizers
  ];
  
  unsigned char index_synthesizer_lazer;
  unsigned char index_synthesizer_explosion;
  
  float* rate_sample;
  
  unsigned char initialized;
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

void space_invaders_audio_output_io_proc_data_destroy(
  struct space_invaders_audio_output_io_proc_data*
);

#endif
