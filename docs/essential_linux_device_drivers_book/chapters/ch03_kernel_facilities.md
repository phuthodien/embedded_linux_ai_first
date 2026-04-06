# **Chapter 3. Kernel Facilities**

# **In This Chapter** Kernel Threads 56 Helper Interfaces 65 Looking at the Sources 85

In this chapter, let's look at some kernel facilities that are useful components in a driver developer's toolbox. We start this chapter by looking at a kernel facility that is similar to user processes; kernel threads are programming abstractions oriented toward concurrent processing.

The kernel offers several helper interfaces that simplify your code, eliminate redundancies, increase code readability, and help in long-term maintenance. We will look at linked lists, hash lists, work queues, notifier chains, completion functions, and error-handling aids. These helpers are bug free and optimized, so your driver also inherits those benefits for free.

## **Kernel Threads**

A *kernel thread* is a way to implement background tasks inside the kernel. The task can be busy handling asynchronous events or sleep-waiting for an event to occur. Kernel threads are similar to user processes, except that they live in kernel space and have access to kernel functions and data structures. Like user processes, kernel threads have the illusion of monopolizing the processor because of preemptive scheduling. Many device drivers utilize the services of kernel threads to implement assistant or helper tasks. For example, the *khubd* kernel thread, which is part of the Linux USB driver core (covered in Chapter 11, "Universal Serial Bus") monitors USB hubs and configures USB devices when they are hot-plugged into the system.

## **Creating a Kernel Thread**

Let's learn about kernel threads with the help of an example. While developing the example thread, you will also learn about kernel concepts such as process states, wait queues, and user mode helpers. When you are comfortable with kernel threads, you can use them as a test vehicle for carrying out various experiments within the kernel.

Assume that you would like the kernel to asynchronously invoke a user mode program to send you an email or pager alert, whenever it senses that the health of certain key kernel data structures is deteriorating. (For instance, free space in network receive buffers has dipped below a low watermark.)

This is a candidate for being implemented as a kernel thread for the following reasons:

- It's a background task because it has to wait for asynchronous events.
- It needs access to kernel data structures because the actual detection of events is done by other parts of the kernel.
- It has to invoke a user mode helper program, which is a time-consuming operation.

#### Built-In Kernel Threads

To see the kernel threads (also called *kernel processes*) running on your system, run the ps command. Names of kernel threads are surrounded by square brackets:

| bash> ps -ef |      |  |                        |                        |
|--------------|------|--|------------------------|------------------------|
| UID          |      |  | PID PPID C STIME TTY   | TIME CMD               |
| root         | 1    |  | 0 0 22:36 ?            | 00:00:00 init [3]      |
| root         | 2    |  | 0 0 22:36 ?            | 00:00:00 [kthreadd]    |
| root         | 3    |  | 2 0 22:36 ?            | 00:00:00 [ksoftirqd/0] |
| root         | 4    |  | 2 0 22:36 ?            | 00:00:00 [events/0]    |
| root         | 38   |  | 2 0 22:36 ?            | 00:00:00 [pdflush]     |
| root         | 39   |  | 2 0 22:36 ?            | 00:00:00 [pdflush]     |
| root         | 29   |  | 2 0 22:36 ?            | 00:00:00 [khubd]       |
| root         | 695  |  | 2 0 22:36 ?            | 00:00:00 [kjournald]   |
|              |      |  |                        |                        |
| root         | 3914 |  | 2 0 22:37 ?            | 00:00:00 [nfsd]        |
| root         | 3915 |  | 2 0 22:37 ?            | 00:00:00 [nfsd]        |
|              |      |  |                        |                        |
| root         |      |  | 4015 3364 0 22:55 tty3 | 00:00:00 -bash         |
| root         |      |  | 4066 4015 0 22:59 tty3 | 00:00:00 ps -ef        |

The *[ksoftirqd/0]* kernel thread is an aid to implement *softirqs.* Softirqs are raised by interrupt handlers to request "bottom half" processing of portions of the handler whose execution can be deferred. We take a detailed look at bottom halves and softirqs in Chapter 4, "Laying the Groundwork," but the basic idea here is to allow as little code as possible to be present inside interrupt handlers. Having small interrupt handlers reduces interrupt-off times in the system, resulting in lower latencies. *Ksoftirqd*'s job is to ensure that a high load of softirqs neither starves the softirqs nor overwhelms the system. On *Symmetric Multi Processing* (SMP) machines where multiple thread instances can run on different processors in parallel, one instance of ksoftirqd is created per CPU to improve throughput (*ksoftirqd/n*, where *n* is the CPU number).

The *events/n* threads (where *n* is the CPU number) help implement *work queues*, which are another way of deferring work in the kernel. Parts of the kernel that desire deferred execution of work can either create their own work queue or make use of the default *events/n* worker thread. Work queues are also dissected in Chapter 4.

The task of the *pdflush* kernel thread is to flush out dirty pages from the page cache. The page cache buffers accesses to the disk. To improve performance, actual writes to the disk are delayed until the pdflush daemon writes out dirtied data to disk. This is done if the available free memory dips below a threshold, or if the page has remained dirty for a sufficiently long time. In 2.4 kernels, these two tasks were respectively performed by separate kernel threads, *bdflush* and

*kupdated.* You might have noticed two instances of pdflush in the ps output. A new instance is created if the kernel senses that existing instances have their hands full, servicing disk queues. This improves throughput, especially if your system has multiple disks and many of them are busy.

As you saw in the preceding chapter, kjournald is the generic kernel journaling thread, which is used by filesystems such as EXT3.

The Linux *Network File System* (NFS) server is implemented using a set of kernel threads named *nfsd*.

Our example kernel thread relinquishes the processor until it gets woken up by parts of the kernel responsible for monitoring the data structures of interest. When awake, it invokes a user mode helper program and passes appropriate identity codes in its environment.

To create a kernel thread, use kernel\_thread():

```
ret = kernel_thread(mykthread, NULL,
 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD);
```

The flags specify the resources to be shared between the parent and child threads. CLONE\_FILES specifies that open files are to be shared, and CLONE\_SIGHAND requests that signal handlers be shared.

Listing 3.1 shows the example implementation. Because kernel threads usually act as helpers to device drivers, they are created when the driver is initialized. In this case, however, the example thread can be created from any suitable place, for instance, *init/main.c*.

The thread starts by invoking daemonize(), which performs initial housekeeping and changes the parent of the calling thread to a kernel thread called *kthreadd.* Each Linux thread has a single parent. If a parent process dies without waiting for its child to exit, the child becomes a *zombie* process and wastes resources. Reparenting the child to kthreadd, avoids this and ensures proper cleanup when the thread exits.[1]

[1] In 2.6.21 and earlier kernels, daemonize() reparents the calling thread to the init task by calling reparent\_to\_init().

Because daemonize() blocks all signals by default, use allow\_signal() to enable delivery if your thread desires to handle a particular signal. There are no signal handlers inside the kernel, so use signal\_pending() to check for signals and take appropriate action. For debugging purposes, the code in Listing 3.1 requests delivery of SIGKILL and dies if it's received.

kernel\_thread() is depreciated in favor of the higher-level *kthread* API, which is built over the former. We will look at kthreads later on.

**Listing 3.1. Implementing a Kernel Thread**

```
static DECLARE_WAIT_QUEUE_HEAD(myevent_waitqueue);
rwlock_t myevent_lock;
extern unsigned int myevent_id; /* Holds the identity of the
 troubled data structure.
 Populated later on */
static int mykthread(void *unused)
{
 unsigned int event_id = 0;
 DECLARE_WAITQUEUE(wait, current);
 /* Become a kernel thread without attached user resources */
 daemonize("mykthread");
 /* Request delivery of SIGKILL */
 allow_signal(SIGKILL);
 /* The thread sleeps on this wait queue until it's
 woken up by parts of the kernel in charge of sensing
 the health of data structures of interest */
 add_wait_queue(&myevent_waitqueue, &wait);
 for (;;) {
 /* Relinquish the processor until the event occurs */
 set_current_state(TASK_INTERRUPTIBLE);
 schedule(); /* Allow other parts of the kernel to run */
 /* Die if I receive SIGKILL */
 if (signal_pending(current)) break;
 /* Control gets here when the thread is woken up */
 read_lock(&myevent_lock); /* Critical section starts */
 if (myevent_id) { /* Guard against spurious wakeups */
 event_id = myevent_id;
 read_unlock(&myevent_lock); /* Critical section ends */
 /* Invoke the registered user mode helper and
 pass the identity code in its environment */
 run_umode_handler(event_id); /* Expanded later on */
 } else {
 read_unlock(&myevent_lock);
 }
 }
 set_current_state(TASK_RUNNING);
 remove_wait_queue(&myevent_waitqueue, &wait);
 return 0;
}
```

