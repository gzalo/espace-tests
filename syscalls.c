#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

#define UART1_BASE        0x08088400
#define UART_THR_OFFSET   0x00      // Transmit Holding Register
#define UART_LSR_OFFSET   0x14      // Line Status Register
#define UART_LSR_THRE     0x20      // Transmit Holding Register Empty

volatile uint32_t* const UART1_THR = (volatile uint32_t*)(UART1_BASE + UART_THR_OFFSET);
volatile uint32_t* const UART1_LSR = (volatile uint32_t*)(UART1_BASE + UART_LSR_OFFSET);

char *__env[1] = { 0 };
char **environ = __env;

void uart_putc(char c) {
    while (!(*UART1_LSR & UART_LSR_THRE));
    *UART1_THR = c;
}

int _wait(int *status) {
	errno = ECHILD;
	return -1;
}

int _unlink(char *name) {
	errno = ENOENT;
	return -1;
}

int _times(struct tms *buf) {
	return -1;
}

int _link(char *old, char *new) {
	errno = EMLINK;
	return -1;
}

int _fork(void) {
	errno = EAGAIN;
	return -1;
}

int _execve(char *name, char **argv, char **env) {
	errno = ENOMEM;
	return -1;
}

int _getpid(void) {
	return 1;
}

int _open(char *path, int flags, ...) {
	return -1;
}

int _kill(int pid, int sig) {
	errno = EINVAL;
	return -1;
}

int _isatty(int file) {
	return 1;
}

int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int _read(int file, char *ptr, int len) {
	return -1;
}

int _close(int file) {
	return -1;
}

int _write(int file, char *ptr, int len) {
	int idx;
	for (idx = 0; idx < len; idx++)
	{
		uart_putc(*ptr++);
	}
	return len;
}

void _exit (int status) {
	while (1) {}
}

int _lseek(int file, int ptr, int dir) {
	return 0;
}

extern uint32_t __end__;
void print_hex(uint32_t value);
void uart_puts(const char* str);
unsigned char *heap = NULL;

void *_sbrk(int incr) {
  unsigned char *prev_heap;
  if (heap == NULL) {
    heap = (unsigned char *)&__end__;
  }
  prev_heap = heap;

  heap += incr;

  return prev_heap;
}

int main(void);
void __libc_init_array(void);
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));

void __libc_init_array (void) {
  size_t count;
  size_t i;

  count = __preinit_array_end - __preinit_array_start;
  for (i = 0; i < count; i++)
    __preinit_array_start[i] ();

  count = __init_array_end - __init_array_start;
  for (i = 0; i < count; i++)
    __init_array_start[i] ();
}


void uart_puts(const char* str) {
    while (*str) {
        uart_putc(*str++);
    }
}

__attribute__((noreturn)) void panic(const char* message) {
    uart_puts("PANIC: ");
    uart_puts(message);
    uart_puts("\n");
    while (1) {}
}

void undefined_handler(void) { panic("UNDEFINED"); }
void swi_handler(void) { panic("SWI"); }
void prefetch_handler(void) { panic("PREFETCH"); }
void data_handler(void) { panic("Data abort"); }
void unused_handler(void) { panic("UNUSED"); }
void irq_handler(void) { panic("Unhandled IRQ"); }
void fiq_handler(void) { panic("Unhandled FIQ"); }

void set_vbar(uint32_t address) {
    __asm volatile ("mcr p15, 0, %0, c12, c0, 0" : : "r" (address) :);
}

extern uint32_t _vector_table;

void reset(void){
    uint32_t size = (uint32_t)&__bss_end__ - (uint32_t)&__bss_start__;
	uint8_t *dst = (uint8_t*)&__bss_start__;
	for(int i =0; i < size ; i++){
		*dst++ = 0;
	}
	__libc_init_array();
	set_vbar((uint32_t)&_vector_table);
	main();
}
