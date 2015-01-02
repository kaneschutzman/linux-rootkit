#include "log.h"
#include <linux/module.h>
#include <linux/kernel.h>

#include "sysenter.h"
#include "hook.h"
#include <asm/percpu.h>

# define SYSENTER_STACK_SZ 512
# define SYSENTER_STACK_TOP "511"

DEFINE_PER_CPU(char[SYSENTER_STACK_SZ], stack_sysenter);
static unsigned long orig_sysenter;
static unsigned long hook_sysenter;

extern asmlinkage void sysenter_fake_dispatcher(void);
void __unused_sysenter_hook(void)
{
    asm volatile (
        ".globl sysenter_fake_dispatcher\n"
        ".align 8, 0x90\n"
        "sysenter_fake_dispatcher:\n"
        "swapgs\n"
        "mov $stack_sysenter + ("SYSENTER_STACK_TOP"), %%rsp\n"
        "add %%gs:this_cpu_off, %%rsp\n"
        SAVE_REGS
        "call *%1\n"
        RESTORE_REGS
        "jmp *%0\n"
        :
        : "m"(orig_sysenter),
          "m"(hook_sysenter));
}

static void local_wrmsrl_sysenter_eip(void *data)
{
    wrmsrl(MSR_IA32_SYSENTER_EIP, (unsigned long)data);
}

void sysenter_entry_hook(void *hook)
{
    unsigned long ip;

    rdmsrl(MSR_IA32_SYSENTER_EIP, ip);
    /* We need to skip the swapgs instruction */
    BUG_ON((*((unsigned long *)ip) & 0xffffff) != 0xf8010f);
    orig_sysenter = ip + 3;
    hook_sysenter = (unsigned long)hook;

    on_each_cpu(local_wrmsrl_sysenter_eip, (void*)sysenter_fake_dispatcher, 1);
}

void sysenter_entry_restore(void)
{
    on_each_cpu(local_wrmsrl_sysenter_eip, (void*)(orig_sysenter - 3), 1);
}
