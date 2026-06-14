#include <space_invaders_audio.h>

#include <space_invaders_game_state.h>

#include <cer0_synthesizer.h>

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
  struct game_state* game_state = (
    data
  );
  
  if (
    game_state->audio.initialized ==
    0x00
  ) {
    for (
      unsigned char index_synthesizer = (
        0x00
      );
      (
        index_synthesizer <
        space_invaders_audio_output_io_proc_data_length_synthesizers
      );
      ++index_synthesizer
    ) {
      struct cer0_synthesizer* cer0_synthesizer = &(
        game_state->audio.synthesizers[
          index_synthesizer
        ]
      );
    
      cer0_synthesizer_initialize(
        cer0_synthesizer,
        *game_state->audio.rate_sample
      );
      
      cer0_synthesizer_oscillator_add(
        cer0_synthesizer,
        square  
      );
    }
  }

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
      float value = (
        0x00
      );
      
      unsigned char active_synthesizers = (
        0x00
      );
      
      for (
        unsigned char index_synthesizer = (
          0x00
        );
        (
          index_synthesizer <
          space_invaders_audio_output_io_proc_data_length_synthesizers
        );
        ++index_synthesizer
      ) {
        struct cer0_synthesizer* cer0_synthesizer = &(
          game_state->audio.synthesizers[
            index_synthesizer
          ]
        );
      
        if (
          cer0_synthesizer->frequency !=
          0x00
        ) {
          active_synthesizers = (
            active_synthesizers +
            0x01
          );
  
          value = (
            value +
            cer0_synthesizer_poll(
              cer0_synthesizer
            )
          );
        }
      }
      
      buffer_out[
        index_buffer_out
      ] = (
        value
      );
    }
  }

  return (
    0x00
  );
}

void space_invaders_audio_output_io_proc_data_destroy(
  struct space_invaders_audio_output_io_proc_data* space_invaders_audio_output_io_proc_data
) {
  for (
    unsigned char index_synthesizer = (
      0x00
    );
    (
      index_synthesizer <
      space_invaders_audio_output_io_proc_data_length_synthesizers
    );
    ++index_synthesizer
  ) {
    cer0_synthesizer_destroy(
      &space_invaders_audio_output_io_proc_data->synthesizers[
        index_synthesizer
      ]
    );
  }  
}
