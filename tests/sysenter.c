#include "userland.h"
#include "assert.h"
#include "../src/sysenter.h"
#include <linux/kernel.h>

static int cnt = 0;

noinline void my_sysenter_hook(struct pt_regs *regs)
{
    ++cnt;
}

void test_sysenter(void)
{
    char *argv[2] = { "/root/rk/test_sysenter", NULL };

    sysenter_entry_hook(my_sysenter_hook);
    cnt = 0;
    user_land_exec(argv);
    assert(cnt != 0, "sysenter entry hijacked");

    cnt = 0;
    sysenter_entry_restore();
    user_land_exec(argv);
    assert(cnt == 0, "sysenter entry restored");
}
