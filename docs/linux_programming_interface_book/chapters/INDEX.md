# The Linux Programming Interface - Chapter Index

Sách gốc: *The Linux Programming Interface* - Michael Kerrisk.
Mỗi chapter là một file markdown riêng biệt trong thư mục này.

## Mục lục

| Ch | File | Chủ đề | Từ khóa |
|----|------|--------|---------|
| 1 | ch01_history_and_standards.md | History and Standards | UNIX history, C language, POSIX, SUSv3, SUSv4, Linux Standard Base |
| 2 | ch02_fundamental_concepts.md | Fundamental Concepts | kernel, shell, users, groups, file system, process, memory mapping, signals, threads, /proc |
| 3 | ch03_system_programming_concepts.md | System Programming Concepts | system calls, library functions, glibc, error handling, errno, portability, feature test macros |
| 4 | ch04_file_io_the_universal_io_model.md | File I/O: The Universal I/O Model | open(), read(), write(), close(), file descriptors, O_CREAT, O_RDONLY, O_WRONLY |
| 5 | ch05_file_io_further_details.md | File I/O: Further Details | fcntl(), pread(), pwrite(), readv(), writev(), scatter-gather I/O, truncate(), /dev/fd, temporary files |
| 6 | ch06_processes.md | Processes | process ID, memory layout, text/data/stack/heap, virtual memory, argc/argv, environment, setjmp(), longjmp() |
| 7 | ch07_memory_allocation.md | Memory Allocation | malloc(), free(), calloc(), realloc(), brk(), sbrk(), alloca(), heap, program break |
| 8 | ch08_users_and_groups.md | Users and Groups | /etc/passwd, /etc/group, /etc/shadow, getpwnam(), getpwuid(), getgrnam() |
| 9 | ch09_process_credentials.md | Process Credentials | real UID/GID, effective UID/GID, saved set-user-ID, set-group-ID, supplementary groups, setuid(), setgid() |
| 10 | ch10_time.md | Time | calendar time, time_t, broken-down time, strftime(), gettimeofday(), clock_gettime(), process time |
| 11 | ch11_system_limits_and_options.md | System Limits and Options | sysconf(), pathconf(), fpathconf(), _POSIX_*, _SC_*, _PC_* |
| 12 | ch12_system_and_process_information.md | System and Process Information | /proc filesystem, /proc/PID, uname(), /proc/sys |
| 13 | ch13_file_io_buffering.md | File I/O Buffering | kernel buffer cache, stdio buffering, setvbuf(), fflush(), fsync(), fdatasync(), O_DIRECT |
| 14 | ch14_file_systems.md | File Systems | device files, major/minor, partitions, inodes, ext2/ext3/ext4, VFS, journaling, mount(), umount(), statvfs() |
| 15 | ch15_file_attributes.md | File Attributes | stat(), lstat(), fstat(), file permissions, chmod(), chown(), umask, sticky bit, set-user-ID |
| 16 | ch16_extended_attributes.md | Extended Attributes | EA namespaces, user/system/trusted/security, setxattr(), getxattr(), listxattr(), removexattr() |
| 17 | ch17_access_control_lists.md | Access Control Lists | ACL, getfacl, setfacl, ACL_MASK, default ACL, POSIX ACL API |
| 18 | ch18_directories_and_links.md | Directories and Links | hard links, symbolic links, link(), symlink(), unlink(), rename(), opendir(), readdir(), nftw(), chroot(), realpath() |
| 19 | ch19_monitoring_file_events.md | Monitoring File Events | inotify, inotify_init(), inotify_add_watch(), IN_CREATE, IN_MODIFY, IN_DELETE, dnotify |
| 20 | ch20_signals_fundamental_concepts.md | Signals: Fundamental Concepts | signal disposition, signal(), sigaction(), kill(), raise(), signal mask, pending signals |
| 21 | ch21_signals_signal_handlers.md | Signals: Signal Handlers | async-signal-safe, sig_atomic_t, SA_SIGINFO, sigaltstack(), signal handler design, system call restart |
| 22 | ch22_signals_advanced_features.md | Signals: Advanced Features | core dump, realtime signals, sigqueue(), signalfd(), sigsuspend(), sigwaitinfo() |
| 23 | ch23_timers_and_sleeping.md | Timers and Sleeping | setitimer(), alarm(), sleep(), nanosleep(), POSIX clocks, timer_create(), timerfd |
| 24 | ch24_process_creation.md | Process Creation | fork(), vfork(), copy-on-write, file sharing parent/child, memory semantics |
| 25 | ch25_process_termination.md | Process Termination | _exit(), exit(), atexit(), on_exit(), exit handlers, stdio buffer flushing |
| 26 | ch26_monitoring_child_processes.md | Monitoring Child Processes | wait(), waitpid(), waitid(), WIFEXITED, WIFSIGNALED, orphans, zombies, SIGCHLD |
| 27 | ch27_program_execution.md | Program Execution | execve(), exec() family, PATH, system(), close-on-exec, interpreter scripts |
| 28 | ch28_process_creation_and_program_execution_in_more_detail.md | Process Creation and Program Execution in More Detail | clone(), process accounting, fork/exec effect on process attributes |
| 29 | ch29_threads_introduction.md | Threads: Introduction | pthreads, pthread_create(), pthread_exit(), pthread_join(), pthread_detach(), threads vs processes |
| 30 | ch30_threads_thread_synchronization.md | Threads: Thread Synchronization | mutex, pthread_mutex_lock(), condition variables, pthread_cond_wait(), pthread_cond_signal(), deadlock |
| 31 | ch31_threads_thread_safety_and_per_thread_storage.md | Threads: Thread Safety and Per-Thread Storage | thread safety, reentrancy, thread-specific data, pthread_key_create(), thread-local storage, __thread |
| 32 | ch32_threads_thread_cancellation.md | Threads: Thread Cancellation | pthread_cancel(), cancellation points, cleanup handlers, pthread_cleanup_push() |
| 33 | ch33_threads_further_details.md | Threads: Further Details | thread stacks, threads and signals, pthread_sigmask(), sigwait(), LinuxThreads, NPTL |
| 34 | ch34_process_groups_sessions_and_job_control.md | Process Groups, Sessions, and Job Control | process group, session, setpgid(), setsid(), controlling terminal, SIGHUP, job control |
| 35 | ch35_process_priorities_and_scheduling.md | Process Priorities and Scheduling | nice value, SCHED_FIFO, SCHED_RR, SCHED_BATCH, SCHED_IDLE, realtime scheduling, sched_setscheduler() |
| 36 | ch36_process_resources.md | Process Resources | getrusage(), resource limits, getrlimit(), setrlimit(), RLIMIT_NOFILE, RLIMIT_NPROC |
| 37 | ch37_daemons.md | Daemons | daemon creation, setsid(), syslog, openlog(), syslog(), /etc/syslog.conf, SIGHUP reinit |
| 38 | ch38_writing_secure_privileged_programs.md | Writing Secure Privileged Programs | set-user-ID security, least privilege, input validation, race conditions, denial-of-service |
| 39 | ch39_capabilities.md | Capabilities | Linux capabilities, CAP_*, permitted/effective/inheritable sets, capset(), capget() |
| 40 | ch40_login_accounting.md | Login Accounting | utmp, wtmp, utmpx API, lastlog, login sessions |
| 41 | ch41_fundamentals_of_shared_libraries.md | Fundamentals of Shared Libraries | static libraries, shared libraries (.so), soname, ldconfig, LD_LIBRARY_PATH, PIC, -fPIC |
| 42 | ch42_advanced_features_of_shared_libraries.md | Advanced Features of Shared Libraries | dlopen(), dlsym(), dlclose(), dlerror(), symbol visibility, LD_PRELOAD, LD_DEBUG, version scripts |
| 43 | ch43_interprocess_communication_overview.md | Interprocess Communication Overview | IPC taxonomy, pipes, FIFOs, sockets, shared memory, message queues, semaphores |
| 44 | ch44_pipes_and_fifos.md | Pipes and FIFOs | pipe(), popen(), pclose(), mkfifo(), named pipes, FIFO semantics |
| 45 | ch45_introduction_to_system_v_ipc.md | Introduction to System V IPC | IPC keys, ftok(), ipc_perm, ipcs, ipcrm, IPC_CREAT, IPC_EXCL |
| 46 | ch46_system_v_message_queues.md | System V Message Queues | msgget(), msgsnd(), msgrcv(), msgctl(), message types, client-server with message queues |
| 47 | ch47_system_v_semaphores.md | System V Semaphores | semget(), semop(), semctl(), semaphore sets, binary semaphore protocol |
| 48 | ch48_system_v_shared_memory.md | System V Shared Memory | shmget(), shmat(), shmdt(), shmctl(), shared memory layout |
| 49 | ch49_memory_mappings.md | Memory Mappings | mmap(), munmap(), MAP_SHARED, MAP_PRIVATE, file mappings, anonymous mappings, mremap() |
| 50 | ch50_virtual_memory_operations.md | Virtual Memory Operations | mprotect(), mlock(), mlockall(), mincore(), madvise() |
| 51 | ch51_introduction_to_posix_ipc.md | Introduction to POSIX IPC | POSIX IPC overview, comparison with System V IPC |
| 52 | ch52_posix_message_queues.md | POSIX Message Queues | mq_open(), mq_send(), mq_receive(), mq_notify(), mq_close(), mq_unlink() |
| 53 | ch53_posix_semaphores.md | POSIX Semaphores | sem_open(), sem_wait(), sem_post(), sem_init(), named semaphores, unnamed semaphores |
| 54 | ch54_posix_shared_memory.md | POSIX Shared Memory | shm_open(), shm_unlink(), ftruncate(), mmap() with shared memory objects |
| 55 | ch55_file_locking.md | File Locking | flock(), fcntl() record locking, F_SETLK, F_SETLKW, mandatory locking, advisory locking |
| 56 | ch56_sockets_introduction.md | Sockets: Introduction | socket(), bind(), listen(), accept(), connect(), close(), stream sockets, datagram sockets |
| 57 | ch57_sockets_unix_domain.md | Sockets: UNIX Domain | sockaddr_un, UNIX domain stream, UNIX domain datagram, socketpair(), SCM_RIGHTS |
| 58 | ch58_sockets_fundamentals_of_tcp_ip_networks.md | Sockets: Fundamentals of TCP/IP Networks | TCP/IP layers, IP addresses, port numbers, TCP, UDP, data-link layer |
| 59 | ch59_sockets_internet_domains.md | Sockets: Internet Domains | sockaddr_in, sockaddr_in6, htons(), inet_pton(), getaddrinfo(), getnameinfo(), byte order |
| 60 | ch60_sockets_server_design.md | Sockets: Server Design | iterative server, concurrent server, inetd, prefork, prethread |
| 61 | ch61_sockets_advanced_topics.md | Sockets: Advanced Topics | partial reads/writes, TCP internals, SO_REUSEADDR, sendmsg(), recvmsg(), passing file descriptors, SCTP |
| 62 | ch62_terminals.md | Terminals | terminal driver, termios, canonical mode, raw mode, cbreak, stty, tcgetattr(), tcsetattr() |
| 63 | ch63_alternative_io_models.md | Alternative I/O Models | select(), poll(), epoll, epoll_create(), epoll_ctl(), epoll_wait(), signal-driven I/O, I/O multiplexing |
| 64 | ch64_pseudoterminals.md | Pseudoterminals | PTY, master/slave, posix_openpt(), grantpt(), unlockpt(), ptsname(), ptyFork() |

