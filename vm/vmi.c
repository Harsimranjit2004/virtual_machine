#include "./vm.c"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
    }
    vm_load_program_from_file(&vm, argv[1]);
    for (int i = 0; i < VM_EXECUTION_LIMIT && !vm.halt; i++)
    {
        // printf("%s\n", inst_type_as_cstr(program[vm.ip].type));
        Trap trap = vm_execute_inst(&vm);
        vm_dump_stack(stdout, &vm);
        if (trap != TRAP_OK)
        {
            fprintf(stderr, "%s", trap_as_cstr(trap));
            exit(1);
        }
    }

    return 0;
}
