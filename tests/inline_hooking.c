#include "assert.h"
#include "../src/hook_inline.h"

static struct in_hook foo_hk;
static volatile int cnt;

static noinline int foo(int a)
{
    ++cnt;
    return a + 1;
}

static noinline int bar(int a)
{
    int ret;

    ++cnt;
    inline_hook_disable(&foo_hk);
    ret = foo(a + 41);
    inline_hook_enable(&foo_hk);
    return ret;
}

static void test_inline_hook(void)
{
    int ret;

    cnt = 0;
    inline_hook_enable(&foo_hk);
    ret = foo(0);
    assert(cnt == 2);
    assert(ret == 42);
    cnt = 0;
    inline_hook_disable(&foo_hk);
    ret = foo(0);
    assert(cnt == 1);
    assert(ret == 1);
}

void test_inline_hooking(void)
{
    inline_hook_init((unsigned long)foo, (unsigned long)bar, REL_JMP, &foo_hk);
    test_inline_hook();
    inline_hook_init((unsigned long)foo, (unsigned long)bar, PUSH_RET_32, &foo_hk);
    test_inline_hook();
    inline_hook_init((unsigned long)foo, (unsigned long)bar, PUSH_RET, &foo_hk);
    test_inline_hook();
    inline_hook_init((unsigned long)foo, (unsigned long)bar, RIP_REL, &foo_hk);
    test_inline_hook();
}
