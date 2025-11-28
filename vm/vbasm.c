#include "./vm.c"
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <source file> <output file>\n", argv[0]);
        exit(1);
    }
    const char *input_file_path = argv[1];
    const char *output_file_path = argv[2];
    String_View source_code = slurp_file(input_file_path);
    vm.program_size = vm_translate_source(source_code, vm.program, VM_PROGRAM_CAPACITY);
    vm_save_program_to_file(vm.program, vm.program_size, output_file_path);
}