If you compile and run this as part of the kernel, you can see the newly created thread, *mykthread*, in the ps output:

```
bash> ps -ef
 UID PID PPID C STIME TTY TIME CMD
 root 1 0 0 21:56 ? 00:00:00 init [3]
 root 2 1 0 22:36 ? 00:00:00 [ksoftirqd/0]
 ...
 root 111 1 0 21:56 ? 00:00:00 [mykthread]
 ...
```

Before we delve further into the thread implementation, let's write a code snippet that monitors the health of a data structure of interest and awakens mykthread if a problem condition is detected:

```
/* Executed by parts of the kernel that own the
 data structures whose health you want to monitor */
/* ... */
if (my_key_datastructure looks troubled) {
 write_lock(&myevent_lock); /* Serialize */
 /* Fill in the identity of the data structure */
 myevent_id = datastructure_id;
 write_unlock(&myevent_lock);
 /* Wake up mykthread */
 wake_up_interruptible(&myevent_waitqueue);
}
/* ... */
```

Listing 3.1 executes in process context, whereas the preceding snippet runs from either process or interrupt context. Process and interrupt contexts communicate via kernel data structures. Our example uses myevent\_id and myevent\_waitqueue for this communication. myevent\_id contains the identity of the data structure in trouble. Access to myevent\_id is serialized using locks.

Note that kernel threads are preemptible only if CONFIG\_PREEMPT is turned on at compile time. If CONFIG\_PREEMPT is off, or if you are still running a 2.4 kernel without the preemption patch, your thread will freeze the system if it does not go to sleep. If you comment out schedule() in Listing 3.1 and disable CONFIG\_PREEMPT in your kernel configuration, your system will lock up.

You will learn how to obtain soft real-time responses from kernel threads when we discuss scheduling policies in Chapter 19, "Drivers in User Space."

**Process States and Wait Queues**

Here's the code region from Listing 3.1 that puts mykthread to sleep while waiting for events:

```
add_wait_queue(&myevent_waitqueue, &wait);
for (;;) {
 /* ... */
 set_current_state(TASK_INTERRUPTIBLE);
 schedule(); /* Relinquish the processor */
 /* Point A */
 /* ... */
}
set_current_state(TASK_RUNNING);
remove_wait_queue(&myevent_waitqueue, &wait);
```

The operation of the preceding snippet is based on two concepts: *wait queues* and *process states*.

Wait queues hold threads that need to wait for an event or a system resource. Threads in a wait queue go to sleep until they are woken up by another thread or an interrupt handler that is responsible for detecting the event. Queuing and dequeuing are respectively done using add\_wait\_queue() and remove\_wait\_queue(), and waking up queued tasks is accomplished via wake\_up\_interruptible().

A kernel thread (or a normal process) can be in any of the following process states: *running*, *interruptible*, *uninterruptible*, *zombie*, *stopped*, *traced*, or *dead*. These states are defined in *include*/*linux*/*sched*.*h*:

- A process in the **running** state (TASK\_RUNNING) is in the scheduler run queue and is a candidate for getting CPU time allotted by the scheduler.
- A task in the **interruptible** state (TASK\_INTERRUPTIBLE) is waiting for an event to occur and is not in the scheduler run queue. When the task gets woken up, or if a signal is delivered to it, it re-enters the run queue.
- The **uninterruptible** state (TASK\_UNINTERRUPTIBLE) is similar to the **interruptible** state except that receipt of a signal will not put the task back into the run queue.
- A **stopped** task (TASK\_STOPPED) has stopped execution due to receipt of certain signals.
- If an application such as strace is using the ptrace support in the kernel to intercept a task, it'll be in the **traced** state (TASK\_TRACED).
- A task in the **zombie** state (EXIT\_ZOMBIE) has terminated, but its parent did not wait for the task to complete. An exiting task is either in the EXIT\_ZOMBIE state or the dead (EXIT\_DEAD) state.

You can use set\_current\_state() to set the run state of your kernel thread.

Let's now turn back to the preceding code snippet. mykthread sleeps on a wait queue (myevent\_waitqueue) and changes its state to TASK\_INTERRUPTIBLE, signaling its desire to opt out of the scheduler run queue. The call to schedule() asks the scheduler to choose and run a new task from its run queue. When code responsible for health monitoring wakes up mykthread using wake\_up\_interruptible(&myevent\_waitqueue), the thread is put back into the scheduler run queue. The process state also gets simultaneously changed to TASK\_RUNNING, so there is no race condition even if the wake up occurs between the time the task state is set to TASK\_INTERRUPTIBLE and the time schedule() is called. The thread also gets back into the run queue if a SIGKILL signal is delivered to it. When the scheduler subsequently picks mykthread from the run queue, execution resumes from Point A.

#### **User Mode Helpers**

Mykthread invokes run\_umode\_handler() in Listing 3.1 to notify user space about detected events:

#### Code View: /\* Called from Listing 3.1 \*/ static void run\_umode\_handler(int event\_id) { int i = 0; char \*argv[2], \*envp[4], \*buffer = NULL;

```
 int value;
 argv[i++] = myevent_handler; /* Defined in
 kernel/sysctl.c */
 /* Fill in the id corresponding to the data structure
 in trouble */
 if (!(buffer = kmalloc(32, GFP_KERNEL))) return;
 sprintf(buffer, "TROUBLED_DS=%d", event_id);
 /* If no user mode handlers are found, return */
 if (!argv[0]) return; argv[i] = 0;
 /* Prepare the environment for /path/to/helper */
 i = 0;
 envp[i++] = "HOME=/";
 envp[i++] = "PATH=/sbin:/usr/sbin:/bin:/usr/bin";
 envp[i++] = buffer; envp[i] = 0;
 /* Execute the user mode program, /path/to/helper */
 value = call_usermodehelper(argv[0], argv, envp, 0);
 /* Check return values */
 kfree(buffer);
}
```

The kernel supports a mechanism for requesting user mode programs to help perform certain functions. run\_umode\_handler() uses this facility by invoking call\_usermodehelper().

