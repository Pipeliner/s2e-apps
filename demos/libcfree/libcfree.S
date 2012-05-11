; https://blogs.oracle.com/ksplice/entry/hello_from_a_libc_free
.globl _start

_start:
    call main
    movl $1, %eax
    xorl %ebx, %ebx
    int $0x80
