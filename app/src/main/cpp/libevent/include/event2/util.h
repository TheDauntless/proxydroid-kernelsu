/*
 * Copyright (c) 2007-2012 Niels Provos and Nick Mathewson
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _EVENT2_UTIL_H_
#define _EVENT2_UTIL_H_

/** @file event2/util.h

  Common convenience functions for cross-platform portability and
  related socket manipulations.

 */

#ifdef __cplusplus
extern "C" {
#endif

#include <event2/event-config.h>
#ifdef _EVENT_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef _EVENT_HAVE_STDINT_H
#include <stdint.h>
#elif defined(_EVENT_HAVE_INTTYPES_H)
#include <inttypes.h>
#endif
#ifdef _EVENT_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef _EVENT_HAVE_STDDEF_H
#include <stddef.h>
#endif
#ifdef _MSC_VER
#include <BaseTsd.h>
#endif
#include <stdarg.h>
#ifdef _EVENT_HAVE_NETDB_H
#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include <netdb.h>
#endif

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

/* Some openbsd autoconf versions get the name of this macro wrong. */
#if defined(_EVENT_SIZEOF_VOID__) && !defined(_EVENT_SIZEOF_VOID_P)
#define _EVENT_SIZEOF_VOID_P _EVENT_SIZEOF_VOID__
#endif

/**
 * @name Standard integer types.
 *
 * Integer type definitions for types that are supposed to be defined in the
 * C99-specified stdint.h.  Shamefully, some platforms do not include
 * stdint.h, so we need to replace it.  (If you are on a platform like this,
 * your C headers are now over 10 years out of date.  You should bug them to
 * do something about this.)
 *
 * We define:
 *
 * <dl>
 *   <dt>ev_uint64_t, ev_uint32_t, ev_uint16_t, ev_uint8_t</dt>
 *      <dd>unsigned integer types of exactly 64, 32, 16, and 8 bits
 *          respectively.</dd>
 *    <dt>ev_int64_t, ev_int32_t, ev_int16_t, ev_int8_t</dt>
 *      <dd>signed integer types of exactly 64, 32, 16, and 8 bits
 *          respectively.</dd>
 *    <dt>ev_uintptr_t, ev_intptr_t</dt>
 *      <dd>unsigned/signed integers large enough
 *      to hold a pointer without loss of bits.</dd>
 *    <dt>ev_ssize_t</dt>
 *      <dd>A signed type of the same size as size_t</dd>
 *    <dt>ev_off_t</dt>
 *      <dd>A signed type typically used to represent offsets within a
 *      (potentially large) file</dd>
 *
 * @{
 */
#ifdef _EVENT_HAVE_UINT64_T
#define ev_uint64_t uint64_t
#define ev_int64_t int64_t
#elif defined(WIN32)
#define ev_uint64_t unsigned __int64
#define ev_int64_t signed __int64
#elif _EVENT_SIZEOF_LONG_LONG == 8
#define ev_uint64_t unsigned long long
#define ev_int64_t long long
#elif _EVENT_SIZEOF_LONG == 8
#define ev_uint64_t unsigned long
#define ev_int64_t long
#elif defined(_EVENT_IN_DOXYGEN)
#define ev_uint64_t ...
#define ev_int64_t ...
#else
#error "No way to define ev_uint64_t"
#endif

#ifdef _EVENT_HAVE_UINT32_T
#define ev_uint32_t uint32_t
#define ev_int32_t int32_t
#elif defined(WIN32)
#define ev_uint32_t unsigned int
#define ev_int32_t signed int
#elif _EVENT_SIZEOF_LONG == 4
#define ev_uint32_t unsigned long
#define ev_int32_t signed long
#elif _EVENT_SIZEOF_INT == 4
#define ev_uint32_t unsigned int
#define ev_int32_t signed int
#elif defined(_EVENT_IN_DOXYGEN)
#define ev_uint32_t ...
#define ev_int32_t ...
#else
#error "No way to define ev_uint32_t"
#endif

#ifdef _EVENT_HAVE_UINT16_T
#define ev_uint16_t uint16_t
#define ev_int16_t  int16_t
#elif defined(WIN32)
#define ev_uint16_t unsigned short
#define ev_int16_t  signed short
#elif _EVENT_SIZEOF_INT == 2
#define ev_uint16_t unsigned int
#define ev_int16_t  signed int
#elif _EVENT_SIZEOF_SHORT == 2
#define ev_uint16_t unsigned short
#define ev_int16_t  signed short
#elif defined(_EVENT_IN_DOXYGEN)
#define ev_uint16_t ...
#define ev_int16_t ...
#else
#error "No way to define ev_uint16_t"
#endif

#ifdef _EVENT_HAVE_UINT8_T
#define ev_uint8_t uint8_t
#define ev_int8_t int8_t
#elif defined(_EVENT_IN_DOXYGEN)
#define ev_uint8_t ...
#define ev_int8_t ...
#else
#define ev_uint8_t unsigned char
#define ev_int8_t signed char
#endif

#ifdef _EVENT_HAVE_UINTPTR_T
#define ev_uintptr_t uintptr_t
#define ev_intptr_t intptr_t
#elif _EVENT_SIZEOF_VOID_P <= 4
#define ev_uintptr_t ev_uint32_t
#define ev_intptr_t ev_int32_t
#elif _EVENT_SIZEOF_VOID_P <= 8
#define ev_uintptr_t ev_uint64_t
#define ev_intptr_t ev_int64_t
#elif defined(_EVENT_IN_DOXYGEN)
#define ev_uintptr_t ...
#define ev_intptr_t ...
#else
#error "No way to define ev_uintptr_t"
#endif

#ifdef _EVENT_ssize_t
#define ev_ssize_t _EVENT_ssize_t
#else
#define ev_ssize_t ssize_t
#endif

#ifdef WIN32
#define ev_off_t ev_int64_t
#else
#define ev_off_t off_t
#endif
/**@}*/

/* Limits for integer types.

   We're making two assumptions here:
     - The compiler does constant folding properly.
     - The platform does signed arithmetic in two's complement.
*/

/**
   @name Limits for integer types

   These macros hold the largest or smallest values possible for the
   ev_[u]int*_t types.

   @{
*/
#define EV_UINT64_MAX ((((ev_uint64_t)0xffffffffUL) << 32) | 0xffffffffUL)
#define EV_INT64_MAX  ((((ev_int64_t) 0x7fffffffL) << 32) | 0xffffffffL)
#define EV_INT64_MIN  ((-EV_INT64_MAX) - 1)
#define EV_UINT32_MAX ((ev_uint32_t)0xffffffffUL)
#define EV_INT32_MAX  ((ev_int32_t) 0x7fffffffL)
#define EV_INT32_MIN  ((-EV_INT32_MAX) - 1)
#define EV_UINT16_MAX ((ev_uint16_t)0xffffUL)
#define EV_INT16_MAX  ((ev_int16_t) 0x7fffL)
#define EV_INT16_MIN  ((-EV_INT16_MAX) - 1)
#define EV_UINT8_MAX  255
#define EV_INT8_MAX   127
#define EV_INT8_MIN   ((-EV_INT8_MAX) - 1)
/** @} */

/**
   @name Limits for SIZE_T and SSIZE_T

   @{
*/
#if _EVENT_SIZEOF_SIZE_T == 8
#define EV_SIZE_MAX EV_UINT64_MAX
#define EV_SSIZE_MAX EV_INT64_MAX
#elif _EVENT_SIZEOF_SIZE_T == 4
#define EV_SIZE_MAX EV_UINT32_MAX
#define EV_SSIZE_MAX EV_INT32_MAX
#elif defined(_EVENT_IN_DOXYGEN)
#define EV_SIZE_MAX ...
#define EV_SSIZE_MAX ...
#else
#error "No way to define SIZE_MAX"
#endif

#define EV_SSIZE_MIN ((-EV_SSIZE_MAX) - 1)
/**@}*/

#ifdef WIN32
#define ev_socklen_t int
#elif defined(_EVENT_socklen_t)
#define ev_socklen_t _EVENT_socklen_t
#else
#define ev_socklen_t socklen_t
#endif

#ifdef _EVENT_HAVE_STRUCT_SOCKADDR_STORAGE___SS_FAMILY
#if !defined(_EVENT_HAVE_STRUCT_SOCKADDR_STORAGE_SS_FAMILY) \
 && !defined(ss_family)
#define ss_family __ss_family
#endif
#endif

/**
 * A type wide enough to hold the output of "socket()" or "accept()".  On
 * Windows, this is an intptr_t; elsewhere, it is an int. */
#ifdef WIN32
#define evutil_socket_t intptr_t
#else
#define evutil_socket_t int
#endif

/** Create two new sockets that are connected to each other.

    On Unix, this simply calls socketpair().  On Windows, it uses the
    loopback network interface on 127.0.0.1, and only
    AF_INET,SOCK_STREAM are supported.

    (This may fail on some Windows hosts where firewall software has cleverly
    decided to keep 127.0.0.1 from talking to itself.)

    Parameters and return values are as for socketpair()
*/
int evutil_socketpair(int d, int type, int protocol, evutil_socket_t sv[2]);
/** Do platform-specific operations as needed to make a socket nonblocking.

    @param sock The socket to make nonblocking
    @return 0 on success, -1 on failure
 */
int evutil_make_socket_nonblocking(evutil_socket_t sock);

/** Do platform-specific operations to make a listener socket reusable.

    Specifically, we want to make sure that another program will be able
    to bind this address right after we've closed the listener.

    This differs from Windows's interpretation of "reusable", which
    allows multiple listeners to bind the same address at the same time.

    @param sock The socket to make reusable
    @return 0 on success, -1 on failure
 */
int evutil_make_listen_socket_reuseable(evutil_socket_t sock);

/** Do platform-specific operations as needed to close a socket upon a
    successful execution of one of the exec*() functions.

    @param sock The socket to be closed
    @return 0 on success, -1 on failure
 */
int evutil_make_socket_closeonexec(evutil_socket_t sock);

/** Do the platform-specific call needed to close a socket returned from
    socket() or accept().

    @param sock The socket to be closed
    @return 0 on success, -1 on failure
 */
int evutil_closesocket(evutil_socket_t sock);
#define EVUTIL_CLOSESOCKET(s) evutil_closesocket(s)


#ifdef WIN32
/** Return the most recent socket error.  Not idempotent on all platforms. */
#define EVUTIL_SOCKET_ERROR() WSAGetLastError()
/** Replace the most recent socket error with errcode */
#define EVUTIL_SET_SOCKET_ERROR(errcode)		\
	do { WSASetLastError(errcode); } while (0)
/** Return the most recent socket error to occur on sock. */
int evutil_socket_geterror(evutil_socket_t sock);
/** Convert a socket error to a string. */
const char *evutil_socket_error_to_string(int errcode);
#elif defined(_EVENT_IN_DOXYGEN)
/**
   @name Socket error functions

   These functions are needed for making programs compatible between
   Windows and Unix-like platforms.

   You see, Winsock handles socket errors differently from the rest of
   the world.  Elsewhere, a socket error is like any other error and is
   stored in errno.  But winsock functions require you to retrieve the
   error with a special function, and don't let you use strerror for
   the error codes.  And handling EWOULDBLOCK is ... different.

   @{
*/
/** Return the most recent socket error.  Not idempotent on all platforms. */
#define EVUTIL_SOCKET_ERROR() ...
/** Replace the most recent socket error with errcode */
#define EVUTIL_SET_SOCKET_ERROR(errcode) ...
/** Return the most recent socket error to occur on sock. */
#define evutil_socket_geterror(sock) ...
/** Convert a socket error to a string. */
#define evutil_socket_error_to_string(errcode) ...
/**@}*/
#else
#define EVUTIL_SOCKET_ERROR() (errno)
#define EVUTIL_SET_SOCKET_ERROR(errcode)		\
		do { errno = (errcode); } while (0)
#define evutil_socket_geterror(sock) (errno)
#define evutil_socket_error_to_string(errcode) (strerror(errcode))
#endif


/**
 * @name Manipulation macros for struct timeval.
 *
 * We define replacements
 * for timeradd, timersub, timerclear, timercmp, and timerisset.
 *
 * @{
 */
#ifdef _EVENT_HAVE_TIMERADD
#define evutil_timeradd(tvp, uvp, vvp) timeradd((tvp), (uvp), (vvp))
#define evutil_timersub(tvp, uvp, vvp) timersub((tvp), (uvp), (vvp))
#else
#define evutil_timeradd(tvp, uvp, vvp)					\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;       \
		if ((vvp)->tv_usec >= 1000000) {			\
			(vvp)->tv_sec++;				\
			(vvp)->tv_usec -= 1000000;			\
		}							\
	} while (0)
