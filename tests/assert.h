#ifndef ROOTKIT_ASSERT_H
# define ROOTKIT_ASSERT_H

# include "../src/log.h"

#define assert(Cond) __assert(Cond, __func__)

#define __assert(Cond, Name) \
    pr_log("test %s %s\n", Name, Cond ? "PASS" : "FAIL");

#endif /* !ROOTKIT_ASSERT_H */
