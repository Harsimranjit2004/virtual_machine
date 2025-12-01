#include "vm.h"

char *shift(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}

void Usage(FILE *stream, const char *program)
{
    fprintf(stream,
            "Usage: %s [-i <input file>] <binary program> [-l <limit>]\n",
            program);
}

int main(int argc, char **argv)
{
    char *program = shift(&argc, &argv); // skip executable name

    const char *input_file_path = NULL;
    const char *binary_program = NULL; // <-- REQUIRED!
    int limit = -1;

    while (argc > 0)
    {
        const char *flag = shift(&argc, &argv);

        if (strcmp(flag, "-i") == 0)
        {
            if (argc == 0)
            {
                Usage(stderr, program);
                exit(1);
            }
            input_file_path = shift(&argc, &argv);
        }
        else if (strcmp(flag, "-l") == 0)
        {
            if (argc == 0)
            {
                Usage(stderr, program);
                exit(1);
            }
            limit = atoi(shift(&argc, &argv));
        }
        else if (strcmp(flag, "-h") == 0)
        {
            Usage(stdout, program);
            exit(0);
        }
        else // <-- THIS PART FIXES YOUR ERROR
        {
            // This must be the binary program (positional)
            binary_program = flag;
        }
    }

    // Check if program was provided
    if (binary_program == NULL)
    {
        Usage(stderr, program);
        exit(1);
    }

    // Now load and execute program
    Vm vm = {0};
    vm_load_program_from_file(&vm, binary_program);

    Trap trap = vm_execute_program(&vm, limit);
    vm_dump_stack(stdout, &vm);

    if (trap != TRAP_OK)
    {
        fprintf(stderr, "TRAP: %s\n", trap_as_cstr(trap));
        return 1;
    }

    return 0;
}
