=================
Process Subsystem
=================


Introduction
=============

Let us start with the process subsystem. Here we will see a couple of data structures and mostly how
the system uses the memory management system. We will see some data structures related to schedular. 

=========
Processes
=========

Processes are the live stated of any executable. If you have an compiler
``a.out`` program and you execute it, it becomes a process.

There are several steps involved in making an executable a process. We will see
all those steps.

A process starts from the ``exec`` system call. A process will start a new
process. There is this ``fork`` system calls which makes a another copy of the
running process. This new copy is called the child process and the main process
is the ``parent`` process.

Once the child process starts, it calls ``exec`` system call with the ``path``
of the new executable. This new executable then starts executing in place of
the child process.

Every running process will have few data structures related to it. The process
will be using some of the system resouces and the process needs to keep a track
of it.  Also the kernel needs to maintain certain things from the process. This
all information is maintained with the process.

``task_struct`` is the major data structure which maintains all the information
related to a process.

``thread_info`` is another data structure which maintains information.

PROCESS - task_struct
=====================

.. todo:: Write detail of task_struct

PROCESS - mm_struct
===================

.. todo:: Write details of mm_struct

PROCESS - vma_struct
====================

.. todo:: Write details of vma_struct


===============
Further Reading
===============

It is highly recommended that you read the "Linux Kernel Development" book before proceeding.
