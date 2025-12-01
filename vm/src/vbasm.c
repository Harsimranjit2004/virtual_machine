// src/vbasm.c
#include "vm.h"
char *shift(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}

int main(int argc, char **argv)
{
    char *program = shift(&argc, &argv); // skip executable name
    if (argc == 0)
    {
        fprintf(stderr, "Usage: %s <source.vm> <output.bin>\n", program);
        exit(1);
    }
    const char *input_file_path = shift(&argc, &argv);
    if (argc == 0)
    {
        fprintf(stderr, "Usage: %s <source.vm> <output.bin>\n", program);
        exit(1);
    }
    const char *output_file_path = shift(&argc, &argv);

    // const char *input_file_path = argv[1];
    // const char *output_file_path = argv[2];

    Vm vm = {0};
    String_View source = slurp_file(input_file_path);

    vm.program_size =
        (Word)vm_translate_source(source, vm.program, VM_PROGRAM_CAPACITY);

    vm_save_program_to_file(&vm, output_file_path);

    return 0;
}
