#ifndef __space_invaders_parameters_h
#define __space_invaders_parameters_h

enum space_invaders_parameter {
  space_invaders_parameter_none        = 0b00000000,
  space_invaders_parameter_fill_screen = 0b00000001,
  space_invaders_parameter_skip_intro  = 0b00000010,
  space_invaders_parameter_error       = 0b11111111
};

unsigned char space_invaders_parameters_parse(
  unsigned int length_parameters,
  char** parameters
);

#endif
