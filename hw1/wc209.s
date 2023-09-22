	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_count_lines            ## -- Begin function count_lines
	.p2align	4, 0x90
_count_lines:                           ## @count_lines
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi0:
	.cfi_def_cfa_offset 16
Lcfi1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi2:
	.cfi_def_cfa_register %rbp
	movb	%dil, %al
	movb	%al, -1(%rbp)
	movq	%rsi, -16(%rbp)
	movsbl	-1(%rbp), %edi
	cmpl	$10, %edi
	jne	LBB0_2
## BB#1:
	movq	-16(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -16(%rbp)
LBB0_2:
	movq	-16(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi3:
	.cfi_def_cfa_offset 16
Lcfi4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi5:
	.cfi_def_cfa_register %rbp
	subq	$96, %rsp
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	movq	$0, -16(%rbp)
	movq	$1, -24(%rbp)
	movq	$0, -32(%rbp)
	movq	$0, -40(%rbp)
	movl	$3, -44(%rbp)
LBB1_1:                                 ## =>This Inner Loop Header: Depth=1
	callq	_getchar
	movl	%eax, -8(%rbp)
	cmpl	$-1, %eax
	je	LBB1_51
## BB#2:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-44(%rbp), %eax
	movl	%eax, %ecx
	movq	%rcx, %rdx
	subq	$5, %rdx
	movq	%rcx, -72(%rbp)         ## 8-byte Spill
	movq	%rdx, -80(%rbp)         ## 8-byte Spill
	ja	LBB1_50
## BB#58:                               ##   in Loop: Header=BB1_1 Depth=1
	leaq	LJTI1_0(%rip), %rax
	movq	-72(%rbp), %rcx         ## 8-byte Reload
	movslq	(%rax,%rcx,4), %rdx
	addq	%rax, %rdx
	jmpq	*%rdx
LBB1_3:                                 ##   in Loop: Header=BB1_1 Depth=1
	cmpl	$42, -8(%rbp)
	jne	LBB1_5
## BB#4:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	$5, -44(%rbp)
LBB1_5:                                 ##   in Loop: Header=BB1_1 Depth=1
	movq	$0, -56(%rbp)
	movq	$0, -64(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -56(%rbp)
	movl	-8(%rbp), %ecx
	movb	%cl, %dl
	movq	-24(%rbp), %rsi
	movsbl	%dl, %edi
	callq	_count_lines
	movq	%rax, -64(%rbp)
	movq	-40(%rbp), %rax
	addq	-64(%rbp), %rax
	subq	-56(%rbp), %rax
	movq	%rax, -40(%rbp)
	movl	-8(%rbp), %ecx
	movb	%cl, %dl
	movq	-24(%rbp), %rsi
	movsbl	%dl, %edi
	callq	_count_lines
	movq	%rax, -24(%rbp)
	jmp	LBB1_50
LBB1_6:                                 ##   in Loop: Header=BB1_1 Depth=1
	cmpl	$47, -8(%rbp)
	jne	LBB1_8
## BB#7:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	$3, -44(%rbp)
	jmp	LBB1_15
LBB1_8:                                 ##   in Loop: Header=BB1_1 Depth=1
	cmpl	$42, -8(%rbp)
	jne	LBB1_10
## BB#9:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	$5, -44(%rbp)
	jmp	LBB1_14
LBB1_10:                                ##   in Loop: Header=BB1_1 Depth=1
	cmpl	$10, -8(%rbp)
	jne	LBB1_12
## BB#11:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$0, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
	jmp	LBB1_13
LBB1_12:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	$0, -44(%rbp)
LBB1_13:                                ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_14
LBB1_14:                                ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_15
LBB1_15:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-8(%rbp), %eax
	movb	%al, %cl
	movq	-24(%rbp), %rsi
	movsbl	%cl, %edi
	callq	_count_lines
	movq	%rax, -24(%rbp)
	jmp	LBB1_50
LBB1_16:                                ##   in Loop: Header=BB1_1 Depth=1
	cmpl	$47, -8(%rbp)
	jne	LBB1_18
## BB#17:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$1, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
	movq	-32(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -32(%rbp)
	jmp	LBB1_22
LBB1_18:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-8(%rbp), %edi
	callq	_isspace
	cmpl	$0, %eax
	je	LBB1_20
## BB#19:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$3, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
	jmp	LBB1_21
LBB1_20:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	$4, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
	movq	-32(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -32(%rbp)
LBB1_21:                                ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_22
LBB1_22:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-8(%rbp), %eax
	movb	%al, %cl
	movq	-24(%rbp), %rsi
	movsbl	%cl, %edi
	callq	_count_lines
	movq	%rax, -24(%rbp)
	jmp	LBB1_50
LBB1_23:                                ##   in Loop: Header=BB1_1 Depth=1
	cmpl	$47, -8(%rbp)
	jne	LBB1_25
## BB#24:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$2, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
	jmp	LBB1_29
LBB1_25:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-8(%rbp), %edi
	callq	_isspace
	cmpl	$0, %eax
	jne	LBB1_27
## BB#26:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$4, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
	jmp	LBB1_28
LBB1_27:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	$3, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
LBB1_28:                                ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_29
LBB1_29:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-8(%rbp), %eax
	movb	%al, %cl
	movq	-24(%rbp), %rsi
	movsbl	%cl, %edi
	callq	_count_lines
	movq	%rax, -24(%rbp)
	jmp	LBB1_50
LBB1_30:                                ##   in Loop: Header=BB1_1 Depth=1
	cmpl	$42, -8(%rbp)
	jne	LBB1_32
## BB#31:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$0, -44(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
	jmp	LBB1_39
LBB1_32:                                ##   in Loop: Header=BB1_1 Depth=1
	cmpl	$47, -8(%rbp)
	jne	LBB1_34
## BB#33:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$2, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
	jmp	LBB1_38
LBB1_34:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-8(%rbp), %edi
	callq	_isspace
	cmpl	$0, %eax
	je	LBB1_36
## BB#35:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$3, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
	jmp	LBB1_37
LBB1_36:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	$4, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
LBB1_37:                                ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_38
LBB1_38:                                ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_39
LBB1_39:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-8(%rbp), %eax
	movb	%al, %cl
	movq	-24(%rbp), %rsi
	movsbl	%cl, %edi
	callq	_count_lines
	movq	%rax, -24(%rbp)
	jmp	LBB1_50
LBB1_40:                                ##   in Loop: Header=BB1_1 Depth=1
	cmpl	$42, -8(%rbp)
	jne	LBB1_42
## BB#41:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$0, -44(%rbp)
	movq	-32(%rbp), %rax
	addq	$-1, %rax
	movq	%rax, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
	jmp	LBB1_49
LBB1_42:                                ##   in Loop: Header=BB1_1 Depth=1
	cmpl	$47, -8(%rbp)
	jne	LBB1_44
## BB#43:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$1, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
	jmp	LBB1_48
LBB1_44:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-8(%rbp), %edi
	callq	_isspace
	cmpl	$0, %eax
	je	LBB1_46
## BB#45:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	$3, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
	jmp	LBB1_47
LBB1_46:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	$4, -44(%rbp)
	movq	-40(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rbp)
LBB1_47:                                ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_48
LBB1_48:                                ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_49
LBB1_49:                                ##   in Loop: Header=BB1_1 Depth=1
	movl	-8(%rbp), %eax
	movb	%al, %cl
	movq	-24(%rbp), %rsi
	movsbl	%cl, %edi
	callq	_count_lines
	movq	%rax, -24(%rbp)
LBB1_50:                                ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_1
LBB1_51:
	cmpl	$0, -44(%rbp)
	jne	LBB1_53
## BB#52:
	leaq	L_.str(%rip), %rsi
	movq	___stderrp@GOTPCREL(%rip), %rax
	movq	(%rax), %rdi
	movq	-16(%rbp), %rdx
	movb	$0, %al
	callq	_fprintf
	movl	$1, %edi
	movl	%eax, -84(%rbp)         ## 4-byte Spill
	callq	_exit
LBB1_53:
	cmpq	$0, -40(%rbp)
	jne	LBB1_55
## BB#54:
	leaq	L_.str.1(%rip), %rdi
	xorl	%eax, %eax
	movl	%eax, %esi
	movl	%eax, %edx
	movl	%eax, %ecx
	movb	$0, %al
	callq	_printf
	movl	%eax, -88(%rbp)         ## 4-byte Spill
	jmp	LBB1_56
LBB1_55:
	leaq	L_.str.2(%rip), %rdi
	movq	-24(%rbp), %rsi
	movq	-32(%rbp), %rdx
	movq	-40(%rbp), %rcx
	movb	$0, %al
	callq	_printf
	movl	%eax, -92(%rbp)         ## 4-byte Spill
LBB1_56:
	jmp	LBB1_57
LBB1_57:
	xorl	%eax, %eax
	addq	$96, %rsp
	popq	%rbp
	retq
	.cfi_endproc
	.p2align	2, 0x90
	.data_region jt32
L1_0_set_3 = LBB1_3-LJTI1_0
L1_0_set_40 = LBB1_40-LJTI1_0
L1_0_set_30 = LBB1_30-LJTI1_0
L1_0_set_16 = LBB1_16-LJTI1_0
L1_0_set_23 = LBB1_23-LJTI1_0
L1_0_set_6 = LBB1_6-LJTI1_0
LJTI1_0:
	.long	L1_0_set_3
	.long	L1_0_set_40
	.long	L1_0_set_30
	.long	L1_0_set_16
	.long	L1_0_set_23
	.long	L1_0_set_6
	.end_data_region
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Error: line %lld: unterminated comment\n"

L_.str.1:                               ## @.str.1
	.asciz	"%d %d %d\n"

L_.str.2:                               ## @.str.2
	.asciz	"%lld %lld %lld\n"


.subsections_via_symbols
