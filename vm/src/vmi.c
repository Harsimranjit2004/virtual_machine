// src/vmi.c
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
        fprintf(stderr, "ERROR: %s <binary program>\n [-l <limit>]\n", program);
        return 1;
    }
    const char *input_file_path = shift(&argc, &argv);
    if (argc == 0)
    {
        fprintf(stderr, "ERROR: %s <binary program>\n [-l <limit>]\n", argv[0]);
        return 1;
    }
    int limit = 70;
    if (argc > 0)
    {
        if (strcmp(argv[0], "-l") == 0)
        {
            limit = atoi(argv[1]);
            shift(&argc, &argv);
        }
    }
    if (input_file_path == NULL)
    {
        fprintf(stderr, "ERROR: %s <binary program>\n [-l <limit>]\n", argv[0]);
        return 1;
    }

    Vm vm = {0};
    vm_load_program_from_file(&vm, argv[1]);
    Trap trap = vm_execute_program(&vm, limit);
    // for (int i = 0; i < VM_EXECUTION_LIMIT && !vm.halt; i++)
    // {
    //     Trap trap = vm_execute_inst(&vm);
    //     vm_dump_stack(stdout, &vm);

    //     if (trap != TRAP_OK)
    //     {
    //         fprintf(stderr, "TRAP: %s\n", trap_as_cstr(trap));
    //         return 1;
    //     }
    // }
    vm_dump_stack(stdout, &vm);
    if (trap != TRAP_OK)
    {
        fprintf(stderr, "TRAP: %s\n", trap_as_cstr(trap));
        return 1;
    }

    return 0;
}
