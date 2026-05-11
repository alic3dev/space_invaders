#ifndef __space_invaders_heart_h
#define __space_invaders_heart_h

#define heart_size_width 0x08
#define heart_size_height 0x08

extern const char heart_frame_filled[
  heart_size_height
][
  heart_size_width
];

extern const char heart_frame_unfilled[
  heart_size_height
][
  heart_size_width
];

void heart_frame_set(
  char**,
  unsigned char
);

#endif
