#include "userland.h"
#include "assert.h"
#include "../src/debug_reg.h"
#include "../src/idt.h"
#include <linux/kernel.h>

static int hcnt = 0;
static int fcnt = 0;

static noinline void debug_reg_test_fct(void)
{
    ++fcnt;
}

static noinline void my_dr_hook(struct pt_regs *regs, long err)
{
    ++hcnt;
}

#include <linux/delay.h>
void test_debug_reg(void)
{
    int bp;

#if 1
    /* Inline hook do_debug */
    hcnt = fcnt = 0;
    debug_register_hijack_handler(INLINE_HOOK);
    bp = debug_register_add_bp(debug_reg_test_fct, my_dr_hook);
    debug_register_enable_bp(bp);
    debug_reg_test_fct();
    assert(hcnt == 1 && fcnt == 1, "debug register with inline hook");
    debug_register_del_bp(bp);
    debug_register_unhijack_handler();
    /* Remove hook */
    hcnt = fcnt = 0;
    debug_reg_test_fct();
    assert(hcnt == 0 && fcnt == 1, "debug register remove inline hook");
#endif

#if 0
    /* IDT TABLE hijack 0x1 */
    hcnt = fcnt = 0;
    debug_register_hijack_handler(IDT_TABLE);
    bp = debug_register_add_bp(debug_reg_test_fct, my_dr_hook);
    debug_register_enable_bp(bp);
    mdelay(2000);
    debug_reg_test_fct();
    assert(hcnt == 1 && fcnt == 1, "debug register with IDT table hijacking");
    debug_register_del_bp(bp);
    debug_register_unhijack_handler();
    /* Remove hook */
    hcnt = fcnt = 0;
    debug_reg_test_fct();
    assert(hcnt == 0 && fcnt == 1, "debug register restore IDT table");
#endif

#if 0
    /* IDT ENTRY hijack 0x1 */
    hcnt = fcnt = 0;
    debug_register_hijack_handler(IDT_ENTRY);
    bp = debug_register_add_bp(debug_reg_test_fct, my_dr_hook);
    debug_register_enable_bp(bp);
    debug_reg_test_fct();
    assert(hcnt == 1 && fcnt == 1, "debug register with IDT entry hijacking");
    debug_register_del_bp(bp);
    debug_register_unhijack_handler();
    /* Remove hook */
    hcnt = fcnt = 0;
    debug_reg_test_fct();
    assert(hcnt == 0 && fcnt == 1, "debug register restore IDT entry");
#endif
}
