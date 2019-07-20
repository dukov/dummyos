#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/isr.h>

void kernel_main(void) {
    terminal_initialize();
    isr_install();
    irq_install();
    asm volatile("sti");
    printf("Hello, kernel World!\n");
    asm("int $1");
    mm_init();
    for(;;) {
        asm("hlt");
    }
}
