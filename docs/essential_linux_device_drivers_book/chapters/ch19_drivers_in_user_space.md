# **Chapter 19. Drivers in User Space**

| In This Chapter                          |     |
|------------------------------------------|-----|
| Process Scheduling and Response<br>Times | 553 |
| Accessing I/O Regions                    | 558 |
| Accessing Memory Regions                 | 562 |
| User Mode SCSI                           | 565 |
| User Mode USB                            | 567 |
| User Mode I2C                            | 571 |
| UIO                                      | 573 |
| Looking at the Sources                   | 574 |

Most device drivers prefer to lead a privileged life inside the kernel, but some are at home in the indeterministic world outside. Several kernel subsystems, such as SCSI, USB, and I2C, offer some level of support for user mode drivers, so you might be able to control those devices without writing a single line of kernel code.

In spite of the inclement weather in user land, user mode drivers enjoy certain advantages. They are easy to develop and debug. You won't have to reboot the system every time you dereference a dangling pointer. Some user mode drivers will even work across operating systems if the device subsystem enjoys the services of a standard user-space programming library. Here are some rules of thumb to help decide whether your driver should reside in user space:

- Apply the possibility test. What can be done in user space should probably stay in user space.
- If you have to talk to a large number of slow devices and if performance requirements are modest, explore the possibility of implementing the drivers in user space. If you have timecritical performance requirements, stay inside the kernel.
- If your code needs the services of kernel APIs, access to kernel variables, or is intertwined with interrupt handling, it has a strong case for being in kernel space.
- If much of what your code does can be construed as policy, user land might be its logical residence.
- If the rest of the kernel needs to invoke your code's services, it's a candidate for staying inside the kernel.
- You can't easily do floating-point arithmetic inside the kernel. *Floating-point unit* (FPU) instructions can, however, be used from user space.

That said, you can't accomplish too much from user space. Many important device classes, such as storage media and network adapters, cannot be driven from user land. But even when a kernel driver is the appropriate solution, it's a good idea to model and test as much code as you can in user space before moving it to kernel space. The testing cycle is faster, and it's easier to traverse all possible code paths and ensure that they are clean.

In this chapter, the term *user space driver* (or *user mode driver*) is used in a generic sense that does not strictly conform to the semantics of a driver implied thus far in the book. An application is considered to be a user mode driver if it's a candidate for being implemented inside the kernel, too.

The 2.6 kernel overhauled a subsystem that is of special interest to user space drivers. The new process scheduler offers huge response-time benefits to user mode code, so let's start with that.

# **Process Scheduling and Response Times**

Many user mode drivers need to perform some work in a time-bound manner. In user space, indeterminism due

to scheduling and paging often come in the way of fast response times, however. To see how you can minimize the impact of the former hurdle, let's dip into recent Linux schedulers and understand their underlying philosophy.

## **The Original Scheduler**

In the 2.4 and earlier days, the scheduler used to recalculate scheduling parameters of each task before taking its pick. The time consumed by the algorithm thus increased linearly with the number of contending tasks in the system. In other words, it used O(n) time, where n is the number of active tasks. On a system running at high loads, this translated to significant overhead. The 2.4 algorithm also didn't work very well on SMP systems.

## **The O(1) Scheduler**

Time consumed by an O(n) algorithm depends linearly on the size of its input, and an O(n2) solution depends quadratically on the length of its input, but an O(1) technique is independent of the input and thus scales well. The 2.6 scheduler replaced the O(n) algorithm with an O(1) method. In addition to being super-scalable, the scheduler has built-in heuristics to improve user responsiveness by providing preferential treatment to tasks involved in I/O activity. Processes are of two kinds: I/O bound and CPU bound. I/O-bound tasks are often sleepwaiting for device I/O, while CPU-bound ones are workaholics addicted to the processor. Paradoxically, to achieve fast response times, lazy tasks get incentives from the O(1) scheduler, while studious ones draw flak. Look at the sidebar "Highlights of the O(1) Scheduler" to find out some of its important features.

