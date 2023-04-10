#include "reimpl/ioctl.h"

#include "utils/logger.h"

int ioctl_soloader(int fildes, int request, ... /* arg */) {
	logv_error("ioctl: fd %i, request %i (0x%x)", fildes, request, request);
	return 0;
}
