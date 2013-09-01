/*
 * Copyright 2013, winocm. <winocm@icloud.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 *   If you are going to use this software in any form that does not involve
 *   releasing the source to this project or improving it, let me know beforehand.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "genboot.h"
#include <stdarg.h>

extern void _semihost_write_char(char c);

static int inited_printf = 1;

/**
 * debug_putc
 *
 * Put a character to the console.
 */
void debug_putchar(int c)
{
    if (!inited_printf)
        return;

    if (c == '\n')
        debug_putchar('\r');

    _semihost_write_char(c);
}

/**
 * debug_getc
 *
 * Get a character from the console.
 */
int debug_getchar(void)
{
    return 'X';
}

static void putc_wrapper(void *p, char c)
{
    debug_putchar(c);
}

/**
 * init_debug
 *
 * Start debugging subsystems.
 */
void init_debug(void)
{
    init_printf(NULL, putc_wrapper);
    printf("debug_init()\n");
}

void __assert_func(const char *file, int line, const char *method,
                   const char *expression)
{
    panic("Assertion failed in file %s, line %d. (%s)\n",
          file, line, expression);
    while (1) ;
}

/**
 * panic
 *
 * Halt the system and explain why.
 */
#undef panic
void panic(const char *panicStr, ...)
{
    void *caller = __builtin_return_address(0);

    /* Prologue */
    printf("panic(caller 0x%08x): ", caller);

    /* Epilogue */
    va_list valist;
    va_start(valist, panicStr);
    vprintf((char *)panicStr, valist);
    va_end(valist);

    /* We are hanging here. */
    printf("\npanic: we are hanging here...\n");

    /* Halt */
    _locore_halt_system();
}
