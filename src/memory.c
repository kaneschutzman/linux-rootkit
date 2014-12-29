#include "memory.h"
#include "symbol.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <asm/cacheflush.h>
#include <linux/semaphore.h>
#include <linux/slab.h>

void set_addr_rw(unsigned long addr)
{
    pte_t *pte;
    unsigned int level;

    pte = lookup_address(addr, &level);
    if (pte->pte &~ _PAGE_RW)
        pte->pte |= _PAGE_RW;
}

void set_addr_ro(unsigned long addr)
{
    pte_t *pte;
    unsigned int level;

    pte = lookup_address(addr, &level);
    pte->pte = pte->pte &~_PAGE_RW;
}