You have to register the user mode program invoked by run\_umode\_handler() via a node in the */proc/sys/* directory. To do so, make sure that CONFIG\_SYSCTL (files that are part of the */proc/sys/* directory are collectively known as the *sysctl* interface) is enabled in your kernel configuration and add an entry to the kern\_table array in *kernel/sysctl.c*:

```
{
 .ctl_name = KERN_MYEVENT_HANDLER, /* Define in
 include/linux/sysctl.h */
 .procname = "myevent_handler",
 .data = &myevent_handler,
 .maxlen = 256,
 .mode = 0644,
 .proc_handler = &proc_dostring,
 .strategy = &sysctl_string,
},
```

This creates the node */proc/sys/kernel/myevent\_handler* in the process filesystem. To register your user mode helper, do the following:

**bash> echo /path/to/helper > /proc/sys/kernel/myevent\_handler**

This results in */path/to/helper* getting executed when mykthread invokes run\_umode\_handler().

Mykthread passes the identity of the troubled kernel data structure to the user mode helper through the environment variable TROUBLED\_DS. The helper can be a simple script like the following that sends you an email alert containing the information it gleaned from its environment:

#### **bash> cat /path/to/helper**

#!/bin/bash

echo Kernel datastructure \$TROUBLED\_DS is in trouble | mail -s Alert root

call\_usermodehelper() has to be executed from process context and runs with root privileges. It's implemented using a work queue, which we will soon discuss.

# **In This Chapter** Kernel Threads 56 Helper Interfaces 65 Looking at the Sources 85

In this chapter, let's look at some kernel facilities that are useful components in a driver developer's toolbox. We start this chapter by looking at a kernel facility that is similar to user processes; kernel threads are programming abstractions oriented toward concurrent processing.

The kernel offers several helper interfaces that simplify your code, eliminate redundancies, increase code readability, and help in long-term maintenance. We will look at linked lists, hash lists, work queues, notifier chains, completion functions, and error-handling aids. These helpers are bug free and optimized, so your driver also inherits those benefits for free.

## **Kernel Threads**

A *kernel thread* is a way to implement background tasks inside the kernel. The task can be busy handling asynchronous events or sleep-waiting for an event to occur. Kernel threads are similar to user processes, except that they live in kernel space and have access to kernel functions and data structures. Like user processes, kernel threads have the illusion of monopolizing the processor because of preemptive scheduling. Many device drivers utilize the services of kernel threads to implement assistant or helper tasks. For example, the *khubd* kernel thread, which is part of the Linux USB driver core (covered in Chapter 11, "Universal Serial Bus") monitors USB hubs and configures USB devices when they are hot-plugged into the system.

## **Creating a Kernel Thread**

Let's learn about kernel threads with the help of an example. While developing the example thread, you will also learn about kernel concepts such as process states, wait queues, and user mode helpers. When you are comfortable with kernel threads, you can use them as a test vehicle for carrying out various experiments within the kernel.

Assume that you would like the kernel to asynchronously invoke a user mode program to send you an email or pager alert, whenever it senses that the health of certain key kernel data structures is deteriorating. (For instance, free space in network receive buffers has dipped below a low watermark.)

This is a candidate for being implemented as a kernel thread for the following reasons:

- It's a background task because it has to wait for asynchronous events.
- It needs access to kernel data structures because the actual detection of events is done by other parts of the kernel.
- It has to invoke a user mode helper program, which is a time-consuming operation.

#### Built-In Kernel Threads

To see the kernel threads (also called *kernel processes*) running on your system, run the ps command. Names of kernel threads are surrounded by square brackets:

| bash> ps -ef |      |  |                        |                        |
|--------------|------|--|------------------------|------------------------|
| UID          |      |  | PID PPID C STIME TTY   | TIME CMD               |
| root         | 1    |  | 0 0 22:36 ?            | 00:00:00 init [3]      |
| root         | 2    |  | 0 0 22:36 ?            | 00:00:00 [kthreadd]    |
| root         | 3    |  | 2 0 22:36 ?            | 00:00:00 [ksoftirqd/0] |
| root         | 4    |  | 2 0 22:36 ?            | 00:00:00 [events/0]    |
| root         | 38   |  | 2 0 22:36 ?            | 00:00:00 [pdflush]     |
| root         | 39   |  | 2 0 22:36 ?            | 00:00:00 [pdflush]     |
| root         | 29   |  | 2 0 22:36 ?            | 00:00:00 [khubd]       |
| root         | 695  |  | 2 0 22:36 ?            | 00:00:00 [kjournald]   |
|              |      |  |                        |                        |
| root         | 3914 |  | 2 0 22:37 ?            | 00:00:00 [nfsd]        |
| root         | 3915 |  | 2 0 22:37 ?            | 00:00:00 [nfsd]        |
|              |      |  |                        |                        |
| root         |      |  | 4015 3364 0 22:55 tty3 | 00:00:00 -bash         |
| root         |      |  | 4066 4015 0 22:59 tty3 | 00:00:00 ps -ef        |

The *[ksoftirqd/0]* kernel thread is an aid to implement *softirqs.* Softirqs are raised by interrupt handlers to request "bottom half" processing of portions of the handler whose execution can be deferred. We take a detailed look at bottom halves and softirqs in Chapter 4, "Laying the Groundwork," but the basic idea here is to allow as little code as possible to be present inside interrupt handlers. Having small interrupt handlers reduces interrupt-off times in the system, resulting in lower latencies. *Ksoftirqd*'s job is to ensure that a high load of softirqs neither starves the softirqs nor overwhelms the system. On *Symmetric Multi Processing* (SMP) machines where multiple thread instances can run on different processors in parallel, one instance of ksoftirqd is created per CPU to improve throughput (*ksoftirqd/n*, where *n* is the CPU number).

The *events/n* threads (where *n* is the CPU number) help implement *work queues*, which are another way of deferring work in the kernel. Parts of the kernel that desire deferred execution of work can either create their own work queue or make use of the default *events/n* worker thread. Work queues are also dissected in Chapter 4.

The task of the *pdflush* kernel thread is to flush out dirty pages from the page cache. The page cache buffers accesses to the disk. To improve performance, actual writes to the disk are delayed until the pdflush daemon writes out dirtied data to disk. This is done if the available free memory dips below a threshold, or if the page has remained dirty for a sufficiently long time. In 2.4 kernels, these two tasks were respectively performed by separate kernel threads, *bdflush* and

*kupdated.* You might have noticed two instances of pdflush in the ps output. A new instance is created if the kernel senses that existing instances have their hands full, servicing disk queues. This improves throughput, especially if your system has multiple disks and many of them are busy.

As you saw in the preceding chapter, kjournald is the generic kernel journaling thread, which is used by filesystems such as EXT3.

The Linux *Network File System* (NFS) server is implemented using a set of kernel threads named *nfsd*.

Our example kernel thread relinquishes the processor until it gets woken up by parts of the kernel responsible for monitoring the data structures of interest. When awake, it invokes a user mode helper program and passes appropriate identity codes in its environment.

To create a kernel thread, use kernel\_thread():

```
ret = kernel_thread(mykthread, NULL,
 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD);
```

The flags specify the resources to be shared between the parent and child threads. CLONE\_FILES specifies that open files are to be shared, and CLONE\_SIGHAND requests that signal handlers be shared.

Listing 3.1 shows the example implementation. Because kernel threads usually act as helpers to device drivers, they are created when the driver is initialized. In this case, however, the example thread can be created from any suitable place, for instance, *init/main.c*.

The thread starts by invoking daemonize(), which performs initial housekeeping and changes the parent of the calling thread to a kernel thread called *kthreadd.* Each Linux thread has a single parent. If a parent process dies without waiting for its child to exit, the child becomes a *zombie* process and wastes resources. Reparenting the child to kthreadd, avoids this and ensures proper cleanup when the thread exits.[1]

[1] In 2.6.21 and earlier kernels, daemonize() reparents the calling thread to the init task by calling reparent\_to\_init().

Because daemonize() blocks all signals by default, use allow\_signal() to enable delivery if your thread desires to handle a particular signal. There are no signal handlers inside the kernel, so use signal\_pending() to check for signals and take appropriate action. For debugging purposes, the code in Listing 3.1 requests delivery of SIGKILL and dies if it's received.

kernel\_thread() is depreciated in favor of the higher-level *kthread* API, which is built over the former. We will look at kthreads later on.

**Listing 3.1. Implementing a Kernel Thread**

```
static DECLARE_WAIT_QUEUE_HEAD(myevent_waitqueue);
rwlock_t myevent_lock;
extern unsigned int myevent_id; /* Holds the identity of the
 troubled data structure.
 Populated later on */
static int mykthread(void *unused)
{
 unsigned int event_id = 0;
 DECLARE_WAITQUEUE(wait, current);
 /* Become a kernel thread without attached user resources */
 daemonize("mykthread");
 /* Request delivery of SIGKILL */
 allow_signal(SIGKILL);
 /* The thread sleeps on this wait queue until it's
 woken up by parts of the kernel in charge of sensing
 the health of data structures of interest */
 add_wait_queue(&myevent_waitqueue, &wait);
 for (;;) {
 /* Relinquish the processor until the event occurs */
 set_current_state(TASK_INTERRUPTIBLE);
 schedule(); /* Allow other parts of the kernel to run */
 /* Die if I receive SIGKILL */
 if (signal_pending(current)) break;
 /* Control gets here when the thread is woken up */
 read_lock(&myevent_lock); /* Critical section starts */
 if (myevent_id) { /* Guard against spurious wakeups */
 event_id = myevent_id;
 read_unlock(&myevent_lock); /* Critical section ends */
 /* Invoke the registered user mode helper and
 pass the identity code in its environment */
 run_umode_handler(event_id); /* Expanded later on */
 } else {
 read_unlock(&myevent_lock);
 }
 }
 set_current_state(TASK_RUNNING);
 remove_wait_queue(&myevent_waitqueue, &wait);
 return 0;
}
```

If you compile and run this as part of the kernel, you can see the newly created thread, *mykthread*, in the ps output:

```
bash> ps -ef
 UID PID PPID C STIME TTY TIME CMD
 root 1 0 0 21:56 ? 00:00:00 init [3]
 root 2 1 0 22:36 ? 00:00:00 [ksoftirqd/0]
 ...
 root 111 1 0 21:56 ? 00:00:00 [mykthread]
 ...
```

Before we delve further into the thread implementation, let's write a code snippet that monitors the health of a data structure of interest and awakens mykthread if a problem condition is detected:

```
/* Executed by parts of the kernel that own the
 data structures whose health you want to monitor */
