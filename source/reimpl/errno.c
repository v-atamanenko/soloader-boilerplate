#include "reimpl/errno.h"

#include <sys/errno.h>
#include <string.h>
#include "_errno_bionic.h"
#include "utils/logger.h"

typedef struct {
	int errno_newlib;
	int errno_bionic;
	const char * strerror;
} newlib_errno_to_bionic_errno;

static newlib_errno_to_bionic_errno errno_translation[] = {
	{ 0, 0, "Success" },
	{ EPERM, EPERM_BIONIC, "Operation not permitted" },
	{ ENOENT, ENOENT_BIONIC, "No such file or directory" },
	{ ESRCH, ESRCH_BIONIC, "No such process" },
	{ EINTR, EINTR_BIONIC, "Interrupted system call" },
	{ EIO, EIO_BIONIC, "I/O error" },
	{ ENXIO, ENXIO_BIONIC, "No such device or address" },
	{ E2BIG, E2BIG_BIONIC, "Argument list too long" },
	{ ENOEXEC, ENOEXEC_BIONIC, "Exec format error" },
	{ EBADF, EBADF_BIONIC, "Bad file descriptor" },
	{ ECHILD, ECHILD_BIONIC, "No child processes" },
	{ EAGAIN, EAGAIN_BIONIC, "Try again" },
	{ ENOMEM, ENOMEM_BIONIC, "Out of memory" },
	{ EACCES, EACCES_BIONIC, "Permission denied" },
	{ EFAULT, EFAULT_BIONIC, "Bad address" },
	{ ENOTBLK, ENOTBLK_BIONIC, "Block device required" },
	{ EBUSY, EBUSY_BIONIC, "Device or resource busy" },
	{ EEXIST, EEXIST_BIONIC, "File exists" },
	{ EXDEV, EXDEV_BIONIC, "Cross-device link" },
	{ ENODEV, ENODEV_BIONIC, "No such device" },
	{ ENOTDIR, ENOTDIR_BIONIC, "Not a directory" },
	{ EISDIR, EISDIR_BIONIC, "Is a directory" },
	{ EINVAL, EINVAL_BIONIC, "Invalid argument" },
	{ ENFILE, ENFILE_BIONIC, "File table overflow" },
	{ EMFILE, EMFILE_BIONIC, "Too many open files" },
	{ ENOTTY, ENOTTY_BIONIC, "Inappropriate ioctl for device" },
	{ ETXTBSY, ETXTBSY_BIONIC, "Text file busy" },
	{ EFBIG, EFBIG_BIONIC, "File too large" },
	{ ENOSPC, ENOSPC_BIONIC, "No space left on device" },
	{ ESPIPE, ESPIPE_BIONIC, "Illegal seek" },
	{ EROFS, EROFS_BIONIC, "Read-only file system" },
	{ EMLINK, EMLINK_BIONIC, "Too many links" },
	{ EPIPE, EPIPE_BIONIC, "Broken pipe" },
	{ EDOM, EDOM_BIONIC, "Math argument out of domain of func" },
	{ ERANGE, ERANGE_BIONIC, "Math result not representable" },
	{ ENOMSG, ENOMSG_BIONIC, "No message of desired type" },
	{ EIDRM, EIDRM_BIONIC, "Identifier removed" },
	{ EDEADLK, EDEADLK_BIONIC, "Resource deadlock would occur" },
	{ ENOLCK, ENOLCK_BIONIC, "No record locks available" },
	{ ENOSTR, ENOSTR_BIONIC, "Device not a stream" },
	{ ENODATA, ENODATA_BIONIC, "No data available" },
	{ ETIME, ETIME_BIONIC, "Timer expired" },
	{ ENOSR, ENOSR_BIONIC, "Out of streams resources" },
	{ EREMOTE, EREMOTE_BIONIC, "Object is remote" },
	{ ENOLINK, ENOLINK_BIONIC, "Link has been severed" },
	{ EPROTO, EPROTO_BIONIC, "Protocol error" },
	{ EMULTIHOP, EMULTIHOP_BIONIC, "Multihop attempted" },
	{ EBADMSG, EBADMSG_BIONIC, "Not a data message" },
	// { EFTYPE, 0, "" }, // EFTYPE is only for newlib internal usage
	{ ENOSYS, ENOSYS_BIONIC, "Function not implemented" },
	{ ENOTEMPTY, ENOTEMPTY_BIONIC, "Directory not empty" },
	{ ENAMETOOLONG, ENAMETOOLONG_BIONIC, "File name too long" },
	{ ELOOP, ELOOP_BIONIC, "Too many symbolic links encountered" },
	{ EOPNOTSUPP, EOPNOTSUPP_BIONIC, "Operation not supported on transport endpoint" },
	{ EPFNOSUPPORT, EPFNOSUPPORT_BIONIC, "Protocol family not supported" },
	{ ECONNRESET, ECONNRESET_BIONIC, "Connection reset by peer" },
	{ ENOBUFS, ENOBUFS_BIONIC, "No buffer space available" },
	{ EAFNOSUPPORT, EAFNOSUPPORT_BIONIC, "Address family not supported by protocol" },
	{ EPROTOTYPE, EPROTOTYPE_BIONIC, "Protocol wrong type for socket" },
	{ ENOTSOCK, ENOTSOCK_BIONIC, "Socket operation on non-socket" },
	{ ENOPROTOOPT, ENOPROTOOPT_BIONIC, "Protocol not available" },
	{ ESHUTDOWN, ESHUTDOWN_BIONIC, "Cannot send after transport endpoint shutdown" },
	{ ECONNREFUSED, ECONNREFUSED_BIONIC, "Connection refused" },
	{ EADDRINUSE, EADDRINUSE_BIONIC, "Address already in use" },
	{ ECONNABORTED, ECONNABORTED_BIONIC, "Software caused connection abort" },
	{ ENETUNREACH, ENETUNREACH_BIONIC, "Network is unreachable" },
	{ ENETDOWN, ENETDOWN_BIONIC, "Network is down" },
	{ ETIMEDOUT, ETIMEDOUT_BIONIC, "Connection timed out" },
	{ EHOSTDOWN, EHOSTDOWN_BIONIC, "Host is down" },
	{ EHOSTUNREACH, EHOSTUNREACH_BIONIC, "No route to host" },
	{ EINPROGRESS, EINPROGRESS_BIONIC, "Operation now in progress" },
	{ EALREADY, EALREADY_BIONIC, "Operation already in progress" },
	{ EDESTADDRREQ, EDESTADDRREQ_BIONIC, "Destination address required" },
	{ EMSGSIZE, EMSGSIZE_BIONIC, "Message too long" },
	{ EPROTONOSUPPORT, EPROTONOSUPPORT_BIONIC, "Protocol not supported" },
	{ ESOCKTNOSUPPORT, ESOCKTNOSUPPORT_BIONIC, "Socket type not supported" },
	{ EADDRNOTAVAIL, EADDRNOTAVAIL_BIONIC, "Cannot assign requested address" },
	{ ENETRESET, ENETRESET_BIONIC, "Network dropped connection because of reset" },
	{ EISCONN, EISCONN_BIONIC, "Transport endpoint is already connected" },
	{ ENOTCONN, ENOTCONN_BIONIC, "Transport endpoint is not connected" },
	{ ETOOMANYREFS, ETOOMANYREFS_BIONIC, "Too many references: cannot splice" },
	{ EUSERS, EUSERS_BIONIC, "Too many users" },
	{ EDQUOT, EDQUOT_BIONIC, "Quota exceeded" },
	{ ESTALE, ESTALE_BIONIC, "Stale NFS file handle" },
	{ ENOTSUP, ENOTSUP_BIONIC, "Operation not supported" },
	{ EILSEQ, EILSEQ_BIONIC, "Illegal byte sequence" },
	{ EOVERFLOW, EOVERFLOW_BIONIC, "Value too large for defined data type" },
	{ ECANCELED, ECANCELED_BIONIC, "Operation Canceled" },
	{ ENOTRECOVERABLE, ENOTRECOVERABLE_BIONIC, "State not recoverable" },
	{ EOWNERDEAD, EOWNERDEAD_BIONIC, "Owner died" },
};

