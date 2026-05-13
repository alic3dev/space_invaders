#include <space_invaders_audio.h>

#include <CoreAudio/CoreAudio.h>

int space_invaders_audio_output_io_proc(
  AudioObjectID id_audio_object,
  const AudioTimeStamp* time_stamp_audio,
  const AudioBufferList* list_buffer_audio_in,
  const AudioTimeStamp* time_stamp_audio_in,
  AudioBufferList* list_buffer_audio_out,
  const AudioTimeStamp* time_stamp_audio_out,
  void* data
) {
  for (
    unsigned long int index_buffer = (
      0x00
    );
    (
      index_buffer <
      list_buffer_audio_out->mNumberBuffers
    );
    ++index_buffer
  ) {
    AudioBuffer audio_buffer_current = (
      list_buffer_audio_out->mBuffers[
        index_buffer
      ]
    );
    
    float* buffer_out = (
      audio_buffer_current.mData
    );
    
    unsigned long int length_buffer_out = (
      audio_buffer_current.mDataByteSize /
      sizeof(
        float
      )
    );
    
    for (
      unsigned long int index_buffer_out = (
        0x00
      );
      (
        index_buffer_out <
        length_buffer_out
      );
      ++index_buffer_out
    ) {
      buffer_out[
        index_buffer_out
      ] = (
        0x00
      );
    }
  }

  return (
    0x00
  );
}
