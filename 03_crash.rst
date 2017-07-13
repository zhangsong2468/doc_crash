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
