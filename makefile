name=space_invaders

directory_include=include
directory_objects=objects
directory_output=output
directory_sources=sources

version_target_cer0=0
version_target_cexil=0
version_target_clic3=0
version_target_interrupt_handler=0
version_target_math_c=0
version_target_rand=0

directory_cer0=../cer0
directory_cer0_include=${directory_cer0}
directory_cer0_library=${directory_cer0}/library/macos/release

directory_cexil=../cexil
directory_cexil_include=${directory_cexil}/include
directory_cexil_library=${directory_cexil}/library/macos/release

directory_clic3=../clic3
directory_clic3_include=${directory_clic3}/include
directory_clic3_library=${directory_clic3}/library/macos/release

directory_interrupt_handler=../interrupt_handler
directory_interrupt_handler_include=${directory_interrupt_handler}/include
directory_interrupt_handler_library=${directory_interrupt_handler}/library/macos/release

directory_math_c=../math_c
directory_math_c_include=${directory_math_c}/include
directory_math_c_library=${directory_math_c}/library/macos/release

directory_rand=../rand
directory_rand_include=${directory_rand}/include
directory_rand_library=${directory_rand}/library/macos/release

file_output=${directory_output}/${name}
file_output_debug=${directory_output}/${name}_debug

file_library_cer0=${directory_cer0_library}/cer0.${version_target_cer0}.dylib
file_library_cexil=${directory_cexil_library}/cexil.${version_target_cexil}.dylib
file_library_clic3=${directory_clic3_library}/clic3.${version_target_clic3}.dylib
file_library_interrupt_handler=${directory_interrupt_handler_library}/interrupt_handler.${version_target_interrupt_handler}.dylib
file_library_math_c=${directory_math_c_library}/math_c.${version_target_math_c}.dylib
file_library_rand=${directory_rand_library}/rand.${version_target_rand}.dylib

files_sources=${wildcard ${directory_sources}/*.c}
files_objects=${patsubst ${directory_sources}/%.c, ${directory_objects}/%.o, ${files_sources}}
files_objects_debug=${addprefix __debug/,${patsubst ${directory_sources}/%.c, ${directory_objects}/%.debug.o, ${files_sources}}}

ifndef target_device_version
target_device_version=26.1
endif

target_platform=arm64-apple-macos${target_device_version}

directory_sdk=${shell xcrun --sdk macosx${target_device_version} --show-sdk-path}

cc=clang
c_flags_platform=-target ${target_platform} -isysroot ${directory_sdk}
c_flags_includes=-I${directory_include} -I${directory_cer0_include} -I${directory_cexil_include} -I${directory_interrupt_handler_include} -I${directory_clic3_include} -I${directory_math_c_include} -I${directory_rand_include}
c_flags=-O3 ${c_flags_platform} ${c_flags_includes}
c_flags_debug=-O0 -g -v ${c_flags_includes}

${name}: ${file_output}

debug: ${file_output_debug}

run:
	./${file_output}

${file_output}: ${files_objects} ${directory_output}
	${cc} ${c_flags} ${files_objects} ${file_library_cer0} ${file_library_cexil} ${file_library_clic3} ${file_library_interrupt_handler} ${file_library_math_c} ${file_library_rand} -o $@

${file_output_debug}: ${files_objects_debug} ${directory_output}
	${cc} ${c_flags_debug} ${patsubst __debug/%,%,${files_objects_debug}} ${file_library_cexil} ${file_library_clic3} ${file_library_interrupt_handler} -o $@

${directory_objects}/%.o: ${directory_sources}/%.c ${directory_objects}
	${cc} ${c_flags} -c $< -o $@

__debug/${directory_objects}/%.debug.o: ${directory_sources}/%.c ${directory_objects}
	${cc} ${c_flags_debug} -c $< -o ${patsubst __debug/%,%,$@}

${directory_output}:
	mkdir -p ${directory_output}

${directory_objects}:
	mkdir -p ${directory_objects}

clean: clean_objects clean_output

clean_objects:
	-rm -r ${directory_objects} 2> /dev/null

clean_output:
	-rm -r ${directory_output} 2> /dev/null