#define	evutil_timersub(tvp, uvp, vvp)					\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
		if ((vvp)->tv_usec < 0) {				\
			(vvp)->tv_sec--;				\
			(vvp)->tv_usec += 1000000;			\
		}							\
	} while (0)
#endif /* !_EVENT_HAVE_HAVE_TIMERADD */

#ifdef _EVENT_HAVE_TIMERCLEAR
#define evutil_timerclear(tvp) timerclear(tvp)
#else
#define	evutil_timerclear(tvp)	(tvp)->tv_sec = (tvp)->tv_usec = 0
#endif
/**@}*/

/** Return true iff the tvp is related to uvp according to the relational
 * operator cmp.  Recognized values for cmp are ==, <=, <, >=, and >. */
#define	evutil_timercmp(tvp, uvp, cmp)					\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?				\
	 ((tvp)->tv_usec cmp (uvp)->tv_usec) :				\
	 ((tvp)->tv_sec cmp (uvp)->tv_sec))

#ifdef _EVENT_HAVE_TIMERISSET
#define evutil_timerisset(tvp) timerisset(tvp)
#else
#define	evutil_timerisset(tvp)	((tvp)->tv_sec || (tvp)->tv_usec)
#endif

/** Replacement for offsetof on platforms that don't define it. */
#ifdef offsetof
#define evutil_offsetof(type, field) offsetof(type, field)
#else
#define evutil_offsetof(type, field) ((off_t)(&((type *)0)->field))
#endif

