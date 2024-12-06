PAUSE - starting on parser. have defined ASS. Now need to add the parse_file function. Think about how to add node to ASS?


TODO 
    [ ] skip transpiling if null input
    [] work out a way of namepsacing the types like int
    [] add print functions for int, and then for all primitive types
    [] add dev mode. guard "____" at start of identifier as protected


NOTES
    - STRINGS! we will only use char* strings as a default. If the strings need to be mutated, we will use malloc. If not, we will use static strings like `const char *str = "Hello, world!";`





EPERM: Operation not permitted.
ENOENT: No such file or directory.
ESRCH: No such process.
EINTR: Interrupted system call.
EIO: I/O error.
ENXIO: No such device or address.
E2BIG: Argument list too long.
ENOEXEC: Exec format error.
EBADF: Bad file number.
ECHILD: No child processes.
EAGAIN: Try again.
ENOMEM: Out of memory.
EACCES: Permission denied.
EFAULT: Bad address.
EBUSY: Device or resource busy.
EEXIST: File exists.
EXDEV: Cross-device link.
ENODEV: No such device.
ENOTDIR: Not a directory.
EISDIR: Is a directory.
ENFILE: File table overflow.
EMFILE: Too many open files.
ENOTTY: Not a typewriter.
EFBIG: File too large.
ENOSPC: No space left on device.
ESPIPE: Illegal seek.
EROFS: Read-only file system.
EMLINK: Too many links.
EPIPE: Broken pipe.
EDOM: Math argument out of domain of function.
EDEADLK: Resource deadlock avoided.
ENAMETOOLONG: File name too long.
ENOLCK: No record locks available.
ENOSYS: Function not implemented.
ENOTEMPTY: Directory not empty.
EINVAL: Invalid argument.
ERANGE: Result too large.
EILSEQ: Illegal byte sequence.
STRUNCATE: String was truncated.
EDEADLOCK: Resource deadlock avoided (same as EDEADLK).
EADDRINUSE: Address already in use.
EADDRNOTAVAIL: Cannot assign requested address.
EAFNOSUPPORT: Address family not supported by protocol.
EALREADY: Operation already in progress.
EBADMSG: Bad message.
ECANCELED: Operation canceled.
ECONNABORTED: Software caused connection abort.
ECONNREFUSED: Connection refused.
ECONNRESET: Connection reset by peer.
EDESTADDRREQ: Destination address required.
EHOSTUNREACH: No route to host.
EIDRM: Identifier removed.
EINPROGRESS: Operation now in progress.
EISCONN: Transport endpoint is already connected.
ELOOP: Too many symbolic links encountered.
EMSGSIZE: Message too long.
ENETDOWN: Network is down.
ENETRESET: Network dropped connection on reset.
ENETUNREACH: Network is unreachable.
ENOBUFS: No buffer space available.
ENODATA: No data available.
ENOLINK: Link has been severed.
ENOMSG: No message of desired type.
ENOPROTOOPT: Protocol not available.
ENOSR: Out of streams resources.
ENOSTR: Device not a stream.
ENOTCONN: Transport endpoint is not connected.
ENOTRECOVERABLE: State not recoverable.
ENOTSOCK: Socket operation on non-socket.
ENOTSUP: Operation not supported.
EOPNOTSUPP: Operation not supported on transport endpoint.
EOTHER: Other error.
EOVERFLOW: Value too large for defined data type.
EOWNERDEAD: Owner died.
EPROTO: Protocol error.
EPROTONOSUPPORT: Protocol not supported.
EPROTOTYPE: Protocol wrong type for socket.
ETIME: Timer expired.
ETIMEDOUT: Connection timed out.
ETXTBSY: Text file busy.
EWOULDBLOCK: Operation would block (same as EAGAIN).