int * __errno_soloader(void) {
	int e = errno;

	for (int i = 0; i < sizeof(errno_translation) / sizeof(errno_translation[0]); ++i) {
		if (e == errno_translation[i].errno_newlib)
			return (int *) &errno_translation[i].errno_bionic;
	}

	logv_error("[errno]: Unexpected newlib errno %i, will return 0 instead of translation", e);
	return (int *) &errno_translation[0].errno_bionic;
}

char * strerror_soloader(int error_number) {
	for (int i = 0; i < sizeof(errno_translation) / sizeof(errno_translation[0]); ++i) {
		if (error_number == errno_translation[i].errno_bionic)
			return (char *) errno_translation[i].strerror;
	}

	logv_error("[strerror]: Unexpected bionic errno %i, will return 'Success' instead of translation", error_number);
	return (char *) errno_translation[0].strerror;
}

int strerror_r_soloader(int error_number, char* buf, size_t buf_len) {
	const char * err = NULL;

	for (int i = 0; i < sizeof(errno_translation) / sizeof(errno_translation[0]); ++i) {
		if (error_number == errno_translation[i].errno_bionic)
			err = errno_translation[i].strerror;
	}

	if (err == NULL) {
		logv_error("[strerror_r]: Unexpected bionic errno %i, will return 'Success' instead of translation", error_number);
		err = errno_translation[0].strerror;
	}

	size_t length = strlcpy(buf, err, buf_len);

	if (length >= buf_len) {
		return ERANGE_BIONIC;
	}

	return 0;
}
