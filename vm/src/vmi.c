// src/vmi.c
#include "vm.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <binary program>\n", argv[0]);
        return 1;
    }

    Vm vm = {0};
    vm_load_program_from_file(&vm, argv[1]);

    for (int i = 0; i < VM_EXECUTION_LIMIT && !vm.halt; i++)
    {
        Trap trap = vm_execute_inst(&vm);
        vm_dump_stack(stdout, &vm);

        if (trap != TRAP_OK)
        {
            fprintf(stderr, "TRAP: %s\n", trap_as_cstr(trap));
            return 1;
        }
    }

    return 0;
}
