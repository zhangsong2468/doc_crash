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


