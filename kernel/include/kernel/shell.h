#ifndef SHELL_H
#define SHELL_H

void shell_end();
/*void shell_page();
void shell_read_all_sectors();
void shell_load_ramfs();
void shell_read_pt();*/
void shell_malloc();
void shell_run_command(char *input);

typedef void (*shell_function_t)(void);
#endif