## Phân nhóm theo chủ đề

### File I/O & File Systems
- Ch 4-5: File I/O (open, read, write, close, fcntl, scatter-gather)
- Ch 13: File I/O Buffering (kernel buffer cache, stdio buffering, O_DIRECT)
- Ch 14: File Systems (inodes, VFS, mount, journaling)
- Ch 15: File Attributes (stat, permissions, chmod, chown)
- Ch 16: Extended Attributes (xattr)
- Ch 17: Access Control Lists (ACL)
- Ch 18: Directories and Links (hard/soft links, nftw, chroot)
- Ch 19: Monitoring File Events (inotify)
- Ch 55: File Locking (flock, fcntl record locking)

### Processes & Program Execution
- Ch 6: Processes (memory layout, environment, setjmp/longjmp)
- Ch 24: Process Creation (fork, vfork, copy-on-write)
- Ch 25: Process Termination (exit, atexit)
- Ch 26: Monitoring Child Processes (wait, waitpid, zombies, SIGCHLD)
- Ch 27: Program Execution (exec family, system())
- Ch 28: Process Creation in More Detail (clone)
- Ch 34: Process Groups, Sessions, Job Control
- Ch 35: Process Priorities and Scheduling (nice, realtime scheduling)
- Ch 36: Process Resources (getrusage, rlimit)
- Ch 37: Daemons (daemon creation, syslog)
- Ch 9: Process Credentials (UID, GID, setuid)

