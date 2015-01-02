#include "assert.h"
#include "../src/hook.h"
#include <linux/kernel.h>

create_hook_ro(epic);
create_hook_rw(epic2);
static volatile int cnt;

static noinline int foo(int a)
{
    ++cnt;
    return a + 1;
}

static noinline int bar(int a)
{
    ++cnt;
    return 41 + a;
}

static noinline int bar2(struct pt_regs *regs)
{
    ++cnt;
    regs->di = 41;
    return 1337;
}

int(*foo_ptr)(int) = foo;

void test_hook(void)
{
    int ret;
    struct my_hook hk = { HOOK_INIT(epic, &foo_ptr) };
    struct my_hook hk2 = { HOOK_INIT(epic2, &foo_ptr) };

    hook_config(epic, foo, bar);
    hook_config(epic2, foo, bar2);

    cnt = 0;
    thook_enable(hk);
    ret = foo_ptr(0);
    assert(cnt == 2, "hook RO is executed");
    assert(ret == 1, "hook RO does not modify values");

    cnt = 0;
    thook_disable(hk);
    ret = foo_ptr(0);
    assert(cnt == 1 && ret == 1, "hook RO is disabled");

    /* RW */
    cnt = 0;
    thook_enable(hk2);
    ret = foo_ptr(0);
    assert(cnt == 2, "hook RW is executed");
    assert(ret == 42, "hook RW does modify values");

    cnt = 0;
    thook_disable(hk2);
    ret = foo_ptr(0);
    assert(cnt == 1 && ret == 1, "hook RW is disabled");
}