/* big-int related functions */
/** Parse a 64-bit value from a string.  Arguments are as for strtol. */
ev_int64_t evutil_strtoll(const char *s, char **endptr, int base);

/** Replacement for gettimeofday on platforms that lack it. */
#ifdef _EVENT_HAVE_GETTIMEOFDAY
#define evutil_gettimeofday(tv, tz) gettimeofday((tv), (tz))
#else
struct timezone;
int evutil_gettimeofday(struct timeval *tv, struct timezone *tz);
#endif

/** Replacement for snprintf to get consistent behavior on platforms for
    which the return value of snprintf does not conform to C99.
 */
int evutil_snprintf(char *buf, size_t buflen, const char *format, ...)
#ifdef __GNUC__
	__attribute__((format(printf, 3, 4)))
#endif
;
/** Replacement for vsnprintf to get consistent behavior on platforms for
    which the return value of snprintf does not conform to C99.
 */
int evutil_vsnprintf(char *buf, size_t buflen, const char *format, va_list ap)
#ifdef __GNUC__
	__attribute__((format(printf, 3, 0)))
#endif
;

/** Replacement for inet_ntop for platforms which lack it. */
const char *evutil_inet_ntop(int af, const void *src, char *dst, size_t len);
/** Replacement for inet_pton for platforms which lack it. */
int evutil_inet_pton(int af, const char *src, void *dst);
struct sockaddr;

