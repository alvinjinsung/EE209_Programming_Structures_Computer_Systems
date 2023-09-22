### --------------------------------------------------------------------
### 
### Name: 최진성
### # of assignment: 4
### Name of the file: mydc.s
###
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1

	## Former parameter offsets
	.equ	IBASE, 8
	.equ	IEXP, 4

	## Local variable offsets
	.equ	IPOWER, -4
	.equ	IINDEX, -8
	
.section ".rodata"

scanfFormat:
	.asciz "%s"

## error message when stack is empty
emptywarning:
	.asciz "dc: stack empty\n"

## printing format for the result
printformat:
	.asciz "%d\n"
### --------------------------------------------------------------------

        .section ".data"

### --------------------------------------------------------------------

        .section ".bss"
buffer:
        .skip  ARRAYSIZE

### --------------------------------------------------------------------

	.section ".text"

	## -------------------------------------------------------------
	## int main(void)
	## Runs desk calculator program.  Returns 0.
	## -------------------------------------------------------------

	.globl  main
	.type   main,@function

main:

	pushl   %ebp
	movl    %esp, %ebp

input:

	## dc number stack initialized. %esp = %ebp
	
	## scanf("%s", buffer)
	pushl	$buffer
	pushl	$scanfFormat
	call    scanf
	addl    $8, %esp

	## check if user input EOF
	cmp	$EOF, %eax
	je	quit

	## check if the input is digit or not
	movl	$0, %eax
	movb	buffer, %al
	pushl 	%eax
	call 	isdigit
	addl	$4, %esp

	## if it is not digit, jump to indicate
	cmpl	$0, %eax
	je	indicate

	## if digit, change to int format and push
	movl	$buffer, %eax
	pushl	%eax
	call	atoi
	addl	$4, %esp
	pushl	%eax

	## jump to input
	jmp	input

indicate:

	## initialize %eax to 0
	movl	$0, %eax

	movb	buffer, %al

	## if 'p', jump to printing
	cmpl	$'p', %eax 
	je	printing
	
	## if 'q', jump to quit
	cmpl	$'q', %eax 
	je	quit

	## if '+', jump to adding
	cmpl	$'+', %eax 
	je	adding

	## if '-', jump to substraction
	cmpl	$'-', %eax
	je	substraction

	## if '*', jump to multiplication
	cmpl	$'*', %eax 
	je	multiplication

	## if '/', jump to division
	cmpl	$'/', %eax 
	je	division

	## if '%', jump to remainder
	cmpl	$'%', %eax 
	je	remainder

	## if '^', jump to exponointiation
	cmpl	$'^', %eax 
	je	exponentiation

	## if '_', jump to underscore
	cmpl	$'_', %eax 
	je	underscore

	## if 'f', jump to fullprint
	cmpl	$'f', %eax 
	je	fullprint

	## if 'c', jump to clear
	cmpl	$'c', %eax 
	je	clear
	
	## if 'd', jump to duplicatte
	cmpl	$'d', %eax 
	je	duplicate

	## if 'r', jump to reverse
	cmpl	$'r', %eax 
	je	reverse

	## if non of the above, jump to input
	jmp input

adding:
	## if stack is empty, jump to empty
	cmpl	%esp, %ebp
	je	empty

	## pop the top value and save it to %ebx
	popl	%ebx

	## if stack is empty, jump to empty2
	cmpl	%esp, %ebp
	je	empty2

	## pop the top value and save it to %eax
	popl	%eax

	## add %eax and %ebx and push it in the stack
	addl	%ebx, %eax
	pushl	%eax

	## jump to input
	jmp input

substraction:
	## if stack is empty, jump to empty
	cmpl	%esp, %ebp
	je	empty

	## pop the top value and save it to %ebx
	popl	%ebx

	## if stack is empty, jump to empty2
	cmpl	%esp, %ebp
	je	empty2

	## pop the top value and save it to %eax
	popl	%eax

	## substract %eax and %ebx and push it in the stack
	subl	%ebx, %eax
	pushl	%eax

	## jump to input
	jmp	input

multiplication:
	## if stack is empty, jump to empty
	cmpl	%esp, %ebp
	je empty

	## pop the top value and save it to %ebx
	popl	%ebx

	## if stack is empty, jump to empty2
	cmpl	%esp, %ebp
	je empty2

	## pop the top value and save it to %eax
	popl	%eax

	## multiply %eax and %ebx and push it in the stack
	imull	%ebx, %eax
	pushl	%eax

	## jump to input
	jmp input

