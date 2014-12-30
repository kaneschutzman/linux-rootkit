#include "userland.h"
#include "assert.h"
#include "../src/syscall_tbl.h"
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

size_t(*orig_write)(int, const void *, size_t);
int cnt = 0;

noinline ssize_t my_write(int fd, const void *buf, size_t n)
{
    if (!strncmp(buf, "{schischi: test_syscall_tbl_32}\n", n))
        ++cnt;
    if (!strncmp(buf, "{schischi: test_syscall_tbl_64}\n", n))
        ++cnt;
    return orig_write(fd, buf, n);
}

void test_syscall_tbl(void)
{
    char *argv[2] = { NULL };
    int nr_write = 1;
    int ia32_nr_write = 4;
    /* x86_64 */
    syscall_tbl_fetch();
    orig_write = (void *)syscall_tbl_orig_entry(nr_write);
    syscall_tbl_set_entry(nr_write, my_write);
    cnt = 0;
    argv[0] = "/root/rk/test_syscall_tbl_64";
    user_land_exec(argv);
    assert(cnt != 0);
    syscall_tbl_restore_entry(nr_write);

    /* i386 */
    ia32_syscall_tbl_fetch();
    ia32_syscall_tbl_set_entry(ia32_nr_write, my_write);
    cnt = 0;
    argv[0] = "/root/rk/test_syscall_tbl_32";
    user_land_exec(argv);
    assert(cnt != 0);
    ia32_syscall_tbl_restore_entry(ia32_nr_write);
}