/** Parse an IPv4 or IPv6 address, with optional port, from a string.

    Recognized formats are:
    - [IPv6Address]:port
    - [IPv6Address]
    - IPv6Address
    - IPv4Address:port
    - IPv4Address

    If no port is specified, the port in the output is set to 0.

    @param str The string to parse.
    @param out A struct sockaddr to hold the result.  This should probably be
       a struct sockaddr_storage.
    @param outlen A pointer to the number of bytes that that 'out' can safely
       hold.  Set to the number of bytes used in 'out' on success.
    @return -1 if the address is not well-formed, if the port is out of range,
       or if out is not large enough to hold the result.  Otherwise returns
       0 on success.
*/
int evutil_parse_sockaddr_port(const char *str, struct sockaddr *out, int *outlen);

/** Compare two sockaddrs; return 0 if they are equal, or less than 0 if sa1
 * preceeds sa2, or greater than 0 if sa1 follows sa2.  If include_port is
 * true, consider the port as well as the address.  Only implemented for
 * AF_INET and AF_INET6 addresses. The ordering is not guaranteed to remain
 * the same between Libevent versions. */
int evutil_sockaddr_cmp(const struct sockaddr *sa1, const struct sockaddr *sa2,
    int include_port);

/** As strcasecmp, but always compares the characters in locale-independent
    ASCII.  That's useful if you're handling data in ASCII-based protocols.
 */