/* ... */
if (my_key_datastructure looks troubled) {
 write_lock(&myevent_lock); /* Serialize */
 /* Fill in the identity of the data structure */
 myevent_id = datastructure_id;
 write_unlock(&myevent_lock);
 /* Wake up mykthread */
 wake_up_interruptible(&myevent_waitqueue);
}
/* ... */
```

Listing 3.1 executes in process context, whereas the preceding snippet runs from either process or interrupt context. Process and interrupt contexts communicate via kernel data structures. Our example uses myevent\_id and myevent\_waitqueue for this communication. myevent\_id contains the identity of the data structure in trouble. Access to myevent\_id is serialized using locks.

Note that kernel threads are preemptible only if CONFIG\_PREEMPT is turned on at compile time. If CONFIG\_PREEMPT is off, or if you are still running a 2.4 kernel without the preemption patch, your thread will freeze the system if it does not go to sleep. If you comment out schedule() in Listing 3.1 and disable CONFIG\_PREEMPT in your kernel configuration, your system will lock up.

You will learn how to obtain soft real-time responses from kernel threads when we discuss scheduling policies in Chapter 19, "Drivers in User Space."

**Process States and Wait Queues**

Here's the code region from Listing 3.1 that puts mykthread to sleep while waiting for events:

```
add_wait_queue(&myevent_waitqueue, &wait);
for (;;) {
 /* ... */
 set_current_state(TASK_INTERRUPTIBLE);
 schedule(); /* Relinquish the processor */
 /* Point A */
 /* ... */
}
set_current_state(TASK_RUNNING);
remove_wait_queue(&myevent_waitqueue, &wait);
```

The operation of the preceding snippet is based on two concepts: *wait queues* and *process states*.

Wait queues hold threads that need to wait for an event or a system resource. Threads in a wait queue go to sleep until they are woken up by another thread or an interrupt handler that is responsible for detecting the event. Queuing and dequeuing are respectively done using add\_wait\_queue() and remove\_wait\_queue(), and waking up queued tasks is accomplished via wake\_up\_interruptible().

A kernel thread (or a normal process) can be in any of the following process states: *running*, *interruptible*, *uninterruptible*, *zombie*, *stopped*, *traced*, or *dead*. These states are defined in *include*/*linux*/*sched*.*h*:

- A process in the **running** state (TASK\_RUNNING) is in the scheduler run queue and is a candidate for getting CPU time allotted by the scheduler.
- A task in the **interruptible** state (TASK\_INTERRUPTIBLE) is waiting for an event to occur and is not in the scheduler run queue. When the task gets woken up, or if a signal is delivered to it, it re-enters the run queue.
- The **uninterruptible** state (TASK\_UNINTERRUPTIBLE) is similar to the **interruptible** state except that receipt of a signal will not put the task back into the run queue.
- A **stopped** task (TASK\_STOPPED) has stopped execution due to receipt of certain signals.
- If an application such as strace is using the ptrace support in the kernel to intercept a task, it'll be in the **traced** state (TASK\_TRACED).
- A task in the **zombie** state (EXIT\_ZOMBIE) has terminated, but its parent did not wait for the task to complete. An exiting task is either in the EXIT\_ZOMBIE state or the dead (EXIT\_DEAD) state.

You can use set\_current\_state() to set the run state of your kernel thread.

Let's now turn back to the preceding code snippet. mykthread sleeps on a wait queue (myevent\_waitqueue) and changes its state to TASK\_INTERRUPTIBLE, signaling its desire to opt out of the scheduler run queue. The call to schedule() asks the scheduler to choose and run a new task from its run queue. When code responsible for health monitoring wakes up mykthread using wake\_up\_interruptible(&myevent\_waitqueue), the thread is put back into the scheduler run queue. The process state also gets simultaneously changed to TASK\_RUNNING, so there is no race condition even if the wake up occurs between the time the task state is set to TASK\_INTERRUPTIBLE and the time schedule() is called. The thread also gets back into the run queue if a SIGKILL signal is delivered to it. When the scheduler subsequently picks mykthread from the run queue, execution resumes from Point A.

#### **User Mode Helpers**

Mykthread invokes run\_umode\_handler() in Listing 3.1 to notify user space about detected events:

#### Code View: /\* Called from Listing 3.1 \*/ static void run\_umode\_handler(int event\_id) { int i = 0; char \*argv[2], \*envp[4], \*buffer = NULL;

```
 int value;
 argv[i++] = myevent_handler; /* Defined in
 kernel/sysctl.c */
 /* Fill in the id corresponding to the data structure
 in trouble */
 if (!(buffer = kmalloc(32, GFP_KERNEL))) return;
 sprintf(buffer, "TROUBLED_DS=%d", event_id);
 /* If no user mode handlers are found, return */
 if (!argv[0]) return; argv[i] = 0;
 /* Prepare the environment for /path/to/helper */
 i = 0;
 envp[i++] = "HOME=/";
 envp[i++] = "PATH=/sbin:/usr/sbin:/bin:/usr/bin";
 envp[i++] = buffer; envp[i] = 0;
 /* Execute the user mode program, /path/to/helper */
 value = call_usermodehelper(argv[0], argv, envp, 0);
 /* Check return values */
 kfree(buffer);
}
```

The kernel supports a mechanism for requesting user mode programs to help perform certain functions. run\_umode\_handler() uses this facility by invoking call\_usermodehelper().

You have to register the user mode program invoked by run\_umode\_handler() via a node in the */proc/sys/* directory. To do so, make sure that CONFIG\_SYSCTL (files that are part of the */proc/sys/* directory are collectively known as the *sysctl* interface) is enabled in your kernel configuration and add an entry to the kern\_table array in *kernel/sysctl.c*:

```
{
 .ctl_name = KERN_MYEVENT_HANDLER, /* Define in
 include/linux/sysctl.h */
 .procname = "myevent_handler",
 .data = &myevent_handler,
 .maxlen = 256,
 .mode = 0644,
 .proc_handler = &proc_dostring,
 .strategy = &sysctl_string,
},
```

This creates the node */proc/sys/kernel/myevent\_handler* in the process filesystem. To register your user mode helper, do the following:

**bash> echo /path/to/helper > /proc/sys/kernel/myevent\_handler**

This results in */path/to/helper* getting executed when mykthread invokes run\_umode\_handler().

Mykthread passes the identity of the troubled kernel data structure to the user mode helper through the environment variable TROUBLED\_DS. The helper can be a simple script like the following that sends you an email alert containing the information it gleaned from its environment:

#### **bash> cat /path/to/helper**

#!/bin/bash

echo Kernel datastructure \$TROUBLED\_DS is in trouble | mail -s Alert root

call\_usermodehelper() has to be executed from process context and runs with root privileges. It's implemented using a work queue, which we will soon discuss.

# **Helper Interfaces**

Several useful helper interfaces exist in the kernel to make life easier for device driver developers. One example is the implementation of the doubly linked list library. Many drivers need to maintain and manipulate linked lists of data structures. The kernel's *list* interface routines eliminate the need for chasing list pointers and debugging messy problems related to list maintenance. Let's learn to use helper interfaces such as lists, hlists, work queues, completion functions, notifier blocks, and kthreads.

There are equivalent ways to do what the helper facilities offer. You can, for example, implement your own list manipulation routines instead of using the list library, or use kernel threads to defer work instead of submitting it to work queues. Using standard kernel helper interfaces, however, simplifies your code, weeds out redundancies from the kernel, increases code readability, and helps long-term maintenance.

Because the kernel is vast, you can always find parts that do not yet take advantage of these helper mechanisms, so updating those code regions might be a good way to start contributing to kernel development.

### **Linked Lists**

To weave doubly linked lists of data structures, use the functions provided in *include/linux/list.h.* Essentially, you embed a struct list\_head inside your data structure:

```
#include <linux/list.h>
struct list_head {
 struct list_head *next, *prev;
};
struct mydatastructure {
 struct list_head mylist; /* Embed */
 /* ... */ /* Actual Fields */
};
```

mylist is the link that chains different instances of mydatastructure. If you have multiple list\_heads embedded inside mydatastructure, each of them constitutes a link that renders mydatastructure a member of a new list. You can use the list library to add or delete membership from individual lists.

To get the lay of the land before the detail, let's summarize the linked list programming interface offered by the list library. This is done in Table 3.1.

**Table 3.1. Linked List Manipulation Functions**

| Function         | Purpose                             |
|------------------|-------------------------------------|
| INIT_LIST_HEAD() | Initializes the list head           |
| list_add()       | Adds an element after the list head |

| Function                                             | Purpose                                           |
|------------------------------------------------------|---------------------------------------------------|
| list_add_tail()                                      | Adds an element to the tail of the list           |
| list_del()                                           | Deletes an element from the list                  |
| list_replace()                                       | Replaces an element in the list with another      |
| list_entry()                                         | Loops through all nodes in the list               |
| list_for_each_entry()/<br>list_for_each_entry_safe() | Simpler list iteration interfaces                 |
| list_empty()                                         | Checks whether there are any elements in the list |
| list_splice()                                        | Joins one list with another                       |

To illustrate list usage, let's implement an example. The example also serves as a foundation to understand the concept of work queues, which is discussed in the next section. Assume that your kernel driver needs to perform a heavy-duty task from an entry point. An example is a task that forces the calling thread to sleep-wait. Naturally, your driver doesn't like to block until the task finishes, because that slows down the responsiveness of applications relying on it. So, whenever the driver needs to perform this expensive work, it defers execution by adding the corresponding routine to a linked list of work functions. The actual work is performed by a kernel thread, which traverses the list and executes the work functions in the background. The driver submits work functions to the tail of the list, while the kernel thread ploughs its way from the head of the list, thus ensuring that work queued first gets done first. Of course, the rest of the driver needs to be designed to suit this scheme of deferred execution. Before understanding this example, however, be aware that we will use the work queue interface in Listing 3.5 to implement the same task in a simpler manner.

Let's first introduce the key driver data structures used by our example:

```
static struct _mydrv_wq {
 struct list_head mydrv_worklist; /* Work List */
 spinlock_t lock; /* Protect the list */
 wait_queue_head_t todo; /* Synchronize submitter
 and worker */
} mydrv_wq;
struct _mydrv_work {
 struct list_head mydrv_workitem; /* The work chain */
 void (*worker_func)(void *); /* Work to perform */
 void *worker_data; /* Argument to worker_func */
 /* ... */ /* Other fields */
} mydrv_work;
```

mydrv\_wq is global to all work submissions. Its members include a pointer to the head of the work list, and a wait queue to communicate between driver functions that submit work and the kernel thread that performs the work. The list helper functions do not protect accesses to list members, so you need to use concurrency mechanisms to serialize simultaneous pointer references. This is done using a spinlock that is also a part of mydrv\_wq. The driver initialization routine mydrv\_init() in Listing 3.2 initializes the spinlock, the list head, and the wait queue, and kick starts the worker thread.

#### **Listing 3.2. Initialize Data Structures**

```
static int __init
mydrv_init(void)
{
 /* Initialize the lock to protect against
 concurrent list access */
 spin_lock_init(&mydrv_wq.lock);
 /* Initialize the wait queue for communication
 between the submitter and the worker */
 init_waitqueue_head(&mydrv_wq.todo);
 /* Initialize the list head */
 INIT_LIST_HEAD(&mydrv_wq.mydrv_worklist);
 /* Start the worker thread. See Listing 3.4 */
 kernel_thread(mydrv_worker, NULL,
 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD);
 return 0;
}
```

Before examining the worker thread that executes submitted work, let's look at work submission itself. Listing 3.3 implements a function that other parts of the kernel can use to submit work. It uses list\_add\_tail() to add a work function to the tail of the list. Look at Figure 3.1 to see the physical structure of the work list.

```
mvd rv worklist-------------------------------------------
     |                                                   |
     v                                                   |
