GLOBAL _syscall

;_syscall(int id, int arg1, int arg2, int arg3)
_syscall:
	push rbp
	mov rbp, rsp

	int 80h

	leave
	ret