int evutil_ascii_strcasecmp(const char *str1, const char *str2);
/** As strncasecmp, but always compares the characters in locale-independent
    ASCII.  That's useful if you're handling data in ASCII-based protocols.
 */
int evutil_ascii_strncasecmp(const char *str1, const char *str2, size_t n);

/* Here we define evutil_addrinfo to the native addrinfo type, or redefine it
 * if this system has no getaddrinfo(). */
#ifdef _EVENT_HAVE_STRUCT_ADDRINFO
#define evutil_addrinfo addrinfo
#else
/** A definition of struct addrinfo for systems that lack it.

    (This is just an alias for struct addrinfo if the system defines
    struct addrinfo.)
*/
struct evutil_addrinfo {
	int     ai_flags;     /* AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST */
	int     ai_family;    /* PF_xxx */
	int     ai_socktype;  /* SOCK_xxx */
	int     ai_protocol;  /* 0 or IPPROTO_xxx for IPv4 and IPv6 */
	size_t  ai_addrlen;   /* length of ai_addr */
	char   *ai_canonname; /* canonical name for nodename */
	struct sockaddr  *ai_addr; /* binary address */
	struct evutil_addrinfo  *ai_next; /* next structure in linked list */
};
#endif
/** @name evutil_getaddrinfo() error codes

    These values are possible error codes for evutil_getaddrinfo() and
    related functions.

    @{
*/
#ifdef EAI_ADDRFAMILY
#define EVUTIL_EAI_ADDRFAMILY EAI_ADDRFAMILY
#else
#define EVUTIL_EAI_ADDRFAMILY -901
#endif
#ifdef EAI_AGAIN
#define EVUTIL_EAI_AGAIN EAI_AGAIN
#else
#define EVUTIL_EAI_AGAIN -902
#endif
#ifdef EAI_BADFLAGS
#define EVUTIL_EAI_BADFLAGS EAI_BADFLAGS
#else
#define EVUTIL_EAI_BADFLAGS -903
#endif
#ifdef EAI_FAIL
#define EVUTIL_EAI_FAIL EAI_FAIL
#else
#define EVUTIL_EAI_FAIL -904
#endif
#ifdef EAI_FAMILY
#define EVUTIL_EAI_FAMILY EAI_FAMILY
#else
#define EVUTIL_EAI_FAMILY -905
#endif
#ifdef EAI_MEMORY
#define EVUTIL_EAI_MEMORY EAI_MEMORY
#else
#define EVUTIL_EAI_MEMORY -906
#endif
/* This test is a bit complicated, since some MS SDKs decide to
 * remove NODATA or redefine it to be the same as NONAME, in a
 * fun interpretation of RFC 2553 and RFC 3493. */
#if defined(EAI_NODATA) && (!defined(EAI_NONAME) || EAI_NODATA != EAI_NONAME)
#define EVUTIL_EAI_NODATA EAI_NODATA
#else
#define EVUTIL_EAI_NODATA -907
#endif
#ifdef EAI_NONAME
#define EVUTIL_EAI_NONAME EAI_NONAME
#else
#define EVUTIL_EAI_NONAME -908
#endif
#ifdef EAI_SERVICE
#define EVUTIL_EAI_SERVICE EAI_SERVICE
#else
#define EVUTIL_EAI_SERVICE -909
#endif
#ifdef EAI_SOCKTYPE
#define EVUTIL_EAI_SOCKTYPE EAI_SOCKTYPE
#else
#define EVUTIL_EAI_SOCKTYPE -910
#endif
#ifdef EAI_SYSTEM
#define EVUTIL_EAI_SYSTEM EAI_SYSTEM
#else
#define EVUTIL_EAI_SYSTEM -911
#endif

