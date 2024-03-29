#	$OpenBSD: Makefile.inc,v 1.122 2014/08/31 04:02:08 guenther Exp $
#	$NetBSD: Makefile.inc,v 1.35 1995/10/16 23:49:07 jtc Exp $
#	@(#)Makefile.inc	8.1 (Berkeley) 6/17/93

# sys sources
.PATH: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/sys ${LIBCSRCDIR}/sys

# modules with non-default implementations on at least one architecture:
SRCS+=	Ovfork.S brk.S cerror.S exect.S fork.S \
	sbrk.S sigpending.S sigprocmask.S sigreturn.S \
	sigsuspend.S syscall.S tfork_thread.S

# glue to offer userland wrappers for some syscalls
SRCS+=	posix_madvise.c

# glue to provide compatibility between GCC 1.X and 2.X and for compat
# with old syscall interfaces.
SRCS+=	ftruncate.c lseek.c mquery.c mmap.c ptrace.c semctl.c truncate.c \
	timer_create.c timer_delete.c timer_getoverrun.c timer_gettime.c \
	timer_settime.c pread.c preadv.c pwrite.c pwritev.c

# stack protector helper functions
SRCS+=	stack_protector.c

# modules with default implementations on all architectures, unless overridden
# below:
ASM=	__get_tcb.o __getcwd.o __semctl.o __set_tcb.o __syscall.o \
	__sysctl.o __threxit.o __thrsigdivert.o __thrsleep.o \
	__thrwakeup.o accept.o accept4.o access.o acct.o adjfreq.o adjtime.o \
	bind.o chdir.o chflags.o chmod.o chown.o chroot.o \
	clock_getres.o clock_gettime.o clock_settime.o close.o \
	closefrom.o connect.o dup.o dup2.o dup3.o execve.o faccessat.o \
	fchdir.o fchflags.o fchmod.o fchmodat.o fchown.o \
	fchownat.o fcntl.o fhopen.o fhstat.o fhstatfs.o \
	flock.o fpathconf.o fstat.o fstatat.o fstatfs.o \
	fsync.o futimens.o futimes.o getentropy.o getdents.o getdtablecount.o \
	getegid.o geteuid.o getfh.o getfsstat.o getgid.o \
	getgroups.o getitimer.o getpeername.o getpgid.o getpgrp.o \
	getpid.o getppid.o getpriority.o getresgid.o getresuid.o \
	getrlimit.o getrtable.o getrusage.o getsid.o getsockname.o \
	getsockopt.o getthrid.o gettimeofday.o getuid.o ioctl.o \
	issetugid.o kevent.o kill.o kqueue.o ktrace.o lchown.o \
	link.o linkat.o listen.o lstat.o madvise.o mincore.o \
	minherit.o mkdir.o mkdirat.o mkfifo.o mkfifoat.o \
	mknod.o mknodat.o mlock.o mlockall.o mount.o mprotect.o \
	msgctl.o msgget.o msgrcv.o msgsnd.o msync.o munlock.o \
	munlockall.o munmap.o nanosleep.o nfssvc.o \
	open.o openat.o pathconf.o pipe.o pipe2.o \
	poll.o ppoll.o profil.o pselect.o \
	quotactl.o read.o readlink.o readlinkat.o readv.o reboot.o \
	recvfrom.o recvmsg.o rename.o renameat.o revoke.o rmdir.o \
	sched_yield.o select.o semget.o semop.o sendmsg.o sendsyslog.o sendto.o \
	setegid.o seteuid.o setgid.o setgroups.o setitimer.o \
	setpgid.o setpriority.o setregid.o setresgid.o setresuid.o \
	setreuid.o setrlimit.o setrtable.o setsid.o setsockopt.o \
	settimeofday.o setuid.o shmat.o shmctl.o shmdt.o \
	shmget.o shutdown.o sigaction.o sigaltstack.o socket.o \
	socketpair.o stat.o statfs.o swapctl.o symlink.o \
	symlinkat.o sync.o sysarch.o umask.o unlink.o unlinkat.o \
	unmount.o utimensat.o utimes.o utrace.o wait4.o write.o writev.o

