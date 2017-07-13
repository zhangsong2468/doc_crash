############################
Introduction To Linux Kernel
############################

.. todo:: Write this Chapter

============
Introduction
============

This chapter will walk you through some of the important data structures in the
linux kernel but this chapter is not intended to be very comprehensive as this
is not what the book has been written.

I have provided references with the data structures which detail the data
structure.

Please read those along with this chapter to have a detailed view of the data structures.

============
Linux Kernel
============

What is Linux Kernel
====================

Simply stated Linux Kernel is software written in ``C Programming Language``
with some parts of it written in ``Assembly``. 

``Kernel`` - its the lowermost layer of an operating system which interacts
with the hardware. On its top sits the user space and on its bottom sits the
``Hardware``. Kernel provides and abstraction layer for the user space
applications to use the hardware. It takes commands from the user space through
``system calls`` and processes data on the hardware to do something meaningful.
Once done the ``system call`` returns back with either the work done or with an
error status clearly stating the reason of failure.  The application checks the
error status and based on the error status proceeds in desired direction.

Role of Kernel
==============

#.  Manage the system resources effeciently so that every body using the system gets their fair share.
#.  Get the desired work done.
#.  Provide mechanisms to the user space applications to communicate with each other.
#.  Provide mechanisms to the user space applications to store the data in files.
#.  To get access to hardware and its full power.


Parts Of A Kernel
=================

As stated in the book ``The Design Of Unix Operating System`` the kernel can be diveded into few major components.

#.  File Subsystem
    
    This block is responsible to provide the system calls which enable the
    applications to manage and store their data in files and directories.

#.  Process Subsystem

    This block is reponsible to privde the system calls to enable applications
    to start running, stop running safely, communicate with each other.

#.  System Call Layer

    This layer is reponsible to interact with the user space applications and
    give them an interface to get work done from the hardware.

#.  Device Driver

    This layer directly talks with the hardware and allows the kernel to use
    device effeciently.


**In this book we will see the File Subsystem, Process Subsystem and System Call Layer in detail.**

==========================================
Introduction to Linux User Space Processes
==========================================




- Introduce library calls, system calls, show static library and dynamic library differences by giving coding examples.

- Also show the four stages of compilation of a small program. Then also show ``objdump`` 

.. note:: Fill in the above
