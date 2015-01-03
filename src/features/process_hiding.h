#ifndef ROOTKIT_PROCESS_HIDING_H
# define ROOTKIT_PROCESS_HIDING_H

int process_hide_init(void);
void process_hide(const char *pid);
void process_unhide(const char *pid);
void process_unhide_all(void);

#endif /* !ROOTKIT_PROCESS_HIDING_H */