+-----------------+                                      |
|    mydrv_wq     |                                      |
+-----------------+                                      |
        ^                                                |
        |                                                |
        v                                                |
+-----------------+    <---->   +-----------------+      |
|    mydrv_work   | <---------> |    mydrv_work   |<-----|
|                 |             |                 |
+-----------------+             +-----------------+
                                         ^
                                         |
                                         |
                                         v
                          Membership in another list
                          (Not shown in structure definition)
```

**Figure 3.1. Linked list of work functions.**

**Listing 3.3. Submitting Work to Be Executed Later**

```
int
submit_work(void (*func)(void *data), void *data)
{
 struct _mydrv_work *mydrv_work;
 /* Allocate the work structure */
 mydrv_work = kmalloc(sizeof(struct _mydrv_work), GFP_ATOMIC);
 if (!mydrv_work) return -1;
 /* Populate the work structure */
 mydrv_work->worker_func = func; /* Work function */
 mydrv_work->worker_data = data; /* Argument to pass */
 spin_lock(&mydrv_wq.lock); /* Protect the list */
 /* Add your work to the tail of the list */
 list_add_tail(&mydrv_work->mydrv_workitem,
 &mydrv_wq.mydrv_worklist);
 /* Wake up the worker thread */
 wake_up(&mydrv_wq.todo);
 spin_unlock(&mydrv_wq.lock);
 return 0;
}
```

To submit a work function void job(void \*) from a driver entry point, do this:

```
submit_work(job, NULL);
```

After submitting the work function, Listing 3.3 wakes up the worker thread. The general structure of the worker thread shown in Listing 3.4 is similar to standard kernel threads discussed in the previous section. The thread uses list\_entry() to work its way through all nodes in the list. list\_entry() returns the container data structure inside which the list node is embedded. Take a closer look at the relevant line in Listing 3.4:

```
mydrv_work = list_entry(mydrv_wq.mydrv_worklist.next,
 struct _mydrv_work, mydrv_workitem);
