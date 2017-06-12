AC_INIT([mpppoe-relay], [1.0], [bug-report@address])
AM_INIT_AUTOMAKE
AC_PROG_CC
dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(fcntl.h sys/dlpi.h sys/ioctl.h sys/time.h syslog.h unistd.h net/if_arp.h netinet/if_ether.h getopt.h sys/uio.h sys/param.h fcntl.h net/bpf.h netpacket/packet.h net/ethernet.h asm/types.h linux/if_packet.h linux/if_ether.h sys/socket.h sys/cdefs.h net/if.h net/if_dl.h net/if_ether.h net/if_types.h netinet/if_ether.h net/if_types.h net/if_dl.h arpa/inet.h errno.h netinet/if_ether.h stdarg.h time.h net/ppp_defs.h)
AC_CHECK_HEADERS(linux/if.h, [], [], [#include<sys/socket.h>])
AC_CHECK_HEADERS(linux/if_pppox.h, [], [],
[
#include<sys/socket.h>
#include<net/ethernet.h>
#include<linux/if.h>
#include<linux/in.h>
#include<linux/in6.h>
])

AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