#define EVUTIL_EAI_CANCEL -90001

#ifdef AI_PASSIVE
#define EVUTIL_AI_PASSIVE AI_PASSIVE
#else
#define EVUTIL_AI_PASSIVE 0x1000
#endif
#ifdef AI_CANONNAME
#define EVUTIL_AI_CANONNAME AI_CANONNAME
#else
#define EVUTIL_AI_CANONNAME 0x2000
#endif
#ifdef AI_NUMERICHOST
#define EVUTIL_AI_NUMERICHOST AI_NUMERICHOST
#else
#define EVUTIL_AI_NUMERICHOST 0x4000
#endif
#ifdef AI_NUMERICSERV
#define EVUTIL_AI_NUMERICSERV AI_NUMERICSERV
#else
#define EVUTIL_AI_NUMERICSERV 0x8000
#endif
#ifdef AI_V4MAPPED
#define EVUTIL_AI_V4MAPPED AI_V4MAPPED
#else
#define EVUTIL_AI_V4MAPPED 0x10000
#endif
#ifdef AI_ALL
#define EVUTIL_AI_ALL AI_ALL
#else
#define EVUTIL_AI_ALL 0x20000
#endif
#ifdef AI_ADDRCONFIG
#define EVUTIL_AI_ADDRCONFIG AI_ADDRCONFIG
#else
#define EVUTIL_AI_ADDRCONFIG 0x40000
#endif
/**@}*/

struct evutil_addrinfo;
/**
 * This function clones getaddrinfo for systems that don't have it.  For full
 * details, see RFC 3493, section 6.1.
 *
 * Limitations:
 * - When the system has no getaddrinfo, we fall back to gethostbyname_r or
 *   gethostbyname, with their attendant issues.
 * - The AI_V4MAPPED and AI_ALL flags are not currently implemented.
 *
 * For a nonblocking variant, see evdns_getaddrinfo.
 */
int evutil_getaddrinfo(const char *nodename, const char *servname,
    const struct evutil_addrinfo *hints_in, struct evutil_addrinfo **res);

/** Release storage allocated by evutil_getaddrinfo or evdns_getaddrinfo. */
void evutil_freeaddrinfo(struct evutil_addrinfo *ai);

const char *evutil_gai_strerror(int err);

/** Generate n bytes of secure pseudorandom data, and store them in buf.
 *
 * By default, Libevent uses an ARC4-based random number generator, seeded
 * using the platform's entropy source (/dev/urandom on Unix-like systems;
 * CryptGenRandom on Windows).
 */
void evutil_secure_rng_get_bytes(void *buf, size_t n);

/**
 * Seed the secure random number generator if needed, and return 0 on
 * success or -1 on failure.
 *
 * It is okay to call this function more than once; it will still return
 * 0 if the RNG has been successfully seeded and -1 if it can't be
 * seeded.
 *
 * Ordinarily you don't need to call this function from your own code;
 * Libevent will seed the RNG itself the first time it needs good random
 * numbers.  You only need to call it if (a) you want to double-check
 * that one of the seeding methods did succeed, or (b) you plan to drop
 * the capability to seed (by chrooting, or dropping capabilities, or
 * whatever), and you want to make sure that seeding happens before your
 * program loses the ability to do it.
 */
int evutil_secure_rng_init(void);


#ifdef HAVE_ARC4RANDOM_ADDRANDOM
/** Seed the random number generator with extra random bytes.

    You should almost never need to call this function; it should be
    sufficient to invoke evutil_secure_rng_init(), or let Libevent take
    care of calling evutil_secure_rng_init() on its own.

    If you call this function as a _replacement_ for the regular
    entropy sources, then you need to be sure that your input
    contains a fairly large amount of strong entropy.  Doing so is
    notoriously hard: most people who try get it wrong.  Watch out!

    @param dat a buffer full of a strong source of random numbers
    @param datlen the number of bytes to read from datlen
 */


void evutil_secure_rng_add_bytes(const char *dat, size_t datlen);
#endif
#ifdef __cplusplus
}
#endif

#endif /* _EVUTIL_H_ */
