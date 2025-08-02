project_name=space_invaders

include_directory=include
objects_directory=objects
output_directory=output
sources_directory=sources

out_file=${output_directory}/${project_name}

cexil_directory=../cexil
cexil_include_directory=${cexil_directory}/include
cexil_object_file=${cexil_directory}/library/cexil.o

clic3_directory=../clic3
clic3_include_directory=${clic3_directory}/include
clic3_object_file=${clic3_directory}/library/clic3.o

interrupt_handler_directory=../interrupt_handler
interrupt_handler_include_directory=${interrupt_handler_directory}/include
interrupt_handler_object_file=${interrupt_handler_directory}/library/interrupt_handler.o

source_files=${wildcard ${sources_directory}/*.c}
object_files=${patsubst ${sources_directory}/%.c, ${objects_directory}/%.o, ${source_files}}

cc=gcc
c_flags=-O3 -I${include_directory} -I${cexil_include_directory} -I${interrupt_handler_include_directory} -I${clic3_include_directory}

${name}: ${out_file}

${out_file}: ${object_files} ${output_directory}
	${cc} ${c_flags} ${object_files} ${cexil_object_file} ${clic3_object_file} ${interrupt_handler_object_file} -o $@

${objects_directory}/%.o: ${sources_directory}/%.c ${objects_directory}
	${cc} ${c_flags} -c $< -o $@

${output_directory}:
	mkdir -p ${output_directory}

${objects_directory}:
	mkdir -p ${objects_directory}

clean:
	-rm ${out_file}
	-rm ${objects_directory}/*.o 2> /dev/null

