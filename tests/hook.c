#include "assert.h"
#include "../src/hook.h"
#include <linux/kernel.h>

create_hook_ro(epic);
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

int(*foo_ptr)(int) = foo;

void test_hook(void)
{
    int ret;
    struct my_hook hk = { HOOK_INIT(epic, &foo_ptr) };

    hook_config(epic, foo, bar);

    cnt = 0;
    thook_enable(hk);
    ret = foo_ptr(0);
    assert(cnt == 2, "hook RO is executed");
    assert(ret == 1, "hook RO does not modify values");

    cnt = 0;
    thook_disable(hk);
    ret = foo_ptr(0);
    assert(cnt == 1 && ret == 1, "hook RO is disabled");
}
