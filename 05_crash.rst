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