### Highlights of the O(1) Scheduler

The following are some of the important features of the O(1) scheduler:

- The algorithm uses two *run queues* made up of 140 priority lists: an *active* queue that holds tasks that have time slices left and an *expired* queue that contains processes whose time slices have expired. When a task finishes its time slice, it's inserted into the expired queue in sorted order of priority. The active and expired queues are swapped when the former becomes empty. To decide which process to run next, the scheduler does not navigate through the entire queue. Instead, it picks that task from the active queue having the highest priority. The overhead of picking the task thus depends not on the number of active tasks, but on the number of priorities. This makes it a constant-time or an O(1) algorithm.
- The scheduler supports two priority ranges: standard *nice*values supported on UNIX systems and internal priorities. The former range from –20 to +19, while the latter extend from 0 to 139. In both cases, lower values correspond to higher priorities. The top 100 (0 to 99) internal priorities are reserved for real time (RT) tasks, and the bottom 40 (100 to 139) are assigned to normal tasks. The 40 *nice* values map to the bottom 40 internal priorities. Internal priorities of normal tasks can be dynamically varied by the scheduler, whereas *nice* values are statistically set by the user. Each internal priority gets an associated run list.
- The scheduler uses a heuristic to figure out whether the nature of a process is I/O-intensive or CPU-intensive. In simple terms, if a task sleeps often, it's likely to be I/O-intensive, but if it uses its time slice fast, it's CPU-intensive. Whenever the scheduler finds that a task has demonstrated I/O-bound characteristics, it rewards it by dynamically increasing its internal priority. CPU-bound characteristics, on the other hand, are punished with negative marks.
- The allotted time slice is directly proportional to the priority. A higher priority task gets a bigger time slice.

- A task will not be preempted by the scheduler as long as it has time slice credit. If it yields the processor before using up its time slice quota, it can roll over the reminder of its slice when it's run next. Because I/O-bound processes are the ones that often yield the CPU, this improves interactive performance.
- The scheduler supports RT scheduling policies. RT tasks preempt normal (SCHED\_OTHER) tasks. Users of RT policies can override the scheduler's dynamic priority assignments. Unlike SCHED\_OTHER tasks, their priorities are not recalculated by the kernel on-the-fly. RT scheduling comes in two flavors: SCHED\_FIFO and SCHED\_RR. They are used for producing "soft" real-time behavior, rather than stringent "hard" RT guarantees. SCHED\_FIFO has no concept of time slices; SCHED\_FIFO tasks run until they sleep-wait for I/O or yield the processor. SCHED\_RR is a round-robin variant of SCHED\_FIFO that also assigns time slices to RT tasks. SCHED\_RR tasks with expired slices are appended to the end of the corresponding priority list.
- The scheduler improves SMP performance by using per-CPU run queues and per-CPU synchronization.

## **The CFS Scheduler**

The Linux scheduler has undergone another rewrite with the 2.6.23 kernel. The *Completely Fair Scheduler* (CFS) for the SCHED\_OTHER class removes much of the complexities associated with the O(1) scheduler by abandoning priority arrays, time slices, interactivity heuristics, and the dependency on HZ. CFS's goal is to implement fairness for all scheduling entities by providing each task the total CPU power divided by the number of running tasks. Dissecting CFS is beyond the scope of this chapter. Have a look at *Documentation/sched-design-CFS.txt* for a brief tutorial.

### **Response Times**

As a user mode driver developer, you have several options to improve your application's response time:

