.global _start
.text
_start:
top:
	# read(0, buffer, 1)
	mov $0, %rax			# system call 0 is read
	mov $0, %rdi			# file handle 0 is stdin
	mov $buffer, %rsi		# Read into buffer
	mov $1, %rdx			# number of bytes
	syscall

	# if (ret = 0) exit; # no bytes read
	cmp $0, %eax
	je exit

	# if (buffer == 4) exit; # ^D/eof
	cmp $4, %eax
	je exit

	# write(1, buffer, 1)
	mov     $1, %rax                # system call 1 is write
	mov     $1, %rdi                # file handle 1 is stdout
	mov     $buffer, %rsi           # address of string to output
	mov     $1, %rdx                # number of bytes
	syscall                         # invoke operating system to do the write
	jmp top
exit:
	# exit(0)
	mov     $60, %rax               # system call 60 is exit
	xor     %rdi, %rdi              # we want return code 0
	syscall                         # invoke operating system to exit
.data
buffer:
	.space 1
