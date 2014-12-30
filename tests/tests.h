#ifndef ROOTKIT_TESTS_H
# define ROOTKIT_TESTS_H

void test_hook(void);
void test_syscall_tbl(void);
void test_idt_hijacking(void);
void test_inline_hooking(void);
void test_sysenter(void);

#endif /* !ROOTKIT_TESTS__H */
