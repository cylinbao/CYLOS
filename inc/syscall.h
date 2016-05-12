#ifndef USR_SYSCALL_H
#define USR_SYSCALL_H
#include <inc/types.h>

/* system call numbers */
enum {
	SYS_puts = 0,
	SYS_getc, // 1
	SYS_getpid, // 2
	SYS_fork, // 3
	SYS_kill, // 4
	SYS_sleep, // 5
  SYS_get_num_used_page, // 6
  SYS_get_num_free_page, // 7
  SYS_get_ticks, // 8
  SYS_settextcolor, // 9
  SYS_cls, // 10
	SYS_test, // 11

	NSYSCALLS
};

void puts(const char *s, size_t len);

int getc(void);

int32_t getpid(void);

int32_t fork(void);

void kill_self();

void sleep(uint32_t ticks);

int32_t get_num_used_page(void);

int32_t get_num_free_page(void);

unsigned long get_ticks(void);

void settextcolor(unsigned char forecolor, unsigned char backcolor);

int32_t cls(void);

int32_t test(void);
#endif
