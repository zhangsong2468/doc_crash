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
    
