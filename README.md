# RISC-V Multithreaded Kernel (OS1 Project)

## Overview

This project is a **minimal multithreaded operating system kernel** developed as part of the **Operating Systems 1** course at the **University of Belgrade – School of Electrical Engineering**.

The kernel is implemented for the **RISC-V RV64IMA architecture** and runs in a simulated environment. It provides core OS abstractions such as **threads, semaphores, memory allocation, preemptive scheduling, and console I/O**, while sharing a single address space with user programs (embedded-system style design).

---

## Key Features

- **Multithreading**
  - Kernel-managed lightweight threads
  - Context switching and scheduling
  - Preemptive execution using timer interrupts

- **Memory Management**
  - Dynamic heap allocator (`mem_alloc`, `mem_free`)
  - Block-based allocation with alignment

- **Synchronization**
  - Semaphores with:
    - `wait`, `signal`
    - `tryWait`
    - `timedWait`

- **Time Management**
  - Thread sleeping (`time_sleep`)
  - Periodic threads support

- **Console I/O**
  - Blocking `getc`
  - Non-blocking `putc`
  - Interrupt-driven UART communication

- **System Call Interface**
  - ABI layer using `ecall`
  - C API wrappers
  - Object-oriented C++ API

---

## Architecture

The system is structured in **four layers**, all statically linked into a single executable:

```
User Application (app.lib)
        ↑
C++ API (Thread, Semaphore, Console)
        ↑
C API (syscalls)
        ↑
ABI (ecall-based system calls)
        ↑
Kernel
        ↑
Hardware Abstraction Layer (hw.lib)
```

- Kernel and hardware access run in **supervisor mode**
- User code runs in **user mode**
- All components share a **single address space**

---

## Supported APIs

### C API
Implemented system calls include:
- Memory management
- Thread creation, dispatch, and termination
- Semaphores
- Time sleep
- Console input/output

### C++ API
Object-oriented wrappers:
- `Thread`
- `Semaphore`
- `PeriodicThread`
- `Console`

Global `new` and `delete` operators are redirected to kernel memory allocation.

---

## Platform & Environment

- **Architecture:** RISC-V RV64IMA
- **Execution:** QEMU emulator
- **Host OS:** Linux (via provided virtual machine)
- **Build System:** `make`
- **Language:** C++ with RISC-V assembly (`.S`)

No standard C/C++ libraries are used—everything is implemented from scratch.

---

## Building and Running

### Prerequisites
- Provided virtual machine (course distribution)
- QEMU RISC-V emulator
- GNU toolchain (already configured in VM)

### Build & Run
```bash
make qemu
```

### Debug Mode
```bash
make qemu-gdb
```

### Clean Build
```bash
make clean
```

The final executable is named `kernel`.

---

## Project Structure

```
.
├── src/        # Kernel source code (C++ and assembly)
├── h/          # Header files
├── lib/        # Provided static libraries (hw.lib, mem.lib, console.lib)
├── Makefile
└── README.md
```

---

## Educational Goals

This project demonstrates:
- Low-level OS development
- Interrupt and exception handling
- Context switching on RISC-V
- Synchronization primitives
- Embedded-style kernel design

---

## Notes

- The kernel terminates the emulator by writing `0x5555` to address `0x100000`
- Floating-point operations are intentionally avoided
- The system is **single-core**, but fully preemptive

---

## Author

**Matija Jovanovic**  
Student, University of Belgrade  
School of Electrical Engineering  
Course: Operating Systems 1
