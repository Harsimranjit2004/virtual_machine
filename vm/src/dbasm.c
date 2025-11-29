#define BM_IMPLEMENTATION
#include "./vm.h"

Vm vm = {0};
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <source.vm>\n", argv[0]);
        return 1;
    }
    const char *input_file_path = argv[1];
    vm_load_program_from_file(&vm, input_file_path);
    for (size_t i = 0; i < (size_t)vm.program_size; i++)
    {
        switch (vm.program[i].type)
        {
        case INST_NOP:
            printf("nop\n");
            break;
        case INST_PUSH:
            printf("push %lld\n", (long long)vm.program[i].operand);
            break;
        case INST_PLUS:
            printf("plus\n");
            break;
        case INST_DUP:
            printf("dup %lld\n", (long long)vm.program[i].operand);
            break;
        case INST_MINUS:
            printf("minus\n");
            break;
        case INST_MULT:
            printf("mult\n");
            break;
        case INST_DIV:
            printf("div\n");
            break;
        case INST_JMP:
            printf("jmp %lld\n", (long long)vm.program[i].operand);
            break;
        case INST_HALT:
            printf("halt\n");
            break;
        case INST_JMP_IF:
            printf("jmp_if %lld\n", (long long)vm.program[i].operand);
            break;
        case INST_EQ:
            printf("eq\n");
            break;
        case INST_PRINT_DEBUG:
            printf("print_debug\n");
            break;
        default:
            assert(0 && "unknown instruction");
            break;
        }
    }
    return 0;
}