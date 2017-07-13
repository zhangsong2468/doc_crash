#####################
Code of an executable
#####################


============
Introduction
============


Let us try to find out where does the code of our compiled executable is loaded when we start the process.


==========
Setting Up
==========

For this assignment we will write code in assembly language so that our code remains very small.

-   Our assembly code, we will loop printing ``Hello World`` 

::

    section .text
    global _start
    _start:                ; ELF entry point
    
    top:    
    mov rax, 1             ; sys_write
    mov rdi, 1             ; STDOUT
    mov rsi, message       ; buffer
    mov rdx, [messageLen]  ; length of buffer
    syscall
    
    jmp top 
    
    mov rax, 60            ; sys_exit
    mov rdi, 0             ; 0
    syscall
    
    section .data
    messageLen: dq message.end-message
    message: db 'Hello World', 10
    .end:



-   The objdump output of the code


::

    a.out:     file format elf64-x86-64
    Disassembly of section .text:
    
    00000000004000b0 <_start>:
    4000b0:   b8 01 00 00 00          mov    $0x1,%eax
    4000b5:   bf 01 00 00 00          mov    $0x1,%edi
    4000ba:   48 be e4 00 60 00 00    movabs $0x6000e4,%rsi
    4000c1:   00 00 00 
    4000c4:   48 8b 14 25 dc 00 60    mov    0x6000dc,%rdx
    4000cb:   00 
    4000cc:   0f 05                   syscall 
    4000ce:   eb e0                   jmp    4000b0 <_start>
    4000d0:   b8 3c 00 00 00          mov    $0x3c,%eax
    4000d5:   bf 00 00 00 00          mov    $0x0,%edi
    4000da:   0f 05                   syscall 
    
    Disassembly of section .data:
    
    00000000006000dc <messageLen>:
    6000dc:   0c 00                   or     $0x0,%al
    6000de:   00 00                   add    %al,(%rax)
    6000e0:   00 00                   add    %al,(%rax)
            ...
    
    00000000006000e4 <message>:
    6000e4:   48                      rex.W
    6000e5:   65                      gs
    6000e6:   6c                      insb   (%dx),%es:(%rdi)
    6000e7:   6c                      insb   (%dx),%es:(%rdi)
    6000e8:   6f                      outsl  %ds:(%rsi),(%dx)
    6000e9:   20 57 6f                and    %dl,0x6f(%rdi)
    6000ec:   72 6c                   jb     60015a <__bss_start+0x6a>
    6000ee:   64                      fs
    6000ef:   0a                      .byte 0xa

-   Compile the code with ``nasm -felf64 fileName.asm``

-   Link the code with ``ld fileName.o -o loopSleep``

-   We will run the process and search for it in ``crash``

::

    crash> ps | grep loop
      24256  15813   3  ffff880424da24c0  IN   0.0     100     16  loopHello

-   We will read the ``vm -p`` output

::

    crash> vm -p 24256
    PID: 24256  TASK: ffff880424da24c0  CPU: 3   COMMAND: "loopHello"
    MM               PGD          RSS    TOTAL_VM
    ffff88042433b580  ffff8803cbdbe000   16k      100k  

    VMA           START       END     FLAGS FILE
    ffff880424ecf368     400000     401000 8001875 /home/support/crash/loopHello
    VIRTUAL     PHYSICAL 
    400000      3b4c0e000
    
    VMA           START       END     FLAGS FILE
    ffff8803dcc54a98     600000     601000 8101877 /home/support/crash/loopHello
    VIRTUAL     PHYSICAL 
    600000      3b4c0e000
    
    VMA           START       END     FLAGS FILE
    ffff8804278f6368 7fffd818a000 7fffd81a0000 100177 
            VIRTUAL     PHYSICAL 
            7fffd818a000  (not mapped)
            7fffd818b000  (not mapped)
    7fffd818c000  (not mapped)

-   Read the RAM location

::

    crash> rd -p -32 3b4c0e000 100
    3b4c0e000:  464c457f 00010102 00000000 00000000   .ELF............
    3b4c0e010:  003e0002 00000001 004000b0 00000000   ..>.......@.....
    3b4c0e020:  00000040 00000000 00000118 00000000   @...............
    3b4c0e030:  00000000 00380040 00400002 00030006   ....@.8...@.....
    3b4c0e040:  00000001 00000005 00000000 00000000   ................
    3b4c0e050:  00400000 00000000 00400000 00000000   ..@.......@.....
    3b4c0e060:  000000dc 00000000 000000dc 00000000   ................
    3b4c0e070:  00200000 00000000 00000001 00000006   .. .............
    3b4c0e080:  000000dc 00000000 006000dc 00000000   ..........`.....
    3b4c0e090:  006000dc 00000000 00000014 00000000   ..`.............
    3b4c0e0a0:  00000014 00000000 00200000 00000000   .......... .....
    3b4c0e0b0:  000001b8 0001bf00 be480000 006000e4   ..........H...`.
    3b4c0e0c0:  00000000 25148b48 006000dc e0eb050f   ....H..%..`.....
    3b4c0e0d0:  00003cb8 0000bf00 050f0000 0000000c   .<..............
    3b4c0e0e0:  00000000 6c6c6548 6f57206f 0a646c72   ....Hello World.
    3b4c0e0f0:  79732e00 6261746d 74732e00 62617472   ..symtab..strtab
    3b4c0e100:  68732e00 74727473 2e006261 74786574   ..shstrtab..text
    3b4c0e110:  61642e00 00006174 00000000 00000000   ..data..........
    3b4c0e120:  00000000 00000000 00000000 00000000   ................
    3b4c0e130:  00000000 00000000 00000000 00000000   ................
    3b4c0e140:  00000000 00000000 00000000 00000000   ................
    3b4c0e150:  00000000 00000000 0000001b 00000001   ................
    3b4c0e160:  00000006 00000000 004000b0 00000000   ..........@.....
    3b4c0e170:  000000b0 00000000 0000002c 00000000   ........,.......
    3b4c0e180:  00000000 00000000 00000010 00000000   ................

-   This is not very evident but you can find out that the output of objdump has mapped to the output of the page.

-   Search for the string ``4000b0:   b8 01 00 00 00          mov    $0x1,%eax`` in the RAM output, you will find it at ``3b4c0e0b0:``

-   Search for the ``syscall`` instruction ``0f05`` - You will find it at ``3b4c0e0d0`` and at ``3b4c0e0c0``
