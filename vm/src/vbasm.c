// src/vbasm.c
#include "vm.h"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <source.vm> <output.bin>\n", argv[0]);
        return 1;
    }

    const char *input_file_path = argv[1];
    const char *output_file_path = argv[2];

    Vm vm = {0};
    String_View source = slurp_file(input_file_path);

    vm.program_size =
        (Word)vm_translate_source(source, vm.program, VM_PROGRAM_CAPACITY);

    vm_save_program_to_file(&vm, output_file_path);

    return 0;
}