- Use RT scheduling policies that give you a finer grain of control than usual. Look at the man pages of sched\_setscheduler() and its relatives for insights into achieving soft RT response times.
- If you are using non-RT scheduling, tune the *nice* values of different processes to achieve the required performance balance.
- If you are using a 2.6.23 or later kernel enabled with the CFS scheduler, you may fine-tune */proc/sys/kernel/sched\_granularity\_ns.* If you are using a pre-2.6.23 kernel, modify #defines in *kernel/sched.c* and *include/linux/sched.h* to suit your application. Change these values cautiously to satisfy the needs of your application suite. Usage scenarios of the scheduler are complex. Settings that delight certain load conditions can depress others, so you may have to experiment by trial and error.
- Response times are not solely the domain of the scheduler; they also depend on the solution architecture. For example, if you mark a busy interrupt handler as *fast*, it disables other local interrupts frequently and that can potentially slow down data acquisition and transmission on other IRQs.

Let's implement an example and see how a user mode driver can achieve fast response times by guarding against indeterminism introduced by scheduling and paging. As you learned in Chapter 2, "A Peek Inside the Kernel," the RTC is a timer source that can generate periodic interrupts with high precision. Listing 19.1 implements an example that uses interrupt reports from */dev/rtc* to perform periodic work with microsecond precision. The Pentium *Time Stamp Counter* (TSC) is used to measure response times.

The program in Listing 19.1 first changes its scheduling policy to SCHED\_FIFO using sched\_setscheduler(). Next, it invokes mlockall() to lock all mapped pages in memory to ensure that swapping won't come in the way of deterministic timing. Only the super-user is allowed to call sched\_setscheduler()and mlockall() and request RTC interrupts at frequencies greater than 64Hz.

**Listing 19.1. Periodic Work with Microsecond Precision**

```
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <pthread.h>
#include <linux/mman.h>
/* Read the lower half of the Pentium Time Stamp Counter
 using the rdtsc instruction */
#define rdtscl(val) __asm__ __volatile__ ("rdtsc" : "=A" (val))
main()
{
 unsigned long ts0, ts1, now, worst; /* Store TSC ticks */
 struct sched_param sched_p; /* Information related to
 scheduling priority */
 int fd, i=0;
 unsigned long data;
 /* Change the scheduling policy to SCHED_FIFO */
 sched_getparam(getpid(), &sched_p);
 sched_p.sched_priority = 50; /* RT Priority */
 sched_setscheduler(getpid(), SCHED_FIFO, &sched_p);
 /* Avoid paging and related indeterminism */
 mlockall(MCL_CURRENT);
 /* Open the RTC */
 fd = open("/dev/rtc", O_RDONLY);
 /* Set the periodic interrupt frequency to 8192Hz
 This should give an interrupt rate of 122uS */
 ioctl(fd, RTC_IRQP_SET, 8192);
 /* Enable periodic interrupts */
 ioctl(fd, RTC_PIE_ON, 0);
 rdtscl(ts0);
 worst = 0;
 while (i++ < 10000) {
 /* Block until the next periodic interrupt */
 read(fd, &data, sizeof(unsigned long));
```

```
 /* Use the TSC to precisely measure the time consumed.
 Reading the lower half of the TSC is sufficient */
 rdtscl(ts1);
 now = (ts1-ts0);
 /* Update the worst case latency */
 if (now > worst) worst = now;
 ts0 = ts1;
 /* Do work that is to be done periodically */
 do_work(); /* NOP for the purpose of this measurement */
 }
 printf("Worst latency was %8ld\n", worst);
 /* Disable periodic interrupts */
 ioctl(fd, RTC_PIE_OFF, 0);
}
```

The code in Listing 19.1 loops for 10,000 iterations and prints out the worst-case latency that occurred during execution. The output was 240899 on a Pentium 1.8GHz box, which roughly corresponds to 133 microseconds. According to the data sheet of the RTC chipset, a timer frequency of 8192Hz should result in a periodic interrupt rate of 122 microseconds. That's close. Rerun the code under varying loads using SCHED\_OTHER instead of SCHED\_FIFO and observe the resultant drift.

You may also run kernel threads in the RT mode. For that, do the following when you start the thread:

```
static int
my_kernel_thread(void *i)
{
 daemonize();
 current->policy = SCHED_FIFO;
 current->rt_priority = 1;
 /* ... */
}
```
