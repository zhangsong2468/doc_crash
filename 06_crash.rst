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

