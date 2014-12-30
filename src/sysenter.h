#ifndef ROOTKIT_SYSENTER_H
# define ROOTKIT_SYSENTER_H

void sysenter_entry_hook(void *hook);
void sysenter_entry_restore(void);

#endif /* !ROOTKIT_SYSENTER_H */