SRCS+=	${SRCS_${MACHINE_CPU}}
.for i in ${SRCS_${MACHINE_CPU}}
ASM:=	${ASM:N${i:R}.o}
.endfor

GASM=	${ASM:.o=.go}
PASM=	${ASM:.o=.po}
SASM=	${ASM:.o=.so}
DASM=	${ASM:.o=.do}

PSEUDO=	_getlogin.o _setlogin.o _ptrace.o
GPSEUDO=${PSEUDO:.o=.go}
PPSEUDO=${PSEUDO:.o=.po}
SPSEUDO=${PSEUDO:.o=.so}
DPSEUDO=${PSEUDO:.o=.do}

PSEUDO_NOERR=	_exit.o
GPSEUDO_NOERR=${PSEUDO_NOERR:.o=.go}
PPSEUDO_NOERR=${PSEUDO_NOERR:.o=.po}
SPSEUDO_NOERR=${PSEUDO_NOERR:.o=.so}
DPSEUDO_NOERR=${PSEUDO_NOERR:.o=.do}

OBJS+=	${ASM} ${PSEUDO} ${PSEUDO_NOERR}

${GASM}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nRSYSCALL(${.PREFIX})\n' | \
	    ${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} -x assembler-with-cpp - \
	    -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${PASM}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nRSYSCALL(${.PREFIX})\n' | \
	    ${COMPILE.S} -DPROF ${CFLAGS:M-[ID]*} ${AINC} \
	    -x assembler-with-cpp - -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${SASM}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nRSYSCALL(${.PREFIX})\n' | \
	    ${COMPILE.S} ${PICFLAG} ${CFLAGS:M-[ID]*} ${AINC} \
	    -x assembler-with-cpp - -o ${.TARGET}

