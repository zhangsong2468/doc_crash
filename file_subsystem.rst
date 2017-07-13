##############
File Subsystem
##############

============
Introduction
============

File Subsystem is responsible for providing the user methods and apis to open/read/write/close and other file related operations.
All the file handling related system calls are provided by the file subsystem.

You need to know atleasr the following operations for a better understadind of the file subsystem.

======
mkfs()
======

=======
mount()
=======


*   Your file will be genrally saved on to a block device - a block device is a
    device which can be read one block at a time. The block size can be anything
    like a 512 Bytes, 1 MB, 4096 Bytes.

*   To make a block device usable under the file subsystem you need to ``mkfs``
    make a file system on the block device. When you make a file system on a block
    device you actually write some initial meta data on the device which makes it
    mountable.

*   When you mount a file system / or a block device - the file subsystem reads
    the block device and then makes the device available under a mentioned folder
    in the system. 

*   The mount system call mounts the block device under a folder. You can see the mounted device by running the ``mount`` command.


======
open()
======

*   Now that you have a file system mounted in a folder - for creating a file or modifying a file you will need to open the file first and then put some data into it using write.

*   For opening a file you need to run the ``open`` system call. 

*   Open system call takes the file name as an input and the mode of the file.

*   Opening a file creates some data struetures in the kernel and returns a file handle to do your work.

======
read()
======

*   Now for reading a file you will need to issue a ``read()`` system call which will read the data from the block device, and fill it in the buffer you have passed to the read system call.

=======
write()
=======

*  Writing data to a open file is quite similar to the read system call where the difference is that the data is copied from the buffer to the kernel and then to the block device.

=======
close()
=======

*  Close just releases the file handle given to you.

======
seek()
======

*   Seek system call changes the current pointer of the data in the file. Using it - you can read the data from a different location in the opened file.


===============
Data Structures
===============

For understanding the above - you will have to understand some of the data
structures which are used in the file subsystem to maintain the required data.

========
vfsmount
========

*   VFS mount in a data structure which is used to keep the information of the
    mounted file systems. It keeps the folder on which a file system is mounted,
    and the SuperBlock object of the mounted file system.

================
VFS - SuperBlock
================

*   SuperBlock object keep the metadata related to the whole file system. It
    keeps the information related to the whole file system.

*   Following information is maintained in the superblock object.q

===========
VFS - inode
===========

*   Inode object keeps the information related to the a particular file or a
    directory.


============
VFS - dentry
============

*   Dentry object keeps information related to a opened or in use folder. It
    also points to its inode.

==========
VFS - file
==========

*   A File object keeps the information related to a file opened by a process.
    It points to the its inode, but here the difference is that the file obejct is
    always in context of the process which has opened the file. You can have
    several file objects pointing to the same inode. For every ``open`` call made
    to same file you will get a new file object but you wont get a new inode object
    for every ``open`` call.

===============
Further Reading
===============

Before you proceed you should give a reading to the ``UTLK`` book.