```

mydrv\_workitem is embedded inside mydrv\_work, so list\_entry() returns a pointer to the corresponding mydrv\_work structure. The parameters passed to list\_entry() are the address of the embedded list node, the type of the container structure, and the field name of the embedded list node.

After executing a submitted work function, the worker thread removes the corresponding node from the list using list\_del(). Note that mydrv\_wq.lock is released and reacquired in the time window when the submitted work function is executed. This is because work functions can go to sleep resulting in potential deadlocks if newly scheduled code tries to acquire the same spinlock.

#### **Listing 3.4. The Worker Thread**

```
static int
mydrv_worker(void *unused)
{
 DECLARE_WAITQUEUE(wait, current);
 void (*worker_func)(void *);
 void *worker_data;
 struct _mydrv_work *mydrv_work;
 set_current_state(TASK_INTERRUPTIBLE);
 /* Spin until asked to die */
 while (!asked_to_die()) {
 add_wait_queue(&mydrv_wq.todo, &wait);
 if (list_empty(&mydrv_wq.mydrv_worklist)) {
 schedule();
 /* Woken up by the submitter */
 } else {
 set_current_state(TASK_RUNNING);
 }
 remove_wait_queue(&mydrv_wq.todo, &wait);
 /* Protect concurrent access to the list */
 spin_lock(&mydrv_wq.lock);
 /* Traverse the list and plough through the work functions
 present in each node */
 while (!list_empty(&mydrv_wq.mydrv_worklist)) {
 /* Get the first entry in the list */
 mydrv_work = list_entry(mydrv_wq.mydrv_worklist.next,
 struct _mydrv_work, mydrv_workitem);
 worker_func = mydrv_work->worker_func;
 worker_data = mydrv_work->worker_data;
 /* This node has been processed. Throw it
 out of the list */
 list_del(mydrv_wq.mydrv_worklist.next);
 kfree(mydrv_work); /* Free the node */
 /* Execute the work function in this node */
 spin_unlock(&mydrv_wq.lock); /* Release lock */
 worker_func(worker_data);
 spin_lock(&mydrv_wq.lock); /* Re-acquire lock */
 }
 spin_unlock(&mydrv_wq.lock);
 set_current_state(TASK_INTERRUPTIBLE);
 }
 set_current_state(TASK_RUNNING);
 return 0;
}
```

For simplicity, the example code does not perform error handling. For example, if the call to kernel\_thread() in Listing 3.2 fails, you need to free memory allocated for the corresponding work structure. Also, asked\_to\_die() in Listing 3.4 is left unwritten. It essentially breaks out of the loop if it either detects a delivered signal or receives a communication from the release() entry point that the module is about to be unloaded from the kernel.

Before ending this section, let's take a look at another useful list library routine, list\_for\_each\_entry(). With this macro, iteration becomes simpler and more readable because you don't have to use list\_entry() inside the loop. Use the list\_for\_each\_entry\_safe() variant if you will delete list elements inside the loop. You can replace the following snippet in Listing 3.4:

```
while (!list_empty(&mydrv_wq.mydrv_worklist)) {
 mydrv_work = list_entry(mydrv_wq.mydrv_worklist.next,
 struct _mydrv_work, mydrv_workitem);
 /* ... */
}
with:
struct _mydrv_work *temp;
list_for_each_entry_safe(mydrv_work, temp,
 &mydrv_wq.mydrv_worklist,
 mydrv_workitem) {
 /* ... */
}
```

You can't use list\_for\_each\_entry() in this case because you are removing the entry pointed to by mydrv\_work inside the loop in Listing 3.4. list\_for\_each\_entry\_safe() solves this problem using the temporary variable passed as the second argument (temp) to save the address of the next entry in the list.

### **Hash Lists**

The doubly linked list implementation discussed previously is not optimal for cases where you want to implement linked data structures such as hash tables. This is because hash tables need only a list head containing a single pointer. To reduce memory overhead for such applications, the kernel provides hash lists (or *hlists*), a variation of lists. Unlike lists, which use the same structure for the list head and list nodes, hlists have separate definitions:

```
struct hlist_head {
 struct hlist_node *first;
};
struct hlist_node {
 struct hlist_node *next, **pprev;
};
```

To suit the scheme of a single-pointer hlist head, the nodes maintain the address of the pointer to the previous node, rather than the pointer itself.

Hash tables are implemented using an array of hlist\_heads. Each hlist\_head sources a doubly linked list of hlist\_nodes. A hash function is used to locate the index (or bucket) in the hlist\_head array. When that is done, you may use hlist helper routines (also defined in *include/linux/list.h*) to operate on the list of hlist\_nodes linked to the chosen bucket. Look at the implementation of the directory cache (dcache) in

*fs/dcache.c* for an example.

### **Work Queues**

Work queues are a way to defer work inside the kernel.[2] Deferring work is useful in innumerable situations. Examples include the following:

[2] Softirqs and tasklets are two other mechanisms available for deferring work inside the kernel. Table 4.1 of Chapter 4 compares softirqs, tasklets, and work queues.

- Triggering restart of a network adapter in response to an error interrupt
- Filesystem tasks such as syncing disk buffers
- Sending a command to a disk and following through with the storage protocol state machine

The functionality of work queues is similar to the example described in Listings 3.2 to 3.4. However, work queues can help you accomplish the same task in a simpler manner.

The work queue helper library exposes two interface structures to users: a workqueue\_struct and a work\_struct. Follow these steps to use work queues:

- **1.** Create a work queue (or a workqueue\_struct) with one or more associated kernel threads. To create a kernel thread to service a workqueue\_struct, use create\_singlethread\_workqueue(). To create one worker thread per CPU in the system, use the create\_workqueue() variant. The kernel also has default per-CPU worker threads (*events/n*, where *n* is the CPU number) that you can timeshare instead of requesting a separate worker thread. Depending on your application, you might incur a performance hit if you don't have a dedicated worker thread.
- **2.** Create a work element (or a work\_struct). A work\_struct is initialized using INIT\_WORK(), which populates it with the address and argument of your work function.
- **3.** Submit the work element to the work queue. A work\_struct can be submitted to a dedicated queue using queue\_work(), or to the default kernel worker thread using schedule\_work().

Let's rewrite Listings 3.2 to 3.4 to take advantage of the work queue interface. This is done in Listing 3.5. The entire kernel thread, as well as the spinlock and the wait queue, vanish inside the work queue interface. Even the call to create\_singlethread\_workqueue() goes away if you are using the default kernel worker thread.

**Listing 3.5. Using Work Queues to Defer Work**

```
#include <linux/workqueue.h>
struct workqueue_struct *wq;
/* Driver Initialization */
static int __init
mydrv_init(void)
{
 /* ... */
 wq = create_singlethread_workqueue("mydrv");
 return 0;
}
 /* Work Submission. The first argument is the work function, and
 the second argument is the argument to the work function */
 int
 submit_work(void (*func)(void *data), void *data)
 {
 struct work_struct *hardwork;
 hardwork = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
 /* Init the work structure */
 INIT_WORK(hardwork, func, data);
 /* Enqueue Work */
 queue_work(wq, hardwork);
 return 0;
}
```

If you are using work queues, you will get linker errors unless you declare your module as licensed under GPL. This is because the kernel exports these functions only to GPLed code. If you look at the kernel work queue implementation, you will see this restriction expressed in statements such as this:

```
EXPORT_SYMBOL_GPL(queue_work);
```

To announce that your module is *copyleft*-ed under GPL, declare the following:

```
MODULE_LICENSE("GPL");
```

### **Notifier Chains**

*Notifier chains* are used to send status change messages to code regions that request them. Unlike hard-coded mechanisms, notifiers offer a versatile technique for getting alerted when events of interest are generated. Notifiers were originally added for passing network events to concerned sections of the kernel but are now used for many other purposes. The kernel implements predefined notifiers for significant events. Examples of such notifications include the following:

- **Die notification**, which is sent when a kernel function triggers a trap or a fault, caused by an "oops," page fault, or a breakpoint hit. If you are, for example, writing a device driver for a medical grade card, you might want to register yourself with the die notifier so that you can attempt to turn off the medical electronics if a kernel panic occurs.
- **Net device notification**, which is generated when a network interface goes up or down.
- **CPU frequency notification**, which is dispatched when there is a transition in the processor frequency.
- **Internet address notification**, which is sent when a change is detected in the IP address of a network interface.

An example user of notifiers is the *High-level Data Link Control* (HDLC) protocol driver *drivers/net/wan/hdlc.c*, which registers itself with the net device notifier chain to sense carrier changes.

To attach your code to a notifier chain, you have to register an event handler with the associated chain. An event identifier and a notifier-specific argument are passed as arguments to the handler routine when the concerned event is generated. To define a custom notifier chain, you have to additionally implement the infrastructure to ignite the chain when the event is detected.

Listing 3.6 contains examples of using predefined and user-defined notifiers. Table 3.2 contains a brief description of the notifier chains used by Listing 3.6 and the events they propagate, so look at the listing and the table in tandem.

**Table 3.2. Notifier Chains and the Events They Propagate**

| Notifier Chain | Description |
|----------------|-------------|
|----------------|-------------|

**Die Notifier Chain** (die\_chain) my\_die\_event\_handler() attaches to the die notifier chain, die\_chain, using register\_die\_notifier(). To trigger invocation of my\_die\_event\_handler(), introduce an invalid dereference somewhere in your code, such as the following:

int \*q = 0; \*q = 1;

When this code snippet is executed, my\_die\_event\_handler() gets called, and you will see a message like this:

```
my_die_event_handler: OOPs! at EIP=f00350e7
```

The die event notifier passes the die\_args structure to the registered event handler. This argument contains a pointer to the regs structure, which carries a snapshot of processor register contents when the fault occurred. my\_die\_event\_handler() prints the contents of the instruction pointer register.

**Netdevice Notifier Chain**(netdev\_chain) my\_dev\_event\_handler() attaches to the net device notifier chain, netdev\_chain, using register\_netdevice\_notifier(). You can generate this event by changing the state of a network interface such as Ethernet (ethX) or loopback (lo):

**bash> ifconfig eth0 up**

This results in the execution of my\_dev\_event\_handler(). The handler is passed a pointer to struct net\_device as argument, which contains the name of the network interface. my\_dev\_event\_handler() uses this information to produce the following message:

```
my_dev_event_handler: Val=1, Interface=eth0
```

Val=1 corresponds to the NETDEV\_UP event as defined in *include/linux/notifier.h*.

**User-Defined Notifier Chain** Listing 3.6 also implements a user-defined notifier chain, my\_noti\_chain. Assume that you want an event to be generated whenever a user reads from a particular file in the process filesystem. Add the following in the associated procfs read routine: blocking\_notifier\_call\_chain(&my\_noti\_chain, 100, NULL);

> This results in the invocation of my\_event\_handler() whenever you read from the corresponding */proc* file and results in the following message:

```
my_event_handler: Val=100
```

Val contains the identity of the generated event, which is 100 for this example. The function argument is left unused.

You have to unregister event handlers from notifier chains when your module is released from the kernel. For example, if you *up* or *down* a network interface after unloading the code in Listing 3.6, you will be rankled by an "oops," unless you perform an unregister\_netdevice\_notifier(&my\_dev\_notifier) from the module's release() method. This is because the notifier chain continues to think that the handler code is valid, even though it has been pulled out of the kernel.

#### **Listing 3.6. Notifier Event Handlers**

```
#include <linux/notifier.h>
#include <asm/kdebug.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
/* Die Notifier Definition */
static struct notifier_block my_die_notifier = {
 .notifier_call = my_die_event_handler,
};
/* Die notification event handler */
int
my_die_event_handler(struct notifier_block *self,
 unsigned long val, void *data)
{
 struct die_args *args = (struct die_args *)data;
 if (val == 1) { /* '1' corresponds to an "oops" */
 printk("my_die_event: OOPs! at EIP=%lx\n", args->regs->eip);
```

```
 } /* else ignore */
 return 0;
}
/* Net Device notifier definition */
static struct notifier_block my_dev_notifier = {
 .notifier_call = my_dev_event_handler,
};
/* Net Device notification event handler */
int my_dev_event_handler(struct notifier_block *self,
 unsigned long val, void *data)
{
 printk("my_dev_event: Val=%ld, Interface=%s\n", val,
 ((struct net_device *) data)->name);
 return 0;
}
/* User-defined notifier chain implementation */
static BLOCKING_NOTIFIER_HEAD(my_noti_chain);
static struct notifier_block my_notifier = {
 .notifier_call = my_event_handler,
};
/* User-defined notification event handler */
int my_event_handler(struct notifier_block *self,
 unsigned long val, void *data)
{
 printk("my_event: Val=%ld\n", val);
 return 0;
}
/* Driver Initialization */
static int __init
my_init(void)
{
 /* ... */
 /* Register Die Notifier */
 register_die_notifier(&my_die_notifier);
 /* Register Net Device Notifier */
 register_netdevice_notifier(&my_dev_notifier);
 /* Register a user-defined Notifier */
 blocking_notifier_chain_register(&my_noti_chain, &my_notifier);
 /* ... */
}
```

my\_noti\_chain in Listing 3.6 is declared as a blocking notifier using BLOCKING\_NOTIFIER\_HEAD() and is registered via blocking\_notifier\_chain\_register(). This means that the notifier handler is always invoked from process context. So, the handler function, my\_event\_handler(), is allowed to go to sleep. If your notifier handler can be called from interrupt context, declare the notifier chain using ATOMIC\_NOTIFIER\_HEAD(), and register it via atomic\_notifier\_chain\_register().

#### The Old Notifier Interface

Kernel releases earlier than 2.6.17 supported only a general-purpose notifier chain. The notifier registration function notifier\_chain\_register() was internally protected using a spinlock, but the routine that walked the notifier chain dispatching events to notifier handlers (notifier\_call\_chain()) was lockless. The lack of locking was because of the possibility that the handler functions may go to sleep, unregister themselves while running, or get called from interrupt context. The lockless implementation introduced race conditions, however. The new notifier API is built over the original interface and is intended to overcome its limitations.

### **Completion Interface**

Many parts of the kernel initiate certain activities as separate execution threads and then wait for them to complete. The *completion* interface is an efficient and easy way to implement such code patterns.

Some example usage scenarios include the following:

- Your driver module is assisted by a kernel thread. If you rmmod the module, the release() method is invoked before removing the module code from kernel space. The release routine asks the thread to kill itself and blocks until the thread completes its exit. Listing 3.7 implements this case.
- You are writing a portion of a block device driver (discussed in Chapter 14, "Block Drivers") that queues a read request to a device. This triggers a state machine change implemented as a separate thread or work queue. The driver wants to wait until the operation completes before proceeding with another activity. Look at *drivers/block/floppy.c* for an example.
- An application requests an *Analog-to-Digital Converter* (ADC) driver for a data sample. The driver initiates a conversion request waits, until an interrupt signals completion of conversion, and returns the data.

#### **Listing 3.7. Synchronizing Using Completion Functions**

```
static DECLARE_COMPLETION(my_thread_exit); /* Completion */
static DECLARE_WAIT_QUEUE_HEAD(my_thread_wait); /* Wait Queue */
int pink_slip = 0; /* Exit Flag */
/* Helper thread */
static int
my_thread(void *unused)
{
 DECLARE_WAITQUEUE(wait, current);
 daemonize("my_thread");
 add_wait_queue(&my_thread_wait, &wait);
 while (1) {
 /* Relinquish processor until event occurs */
```

```
 set_current_state(TASK_INTERRUPTIBLE);
 schedule();
 /* Control gets here when the thread is woken
 up from the my_thread_wait wait queue */
 /* Quit if let go */
 if (pink_slip) {
 break;
 }
 /* Do the real work */
 /* ... */
 }
 /* Bail out of the wait queue */
 __set_current_state(TASK_RUNNING);
 remove_wait_queue(&my_thread_wait, &wait);
 /* Atomically signal completion and exit */
 complete_and_exit(&my_thread_exit, 0);
}
/* Module Initialization */
static int __init
my_init(void)
{
 /* ... */
 /* Kick start the thread */
 kernel_thread(my_thread, NULL,
 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD);
 /* ... */
}
/* Module Release */
static void __exit
my_release(void)
{
 /* ... */
 pink_slip = 1; /* my_thread must go */
 wake_up(&my_thread_wait); /* Activate my_thread */
 wait_for_completion(&my_thread_exit); /* Wait until my_thread
 quits */
 /* ... */
}
```

A completion object can be declared statically using DECLARE\_COMPLETION() or created dynamically with init\_completion(). A thread can signal completion with the help of complete() or complete\_all(). A caller can wait for completion via wait\_for\_completion().

In Listing 3.7, my\_release() raises an exit request flag by setting pink\_slip before waking up my\_thread(). It then calls wait\_for\_completion() to wait until my\_thread() completes its exit. my\_thread(), on its part, wakes up to find pink\_slip set, and does the following:

- **1.** Signals completion to my\_release()
- **2.** Kills itself

my\_thread() accomplishes these two steps atomically using complete\_and\_exit(). Using complete\_and\_exit() shuts the window between module exit and thread exit that opens if you separately invoke complete() and exit().

We will use the completion API when we develop an example telemetry driver in Chapter 11.

### **Kthread Helpers**

*Kthread helpers* add a coating over the raw thread creation routines and simplify the task of thread management.

Listing 3.8 rewrites Listing 3.7 using the kthread helper interface. my\_init() now uses kthread\_create() rather than kernel\_thread(). You can pass the thread's name to kthread\_create() rather than explicitly call daemonize() within the thread.

The kthread interface provides you free access to a built-in exit synchronization mechanism implemented using the completion interface. So, as my\_release() does in Listing 3.8, you may directly call kthread\_stop() instead of laboriously setting pink\_slip, waking up my\_thread(), and waiting for it to complete using wait\_for\_completion(). Similarly, my\_thread() can make a neat call to kthread\_should\_stop() to check whether it ought to call it a day.

**Listing 3.8. Synchronizing Using Kthread Helpers**

```
/* '+' and '-' show the differences from Listing 3.7 */
#include <linux/kthread.h>
/* Assistant Thread */
static int
my_thread(void *unused)
{
 DECLARE_WAITQUEUE(wait, current);
- daemonize("my_thread");
- while (1) {
+ /* Continue work if no other thread has
+ * invoked kthread_stop() */
+ while (!kthread_should_stop()) {
 /* ... */
- /* Quit if let go */
- if (pink_slip) {
- break;
- }
 /* ... */
 }
 __set_current_state(TASK_RUNNING);
 remove_wait_queue(&my_thread_wait, &wait);
```

```
- complete_and_exit(&my_thread_exit, 0);
+ return 0;
 }
+ struct task_struct *my_task;
 /* Module Initialization */
 static int __init
 my_init(void)
 {
 /* ... */
- kernel_thread(my_thread, NULL,
- CLONE_FS | CLONE_FILES | CLONE_SIGHAND |
 SIGCHLD);
+ my_task = kthread_create(my_thread, NULL, "%s", "my_thread");
+ if (my_task) wake_up_process(my_task);
 /* ... */
 }
 /* Module Release */
 static void __exit
 my_release(void)
 {
 /* ... */
- pink_slip = 1;
- wake_up(&my_thread_wait);
- wait_for_completion(&my_thread_exit);
+ kthread_stop(my_task);
 /* ... */
 }
```

Instead of creating the thread using kthread\_create() and activating it via wake\_up\_process() as done in Listing 3.8, you may use the following single call:

```
kthread_run(my_thread, NULL, "%s", "my_thread");
```

#### **Error-Handling Aids**

Several kernel functions return pointer values. Whereas callers usually check for failure by comparing the return value with NULL, they typically need more information to decipher the exact nature of the error that has occurred. Because kernel addresses have redundant bits, they can be overloaded to encode error semantics. This is done with the help of a set of helper routines. Listing 3.9 implements a simple usage example.

#### **Listing 3.9. Using Error-Handling Aids**

```
#include <linux/err.h>
char *
collect_data(char *userbuffer)
{
 char *buffer;
 /* ... */
 buffer = kmalloc(100, GFP_KERNEL);
 if (!buffer) { /* Out of memory */
 return ERR_PTR(-ENOMEM);
 }
 /* ... */
 if (copy_from_user(buffer, userbuffer, 100)) {
 return ERR_PTR(-EFAULT);
 }
 /* ... */
 return(buffer);
}
int
my_function(char *userbuffer)
{
 char *buf;
 /* ... */
 buf = collect_data(userbuffer);
 if (IS_ERR(buf)) {
 printk("Error returned is %d!\n", PTR_ERR(buf));
 }
 /* ... */
}
```

If kmalloc() fails inside collect\_data() in Listing 3.9, you will get the following message:

```
Error returned is -12!
```

However, if collect\_data() is successful, it returns a valid pointer to a data buffer. As another example, let's add error handling using IS\_ERR() and PTR\_ERR() to the thread creation code in Listing 3.8:

```
 my_task = kthread_create(my_thread, NULL, "%s", "mydrv");
+ if (!IS_ERR(my_task)) {
+ /* Success */
 wake_up_process(my_task);
+ } else {
+ /* Failure */
```

```
+ printk("Error value returned=%d\n", PTR_ERR(my_task));
+ }
```

# **Looking at the Sources**

The ksoftirqd, pdflush, and khubd kernel threads live in *kernel/softirq.c*, *mm/pdflush.c*, and *drivers/usb/core/hub.c*, respectively.

The daemonize() function can be found in *kernel/exit.c.* For the implementation of user mode helpers, look at *kernel/kmod.c*.

The list and hlist library routines reside in *include/linux/list.h*. They are used all over the kernel, so you will find usage examples in most subdirectories. An example is the request\_queue structure defined in *include/linux/blkdev.h*, which holds a linked list of disk I/O requests. We look at this data structure in Chapter 14.

Go to www.ussg.iu.edu/hypermail/linux/kernel/0007.3/0805.html and follow the discussion thread in the mailing list for an interesting debate between Linus Torvalds and Andi Kleen about the pros and cons of complementing the list library with hlist helper routines.

The kernel work queue implementation lives in *kernel/workqueue.c.* To understand the real-world use of work queues, look at the PRO/Wireless 2200 network driver, *drivers/net/wireless/ipw2200.c*.

The kernel notifier chain implementation lives in *kernel/sys.c* and *include/linux/notifier.h.* Look at *kernel/sched.c* and *include/linux/completion.h* for the guts of the completion interface. *kernel/kthread.c* contains the source code for kthread helpers, and *include/linux/err.h* includes definitions of error handling aids.

Table 3.3 contains a summary of the main data structures used in this chapter and the location of their definitions in the source tree. Table 3.4 lists the main kernel programming interfaces that you used in this chapter along with the location of their definitions.

|                | Table 3.3. Summary of Data Structures |                                                                                                                     |  |  |
|----------------|---------------------------------------|---------------------------------------------------------------------------------------------------------------------|--|--|
| Data Structure | Location                              | Description                                                                                                         |  |  |
| wait_queue_t   | include/linux/wait.h                  | Used by threads that desire to wait for<br>an event or a system resource                                            |  |  |
| list_head      | include/linux/list.h                  | Kernel structure to weave a doubly<br>linked list of data structures                                                |  |  |
| hlist_head     | include/linux/list.h                  | Kernel structure to implement hash<br>tables                                                                        |  |  |
| work_struct    |                                       | include/linux/workqueue.h Implements work queues, which are a<br>way to defer work inside the kernel                |  |  |
| notifier_block | include/linux/notifier.h              | Implements notifier chains, which are<br>used to send status changes to code<br>regions that request them           |  |  |
| completion     |                                       | include/linux/completion.h Used to initiate activities as separate<br>threads and then wait for them to<br>complete |  |  |

**Table 3.4. Summary of Kernel Programming Interfaces**

| Kernel Interface                                | Location                               | Description                                                                                                                                                                              |
|-------------------------------------------------|----------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| DECLARE_WAITQUEUE()                             | include/linux/wait.h                   | Declares a wait queue.                                                                                                                                                                   |
| add_wait_queue()                                | kernel/wait.c                          | Queues a task to a wait queue.<br>The task goes to sleep until it's<br>woken up by another thread or<br>interrupt handler.                                                               |
| remove_wait_queue()                             | kernel/wait.c                          | Dequeues a task from a wait<br>queue.                                                                                                                                                    |
| wake_up_interruptible()                         | include/linux/wait.h<br>kernel/sched.c | Wakes up a task sleeping inside<br>a wait queue and puts it back<br>into the scheduler run queue.                                                                                        |
| schedule()                                      | kernel/sched.c                         | Relinquishes the processor and<br>allows other parts of the kernel<br>to run.                                                                                                            |
| set_current_state()                             | include/linux/sched.h                  | Sets the run state of a process.<br>The state can be one of<br>TASK_RUNNING,<br>TASK_INTERRUPTIBLE,<br>TASK_UNINTERRUPTIBLE,<br>TASK_STOPPED, TASK_TRACED,<br>EXIT_ZOMBIE, or EXIT_DEAD. |
| kernel_thread()                                 | arch/your<br>arch/kernel/process.c     | Creates a kernel thread.                                                                                                                                                                 |
| daemonize()                                     | kernel/exit.c                          | Activates a kernel thread<br>without attaching user resources<br>and changes the parent of the<br>calling thread to kthreadd.                                                            |
| allow_signal()                                  | kernel/exit.c                          | Enables delivery of a specified<br>signal.                                                                                                                                               |
| signal_pending()                                | include/linux/sched.h                  | Checks whether a signal has<br>been delivered. There are no<br>signal handlers inside the kernel,<br>so you have to explicitly check<br>whether a signal has been<br>delivered.          |
| call_usermodehelper()                           | include/linux/kmod.h<br>kernel/kmod.c  | Executes a user mode program.                                                                                                                                                            |
| Linked list library functions                   | include/linux/list.h                   | See Table 3.1.                                                                                                                                                                           |
| register_die_notifier()                         | arch/your<br>arch/kernel/traps.c       | Registers a die notifier.                                                                                                                                                                |
| register_netdevice_notifier()                   | net/core/dev.c                         | Registers a netdevice notifier.                                                                                                                                                          |
| register_inetaddr_notifier()                    | net/ipv4/devinet.c                     | Registers an inetaddr notifier.                                                                                                                                                          |
| BLOCKING_NOTIFIER_HEAD()                        | include/linux/notifier.h               | Creates a user-defined blocking<br>notifier.                                                                                                                                             |
| blocking_notifier_chain_register() kernel/sys.c |                                        | Registers a blocking notifier.                                                                                                                                                           |

| Kernel Interface                 | Location                                          | Description                                                                                                                  |
|----------------------------------|---------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------|
| blocking_notifier_call_chain()   | kernel/sys.c                                      | Dispatches an event to a<br>blocking notifier chain.                                                                         |
| ATOMIC_NOTIFIER_HEAD()           | include/linux/notifier.h                          | Creates an atomic notifier.                                                                                                  |
| atomic_notifier_chain_register() | kernel/sys.c                                      | Registers an atomic notifier.                                                                                                |
| DECLARE_COMPLETION()             |                                                   | include/linux/completion.h Statically declares a completion<br>object.                                                       |
| init_completion()                | include/linux/completion.h Dynamically declares a | completion object.                                                                                                           |
| complete()                       | kernel/sched.c                                    | Announces completion.                                                                                                        |
| wait_for_completion()            | kernel/sched.c                                    | Waits until the completion object<br>completes.                                                                              |
| complete_and_exit()              | kernel/exit.c                                     | Atomically signals completion<br>and exit.                                                                                   |
| kthread_create()                 | kernel/kthread.c                                  | Creates a kernel thread.                                                                                                     |
| kthread_stop()                   | kernel/kthread.c                                  | Asks a kernel thread to stop.                                                                                                |
| kthread_should_stop()            | kernel/kthread.c                                  | A kernel thread can poll on this<br>function to detect whether<br>another thread has asked it to<br>stop via kthread_stop(). |
| IS_ERR()                         | include/linux/err.h                               | Finds out whether the return<br>value is an error code.                                                                      |
