section .text
	global main
	extern scanf
	extern printf

section .data
	inpformat:  db "%hd",0
	outformat:  db "%hd",10,0,
	b3c45:	dw	1
	b2d6:	dw	1
	d6:	dw	1


main:
push rsi
push rdi
push ax
mov rsi, b3c45
mov rdi, inpformat
mov al, 0
call scanf
pop ax
pop rdi
pop rsi


push rsi
push rdi
push ax
mov rsi, b2d6
mov rdi, inpformat
mov al, 0
call scanf
pop ax
pop rdi
pop rsi


push ax
mov ax, 100
mov [d6],  ax
pop ax


push bx
mov ax,[b3c45]
mov bx,[b2d6]
cmp ax,bx
jle LE0
mov ax,0
jmp LE1
LE0 : mov ax,0ffffh
LE1 : nop
pop bx

cmp ax, 0
je ELSE0
push ax
push bx
mov ax, [d6]
push ax
mov ax, 100
push ax
pop bx
pop ax
add ax,bx
pop bx
mov [d6],  ax
pop ax


jmp ENDIF0
ELSE0:
push ax
push bx
mov ax, [d6]
push ax
mov ax, 200
push ax
pop bx
pop ax
sub ax,bx
pop bx
mov [d6],  ax
pop ax



ENDIF0:
push rdi
push rsi
push rax
push rcx
push ax
mov rsi, [d6]
mov rdi, outformat
mov al,0
call printf
pop ax
pop rcx
pop rax
pop rsi
pop rdi




ret
