// Copyright 2020 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifdef OPENOCD_SEMIHOSTING

#include <openocd.h>

#define PUTC_BUFFER_LEN 1024
static int putchar_index[SNRT_CLUSTER_NUM * SNRT_CLUSTER_CORE_NUM] = {0};
static char putchar_data[SNRT_CLUSTER_NUM * SNRT_CLUSTER_CORE_NUM]
                        [PUTC_BUFFER_LEN];

// Provide an implementation for putchar.
void _putchar(char character) {
    volatile char *buf = putchar_data[snrt_hartid()];
    buf[putchar_index[snrt_hartid()]++] = character;
    if (putchar_index[snrt_hartid()] == PUTC_BUFFER_LEN || character == '\n') {
        __ocd_semihost_write(1, (uint8_t *)buf, putchar_index[snrt_hartid()]);
        putchar_index[snrt_hartid()] = 0;
    }
}

#else
extern uintptr_t volatile tohost, fromhost;

// Rudimentary string buffer for putc calls.
extern uint32_t _edram;
#define PUTC_BUFFER_LEN (1024 - sizeof(size_t))
struct putc_buffer_header {
    size_t size;
    uint64_t syscall_mem[8];
};
static volatile struct putc_buffer {
    struct putc_buffer_header hdr;
    char data[PUTC_BUFFER_LEN];
} *const putc_buffer = (void *)&_edram;

// Provide an implementation for putchar.
void _putchar(char character) {
    volatile struct putc_buffer *buf = &putc_buffer[snrt_hartid()];
    buf->data[buf->hdr.size++] = character;
    if (buf->hdr.size == PUTC_BUFFER_LEN || character == '\n') {
        buf->hdr.syscall_mem[0] = 64;  // sys_write
        buf->hdr.syscall_mem[1] = 1;   // file descriptor (1 = stdout)
        buf->hdr.syscall_mem[2] = (uintptr_t)&buf->data;  // buffer
        buf->hdr.syscall_mem[3] = buf->hdr.size;          // length

        tohost = (uintptr_t)buf->hdr.syscall_mem;
        while (fromhost == 0)
            ;
        fromhost = 0;

        buf->hdr.size = 0;
    }
}

#endif