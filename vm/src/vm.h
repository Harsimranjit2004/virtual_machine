// src/vm.h
#ifndef VM_H_
#define VM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define VM_STACK_CAPACITY 1024
#define VM_PROGRAM_CAPACITY 1024
#define VM_EXECUTION_LIMIT 69

#define ARRAY_SIZE(xs) (sizeof(xs) / sizeof((xs)[0]))

typedef int64_t Word;

// ---------- Trap / Error codes ----------
typedef enum
{
    TRAP_OK,
    TRAP_STACK_OVERFLOW,
    TRAP_STACK_UNDERFLOW,
    TRAP_ILLEGAL_INST,
    TRAP_DIV_BY_ZERO,
    TRAP_ILLEGAL_INST_ACCESS,
    TRAP_ILLEGAL_OPERAND,
} Trap;

const char *trap_as_cstr(Trap trap);

// ---------- Instruction types ----------
typedef enum
{
    INST_NOP = 0,
    INST_PUSH,
    INST_PLUS,
    INST_DUP,
    INST_MINUS,
    INST_MULT,
    INST_DIV,
    INST_JMP,
    INST_HALT,
    INST_JMP_IF,
    INST_EQ,
    INST_PRINT_DEBUG,
} Inst_Type;

const char *inst_type_as_cstr(Inst_Type type);

typedef struct
{
    Inst_Type type;
    Word operand;
} Inst;

// ---------- VM State ----------
typedef struct
{
    Word stack[VM_STACK_CAPACITY];
    Word stack_size;

    Inst program[VM_PROGRAM_CAPACITY];
    Word program_size;

    Word ip; // instruction pointer
    int halt;
} Vm;

// ---------- VM API ----------
Trap vm_execute_inst(Vm *vm);
void vm_dump_stack(FILE *stream, Vm *vm);
void vm_load_program_from_memory(Vm *vm, Inst *program, size_t program_size);
void vm_load_program_from_file(Vm *vm, const char *file_path);
void vm_save_program_to_file(const Vm *vm,
                             const char *file_path);

// ---------- String_View & Assembler API ----------
typedef struct
{
    size_t count;
    const char *data;
} String_View;

String_View cstr_as_sv(const char *cstr);
String_View sv_trim_left(String_View sv);
String_View sv_trim_right(String_View sv);
String_View sv_trim(String_View sv);
String_View chop_by_delimiter(String_View *sv, char delimiter);
int sv_equals(String_View a, String_View b);
int sv_int(String_View sv);
String_View slurp_file(const char *file_path);

Inst vm_translate_line(String_View line);
size_t vm_translate_source(String_View source,
                           Inst *program, size_t program_capacity);

#endif // VM_H_
