#include "../assert.h"
#include "../userland.h"
#include "../../src/features/process_hiding.h"

void test_process_hiding(void)
{
    int ret;
    char *argv[] = {
        "/bin/sh", "-c", "/bin/ls -1 /proc | /bin/grep -q \"^1$\"", NULL
    };

    process_hide_init();

    process_hide("1");
    ret = user_land_exec(argv);
    assert(ret != 0, "hide 'init' process");

    process_unhide("1");
    ret = user_land_exec(argv);
    assert(ret == 0, "unhide 'init' process");

    process_unhide_all();
}