### Threads (Pthreads)
- Ch 29: Threads Introduction (create, join, detach)
- Ch 30: Thread Synchronization (mutex, condition variables)
- Ch 31: Thread Safety (thread-specific data, TLS)
- Ch 32: Thread Cancellation
- Ch 33: Threads Further Details (stacks, signals, NPTL)

### Signals
- Ch 20: Signals Fundamental Concepts (signal, sigaction, kill)
- Ch 21: Signal Handlers (async-signal-safe, SA_SIGINFO, sigaltstack)
- Ch 22: Signals Advanced (core dump, realtime signals, signalfd)

### Memory
- Ch 7: Memory Allocation (malloc, free, brk, alloca)
- Ch 49: Memory Mappings (mmap, munmap, shared/private)
- Ch 50: Virtual Memory Operations (mprotect, mlock, madvise)

### Interprocess Communication (IPC)
- Ch 43: IPC Overview (taxonomy, comparison)
- Ch 44: Pipes and FIFOs
- **System V IPC:** Ch 45 (intro), Ch 46 (message queues), Ch 47 (semaphores), Ch 48 (shared memory)
- **POSIX IPC:** Ch 51 (intro), Ch 52 (message queues), Ch 53 (semaphores), Ch 54 (shared memory)

### Sockets & Networking
- Ch 56: Sockets Introduction (socket, bind, listen, accept, connect)
- Ch 57: UNIX Domain Sockets
- Ch 58: TCP/IP Fundamentals
- Ch 59: Internet Domain Sockets (getaddrinfo, byte order)
- Ch 60: Server Design (iterative, concurrent, inetd)
- Ch 61: Advanced Socket Topics (sendmsg, recvmsg, fd passing)

### Timers & Time
- Ch 10: Time (calendar time, process time, clock_gettime)
- Ch 23: Timers and Sleeping (setitimer, POSIX timers, timerfd)

### Security & Privileges
- Ch 38: Writing Secure Privileged Programs
- Ch 39: Capabilities (Linux capabilities)

### Shared Libraries
- Ch 41: Shared Library Fundamentals (soname, PIC, ldconfig)
- Ch 42: Advanced Shared Libraries (dlopen, dlsym, LD_PRELOAD)

### Terminals & I/O Models
- Ch 62: Terminals (termios, canonical/raw mode)
- Ch 63: Alternative I/O Models (select, poll, epoll, signal-driven I/O)
- Ch 64: Pseudoterminals (PTY)

### System Info & Misc
- Ch 1: History and Standards (UNIX, POSIX)
- Ch 2: Fundamental Concepts (overview)
- Ch 3: System Programming Concepts (system calls, glibc, errno)
- Ch 8: Users and Groups (/etc/passwd, /etc/group)
- Ch 11: System Limits and Options (sysconf, pathconf)
- Ch 12: System and Process Information (/proc)
- Ch 40: Login Accounting (utmp, wtmp)
