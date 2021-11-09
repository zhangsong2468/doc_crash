
=================================
Another way of reading page data 
=================================



- Open a process ``less`` and a file with it 

::

             crash> ps | grep less
               16937    855   0  ffff8803c53ae280  IN   0.0    9868   1016  less


- See the files

::

    crash> files ffff8803c53ae280
    PID: 16937  TASK: ffff8803c53ae280  CPU: 0   COMMAND: "less"
    ROOT: /    CWD: /home/support
    FD       FILE            DENTRY           INODE       TYPE PATH
    0 ffff88043a1fb380 ffff88042735f240 ffff88042d913418 CHR  /dev/pts/5
    1 ffff88043a1fb380 ffff88042735f240 ffff88042d913418 CHR  /dev/pts/5
    2 ffff88043a1fb380 ffff88042735f240 ffff88042d913418 CHR  /dev/pts/5
    3 ffff8804399cf780 ffff88042f852080 ffff8804383b4d98 CHR  /dev/tty
    4 ffff880424d15480 ffff88042462f5c0 ffff88042731f0d8 REG  /home/support/a.c

- Read file's mapping

::

                          crash> file ffff880424d15480 | grep mapping
                            f_mapping = 0xffff88042731f1f8, 

- See the address space

::

    crash> address_space 0xffff88042731f1f8
    struct address_space {
            host = 0xffff88042731f0d8, 
            page_tree = {
                    height = 0, 
                    gfp_mask = 32, 
                    rnode = 0xffffea000d973f70
            }, 
            tree_lock = {
                    raw_lock = {
                            slock = 1114129
                    }
            }, 
            i_mmap_writable = 0, 
            i_mmap = {
                    prio_tree_node = 0x0, 
                    index_bits = 1, 
                    raw = 1
            }, 
            i_mmap_nonlinear = {
                    next = 0xffff88042731f228, 
                    prev = 0xffff88042731f228
            }, 
            i_mmap_lock = {
                    raw_lock = {
                            slock = 393222
                    }
            }, 
            truncate_count = 2, 
            nrpages = 1, 
            writeback_index = 0, 
            a_ops = 0xffffffffa01dc320, 
            flags = 131290, 
            backing_dev_info = 0xffff880438fd13a8, 
            private_lock = {
                    raw_lock = {
                            slock = 524296
                    }
            }, 
            private_list = {
                    next = 0xffff88042731f270, 
                    prev = 0xffff88042731f270
            }, 
            assoc_mapping = 0x0
    }


- See the radix tree - note that here the page_tree ends with 0 so it is actually the page's address. Dereferencing it will give you error values

::        

    crash> radix_tree_node 0xffffea000d973f70
    struct radix_tree_node {
            height = 524504, 
                   count = 4194304, 
                   rcu_head = {
                           next = 0xffffffff00000001, 
                           func = 0
                   }, 
                   slots = {0xffff88042731f1f8, 0x0, 0xffffea000e45c198, 0xffffea000d12ba88, 0x400000004000d0, 0x1, 0x0, 0xffff8804256a9931, 0x859, 0xffffea000e516878, 0xffff ea000e0dfc70, 0x400000004000d0, 0x500000006, 0x0, 0xffff880428f81361, 0xc, 0xffffea000d986448, 0xffffea000e3119c0, 0x400000004000d0, 0x1, 0x0, 0xffff8804298b 8e11, 0xe3aab, 0xffffea000d973940, 0xffffea000d974078, 0x400000004000d0, 0x1, 0x0, 0xffff8804298b8e11, 0xe3aac, 0xffffea000d974040, 0xffffea000d9740b0, 0x400 000004000d0, 0x1, 0x0, 0xffff8804298b8e11, 0xe32d8, 0xffffea000d974078, 0xffffea000d9740e8, 0x400000004000d0, 0x1, 0x0, 0xffff8804298b8e11, 0xe3aad, 0xffffea 000d9740b0, 0xffffea000d974120, 0x400000004000d0, 0x1, 0x0, 0xffff8804298b8e11, 0xe32d9, 0xffffea000d9740e8, 0xffffea000d974158, 0x400000004000d0, 0x1, 0x0, 0xffff8804298b8e11, 0xe3aae, 0xffffea000d974120, 0xffffea000d974190, 0x400000004000d0, 0x1, 0x0, 0xffff8804298b8e11}, 
                   tags = {{932527}, {18446719884681757016}}
    }
         
- Directly read the page

::

    crash> kmem -p 0xffffea000d973f70
    PAGE        PHYSICAL      MAPPING       INDEX CNT FLAGS
    ffffea000d973f70 3e2122000 ffff88042731f1f8        0  1 400000000800d8

- You need to find the virtual address of the page                       

::                       

                       crash> ptov 3e2122000
                       VIRTUAL           PHYSICAL        
                       ffff8803e2122000  3e2122000       

- Read the data at the virtual address

