#include "log.h"
#include "memory.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/slab.h>

#define IDT_SZ (sizeof (struct gate_struct64) * 256)

static struct gate_struct64 *old_idt_table;
static struct gate_struct64 new_idt_table[IDT_SZ];
static struct gate_struct64 *cur_idt_table;
static size_t idt_size;

unsigned long idt_get_entry(int n)
{
    unsigned long addr = (((unsigned long)cur_idt_table[n].offset_high) << 32)
         + (((unsigned long)cur_idt_table[n].offset_middle) << 16)
         + (((unsigned long)cur_idt_table[n].offset_low));
    return addr;
}

/* Warning old_idt_table is RO, use substitute() first or change permissions */
void idt_set_entry(unsigned long addr, int n)
{
    if (cur_idt_table == old_idt_table)
        set_addr_rw(old_idt_table);
    cur_idt_table[n].offset_high = (addr >> 32) & 0xffffffff;
    cur_idt_table[n].offset_middle = (addr >> 16) & 0xffff;
    cur_idt_table[n].offset_low = addr & 0xffff;
    if (cur_idt_table == old_idt_table)
        set_addr_ro(old_idt_table);
}

static void local_store_idt(void *dtr)
{
    asm volatile("sidt %0":"=m" (*((struct desc_ptr *)dtr)));
}

static void local_load_idt(void *dtr)
{
    asm volatile("lidt %0"::"m" (*((struct desc_ptr *)dtr)));
}

void idt_init(void)
{
    struct desc_ptr idtr;

    if (cur_idt_table)
        return;
    on_each_cpu(local_store_idt, &idtr, 1);
    old_idt_table = (struct gate_struct64 *)idtr.address;
    idt_size = idtr.size;
    cur_idt_table = old_idt_table;
}

void idt_substitute(void)
{
    struct desc_ptr idtr;

    memcpy(new_idt_table, cur_idt_table, IDT_SZ);
    idtr.address = (unsigned long)new_idt_table;
    idtr.size = idt_size;
    on_each_cpu(local_load_idt, &idtr, 1);
    cur_idt_table = new_idt_table;
}

void idt_restore(void)
{
    struct desc_ptr idtr;

    idtr.address = (unsigned long)old_idt_table;
    idtr.size = idt_size;
    on_each_cpu(local_load_idt, &idtr, 1);
    cur_idt_table = old_idt_table;
}

int idt_spoofed(void)
{
    return cur_idt_table == new_idt_table;
}