${DASM}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nRSYSCALL(${.PREFIX})\n' | \
	    ${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} -x assembler-with-cpp - \
	     ${DIST_CFLAGS} -o ${.TARGET}.o
	@${LD} -x -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${ASM}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nRSYSCALL(${.PREFIX})\n' | \
	    ${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} -x assembler-with-cpp - \
	     -o ${.TARGET}.o
	@${LD} -x -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${GPSEUDO}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nPSEUDO(${.PREFIX},${.PREFIX:S/_//})\n' | \
	    ${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} -x assembler-with-cpp - \
	    -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${PPSEUDO}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nPSEUDO(${.PREFIX},${.PREFIX:S/_//})\n' | \
	    ${COMPILE.S} -DPROF ${CFLAGS:M-[ID]*} ${AINC} \
	    -x assembler-with-cpp - -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${SPSEUDO}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nPSEUDO(${.PREFIX},${.PREFIX:S/_//})\n' | \
	    ${COMPILE.S} ${PICFLAG} ${CFLAGS:M-[ID]*} ${AINC} \
	    -x assembler-with-cpp - -o ${.TARGET}

${DPSEUDO}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nPSEUDO(${.PREFIX},${.PREFIX:S/_//})\n' | \
	    ${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} -x assembler-with-cpp - \
	    ${DIST_CFLAGS} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${PSEUDO}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nPSEUDO(${.PREFIX},${.PREFIX:S/_//})\n' | \
	    ${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} -x assembler-with-cpp - \
	    -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${GPSEUDO_NOERR}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nPSEUDO_NOERROR(${.PREFIX},${.PREFIX:S/_//})\n' | \
	    ${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} -x assembler-with-cpp - \
	    -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${PPSEUDO_NOERR}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nPSEUDO_NOERROR(${.PREFIX},${.PREFIX:S/_//})\n' | \
	    ${COMPILE.S} -DPROF ${CFLAGS:M-[ID]*} ${AINC} \
	    -x assembler-with-cpp - -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${SPSEUDO_NOERR}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nPSEUDO_NOERROR(${.PREFIX},${.PREFIX:S/_//})\n' | \
	    ${COMPILE.S} ${PICFLAG} ${CFLAGS:M-[ID]*} ${AINC} \
	    -x assembler-with-cpp - -o ${.TARGET}

${DPSEUDO_NOERR}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nPSEUDO_NOERROR(${.PREFIX},${.PREFIX:S/_//})\n' | \
	    ${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} -x assembler-with-cpp - \
	    ${DIST_CFLAGS} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

${PSEUDO_NOERR}: ${LIBCSRCDIR}/arch/${MACHINE_CPU}/SYS.h /usr/include/sys/syscall.h
	@echo creating ${.TARGET}
	@printf '#include "SYS.h"\nPSEUDO_NOERROR(${.PREFIX},${.PREFIX:S/_//})\n' | \
	    ${COMPILE.S} ${CFLAGS:M-[ID]*} ${AINC} -x assembler-with-cpp - \
	    -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

MAN+=	__get_tcb.2 __thrsigdivert.2 __thrsleep.2 _exit.2 accept.2 \
	access.2 acct.2 adjfreq.2 adjtime.2 bind.2 brk.2 chdir.2 \
	chflags.2 chmod.2 chown.2 chroot.2 clock_gettime.2 close.2 \
	closefrom.2 connect.2 dup.2 execve.2 fcntl.2 fhopen.2 flock.2 \
	fork.2 fsync.2 getentropy.2 getdents.2 getdtablecount.2 \
	getfh.2 getfsstat.2 getgid.2 getgroups.2 getitimer.2 getlogin.2 \
	getpeername.2 getpgrp.2 getpid.2 getpriority.2 getrlimit.2 \
	getrtable.2 getrusage.2 getsid.2 getsockname.2 getsockopt.2 \
	getthrid.2 gettimeofday.2 getuid.2 intro.2 ioctl.2 issetugid.2 \
	kill.2 kqueue.2 ktrace.2 link.2 listen.2 lseek.2 madvise.2 \
	mincore.2 minherit.2 mkdir.2 mkfifo.2 mknod.2 mlock.2 \
	mlockall.2 mmap.2 mount.2 mprotect.2 mquery.2 msgctl.2 \
	msgget.2 msgrcv.2 msgsnd.2 msync.2 munmap.2 nanosleep.2 \
	nfssvc.2 open.2 pathconf.2 pipe.2 poll.2 profil.2 \
	ptrace.2 quotactl.2 read.2 readlink.2 reboot.2 recv.2 \
	rename.2 revoke.2 rmdir.2 select.2 semctl.2 semget.2 \
	semop.2 send.2 setgroups.2 setpgid.2 setregid.2 \
	setresuid.2 setreuid.2 setsid.2 sendsyslog.2 setuid.2 shmat.2 \
	shmctl.2 shmget.2 shutdown.2 sigaction.2 sigaltstack.2 sigpending.2 \
	sigprocmask.2 sigreturn.2 sigsuspend.2 socket.2 \
	socketpair.2 stat.2 statfs.2 swapctl.2 symlink.2 \
	sync.2 sysarch.2 syscall.2 truncate.2 umask.2 unlink.2 \
	utimes.2 utrace.2 vfork.2 wait.2 write.2

MLINKS+=__get_tcb.2 __set_tcb.2
MLINKS+=__thrsleep.2 __thrwakeup.2
MLINKS+=_exit.2 _Exit.2
MLINKS+=accept.2 accept4.2
MLINKS+=access.2 faccessat.2
MLINKS+=brk.2 sbrk.2
MLINKS+=chdir.2 fchdir.2
MLINKS+=chflags.2 fchflags.2
MLINKS+=chmod.2 fchmod.2 chmod.2 fchmodat.2
MLINKS+=chown.2 fchown.2 chown.2 fchownat.2 chown.2 lchown.2
MLINKS+=clock_gettime.2 clock_getres.2
MLINKS+=clock_gettime.2 clock_settime.2
MLINKS+=dup.2 dup2.2
MLINKS+=dup.2 dup3.2
MLINKS+=execve.2 exect.2
MLINKS+=fhopen.2 fhstat.2 fhopen.2 fhstatfs.2
MLINKS+=fsync.2 fdatasync.2
MLINKS+=getgid.2 getegid.2
MLINKS+=getitimer.2 setitimer.2
MLINKS+=getitimer.2 timeradd.3
MLINKS+=getitimer.2 timerclear.3
MLINKS+=getitimer.2 timercmp.3
MLINKS+=getitimer.2 timerisset.3
MLINKS+=getitimer.2 timersub.3
MLINKS+=getlogin.2 getlogin_r.2 getlogin.2 setlogin.2
MLINKS+=getpgrp.2 getpgid.2
MLINKS+=getpid.2 getppid.2
MLINKS+=getpriority.2 setpriority.2
MLINKS+=getrlimit.2 setrlimit.2
MLINKS+=getrtable.2 setrtable.2
MLINKS+=getsockopt.2 setsockopt.2
MLINKS+=gettimeofday.2 settimeofday.2
MLINKS+=getuid.2 geteuid.2
MLINKS+=intro.2 errno.2
MLINKS+=kqueue.2 kevent.2 kqueue.2 EV_SET.2
MLINKS+=link.2 linkat.2
MLINKS+=madvise.2 posix_madvise.2
MLINKS+=mkdir.2 mkdirat.2
MLINKS+=mkfifo.2 mkfifoat.2
MLINKS+=mknod.2 mknodat.2
MLINKS+=mlock.2 munlock.2
MLINKS+=mlockall.2 munlockall.2
MLINKS+=mount.2 unmount.2
MLINKS+=open.2 openat.2
MLINKS+=pathconf.2 fpathconf.2
MLINKS+=pipe.2 pipe2.2
MLINKS+=poll.2 ppoll.2
MLINKS+=read.2 readv.2 read.2 pread.2 read.2 preadv.2
MLINKS+=readlink.2 readlinkat.2
MLINKS+=recv.2 recvfrom.2 recv.2 recvmsg.2
MLINKS+=rename.2 renameat.2
MLINKS+=select.2 pselect.2
MLINKS+=select.2 FD_ISSET.3 select.2 FD_ZERO.3
MLINKS+=select.2 FD_SET.3 select.2 FD_CLR.3
MLINKS+=send.2 sendmsg.2 send.2 sendto.2
MLINKS+=setpgid.2 setpgrp.2
MLINKS+=setresuid.2 getresgid.2 setresuid.2 getresuid.2
MLINKS+=setresuid.2 setresgid.2
MLINKS+=setuid.2 setegid.2 setuid.2 seteuid.2 setuid.2 setgid.2
MLINKS+=shmat.2 shmdt.2
MLINKS+=stat.2 S_ISBLK.2 stat.2 S_ISCHR.2 stat.2 S_ISDIR.2
MLINKS+=stat.2 S_ISFIFO.2 stat.2 S_ISLNK.2 stat.2 S_ISREG.2 stat.2 S_ISSOCK.2
MLINKS+=stat.2 fstat.2 stat.2 fstatat.2 stat.2 lstat.2
MLINKS+=statfs.2 fstatfs.2
MLINKS+=symlink.2 symlinkat.2
MLINKS+=syscall.2 __syscall.2
MLINKS+=truncate.2 ftruncate.2
MLINKS+=unlink.2 unlinkat.2
MLINKS+=utimes.2 futimens.2 utimes.2 futimes.2 utimes.2 utimensat.2
MLINKS+=wait.2 WEXITSTATUS.2 wait.2 WCOREDUMP.2 wait.2 WSTOPSIG.2
MLINKS+=wait.2 WIFCONTINUED.2 wait.2 WIFEXITED.2
MLINKS+=wait.2 WIFSIGNALED.2 wait.2 WIFSTOPPED.2 wait.2 WTERMSIG.2
MLINKS+=wait.2 wait3.2 wait.2 wait4.2 wait.2 waitpid.2
MLINKS+=write.2 writev.2 write.2 pwrite.2 write.2 pwritev.2
