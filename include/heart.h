#ifndef __HEART_H
#define __HEART_H

#define HEART_SIZE_WIDTH 8
#define HEART_SIZE_HEIGHT 8

extern const char heart_frame_filled[HEART_SIZE_HEIGHT][HEART_SIZE_WIDTH];
extern const char heart_frame_unfilled[HEART_SIZE_HEIGHT][HEART_SIZE_WIDTH];

void heart_frame_set(
  char**,
  unsigned char
);

#endif
