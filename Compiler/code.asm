section .text
	global main
	extern scanf
	extern printf

section .data
	inpformat:  db "%hd",0
	outformat:  db "%hd",10,0,
	b3:	dw	1
	b2:	dw	1
	c2:	dw	1
	d2:	dw	1


main:
push rsi
push rdi
push ax
mov rsi, b2
mov rdi, inpformat
mov al, 0
call scanf
pop ax
pop rdi
pop rsi


push rdi
push rsi
push rax
push rcx
push ax
mov rsi, [b2]
mov rdi, outformat
mov al,0
call printf
pop ax
pop rcx
pop rax
pop rsi
pop rdi




ret