::

    crash> rd -8 ffff8803e2122000 100
    ffff8803e2122000:  23 69 6e 63 6c 75 64 65 20 3c 66 63 6e 74 6c 2e   #include <fcntl.
    ffff8803e2122010:  68 3e 0a 23 69 6e 63 6c 75 64 65 20 3c 73 74 64   h>.#include <std
    ffff8803e2122020:  69 6f 2e 68 3e 0a 23 69 6e 63 6c 75 64 65 20 3c   io.h>.#include <
    ffff8803e2122030:  75 6e 69 73 74 64 2e 68 3e 0a 23 69 6e 63 6c 75   unistd.h>.#inclu
    ffff8803e2122040:  64 65 20 3c 73 79 73 2f 74 79 70 65 73 2e 68 3e   de <sys/types.h>
    ffff8803e2122050:  0a 23 69 6e 63 6c 75 64 65 20 3c 73 79 73 2f 73   .#include <sys/s
    ffff8803e2122060:  74 61 74 2e                                       tat.
    crash> rd -8 ffff8803e2122000 1000
    ffff8803e2122000:  23 69 6e 63 6c 75 64 65 20 3c 66 63 6e 74 6c 2e   #include <fcntl.
    ffff8803e2122010:  68 3e 0a 23 69 6e 63 6c 75 64 65 20 3c 73 74 64   h>.#include <std
    ffff8803e2122020:  69 6f 2e 68 3e 0a 23 69 6e 63 6c 75 64 65 20 3c   io.h>.#include <
    ffff8803e2122030:  75 6e 69 73 74 64 2e 68 3e 0a 23 69 6e 63 6c 75   unistd.h>.#inclu
    ffff8803e2122040:  64 65 20 3c 73 79 73 2f 74 79 70 65 73 2e 68 3e   de <sys/types.h>
    ffff8803e2122050:  0a 23 69 6e 63 6c 75 64 65 20 3c 73 79 73 2f 73   .#include <sys/s
    ffff8803e2122060:  74 61 74 2e 68 3e 0a 0a 69 6e 74 20 6d 61 69 6e   tat.h>..int main
    ffff8803e2122070:  20 28 29 20 7b 0a 09 69 6e 74 20 66 64 31 2c 20    () {..int fd1, 
    ffff8803e2122080:  66 64 32 2c 20 66 64 33 2c 20 66 64 34 2c 20 66   fd2, fd3, fd4, f
    ffff8803e2122090:  64 35 3b 0a 09 0a 09 66 64 31 3d 6f 70 65 6e 28   d5;....fd1=open(
    ffff8803e21220a0:  22 31 22 2c 20 4f 5f 43 52 45 41 54 29 3b 0a 09   "1", O_CREAT);..
    ffff8803e21220b0:  66 64 32 3d 6f 70 65 6e 28 22 32 22 2c 20 4f 5f   fd2=open("2", O_
    ffff8803e21220c0:  43 52 45 41 54 29 3b 0a 09 66 64 33 3d 6f 70 65   CREAT);..fd3=ope
    ffff8803e21220d0:  6e 28 22 33 22 2c 20 4f 5f 43 52 45 41 54 29 3b   n("3", O_CREAT);
    ffff8803e21220e0:  0a 09 66 64 34 3d 6f 70 65 6e 28 22 34 22 2c 20   ..fd4=open("4", 
    ffff8803e21220f0:  4f 5f 43 52 45 41 54 29 3b 0a 09 66 64 35 3d 6f   O_CREAT);..fd5=o
    ffff8803e2122100:  70 65 6e 28 22 35 22 2c 20 4f 5f 43 52 45 41 54   pen("5", O_CREAT
    ffff8803e2122110:  29 3b 0a 09 77 68 69 6c 65 20 28 31 29 20 7b 0a   );..while (1) {.


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


####################################
Finding out the data written on page
####################################

- Let us write a small piece of code to simulate a situation.

- This code will allocate around 10MB of space but will write only to the first 1000 integers (32*1000 bytes)

::

    #include <stdlib.h>
    int main () {
            int i;
            int *p=malloc(1024*1024*10);
            for (i=0; i<1000; i++) {
    
                    p[i]=i;
            }
            while (1) {
                    sleep(100);
            }
            return 0;
    }

- compile the code with ``gcc a.c -o sleepSleep``


- Find the running process

::

    crash> ps | grep sleep
    23996   4783   1  ffff880429e04200  IN   0.0    8100    600  sleep
    30965  19964   0  ffff8803c4160100  IN   0.0    8104    600  sleep
    30966  19961   5  ffff8804281502c0  IN   0.0    8104    600  sleep
    31437  11660   0  ffff8803dce6c280  IN   0.0    8100    596  sleep
    31633  15813   1  ffff8803c7ce0180  IN   0.0   13948    392  sleepSleep
    31754  12345   5  ffff8803b8ab6300  IN   0.0    8104    600  sleep
    31755  12503   4  ffff8803b9280500  IN   0.0    8104    600  sleep
    31756  12524   1  ffff8803c40e62c0  IN   0.0    8104    600  sleep


- See the ``vm`` of the running process    


::

    crash> vm -p 31633
    PID: 31633  TASK: ffff8803c7ce0180  CPU: 1   COMMAND: "sleepSleep"
    MM               PGD          RSS    TOTAL_VM
    ffff8803dcc79580  ffff8803c53f4000   392k    13948k 
    VMA           START       END     FLAGS FILE
    ffff8803dcc244d8     400000     401000 8001875 /home/support/crash/sleepSleep
    VIRTUAL     PHYSICAL 
    400000      3c0238000
    VMA           START       END     FLAGS FILE
    ffff8803dcc24cc0     600000     601000 8101871 /home/support/crash/sleepSleep
    VIRTUAL     PHYSICAL 
    600000      3c0444000
    VMA           START       END     FLAGS FILE
    ffff8804298392b0     601000     602000 8101873 /home/support/crash/sleepSleep
    VIRTUAL     PHYSICAL 
    601000      3b332e000
    VMA           START       END     FLAGS FILE
    ffff8803cbcae1f8 7fe5b01a0000 7fe5b0ba1000 100073 
    VIRTUAL     PHYSICAL 
            7fe5b01a0000  3b1464000
            7fe5b01a1000  (not mapped) <<< these pages are not mapped as these pages have not been written to
            7fe5b01a2000  (not mapped)
            7fe5b01a3000  (not mapped)
            7fe5b01a4000  (not mapped)
            7fe5b01a5000  (not mapped)
            7fe5b01a6000  (not mapped)
            7fe5b01a7000  (not mapped)
            7fe5b01a8000  (not mapped)
            7fe5b01a9000  (not mapped)
            7fe5b01aa000  (not mapped)
            7fe5b01ab000  (not mapped)
            7fe5b01ac000  (not mapped)
            7fe5b01ad000  (not mapped)
            7fe5b01ae000  (not mapped)
            7fe5b01af000  (not mapped)
            7fe5b01b0000  (not mapped)
            7fe5b01b1000  (not mapped)
            7fe5b01b2000  (not mapped)
            7fe5b01b3000  (not mapped)
            7fe5b01b4000  (not mapped)
            7fe5b01b5000  (not mapped)
            7fe5b01b6000  (not mapped)
            7fe5b01b7000  (not mapped)
            7fe5b01b8000  (not mapped)
            7fe5b01b9000  (not mapped)
            7fe5b01ba000  (not mapped)
            7fe5b01bb000  (not mapped)
            7fe5b01bc000  (not mapped)
            7fe5b01bd000  (not mapped)
            7fe5b01be000  (not mapped)
            7fe5b01bf000  (not mapped)
            7fe5b01c0000  (not mapped)
    7fe5b01c1000  (not mapped)


- Let us read the mapped page

::

    crash> rd -32 -p 3b1464000 1000
    3b1464000:  00000000 00000000 00a01002 00000000   ................
    3b1464010:  00000000 00000001 00000002 00000003   ................
    3b1464020:  00000004 00000005 00000006 00000007   ................
    3b1464030:  00000008 00000009 0000000a 0000000b   ................
    3b1464040:  0000000c 0000000d 0000000e 0000000f   ................
    3b1464050:  00000010 00000011 00000012 00000013   ................
    3b1464060:  00000014 00000015 00000016 00000017   ................
    3b1464070:  00000018 00000019 0000001a 0000001b   ................
    3b1464080:  0000001c 0000001d 0000001e 0000001f   ................
    3b1464090:  00000020 00000021 00000022 00000023    ...!..."...#...
    3b14640a0:  00000024 00000025 00000026 00000027   $...%...&...'...
    3b14640b0:  00000028 00000029 0000002a 0000002b   (...)...*...+...
    3b14640c0:  0000002c 0000002d 0000002e 0000002f   ,...-......./...
    3b14640d0:  00000030 00000031 00000032 00000033   0...1...2...3...
    3b14640e0:  00000034 00000035 00000036 00000037   4...5...6...7...
    3b14640f0:  00000038 00000039 0000003a 0000003b   8...9...:...;...
    3b1464100:  0000003c 0000003d 0000003e 0000003f   <...=...>...?...
    3b1464110:  00000040 00000041 00000042 00000043   @...A...B...C...
    3b1464120:  00000044 00000045 00000046 00000047   D...E...F...G...
    3b1464130:  00000048 00000049 0000004a 0000004b   H...I...J...K...
    3b1464140:  0000004c 0000004d 0000004e 0000004f   L...M...N...O...
    3b1464150:  00000050 00000051 00000052 00000053   P...Q...R...S...
    3b1464160:  00000054 00000055 00000056 00000057   T...U...V...W...
    3b1464170:  00000058 00000059 0000005a 0000005b   X...Y...Z...[...
    3b1464180:  0000005c 0000005d 0000005e 0000005f   \...]...^..._...
    3b1464190:  00000060 00000061 00000062 00000063   `...a...b...c...
    3b14641a0:  00000064 00000065 00000066 00000067   d...e...f...g...
    3b14641b0:  00000068 00000069 0000006a 0000006b   h...i...j...k...
    3b14641c0:  0000006c 0000006d 0000006e 0000006f   l...m...n...o...
    3b14641d0:  00000070 00000071 00000072 00000073   p...q...r...s...
    3b14641e0:  00000074 00000075 00000076 00000077   t...u...v...w...
    3b14641f0:  00000078 00000079 0000007a 0000007b   x...y...z...{...
    3b1464200:  0000007c 0000007d 0000007e 0000007f   |...}...~.......
    3b1464210:  00000080 00000081 00000082 00000083   ................
    3b1464220:  00000084 00000085 00000086 00000087   ................
    3b1464230:  00000088 00000089 0000008a 0000008b   ................
    3b1464240:  0000008c 0000008d 0000008e 0000008f   ................
    3b1464250:  00000090 00000091 00000092 00000093   ................
    3b1464260:  00000094 00000095 00000096 00000097   ................
    3b1464270:  00000098 00000099 0000009a 0000009b   ................
    3b1464280:  0000009c 0000009d 0000009e 0000009f   ................
    3b1464290:  000000a0 000000a1 000000a2 000000a3   ................
    3b14642a0:  000000a4 000000a5 000000a6 000000a7   ................
    3b14642b0:  000000a8 000000a9 000000aa 000000ab   ................
    3b14642c0:  000000ac 000000ad 000000ae 000000af   ................
    3b14642d0:  000000b0 000000b1 000000b2 000000b3   ................
    3b14642e0:  000000b4 000000b5 000000b6 000000b7   ................
    3b14642f0:  000000b8 000000b9 000000ba 000000bb   ................
    3b1464300:  000000bc 000000bd 000000be 000000bf   ................
    
- Here you can see the data which you have written to the page                    


##################################
Data of a file opened by a process
##################################

In this section we will see how the data of a file object is stored in the memory. 

We will see how to find out the data related to a open file in the memory.

File Object -> Inode Object -> f_mapping -> address_space -> rnode -> RAM


==========
Setting Up
==========

-   I have made a file with characters files as PAGE0000 - 512 times, PAGE0001 - 512 times and PAGE0002 - 512 times. This makes the data easy to identify in the file, the PAGE0001 will start at the second page and so on.

-   Open the file with ``less``


=====
Steps
=====


-   Find the PID

::

    crash> ps | grep less
    15519  14909   0  ffff8803dd77e540  IN   0.0    9868   1076  less


-   File the files

::

    crash> files ffff8803dd77e540
    PID: 15519  TASK: ffff8803dd77e540  CPU: 0   COMMAND: "less"
    ROOT: /    CWD: /home/support/crash
    FD       FILE            DENTRY           INODE       TYPE PATH
    0 ffff880429501cc0 ffff88042735c900 ffff8804289c2a88 CHR  /dev/pts/1
    1 ffff880429501cc0 ffff88042735c900 ffff8804289c2a88 CHR  /dev/pts/1
    2 ffff880429501cc0 ffff88042735c900 ffff8804289c2a88 CHR  /dev/pts/1
    3 ffff880427935380 ffff88042f852080 ffff8804383b4d98 CHR  /dev/tty



-   Find the mapping of the file 

::

    crash> file ffff8804393e9dc0 | grep mapping 
    f_mapping = 0xffff8804273ae848, 


-   Find the rnode from the address space 

::

    crash> address_space 0xffff8804273ae848 | grep rnode
    rnode = 0xffff8803b7043b19

-   As the address of the rnode is ending with bit set as 1 we need to remove the set bit 

::

    crash> eval 0xffff8803b7043b19&~1
    hexadecimal: ffff8803b7043b18  
    decimal: 18446612148269628184  (-131925439923432)
    octal: 1777774200166701035430
    binary: 1111111111111111100010000000001110110111000001000011101100011000


-   See the tree node

::

    crash> radix_tree_node 0xffff8803b7043b18
    struct radix_tree_node {
    height = 1, 
    count = 4, 
    rcu_head = {
    next = 0xffff8803b719ce50, 
    func = 0xffffffff811e5a90 <radix_tree_node_rcu_free>
    }, 
    slots = {0xffffea000d179cf8, 0xffffea000d030cc0, 0xffffea000d041538, 0xffffea000d235a60, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 
    tags = {{0}, {0}}
    }


-   See the first page's data 

::

    crash> kmem -p 0xffffea000d179cf8
    PAGE        PHYSICAL      MAPPING       INDEX CNT FLAGS
    ffffea000d179cf8 3bd9a9000 ffff8804273ae848        0  2 400000000010d8
    crash> ptov 3bd9a9000
    VIRTUAL           PHYSICAL        
    ffff8803bd9a9000  3bd9a9000       
    crash> rd -8 ffff8803bd9a9000 100
    ffff8803bd9a9000:  50 41 47 45 30 30 30 30 50 41 47 45 30 30 30 30   PAGE0000PAGE0000
    ffff8803bd9a9010:  50 41 47 45 30 30 30 30 50 41 47 45 30 30 30 30   PAGE0000PAGE0000
    ffff8803bd9a9020:  50 41 47 45 30 30 30 30 50 41 47 45 30 30 30 30   PAGE0000PAGE0000
    ffff8803bd9a9030:  50 41 47 45 30 30 30 30 50 41 47 45 30 30 30 30   PAGE0000PAGE0000
    ffff8803bd9a9040:  50 41 47 45 30 30 30 30 50 41 47 45 30 30 30 30   PAGE0000PAGE0000
    ffff8803bd9a9050:  50 41 47 45 30 30 30 30 50 41 47 45 30 30 30 30   PAGE0000PAGE0000
    ffff8803bd9a9060:  50 41 47 45                                       PAGE


-   See the second page's data 

::

    crash> kmem -p 0xffffea000d030cc0
    PAGE        PHYSICAL      MAPPING       INDEX CNT FLAGS
    ffffea000d030cc0 3b7ba8000 ffff8804273ae848        1  2 400000000010d8
    crash> ptov 3b7ba8000
    VIRTUAL           PHYSICAL        
    ffff8803b7ba8000  3b7ba8000       
    crash> rd -8 ffff8803b7ba8000
    ffff8803b7ba8000:  50                                                P
    crash> rd -8 ffff8803b7ba8000 100
    ffff8803b7ba8000:  50 41 47 45 30 30 30 31 50 41 47 45 30 30 30 31   PAGE0001PAGE0001
    ffff8803b7ba8010:  50 41 47 45 30 30 30 31 50 41 47 45 30 30 30 31   PAGE0001PAGE0001
    ffff8803b7ba8020:  50 41 47 45 30 30 30 31 50 41 47 45 30 30 30 31   PAGE0001PAGE0001
    ffff8803b7ba8030:  50 41 47 45 30 30 30 31 50 41 47 45 30 30 30 31   PAGE0001PAGE0001
    ffff8803b7ba8040:  50 41 47 45 30 30 30 31 50 41 47 45 30 30 30 31   PAGE0001PAGE0001
    ffff8803b7ba8050:  50 41 47 45 30 30 30 31 50 41 47 45 30 30 30 31   PAGE0001PAGE0001
    ffff8803b7ba8060:  50 41 47 45                

-   See the third page's data 

::

    crash>  kmem -p 0xffffea000d041538
    PAGE        PHYSICAL      MAPPING       INDEX CNT FLAGS
    ffffea000d041538 3b8061000 ffff8804273ae848        2  2 400000000010d8
    crash> ptov 3b8061000
    VIRTUAL           PHYSICAL        
    ffff8803b8061000  3b8061000       
    crash> rd -8 ffff8803b8061000 100
    ffff8803b8061000:  50 41 47 45 30 30 30 32 50 41 47 45 30 30 30 32   PAGE0002PAGE0002
    ffff8803b8061010:  50 41 47 45 30 30 30 32 50 41 47 45 30 30 30 32   PAGE0002PAGE0002
    ffff8803b8061020:  50 41 47 45 30 30 30 32 50 41 47 45 30 30 30 32   PAGE0002PAGE0002
    ffff8803b8061030:  50 41 47 45 30 30 30 32 50 41 47 45 30 30 30 32   PAGE0002PAGE0002
    ffff8803b8061040:  50 41 47 45 30 30 30 32 50 41 47 45 30 30 30 32   PAGE0002PAGE0002
    ffff8803b8061050:  50 41 47 45 30 30 30 32 50 41 47 45 30 30 30 32   PAGE0002PAGE0002
    ffff8803b8061060:  50 41 47 45                                       PAGE


###############################################
Printing the data of a file opened by a process
###############################################

============
Introduction
============


Previously we some of the commands in crash to find the files associated with a process. In this section we will see the how is the ``task_struct`` maintains the list of opened files and how can we see them.

We will also find the associated dentry object, read the name of the file, see the associated inode object, see the associated superblock object.

Once done with the above data structures we will then proceed to read the file in the RAM.

task_struct -> fd_table -> file object -> dentry -> inode -> superblock

==========
Setting Up
==========

- Get the PID of the process 

::

    crash> ps | grep vim 
    1076    855   7  ffff8803c53ae280  IN   0.0   21036   3268  vim


- Get the files structure

::

          crash> files_struct 0xffff8804299c6c80
          struct files_struct {
                  count = {
                          counter = 1
                  }, 
                  fdt = 0xffff8803dcffba40, 
                  fdtab = {
                          max_fds = 64, 
                          fd = 0xffff8804299c6d18, 
                          close_on_exec = 0xffff8804299c6d08, 
                          open_fds = 0xffff8804299c6d10, 
                          rcu = {
                                  next = 0x0, 
                                  func = 0
                          }, 
                          next = 0x0
                  }, 
                  file_lock = {
                          raw_lock = {
                                  slock = 20578618
                          }
                  }, 
                  next_fd = 3, 
                  close_on_exec_init = {
                          fds_bits = {0}
                  }, 
                  open_fds_init = {
                          fds_bits = {1}
                  }, 
                  fd_array = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
                          , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
                          0x0, 0x0, 0x0, 0x0}
          }

- Get the fdtable

::

    crash> files_struct 0xffff8804299c6c80 | grep fdt 
    fdt = 0xffff8803dcffba40, 
        fdtab = {
                crash> fdtable 0xffff8803dcffba40
                        struct fdtable {
                                max_fds = 256, 
                                fd = 0xffff880425500800, 
                                close_on_exec = 0xffff8803dd7ce560, 
                                open_fds = 0xffff8803dd7ce540, 
                                rcu = {
                                        next = 0x0, 
                                        func = 0
                                }, 
                                next = 0x0
                        }


- Read the fd table

::      

    crash> rd -64 0xffff880425500800 10 
    ffff880425500800:  ffff88043a1fb380 ffff88043a1fb380   ...:.......:....
    ffff880425500810:  ffff88043a1fb380 0000000000000000   ...:............
    ffff880425500820:  ffff880424e55e80 0000000000000000   .^.$............
    ffff880425500830:  0000000000000000 0000000000000000   ................
    ffff880425500840:  0000000000000000 0000000000000000   ................

- Read the file pointer, find dentry 

::

    crash> file ffff880424e55e80 | grep dentry 
    dentry = 0xffff8804272c3980

- Read the dentry, get the name 

::

    crash> dentry 0xffff8804272c3980 | grep name 
    d_name = {
    name = 0xffff8804272c3a20 ".a.c.swp"
    d_iname = ".a.c.swp\000ms\000\000\210\377\377\020']\r\000\352\377\377\346u\000\000\000\000\000"


- Read the dentry, get the inode                                        

::

    crash> dentry 0xffff8804272c3980 | grep inode 
    d_inode = 0xffff8804273eed78, 

- Read the mappings

::        

    crash> inode 0xffff8804273eed78 | grep map
    i_mapping = 0xffff8804273eee98, 
    i_mmap_writable = 0, 
    i_mmap = {
    i_mmap_nonlinear = {
    i_mmap_lock = {
    assoc_mapping = 0x0


- Get the address space 

::

    crash> address_space 0xffff8804273eee98
    struct address_space {
        host = 0xffff8804273eed78, 
        page_tree = {
                height = 1, 
                gfp_mask = 32, 
                rnode = 0xffff8804273d5d41
        }, 
        tree_lock = {
                raw_lock = {
                        slock = 23200098
                }
        }, 
        i_mmap_writable = 0, 
        i_mmap = {
                prio_tree_node = 0x0, 
                index_bits = 1, 
                raw = 1
        }, 
        i_mmap_nonlinear = {
                next = 0xffff8804273eeec8, 
                prev = 0xffff8804273eeec8
        }, 
        i_mmap_lock = {
                raw_lock = {
                        slock = 3407924
                }
        }, 
        truncate_count = 20, 
        nrpages = 3, 
        writeback_index = 3, 
        a_ops = 0xffffffffa01dc320, 
        flags = 131290, 
        backing_dev_info = 0xffff880438fd13a8, 
        private_lock = {
                raw_lock = {
                        slock = 14352603
                }
        }, 
        private_list = {
                next = 0xffff8804273eef10, 
                prev = 0xffff8804273eef10
        }, 
        assoc_mapping = 0x0
    }

- Read the radix tree node, the values will be wrong,                                  

- Here is a small trick which is applied for the files whose size is less than one page. The page_tree is then not a radix_tree_node it is actually the page itself. This is identified by the last bit set in the page_tree address. See the following code. 

::

    static void *radix_tree_lookup_element(struct radix_tree_root *root,
                    unsigned long index, int is_slot)
    {
    unsigned int height, shift;
    struct radix_tree_node *node, **slot;
    
    node = rcu_dereference(root->rnode);
    if (node == NULL)
            return NULL;
        
    if (!radix_tree_is_indirect_ptr(node)) { <<<<< This function checks if the last bit is 1 and returns true or false
            if (index > 0) 
                    return NULL;
            return is_slot ? (void *)&root->rnode : node;
    }    

    /*
       Other wise here it converts the address to proper address of a radix_tree_node by ADDRESS & ~1 and returns the address.
    node = indirect_to_ptr(node);
    
    height = node->height;
    if (index > radix_tree_maxindex(height))
            return NULL;
    
    shift = (height-1) * RADIX_TREE_MAP_SHIFT;



- When looking at the root of radix tree in address space you need to 

::

    static inline int radix_tree_is_indirect_ptr(void *ptr)
    {
                return (int)((unsigned long)ptr & RADIX_TREE_INDIRECT_PTR);
    } In function find_get_page

::

    crash> radix_tree_node 0xffff8804273d5d41
    struct radix_tree_node {
    height = 50331648, 
    count = 2415919104, 
    rcu_head = {
    next = 0x90ffff8804273988, 
    func = 0x60ffffffff811e5a
    }, 
    slots = {0x8ffffea000d3884, 0x28ffffea000d3131, 0xffffea000d4916, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0 x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 
    tags = {{0}, {72057594037927936}}
    }


- Notice that you have to substract 1 from the address

::  
          
    crash> page 0x8ffffea000d3884
     struct: invalid kernel virtual address: 0x8ffffea000d3884
     crash> radix_tree_node 0xffff8804273d5d40
     struct radix_tree_node {
     height = 1, 
     count = 3, 
     rcu_head = {
     next = 0xffff880427398890, 
     func = 0xffffffff811e5a90 <radix_tree_node_rcu_free>
     }, 
     slots = {0xffffea000d388460, 0xffffea000d313108, 0xffffea000d491628, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0
                                        x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
                                                , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 
     tags = {{0}, {0}}
     } 

- In the slots you will get the page address

::

    crash> page 0xffffea000d388460
    struct page {
            flags = 18014398509486168, 
                  _count = {
                          counter = 2
                  }, 
                  {
                          _mapcount = {
                                  counter = -1
                          }, 
                          {
                                  inuse = 65535, 
                                  objects = 65535
                          }
                  }, 
                  {
                          {
                                  private = 18446612150104959936, 
                                  mapping = 0xffff8804273eee98
                          }, 
                          ptl = {
                                  raw_lock = {
                                          slock = 610872256
                                  }
                          }, 
                          slab = 0xffff880424692bc0, 
                          first_page = 0xffff880424692bc0
                  }, 
                  {
                          index = 0, 
                          freelist = 0x0, 
                          reserve = 0, 
                          frag_count = {
                                  counter = 0
                          }
                  }, 
                  lru = {
                          next = 0xffffea000d4b06e0, 
                          prev = 0xffffea000d973f98
                  }
    }


- Read the page

::        

    crash> kmem -p 0xffffea000d388460
        PAGE        PHYSICAL      MAPPING       INDEX CNT FLAGS
        ffffea000d388460 3c7014000 ffff8804273eee98        0  2 40000000001058
        crash> ptov 3c7014000
        VIRTUAL           PHYSICAL        
        ffff8803c7014000  3c7014000       
        crash> rd -8 ffff8803c7014000 100
        ffff8803c7014000:  62 30 56 49 4d 20 37 2e 32 00 00 00 00 10 00 00   b0VIM 7.2.......
        ffff8803c7014010:  2c 18 65 52 5b 65 80 02 34 04 00 00 72 6f 6f 74   ,.eR[e..4...root
        ffff8803c7014020:  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
        ffff8803c7014030:  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
        ffff8803c7014040:  00 00 00 00 72 69 73 68 69 5f 30 31 00 00 00 00   ....rishi_01....
        ffff8803c7014050:  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
        ffff8803c7014060:  00 00 00 00                                       ....

- Above is wrong, read the radix_tree_node

::

    crash> radix_tree_node 0xffff8804273d5d40
    struct radix_tree_node {
            height = 1, 
                   count = 3, 
                   rcu_head = {
                           next = 0xffff880427398890, 
                           func = 0xffffffff811e5a90 <radix_tree_node_rcu_free>
                   }, 
                   slots = {0xffffea000d388460, 0xffffea000d313108, 0xffffea000d491628, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0 x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 
                   tags = {{0}, {0}}
    }
    
- Read the page

::  
      
    crash> kmem -p 0xffffea000d313108
    PAGE        PHYSICAL      MAPPING       INDEX CNT FLAGS
    ffffea000d313108 3c4e97000 ffff8804273eee98        1  2 40000000001058

- Convert phyical-to-virtual                                                                

::

    crash> ptov 3c4e97000
    VIRTUAL           PHYSICAL        
    ffff8803c4e97000  3c4e97000       


- Read the virutal address

::

    crash> rd -8 ffff8803c4e97000 100
    ffff8803c4e97000:  74 70 01 00 7f 00 00 00 02 00 00 00 00 00 00 00   tp..............
    ffff8803c4e97010:  12 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00   ................
    ffff8803c4e97020:  01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    ffff8803c4e97030:  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    ffff8803c4e97040:  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    ffff8803c4e97050:  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    ffff8803c4e97060:  00 00 00 00                                       ....

- Above is wrong I suppose, will do it again todo                                                                

::

    crash> kmem -p 0xffffea000d491628
    PAGE        PHYSICAL      MAPPING       INDEX CNT FLAGS
    ffffea000d491628 3cbbd3000 ffff8804273eee98        2  2 40000000001058
    crash> ptov 3cbbd3000
    VIRTUAL           PHYSICAL        
    ffff8803cbbd3000  3cbbd3000       
    crash> rd -8 ffff8803cbbd3000 100
    ffff8803cbbd3000:  61 64 00 00 6c 0e 00 00 d0 0e 00 00 00 10 00 00   ad..l...........
    ffff8803cbbd3010:  12 00 00 00 00 00 00 00 ed 0f 00 00 da 0f 00 00   ................
    ffff8803cbbd3020:  c6 0f 00 00 af 0f 00 00 99 0f 00 00 98 0f 00 00   ................
    ffff8803cbbd3030:  8a 0f 00 00 6c 0f 00 00 6a 0f 00 00 51 0f 00 00   ....l...j...Q...
    ffff8803cbbd3040:  38 0f 00 00 1f 0f 00 00 06 0f 00 00 ed 0e 00 00   8...............
    ffff8803cbbd3050:  e0 0e 00 00 dd 0e 00 00 d2 0e 00 00 d0 0e 00 00   ................
    ffff8803cbbd3060:  cf 0e 00 00                                       ....
    crash> 
    


#####################################
Finding the files opened by a process
#####################################

- Open a file in vim and write something in it
- Start ``crash``
- Find the PID of the process

::

        crash> ps | grep vim
        11012  11001   4  ffff8803c3998140  IN   0.0   21032   3264  vim
        26698  18988   4  ffff8804115e8180  IN   0.0   21004   3320  vim


- Run ``files`` command on the PID

::

        crash> files 26698
        PID: 26698  TASK: ffff8804115e8180  CPU: 4   COMMAND: "vim"
        ROOT: /    CWD: /home/support/a/b/c
        FD       FILE            DENTRY           INODE       TYPE PATH
        0 ffff880417852380 ffff880100a92e40 ffff8803697550c8 CHR  /dev/pts/5
        1 ffff880417852380 ffff880100a92e40 ffff8803697550c8 CHR  /dev/pts/5
        2 ffff880417852380 ffff880100a92e40 ffff8803697550c8 CHR  /dev/pts/5
        3 ffff88043a1d98c0 ffff8803c64a6b40 ffff88010099bd78 REG  /home/support/a/b/c/.test.c.swp

        crash> files 11012
        PID: 11012  TASK: ffff8803c3998140  CPU: 4   COMMAND: "vim"
        ROOT: /    CWD: /home/support
        FD       FILE            DENTRY           INODE       TYPE PATH
        0 ffff880439181ac0 ffff8803739c3a80 ffff8803c54088b8 CHR  /dev/pts/7
        1 ffff880439181ac0 ffff8803739c3a80 ffff8803c54088b8 CHR  /dev/pts/7
        2 ffff880439181ac0 ffff8803739c3a80 ffff8803c54088b8 CHR  /dev/pts/7
        4 ffff8803e1c24480 ffff880100a9c240 ffff8803738f1400 REG  /home/support/.a.c.swp

        
- Let us do it another way, find it out from the process's file descriptor array.
         
::

    crash> files_struct 0xffff88043d7473c0
    struct files_struct {
    count = {
    counter = 1
    }, 
    fdt = 0xffff880037a81c40, 
    fdtab = {
    max_fds = 64, 
    fd = 0xffff88043d747458, 
    close_on_exec = 0xffff88043d747448, 
    open_fds = 0xffff88043d747450, 
    rcu = {
    next = 0x0, 
    func = 0
    }, 
    next = 0x0
    }, 
    file_lock = {
    raw_lock = {
    slock = 28901817
    }
    }, 
    next_fd = 3, 
    close_on_exec_init = {
    fds_bits = {0}
    }, 
    open_fds_init = {
    fds_bits = {1}
    }, 
    fd_array = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
    0x0, 0x0, 0x0, 0x0}
    }

- Fdtable has the pointers

::

    crash> fdtable 0xffff880037a81c40
    struct fdtable {
    max_fds = 256, 
    fd = 0xffff880411558000, 
    close_on_exec = 0xffff8803c5f9f3e0, 
    open_fds = 0xffff8803c5f9f3c0, 
    rcu = {
    next = 0x0, 
    func = 0
    }, 
    next = 0x0
    }

- fd in fdtable has the actual values

::

    crash> rd -64 0xffff880411558000 10
    ffff880411558000:  ffff880439181ac0 ffff880439181ac0   ...9.......9....
    ffff880411558010:  ffff880439181ac0 0000000000000000   ...9............
    ffff880411558020:  ffff8803e1c24480 0000000000000000   .D..............
    ffff880411558030:  0000000000000000 0000000000000000   ................
    ffff880411558040:  0000000000000000 0000000000000000   ................
  

- Let us check the dentry

::  

    crash> file ffff8803e1c24480 | grep dentry 
    dentry = 0xffff880100a9c240
    
    crash> dentry 0xffff880100a9c240 | grep name 
    d_name = {
    name = 0xffff880100a9c2e0 ".a.c.swp"
    d_iname = ".a.c.swp\000s\000\063\065\060\063\062\060\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
    crash> 
    

- As you have the dentry now  you can try to find out the root, mount, superbloc, everything

Now let us see the data of the file                    
------------------------------------

- http://tuxthink.blogspot.in/2012/05/module-to-print-open-files-of-process.html

- Here if you see the values of the i_data structure (address_space) you will find that values are same to what you get if you de-reference the i_mapping

- Also the host is the address of the inode

::

    crash> ps | grep vim 
    11540  18988   0  ffff8803e1cbe200  IN   0.0   21008   3352  vim
    
    crash> task ffff8803e1cbe200 | grep files
    files = 0xffff88042a2b19c0, 
    
    crash> files_struct 0xffff88042a2b19c0
    fdt = 0xffff8803e1270e40, 
    fdtab = {
    
    crash> fdtable 0xffff8803e1270e40
    struct fdtable {
    max_fds = 256, 
    fd = 0xffff88043a3d5000, 
    close_on_exec = 0xffff880391ee6ee0, 
    open_fds = 0xffff880391ee6ec0, 
    rcu = {
    next = 0x0, 
    func = 0
    }, 
    next = 0x0
    }
    
    crash> rd -64 0xffff88043a3d5000 10 
    ffff88043a3d5000:  ffff880417852380 ffff880417852380   .#.......#......
    ffff88043a3d5010:  ffff880417852380 ffff8804164160c0   .#.......`A.....
    ffff88043a3d5020:  0000000000000000 0000000000000000   ................
    ffff88043a3d5030:  0000000000000000 0000000000000000   ................
    ffff88043a3d5040:  0000000000000000 0000000000000000   ................
    
    crash> file ffff8804164160c0 | grep dentry 
    dentry = 0xffff8803c64a6b40
    
    crash> dentry 0xffff8803c64a6b40 | grep name
    d_name = {
    name = 0xffff8803c64a6be0 ".test.c.swp"
    d_iname = ".test.c.swp\000\000\062\061\062\060\070_32552b36\000\000\000\000"
        


::

    crash> dentry 0xffff8803c64a6b40 | grep inode
    d_inode = 0xffff8803738d7728, 
    
    crash> inode 0xffff8803738d7728 | grep mapping
    i_mapping = 0xffff8803738d7848, 
    assoc_mapping = 0x0


############################
Finding the PWD of a process
############################


 - Open a file in vim 

::

    pwd -  /home/support/a/b/c

::

     ps ax | grep vim  | grep -v grep 
     26698 pts/5    S+     0:00 vim test.c

 - find the task struct of the process

::

    crash> ps | grep vim 
    26698  18988   4  ffff8804115e8180  IN   0.0   21004   3320  vim

- See the values of the task_struct

::

    crash> task_struct ffff8804115e8180 | grep fs
    cfs_rq = 0xffff88002c313768, 
    fsindex = 0, 
    fs = 139810743506688, 
    fs = 0xffff88043d3647c0, <<<<<<<<<<<<<
    ^^^^^^^^^^^^^^^^^^^^^^^^^
    fs_excl = {


- struct files_struct \*fs keeps the information of the process's file system


::

   crash> fs_struct 0xffff88043d3647c0
   struct fs_struct {
   users = 1, 
   lock = {
   raw_lock = {
   lock = 16777216
   }
   }, 
   umask = 18, 
   in_exec = 0, 
   root = {
   mnt = 0xffff88043a386e80, 
   dentry = 0xffff88042f9d22c0
   }, 
   pwd = {
   mnt = 0xffff88043a386e80, 
   dentry = 0xffff8803c64a6e40 <<<<<<
   ^^^^^^^^^^^^^^^^^^^^
   }
   }

- The dentry of the files_struct keeps the information of the Current Working Directory of the process, see the dentry of the PWD, the root's dentry will directly take you to the root's dentry

- De-reference the dentry's address        
- The d_name field in the dentry keeps the name of the current dentry, here it is "c"
- d_parent keeps the dentry of the parent, we will keep on de-referencing the parent's dentrys

::

    crash> dentry 0xffff8803c64a6e40
    struct dentry {
            d_count = {
                    counter = 5
            }, 
                    d_flags = 8, 
                    d_lock = {
                            raw_lock = {
                                    slock = 2555943
                            }
                    }, 
                    d_mounted = 0, 
                    d_inode = 0xffff8803c54f5d78, 
                    d_hash = {
                            next = 0x0, 
                            pprev = 0xffffc900005dd918
                    }, 
                    d_parent = 0xffff8803c64a66c0, <<<<<<<<<<<<<<<<<<
                            ^^^^^^^^^^^^^^^^^^^^^^^^^
                            d_name = {
                                    hash = 17490, 
                                    len = 1, 
                                    name = 0xffff8803c64a6ee0 "c" <<<<<<<<<<<<<<<<<<
                                            ^^^^^^^^^^^^^^^^^^^^^^
                            }, 
                    d_lru = {
                            next = 0xffff8803c64a6700, 
                            prev = 0xffff8803696d20c0
                    }, 
                    d_u = {
                            d_child = {
                                    next = 0xffff8803c64a6720, 
                                    prev = 0xffff8803c64a6720
                            }, 
                            d_rcu = {
                                    next = 0xffff8803c64a6720, 
                                    func = 0xffff8803c64a6720
                            }
                    }, 
                    d_subdirs = {
                            next = 0xffff8803696d9a10, 
                            prev = 0xffff8803696d20d0
                    }, 
                    d_alias = {
                            next = 0xffff8803c54f5da8, 
                            prev = 0xffff8803c54f5da8
                    }, 
                    d_time = 18446744071590877984, 
                    d_op = 0x0, 
                    d_sb = 0xffff880438bdb000, 
                    d_fsdata = 0x0, 
                    d_iname = "c\000s3\000ocket\000\000\000\210\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
    }


- Again dereference the d_parent, you get the d_name as "b", take the d_parent and dereference the parent


::

    crash> dentry 0xffff8803c64a66c0
    struct dentry {
    d_count = {
    counter = 1
    }, 
    d_flags = 8, 
    d_lock = {
    raw_lock = {
    slock = 2687017
    }
    }, 
    d_mounted = 0, 
    d_inode = 0xffff8801009dc400, 
    d_hash = {
    next = 0xffff8804238209d8, 
    pprev = 0xffffc900005dd230
    }, 
    d_parent = 0xffff8803c64a6c00, <<<<<<<<<<<<<<<<
    ^^^^^^^^^^^^^^^^^^^^^^
    d_name = {
    hash = 17314, 
    len = 1, 
    name = 0xffff8803c64a6760 "b" <<<<<<<<<<<<<
    ^^^^^^^^^^^^^^^^
    }, 
    d_lru = {
    next = 0xffff8803c64a6c40, 
    prev = 0xffff8803c64a6e80
    }, 
    d_u = {
    d_child = {
    next = 0xffff8803c64a6c60, 
    prev = 0xffff8803c64a6c60
    }, 
    d_rcu = {
    next = 0xffff8803c64a6c60, 
    func = 0xffff8803c64a6c60
    }
    }, 
    d_subdirs = {
    next = 0xffff8803c64a6e90, 
    prev = 0xffff8803c64a6e90
    }, 
    d_alias = {
    next = 0xffff8801009dc430, 
    prev = 0xffff8801009dc430
    }, 
    d_time = 0, 
    d_op = 0x0, 
    d_sb = 0xffff880438bdb000, 
    d_fsdata = 0x0, 
    d_iname = "b\000s3\000\000-1382319053\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
    }

- Repeat the process

::

    crash> dentry 0xffff8803c64a6c00
    struct dentry {
    d_count = {
    counter = 1
    }, 
    d_flags = 8, 
    d_lock = {
    raw_lock = {
            slock = 3407924
    }
    }, 
    d_mounted = 0, 
    d_inode = 0xffff88021245ad78, 
    d_hash = {
    next = 0xffff880105afd458, 
    pprev = 0xffffc90000f23e30
    }, 
    d_parent = 0xffff88040d814a40, <<<<<<<<<<<
    ^^^^^^^^^^^^^^^^^
    d_name = {
    hash = 17138, 
    len = 1, 
    name = 0xffff8803c64a6ca0 "a" <<<<<<<<<<<<
          ^^^^^^^^^^^^^^^^^
    }, 
    d_lru = {
    next = 0xffff880100a0a640, 
    prev = 0xffff8803c64a6700
    }, 
    d_u = {
    d_child = {
    next = 0xffff8803698d9550, 
    prev = 0xffff880100a0a650
    }, 
    d_rcu = {
    next = 0xffff8803698d9550, 
    func = 0xffff880100a0a650
    }
    }, 
    d_subdirs = {
    next = 0xffff8803c64a6710, 
    prev = 0xffff8803c64a6710
    }, 
    d_alias = {
    next = 0xffff88021245ada8, 
    prev = 0xffff88021245ada8
    }, 
    d_time = 18446612148525886584, 
    d_op = 0x0, 
    d_sb = 0xffff880438bdb000, 
    d_fsdata = 0x0, 
    d_iname = "a\000s2\000\000\000tems\000\000\065\065\070\064\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
    }

- Repeat again

::

         crash> dentry 0xffff88040d814a40
                 struct dentry {
                         d_count = {
                                 counter = 134
                         }, 
                                 d_flags = 0, 
                                 d_lock = {
                                         raw_lock = {
                                                 slock = 3305161984
                                         }
                                 }, 
                                 d_mounted = 0, 
                                 d_inode = 0xffff88040d8c0400, 
                                 d_hash = {
                                         next = 0x0, 
                                         pprev = 0xffffc9000078f800
                                 }, 
                                 d_parent = 0xffff880423ab0300, <<<<<<<<<<<<<<
                                         ^^^^^^^^^^^^^^^^^^^^
                                 d_name = {
                                         hash = 985546086, 
                                         len = 7, 
                                         name = 0xffff88040d814ae0 "support" <<<<<<<<<<<<<<
                                                 ^^^^^^^^^^^^^^^^^
                                 }, 
                                 d_lru = {
                                         next = 0xffff88040d814a80, 
                                         prev = 0xffff88040d814a80
                                 }, 
                                 d_u = {
                                         d_child = {
                                                 next = 0xffff88042de4e310, 
                                                 prev = 0xffff88040daf0850
                                         }, 
                                         d_rcu = {
                                                 next = 0xffff88042de4e310, 
                                                 func = 0xffff88040daf0850
                                         }
                                 }, 
                                 d_subdirs = {
                                         next = 0xffff8801008903d0, 
                                         prev = 0xffff88041b5a4710
                                 }, 
                                 d_alias = {
                                         next = 0xffff88040d8c0430, 
                                         prev = 0xffff88040d8c0430
                                 }, 
                                 d_time = 0, 
                                 d_op = 0x0, 
                                 d_sb = 0xffff880438bdb000, 
                                 d_fsdata = 0x0, 
                                 d_iname = "support\000(\033,\200\320\177\000\000\001\000\000\000\000\000\000\000\020\aQ\327\317\177\000"
                 }

- Repeat Again

::        

         crash> dentry 0xffff880423ab0300
                 struct dentry {
                         d_count = {
                                 counter = 14
                         }, 
                                 d_flags = 8, 
                                 d_lock = {
                                         raw_lock = {
                                                 slock = 3419261901
                                         }
                                 }, 
                                 d_mounted = 0, 
                                 d_inode = 0xffff880423ab1400, 
                                 d_hash = {
                                         next = 0x0, 
                                         pprev = 0xffffc900002c9f98
                                 }, 
                                 d_parent = 0xffff88042f9d22c0, <<<<<<<<<<<<<<<<<<<
                                         ^^^^^^^^^^^^^^^^^^^^^
                                 d_name = {
                                         hash = 27051904, 
                                         len = 4, 
                                         name = 0xffff880423ab03a0 "home" <<<<<<<<<<<<<<<
                                                 ^^^^^^^^^^^^^^^^
                                 }, 
                                 d_lru = {
                                         next = 0xffff880423ab0400, 
                                         prev = 0xffff88042de4e300
                                 }, 
                                 d_u = {
                                         d_child = {
                                                 next = 0xffff88042faa5650, 
                                                 prev = 0xffff88042de4e3d0
                                         }, 
                                         d_rcu = {
                                                 next = 0xffff88042faa5650, 
                                                 func = 0xffff88042de4e3d0
                                         }
                                 }, 
                                 d_subdirs = {
                                         next = 0xffff88011aac6e90, 
                                         prev = 0xffff88042de4e310
                                 }, 
                                 d_alias = {
                                         next = 0xffff880423ab1430, 
                                         prev = 0xffff880423ab1430
                                 }, 
                                 d_time = 0, 
                                 d_op = 0x0, 
                                 d_sb = 0xffff880438bdb000, 
                                 d_fsdata = 0x0, 
                                 d_iname = "home\000p.gz\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
                 }

- Now you will get the root's dentry

::

         crash> dentry 0xffff88042f9d22c0
                 struct dentry {
                         d_count = {
                                 counter = 291
                         }, 
                                 d_flags = 16, 
                                 d_lock = {
                                         raw_lock = {
                                                 slock = 131074
                                         }
                                 }, 
                                 d_mounted = 0, 
                                 d_inode = 0xffff88042f80ea50, 
                                 d_hash = {
                                         next = 0x0, 
                                         pprev = 0x0
                                 }, 
                                 d_parent = 0xffff88042f9d22c0, <<<<<<<<<<<<<<
                                         ^^^^^^^^^^^^^^^^^^^^^^
                                 d_name = {
                                         hash = 0, 
                                         len = 1, 
                                         name = 0xffff88042f9d2360 "/" <<<<<<<<<<<<<<<<
                                                 ^^^^^^^^^^^^^^^^^^
                                 }, 
                                 d_lru = {
                                         next = 0xffff88042f9d2300, 
                                         prev = 0xffff88042f9d2300
                                 }, 
                                 d_u = {
                                         d_child = {
                                                 next = 0xffff88042f9d2310, 
                                                 prev = 0xffff88042f9d2310
                                         }, 
                                         d_rcu = {
                                                 next = 0xffff88042f9d2310, 
                                                 func = 0xffff88042f9d2310
                                         }
                                 }, 
                                 d_subdirs = {
                                         next = 0xffff88036975cb90, 
                                         prev = 0xffff88042f8c2c50
                                 }, 
                                 d_alias = {
                                         next = 0xffff88042f80ea80, 
                                         prev = 0xffff88042f80ea80
                                 }, 
                                 d_time = 0, 
                                 d_op = 0x0, 
                                 d_sb = 0xffff880438bdb000, 
                                 d_fsdata = 0x0, 
                                 d_iname = "/\000\066_64\000ache\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
                 }
         crash> 



             - So you get the directory as ``/home/support/a/b/c``



##########
Setting Up
##########

*   Install

::

    $ sudo apt-get install linux-crashdump
    [sudo] password for rishi: 
    Reading package lists... Done
    Building dependency tree       
    Reading state information... Done
    The following additional packages will be installed:
    crash kdump-tools kexec-tools libdw1 makedumpfile
    The following NEW packages will be installed:
    crash kdump-tools kexec-tools libdw1 linux-crashdump makedumpfile
    0 upgraded, 6 newly installed, 0 to remove and 456 not upgraded.
    Need to get 2,964 kB of archives.
    After this operation, 9,395 kB of additional disk space will be used.
    Do you want to continue? [Y/n] y
    Get:1 http://in.archive.ubuntu.com/ubuntu xenial-updates/main amd64 crash amd64 7.1.4-1ubuntu4.1 [2,529 kB]
    Get:2 http://in.archive.ubuntu.com/ubuntu xenial/main amd64 libdw1 amd64 0.165-3ubuntu1 [191 kB]
    Get:3 http://in.archive.ubuntu.com/ubuntu xenial-updates/main amd64 makedumpfile amd64 1:1.5.9-5ubuntu0.3 [145 kB]
    Get:4 http://in.archive.ubuntu.com/ubuntu xenial/main amd64 kexec-tools amd64 1:2.0.10-1ubuntu2 [77.3 kB]
    Get:5 http://in.archive.ubuntu.com/ubuntu xenial-updates/main amd64 kdump-tools all 1:1.5.9-5ubuntu0.3 [20.6 kB]
    Get:6 http://in.archive.ubuntu.com/ubuntu xenial-updates/main amd64 linux-crashdump amd64 4.4.0.62.65 [2,514 B]
    Fetched 2,964 kB in 3s (898 kB/s)     
    Preconfiguring packages ...
    Selecting previously unselected package crash.
    (Reading database ... 178576 files and directories currently installed.)
    Preparing to unpack .../crash_7.1.4-1ubuntu4.1_amd64.deb ...
    Unpacking crash (7.1.4-1ubuntu4.1) ...
    Selecting previously unselected package libdw1:amd64.
    Preparing to unpack .../libdw1_0.165-3ubuntu1_amd64.deb ...
    Unpacking libdw1:amd64 (0.165-3ubuntu1) ...
    Selecting previously unselected package makedumpfile.
    Preparing to unpack .../makedumpfile_1%3a1.5.9-5ubuntu0.3_amd64.deb ...
    Unpacking makedumpfile (1:1.5.9-5ubuntu0.3) ...
    Selecting previously unselected package kexec-tools.
    Preparing to unpack .../kexec-tools_1%3a2.0.10-1ubuntu2_amd64.deb ...
    Unpacking kexec-tools (1:2.0.10-1ubuntu2) ...
    Selecting previously unselected package kdump-tools.
    Preparing to unpack .../kdump-tools_1%3a1.5.9-5ubuntu0.3_all.deb ...
    Unpacking kdump-tools (1:1.5.9-5ubuntu0.3) ...
    Selecting previously unselected package linux-crashdump.
    Preparing to unpack .../linux-crashdump_4.4.0.62.65_amd64.deb ...
    Unpacking linux-crashdump (4.4.0.62.65) ...
    Processing triggers for man-db (2.7.5-1) ...
    Processing triggers for libc-bin (2.23-0ubuntu3) ...
    Processing triggers for ureadahead (0.100.0-19) ...
    Processing triggers for systemd (229-4ubuntu4) ...
    Setting up crash (7.1.4-1ubuntu4.1) ...
    Setting up libdw1:amd64 (0.165-3ubuntu1) ...
    Setting up makedumpfile (1:1.5.9-5ubuntu0.3) ...
    Setting up kexec-tools (1:2.0.10-1ubuntu2) ...
    Generating /etc/default/kexec...
    Generating grub configuration file ...
    Warning: Setting GRUB_TIMEOUT to a non-zero value when GRUB_HIDDEN_TIMEOUT is set is no longer supported.
    Found linux image: /boot/vmlinuz-4.4.0-21-generic
    Found initrd image: /boot/initrd.img-4.4.0-21-generic
    Found memtest86+ image: /boot/memtest86+.elf
    Found memtest86+ image: /boot/memtest86+.bin
    done
    Setting up kdump-tools (1:1.5.9-5ubuntu0.3) ...
    Setting up linux-crashdump (4.4.0.62.65) ...
    Processing triggers for libc-bin (2.23-0ubuntu3) ...
    Processing triggers for ureadahead (0.100.0-19) ...
    Processing triggers for systemd (229-4ubuntu4) ...


*   Verify 

::

    grep USE_KDUMP  /etc/default/kdump-tools 
    # USE_KDUMP - controls kdump will be configured
    USE_KDUMP=1

Reference
=========

http://blog.zedroot.org/linux-kernel-debuging-using-kdump-and-crash/


