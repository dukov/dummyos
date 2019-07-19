#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/isr.h>

void kernel_main(void) {
    terminal_initialize();
    isr_install();
    irq_install();
    asm volatile("sti");
    printf("Hello, kernel World!\n");
    asm("int $1");
    for(;;) {
        asm("hlt");
    }
}
