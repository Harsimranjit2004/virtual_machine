# 🧠 Minimal Virtual Machine (VM)

A small **stack-based virtual machine** written in C with:

- **Assembler** (`vbasm`)
- **Interpreter** (`vmi`)
- **Disassembler** (`dbasm`)
- Clean **modular code structure**

---

## 📁 Folder Structure

VM/
├─ src/
│ ├─ vm.h → Core VM API (header)
│ ├─ vm.c → VM implementation
│ ├─ vmi.c → Interpreter (runs bytecode)
│ ├─ vbasm.c → Assembler (text → bytecode)
│ ├─ dbasm.c → Disassembler (bytecode → text)
│ ├─ fib.vm → Sample VM source file
│ └─ makefile

yaml
Copy code

---

## ⚙️ Build

```bash
make
This creates 3 executables:

vbasm → assembler

vmi → VM interpreter

dbasm → disassembler

🚀 Usage
1️⃣ Assemble source → bytecode
bash
Copy code
./vbasm src/fib.vm fib.out
2️⃣ Run bytecode
bash
Copy code
./vmi fib.out
3️⃣ Disassemble binary
bash
Copy code
./dbasm fib.out
🧾 Example VM Source (fib.vm)
vm
Copy code
push 0
push 1
dup 1
plus
jmp 2
📌 Notes
Stack-based architecture

Bytecode is written/read as raw binary

Easy to extend with new instructions

Perfect for learning interpreters & compilers
```
