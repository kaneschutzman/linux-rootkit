#include "assert.h"
#include "userland.h"
#include "../src/idt.h"
#include "../src/hook.h"
#include "../src/memory.h"
#include <linux/kernel.h>

static int cnt = 0;
create_hook_ro(my_hk);

noinline void foo(void)
{
    ++cnt;
}

void test_idt_hijacking(void)
{
    char *argv[2] = { NULL };

    idt_init();

    /* Change IDT entry 0x80 */
    orig_my_hk = idt_get_entry(0x80);
    hook_my_hk = (unsigned long)foo;
    idt_set_entry((unsigned long)my_hk, 0x80);

    cnt = 0;
    argv[0] = "/root/rk/test_syscall_tbl_32";
    user_land_exec(argv);
    assert(cnt != 0, "IDT entry 0x80 executed");
    idt_set_entry(orig_my_hk, 0x80);

    /* Completly change the table */
    idt_substitute();
    idt_set_entry((unsigned long)my_hk, 0x80);
    cnt = 0;
    argv[0] = "/root/rk/test_syscall_tbl_32";
    user_land_exec(argv);
    assert(cnt != 0, "IDT table changed");
    idt_restore();
}
