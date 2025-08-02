#ifndef __heart_h
#define __heart_h

#define heart_size_width 8
#define heart_size_height 8

extern const char heart_frame_filled[heart_size_height][heart_size_width];
extern const char heart_frame_unfilled[heart_size_height][heart_size_width];

void heart_frame_set(
  char**,
  unsigned char
);

#endif
