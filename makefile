name=space_invaders

directory_include=include
directory_objects=objects
directory_output=output
directory_sources=sources

file_output=${directory_output}/${name}
file_output_debug=${directory_output}/${name}_debug

directory_cexil=../cexil
directory_cexil_include=${directory_cexil}/include

directory_clic3=../clic3
directory_clic3_include=${directory_clic3}/include

directory_interrupt_handler=../interrupt_handler
directory_interrupt_handler_include=${directory_interrupt_handler}/include

file_library_cexil=${directory_cexil}/library/cexil.o
file_library_clic3=${directory_clic3}/library/clic3.o
file_library_interrupt_handler=${directory_interrupt_handler}/library/interrupt_handler.o

files_sources=${wildcard ${directory_sources}/*.c}
files_objects=${patsubst ${directory_sources}/%.c, ${directory_objects}/%.o, ${files_sources}}
files_objects_debug=${addprefix __debug/,${patsubst ${directory_sources}/%.c, ${directory_objects}/%.debug.o, ${files_sources}}}

cc=gcc
c_flags=-O3 -I${directory_include} -I${directory_cexil_include} -I${directory_interrupt_handler_include} -I${directory_clic3_include}
c_flags_debug=-O0 -g -v -da -Q -I${directory_include} -I${directory_cexil_include} -I${directory_interrupt_handler_include} -I${directory_clic3_include}

${name}: ${file_output}

debug: ${file_output_debug}

run:
	./${file_output}

${file_output}: ${files_objects} ${directory_output}
	${cc} ${c_flags} ${files_objects} ${file_library_cexil} ${file_library_clic3} ${file_library_interrupt_handler} -o $@

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

