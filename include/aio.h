#ifndef _AIO_H
#define _AIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
#include <signal.h>
#include <time.h>

#define __NEED_ssize_t
#define __NEED_off_t

#include <bits/alltypes.h>


/*
** AIO is a asynchronous I/O interface.  It allows an application to
** issue I/O requests and then continue processing.  The application
** can later check to see if the I/O is complete and retrieve the results.
**
** Example:
**   struct aiocb cb;
**   char buf[100];
**   cb.aio_fildes = fd;
**   cb.aio_buf = buf;
**   cb.aio_nbytes = 100;
**   cb.aio_offset = 0;
**   aio_read(&cb);
**   do_something_while_io_is_in_progress();
**   while (aio_error(&cb) == EINPROGRESS) {
**     do_something_while_io_is_in_progress();
**   }
**   aio_return(&cb);
*/



/* * The following structure is used to describe an I/O request.
** * The application must initialize the fields in this structure
** * before calling aio_read(), aio_write(), or aio_error().
** * The application can use the aio_return() function to retrieve
** * the return status of the I/O request.
** * The application can use the aio_cancel() function to cancel
** * an I/O request.
** * The application can use the aio_suspend() function to wait
** * for an I/O request to complete.
** * The application can use the lio_listio() function to issue
** * multiple I/O requests.
** * The application can use the aio_fsync() function to wait
** * for an I/O request to complete and then synchronize the file.
** *
** * struct aiocb, data member descriptions:
** *   aio_fildes - file descriptor of file to be read or written
** *   aio_lio_opcode - operation to be performed (LIO_READ, LIO_WRITE, LIO_NOP)
** *   aio_reqprio - request priority
** *   aio_buf - pointer to buffer for read or write
** *   aio_nbytes - number of bytes to read or write
** *   struct sigevent aio_sigevent - notification mechanism to be used when I/O completes, (XXX: signal or callback function)
** *   __td - thread id of thread that issued the I/O request
** *   __lock[2] - lock for thread that issued the I/O request
** *  __err - error code for I/O request
** *   __ret - return value for I/O request
** *   aio_offset - file offset for read or write
** *   __next - pointer to next I/O request in list
** *   __prev - pointer to previous I/O request in list
** */
struct aiocb {
	int aio_fildes, aio_lio_opcode, aio_reqprio;
	volatile void *aio_buf;
	size_t aio_nbytes;
	struct sigevent aio_sigevent;
	void *__td;
	int __lock[2];
	volatile int __err;
	ssize_t __ret;
	off_t aio_offset;
	void *__next, *__prev;
	char __dummy4[32-2*sizeof(void *)];
};

#define AIO_CANCELED 0
#define AIO_NOTCANCELED 1
#define AIO_ALLDONE 2

#define LIO_READ 0
#define LIO_WRITE 1
#define LIO_NOP 2

#define LIO_WAIT 0
#define LIO_NOWAIT 1

/* aio_read() - initiate an asynchronous read operation */
int aio_read(struct aiocb *);
/* aio_write() - initiate an asynchronous write operation */
int aio_write(struct aiocb *);
/* aio_error() - return the error status of an asynchronous operation */
int aio_error(const struct aiocb *);
/* aio_return() - return the return status of an asynchronous operation */
ssize_t aio_return(struct aiocb *);
/* aio_cancel() - cancel an asynchronous operation */
int aio_cancel(int, struct aiocb *);
/* aio_suspend() - suspend execution until one or more asynchronous operations complete */
int aio_suspend(const struct aiocb *const [], int, const struct timespec *);
/* aio_fsync() - wait for an asynchronous operation to complete and synchronize the file */
int aio_fsync(int, struct aiocb *);

/* lio_listio() - issue multiple asynchronous operations */
int lio_listio(int, struct aiocb *__restrict const *__restrict, int, struct sigevent *__restrict);

#if defined(_LARGEFILE64_SOURCE)
#define aiocb64 aiocb
#define aio_read64 aio_read
#define aio_write64 aio_write
#define aio_error64 aio_error
#define aio_return64 aio_return
#define aio_cancel64 aio_cancel
#define aio_suspend64 aio_suspend
#define aio_fsync64 aio_fsync
#define lio_listio64 lio_listio
#define off64_t off_t
#endif

#if _REDIR_TIME64
__REDIR(aio_suspend, __aio_suspend_time64);
#endif

#ifdef __cplusplus
}
#endif

#endif
