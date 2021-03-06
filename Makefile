TARGET = rootkit
obj-m := $(TARGET).o
#KDIR := /lib/modules/$(shell uname -r)/build
KDIR := ~/dev/vm/amd64/src/linux-3.18.1
#KDIR := ~/dev/vm/amd64/src/linux-3.17.6
PWD := $(shell pwd)
rootkit-objs := src/module.o              \
		src/syscall_tbl.o         \
		src/memory.o              \
		src/hook_inline.o         \
		src/symbol.o              \
		src/idt.o                 \
		src/sysenter.o            \
		src/debug_reg.o           \
		src/features/process_hiding.o \
		tests/inline_hooking.o    \
		tests/syscall_tbl.o       \
		tests/hook.o              \
		tests/userland.o          \
		tests/idt.o               \
		tests/debug_reg.o         \
		tests/sysenter.o          \
		tests/features/process_hiding.o

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
