GLOBAL read_port
GLOBAL write_port
GLOBAL read_port_word
GLOBAL write_port_word

section .text

;read_port(port)
read_port:
	push rbp
	mov rbp, rsp
	
	mov rdx, rdi
	in al, dx	

	mov rsp, rbp
	pop rbp
	ret

;write_port(port, data)
write_port:
	push rbp
	mov rbp, rsp

	mov rdx, rdi    
	mov rax, rsi
	out dx, al  

	mov rsp, rbp
	pop rbp
	ret

;read_port_word(port)
read_port_word:
	push rbp
	mov rbp, rsp
	
	mov rdx, rdi
	in ax, dx	

	mov rsp, rbp
	pop rbp
	ret

;write_port_word(port, data)
write_port_word:
	push rbp
	mov rbp, rsp

	mov rdx, rdi    
	mov rax, rsi
	out dx, ax

	mov rsp, rbp
	pop rbp
	ret