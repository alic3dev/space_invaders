#include "heart.h"

// *--------
// |
// | ..  ..
// |........
// |........
// | ......
// |  ....
// |   ..
// |

const char heart_frame_filled[HEART_SIZE_HEIGHT][HEART_SIZE_WIDTH] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

// *--------
// |
// | ..  ..
// |.  ..  .
// |.      .
// | .    .
// |  .  .
// |   ..
// |

const char heart_frame_unfilled[HEART_SIZE_HEIGHT][HEART_SIZE_WIDTH] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {1, 0, 0, 1, 1, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

void heart_frame_set(
  char** pixels,
  unsigned char filled
) {
  const char* heart_frame = (const char*) (
    filled
    ? heart_frame_filled
    : heart_frame_unfilled
  );

  for (
    unsigned char y_index_heart = 0;
    y_index_heart < HEART_SIZE_HEIGHT;
    ++y_index_heart
  ) {
    unsigned char y_index_heart_offset = (
      y_index_heart * HEART_SIZE_WIDTH
    );

    for (
      unsigned char x_index_heart = 0;
      x_index_heart < HEART_SIZE_WIDTH;
      ++x_index_heart
    ) {
      pixels[y_index_heart][x_index_heart] = (
        heart_frame[
          y_index_heart_offset +
          x_index_heart
        ]
      );
    }
  }
}
