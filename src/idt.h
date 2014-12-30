#ifndef ROOTKIT_IDT_H
# define ROOTKIT_IDT_H

void idt_init(void);
unsigned long idt_get_entry(int n);
void idt_set_entry(unsigned long addr, int n);
void idt_substitute(void);
void idt_restore(void);
unsigned long idt_addr(void);

#endif /* !ROOTKIT_IDT_H */
