#include <space_invaders_parameters.h>

#include <clic3_char_arrays.h>

#include <stdio.h>

unsigned char space_invaders_parameters_parse(
  unsigned int length_parameters,
  char** parameters
) {
  unsigned char space_invaders_parameters = (
    space_invaders_parameter_none
  );

  for (
    unsigned int index_parameter = (
      0x00
    );
    (
      index_parameter <
      length_parameters
    );
    ++index_parameter
  ) {
    switch (
      clic3_char_arrays_within(
        parameters[
          index_parameter
        ],
        0x04,
        "--fill_screen",
        "--fill-screen",
        "--skip_intro",
        "--skip-intro"
      )
    ) {
      case 0x00:
      case 0x01: {
        space_invaders_parameters = (
          space_invaders_parameters |
          space_invaders_parameter_fill_screen
        );
      
        break;
      }
      case 0x02:
      case 0x03: {
        space_invaders_parameters = (
          space_invaders_parameters |
          space_invaders_parameter_skip_intro
        );
        
        break;
      }
      default: {
        fprintf(
          stderr,
          "unknown_parameter->{%s};\n",
          parameters[
            index_parameter
          ]
        );
        
        return (
          space_invaders_parameter_error
        );
      }
    }
  }
  
  return (
    space_invaders_parameters
  );
}