division:
	## if stack is empty, jump to empty
	cmpl	%esp, %ebp
	je empty

	## pop the top value and save it to %ebx
	popl	%ebx

	## if stack is empty, jump to empty2
	cmpl	%esp, %ebp
	je empty2

	## pop the top value and save it to %eax
	popl	%eax

	## divide %eax and %ebx
	cdq
	idiv	%ebx

	## if remainder is below 0, jump to divretool
	cmpl	$0, %edx
	jl	divretool

	## if remainder is not below 0, push the quotient into stack
	pushl	%eax

	## jump to input
	jmp	input

divretool:
	## decline the result and push it into stack
	decl	%eax
	pushl	%eax

	## jump to input
	jmp	input

remainder:
	## if stack is empty, jump to empty
	cmpl	%esp, %ebp
	je empty

	## pop the top value and save it to %ebx
	popl	%ebx

	## if stack is empty, jump to empty2
	cmpl	%esp, %ebp
	je empty2

	## pop the top value and save it to %eax
	popl	%eax

	## divide %eax and %ebx
	cdq
	idiv	%ebx

	## if remainder is below 0, jump to reretool
	cmpl	$0,%edx
	jl	reretool

	## if remainder is not below 0, push the remainder into stack
	pushl	%edx

	## jump to input
	jmp	input

reretool:
	## add divisor to quotient and push it into stack
	addl %ebx, %edx
	pushl	%edx

	## jump to input
	jmp input

exponentiation:
	## iPower=exponentiation(iBase, iExp)

	## int iPower = 1; int iIndex;
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$1
	subl	$4, %esp
	## iIndex = 1
	movl	$1, IINDEX(%ebp)

exploop:
	## if (iIndex > iExp) goto exploopend
	movl    IINDEX(%ebp), %eax
    cmpl    IEXP(%ebp), %eax
    jg      exploopend

    ## iPower *= iBase
    movl    IPOWER(%ebp), %eax
    imull   IBASE(%ebp), %eax
    movl    %eax, IPOWER(%ebp)

    ## iIndex++
    incl	IINDEX(%ebp)

    ## jump to input
    jmp	exploop

exploopend:
	## push the result into stack
	movl    IPOWER(%ebp), %eax
    movl    %ebp, %esp
    popl    %ebp
    pushl	%eax

    ## jump to input
    jmp	input

underscore:
	## change '_' to '0'
	movb	$'0', buffer
	## change the string to int value
	movl	$buffer, %eax
	pushl	%eax
	call atoi
	addl	$4, %esp

	## change sign of the value and push it in the stack
	negl	%eax
	pushl	%eax

	## jump to input
	jmp input

printing:
	## if stack is empty, jump to empty
	cmpl	%esp, %ebp
	je empty

	## print the top valu of the stack
	pushl	$printformat
	call	printf
	addl	$4, %esp

	## jump to input
	jmp input

fullprint:
	## save the %esp adress to %esi
	movl	%esp, %esi

fullprintloop:
	## if %esi=%ebp, goto floopend
	cmpl	%esi, %ebp
	je	floopend

	## print the value of %esi
	pushl	(%esi)
	pushl	$printformat
	call	printf
	addl	$8, %esp

	## %esi=%esi+4
	addl	$4, %esi

	## jump to fullprintloop
	jmp	fullprintloop

floopend:
	## jump to input
	jmp input

clear:
	## by changing %esp to %ebp, clears the content of the stack
	movl	%ebp, %esp

	## jump to input
	jmp input

duplicate:
	## if stack is empty, jump to empty
	cmpl	%esp, %ebp
	je	empty

	## pop the top value and save it to %ebx
	popl	%ebx

	## duplicate %ebx to %eax
	movl	%ebx, %eax

	## push both %eax and %ebx into stack
	pushl	%ebx
	pushl	%eax

	## jump to input
	jmp input

reverse:
	## if stack is empty, jump to empty
	cmpl	%esp, %ebp
	je	empty

	# pop the top value and save it to %ebx
	popl	%ebx

	## if stack is empty, jump to empty2
	cmpl	%esp, %ebp
	je	empty2

	## pop the top value and save it to %eax
	popl	%eax

	## reverse the order of %eax and %ebx to push into stack 
	pushl	%ebx
	pushl	%eax

	## jump to input
	jmp input

empty2:
	## print error meassage
	pushl	$emptywarning
	call	printf
	addl	$4, %esp

	## push the first value poped out into the stack again
	pushl	%ebx

	## jump to input
	jmp	input

empty:
	## print error message
	pushl	$emptywarning
	call	printf
	addl	$4, %esp

	## jimp to input
	jmp	input

quit:	
	## return 0
	movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret


