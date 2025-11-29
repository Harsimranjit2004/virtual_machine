// src/vm.c
#include "vm.h"

// ---------- Trap → string ----------
const char *trap_as_cstr(Trap trap)
{
    switch (trap)
    {
    case TRAP_OK:
        return "TRAP_OK";
    case TRAP_STACK_OVERFLOW:
        return "TRAP_STACK_OVERFLOW";
    case TRAP_STACK_UNDERFLOW:
        return "TRAP_STACK_UNDERFLOW";
    case TRAP_ILLEGAL_INST:
        return "TRAP_ILLEGAL_INST";
    case TRAP_DIV_BY_ZERO:
        return "TRAP_DIV_BY_ZERO";
    case TRAP_ILLEGAL_INST_ACCESS:
        return "TRAP_ILLEGAL_INST_ACCESS";
    case TRAP_ILLEGAL_OPERAND:
        return "TRAP_ILLEGAL_OPERAND";
    default:
        assert(0 && "trap_as_cstr: Unreachable");
    }
}

// ---------- Inst_Type → string ----------
const char *inst_type_as_cstr(Inst_Type type)
{
    switch (type)
    {
    case INST_NOP:
        return "INST_NOP";
    case INST_PUSH:
        return "INST_PUSH";
    case INST_PLUS:
        return "INST_PLUS";
    case INST_DUP:
        return "INST_DUP";
    case INST_MINUS:
        return "INST_MINUS";
    case INST_MULT:
        return "INST_MULT";
    case INST_DIV:
        return "INST_DIV";
    case INST_JMP:
        return "INST_JMP";
    case INST_HALT:
        return "INST_HALT";
    case INST_JMP_IF:
        return "INST_JMP_IF";
    case INST_EQ:
        return "INST_EQ";
    case INST_PRINT_DEBUG:
        return "INST_PRINT_DEBUG";
    default:
        assert(0 && "inst_type_as_cstr: unreachable");
        return "UNKNOWN_INST";
    }
}

// ---------- Execute a single instruction ----------
Trap vm_execute_inst(Vm *vm)
{
    if (vm->ip < 0 || vm->ip >= vm->program_size)
    {
        return TRAP_ILLEGAL_INST_ACCESS;
    }

    Inst inst = vm->program[vm->ip];

    switch (inst.type)
    {
    case INST_NOP:
        vm->ip += 1;
        break; // IMPORTANT: no fallthrough

    case INST_PUSH:
        if (vm->stack_size >= VM_STACK_CAPACITY)
        {
            return TRAP_STACK_OVERFLOW;
        }
        vm->stack[vm->stack_size++] = inst.operand;
        vm->ip += 1;
        break;

    case INST_PLUS:
        if (vm->stack_size < 2)
            return TRAP_STACK_UNDERFLOW;
        vm->stack[vm->stack_size - 2] += vm->stack[vm->stack_size - 1];
        vm->stack_size -= 1;
        vm->ip += 1;
        break;

    case INST_MINUS:
        if (vm->stack_size < 2)
            return TRAP_STACK_UNDERFLOW;
        vm->stack[vm->stack_size - 2] -= vm->stack[vm->stack_size - 1];
        vm->stack_size -= 1;
        vm->ip += 1;
        break;

    case INST_MULT:
        if (vm->stack_size < 2)
            return TRAP_STACK_UNDERFLOW;
        vm->stack[vm->stack_size - 2] *= vm->stack[vm->stack_size - 1];
        vm->stack_size -= 1;
        vm->ip += 1;
        break;

    case INST_DIV:
        if (vm->stack_size < 2)
            return TRAP_STACK_UNDERFLOW;
        if (vm->stack[vm->stack_size - 1] == 0)
            return TRAP_DIV_BY_ZERO;
        vm->stack[vm->stack_size - 2] /= vm->stack[vm->stack_size - 1];
        vm->stack_size -= 1;
        vm->ip += 1;
        break;

    case INST_JMP:
        vm->ip = inst.operand;
        break;

    case INST_HALT:
        vm->halt = 1;
        break;

    case INST_EQ:
        if (vm->stack_size < 2)
            return TRAP_STACK_UNDERFLOW;
        vm->stack[vm->stack_size - 2] =
            (vm->stack[vm->stack_size - 1] == vm->stack[vm->stack_size - 2]);
        vm->stack_size -= 1;
        vm->ip += 1;
        break;

    case INST_JMP_IF:
        if (vm->stack_size < 1)
            return TRAP_STACK_UNDERFLOW;
        if (vm->stack[vm->stack_size - 1])
        {
            vm->stack_size -= 1;
            vm->ip = inst.operand;
        }
        else
        {
            vm->ip += 1;
        }
        break;

    case INST_PRINT_DEBUG:
        if (vm->stack_size < 1)
            return TRAP_STACK_UNDERFLOW;
        printf("%lld\n", (long long)vm->stack[vm->stack_size - 1]);
        vm->stack_size -= 1;
        vm->ip += 1;
        break;

    case INST_DUP:
        if (inst.operand < 0)
            return TRAP_ILLEGAL_OPERAND;
        if (inst.operand >= vm->stack_size)
            return TRAP_STACK_UNDERFLOW;
        if (vm->stack_size >= VM_STACK_CAPACITY)
            return TRAP_STACK_OVERFLOW;

        vm->stack[vm->stack_size] =
            vm->stack[vm->stack_size - 1 - inst.operand]; // 0 = top
        vm->stack_size += 1;
        vm->ip += 1;
        break;

    default:
        return TRAP_ILLEGAL_INST;
    }

    return TRAP_OK;
}

// ---------- Debug stack ----------
void vm_dump_stack(FILE *stream, Vm *vm)
{
    fprintf(stream, "Stack:\n");
    if (vm->stack_size > 0)
    {
        for (Word i = 0; i < vm->stack_size; ++i)
        {
            fprintf(stream, "  %lld\n", (long long)vm->stack[i]);
        }
    }
    else
    {
        fprintf(stream, "  empty\n");
    }
}

// ---------- Program load/save ----------
void vm_load_program_from_memory(Vm *vm, Inst *program, size_t program_size)
{
    assert(program_size <= VM_PROGRAM_CAPACITY);
    memcpy(vm->program, program, sizeof(program[0]) * program_size);
    vm->program_size = (Word)program_size;
}

void vm_save_program_to_file(Inst *program, size_t program_size,
                             const char *file_path)
{
    FILE *f = fopen(file_path, "wb");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR: could not open file `%s`: %s\n",
                file_path, strerror(errno));
        exit(1);
    }
    fwrite(program, sizeof(program[0]), program_size, f);
    if (ferror(f))
    {
        fprintf(stderr, "ERROR: could not write to file `%s`: %s\n",
                file_path, strerror(errno));
        fclose(f);
        exit(1);
    }
    fclose(f);
}

void vm_load_program_from_file(Vm *vm, const char *file_path)
{
    FILE *f = fopen(file_path, "rb");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR: Could not open file `%s`: %s\n",
                file_path, strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_END) < 0)
    {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n",
                file_path, strerror(errno));
        fclose(f);
        exit(1);
    }

    long m = ftell(f);
    if (m < 0)
    {
        fprintf(stderr, "ERROR: Could not ftell file `%s`: %s\n",
                file_path, strerror(errno));
        fclose(f);
        exit(1);
    }

    assert(m % sizeof(vm->program[0]) == 0);
    assert((size_t)m <= VM_PROGRAM_CAPACITY * sizeof(vm->program[0]));

    if (fseek(f, 0, SEEK_SET) < 0)
    {
        fprintf(stderr, "ERROR: Could not rewind file `%s`: %s\n",
                file_path, strerror(errno));
        fclose(f);
        exit(1);
    }

    vm->program_size =
        (Word)fread(vm->program, sizeof(vm->program[0]),
                    (size_t)m / sizeof(vm->program[0]), f);

    if (ferror(f))
    {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n",
                file_path, strerror(errno));
        fclose(f);
        exit(1);
    }

    fclose(f);
}

// ---------- String_View helpers ----------
String_View cstr_as_sv(const char *cstr)
{
    return (String_View){.count = strlen(cstr), .data = cstr};
}

String_View sv_trim_left(String_View sv)
{
    size_t i = 0;
    while (i < sv.count && isspace((unsigned char)sv.data[i]))
    {
        i++;
    }
    return (String_View){.count = sv.count - i, .data = sv.data + i};
}

String_View sv_trim_right(String_View sv)
{
    size_t i = 0;
    while (i < sv.count &&
           isspace((unsigned char)sv.data[sv.count - i - 1]))
    {
        i++;
    }
    return (String_View){.count = sv.count - i, .data = sv.data};
}

String_View sv_trim(String_View sv)
{
    return sv_trim_left(sv_trim_right(sv));
}

String_View chop_by_delimiter(String_View *sv, char delimiter)
{
    size_t i = 0;
    while (i < sv->count && sv->data[i] != delimiter)
    {
        i++;
    }

    String_View result = {.count = i, .data = sv->data};

    if (i < sv->count)
    {
        sv->count -= i + 1;
        sv->data += i + 1;
    }
    else
    {
        sv->count -= i;
        sv->data += i;
    }

    return result;
}

int sv_equals(String_View sv1, String_View sv2)
{
    return sv1.count == sv2.count &&
           memcmp(sv1.data, sv2.data, sv1.count) == 0;
}

int sv_int(String_View sv)
{
    int result = 0;
    size_t i = 0;

    while (i < sv.count && isdigit((unsigned char)sv.data[i]))
    {
        result = result * 10 + (sv.data[i] - '0');
        i++;
    }

    return result;
}

// ---------- slurp_file ----------
String_View slurp_file(const char *file_path)
{
    FILE *f = fopen(file_path, "r");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR: Could not open file `%s`: %s\n",
                file_path, strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_END) < 0)
    {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n",
                file_path, strerror(errno));
        fclose(f);
        exit(1);
    }

    long m = ftell(f);
    if (m < 0)
    {
        fprintf(stderr, "ERROR: Could not ftell file `%s`: %s\n",
                file_path, strerror(errno));
        fclose(f);
        exit(1);
    }

    if (fseek(f, 0, SEEK_SET) < 0)
    {
        fprintf(stderr, "ERROR: Could not rewind file `%s`: %s\n",
                file_path, strerror(errno));
        fclose(f);
        exit(1);
    }

    char *buffer = malloc((size_t)m);
    if (buffer == NULL)
    {
        fprintf(stderr, "ERROR: Could not allocate memory for file `%s`\n",
                file_path);
        fclose(f);
        exit(1);
    }

    size_t read_count = fread(buffer, 1, (size_t)m, f);
    if (ferror(f))
    {
        fprintf(stderr, "ERROR: Could not read file `%s`: %s\n",
                file_path, strerror(errno));
        fclose(f);
        free(buffer);
        exit(1);
    }

    fclose(f);

    return (String_View){.count = read_count, .data = buffer};
}

// ---------- Assembler ----------
Inst vm_translate_line(String_View line)
{
    line = sv_trim_left(line);
    String_View inst_name = chop_by_delimiter(&line, ' ');

    if (sv_equals(inst_name, cstr_as_sv("push")))
    {
        line = sv_trim_left(line);
        int operand = sv_int(sv_trim_right(line));
        return (Inst){.type = INST_PUSH, .operand = operand};
    }
    else if (sv_equals(inst_name, cstr_as_sv("dup")))
    {
        line = sv_trim_left(line);
        int operand = sv_int(sv_trim_right(line));
        return (Inst){.type = INST_DUP, .operand = operand};
    }
    else if (sv_equals(inst_name, cstr_as_sv("plus")))
    {
        return (Inst){.type = INST_PLUS};
    }
    else if (sv_equals(inst_name, cstr_as_sv("jmp")))
    {
        line = sv_trim_left(line);
        int operand = sv_int(sv_trim_right(line));
        return (Inst){.type = INST_JMP, .operand = operand};
    }
    else
    {
        fprintf(stderr, "ERROR: unknown instruction `%.*s`\n",
                (int)inst_name.count, inst_name.data);
        exit(1);
    }
}

size_t vm_translate_source(String_View source,
                           Inst *program, size_t program_capacity)
{
    size_t program_size = 0;

    while (source.count > 0)
    {
        assert(program_size < program_capacity);
        String_View line = sv_trim(
            chop_by_delimiter(&source, '\n'));
        if (line.count > 0)
        {
            program[program_size++] = vm_translate_line(line);
        }
    }

    return program_size;
}
