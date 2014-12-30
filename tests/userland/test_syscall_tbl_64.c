#include <unistd.h>

int main(void)
{
    write(1, "{schischi: test_syscall_tbl_64}\n", 32);
    return 0;
}
