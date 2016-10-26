GLOBAL call_task_handler

;call_task_handler(unsigned long handler)
call_task_handler:
	push rbp
	mov rbp, rsp

	call rdi

	mov rsp, rbp
	pop rbp
	ret