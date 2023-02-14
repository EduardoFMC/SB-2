
%define max_int 12

;macro para ler uma string
%macro read_str_macro 2
mov eax, 3
mov ebx, 0
mov ecx, %1 
mov edx, %2 
int 80h
%endmacro

;macro para printar uma string
%macro print_str_macro 2
mov eax, 4
mov ebx, 1 
mov ecx, %1
mov edx, %2
int 80h
%endmacro

;;;;; INPUT. INPUT INT
;; STR -> INT
input_int:
	enter	4,0

	push	EBX
	push	ECX
	push	EDX

	read_str_macro [ebp+8], max_int

	mov		ECX,EAX ;edx vai guardar o tamanho da string
	mov		EDI,[EBP+8] ;edi guarda o ponteiro para a string
	sub		ECX,1

	mov 	EBX,10 ;multiplicador

	push 	EDX
	mov 	DL,[EDI] ;valor do primeiro caractere da string
	cmp 	EDX,'-'
	jne 	positivo

	pop 	EDX
	inc		EDI ;pula -
	sub 	ECX,1 ;loop pula 1 posição, já que em 0 tem -
	mov 	DWORD [EBP-4],-1 ;sinaliza que é negativo
	jmp 	continua

positivo:
	pop 	EDX
	mov 	DWORD [EBP-4],1 ;sinaliza que é positivo

continua:
	push 	EAX
	and 	EAX,0 ;soma começa zerada

passa_input:
	push	EDX ;edx é alterado com mul, então precisa salvar o tamanho da string
	imul 	EBX
	add 	AL,[EDI]
	sub 	AL,0x30
	pop 	EDX
	inc		EDI
	loop 	passa_input
	imul 	DWORD [EBP-4]

	mov 	[num_int],eax

	pop		EAX
	pop 	EDX
	pop 	ECX
	pop 	EBX

	sub		eax,1
    mov   	[size_of_s_input_output_int], eax
    push 	eax

    push	DWORD [size_of_s_input_output_int] ; empilha o tamanho em INT
	push	size_of_s_input_output_str ; empilha onde vai guardar o tamanho em STR
	call	get_pos_int_str

    print_str_macro     msg_read_bytes, size_read_bytes
    print_str_macro     size_of_s_input_output_str, [size_of_s_input_output_int]
    print_str_macro     msg_bytes, size_bytes_msg
    print_str_macro     newl, 1

    pop eax

	leave
	ret		4

get_pos_int_str:
	enter	0,0

	push	EBX
	push	ECX
	push	EDX

	mov		EDI,[EBP+8] ;edi = string vazia
	mov		EAX,[EBP+12] ;eax = número
	mov		EBX,10 ;ebx = divisor

	and		ECX,0 ;ecx = iterador

passa_int_str:
	and		edx,0
	idiv	ebx

	add		edx,0x30
	push 	EDX

	inc		ecx ;incrementa ecx para depois desempilhar
	cmp 	eax,0
	jne		passa_int_str

	and		ebx,0

digitos_int_str:
	pop		edx
	mov 	[edi],edx
	inc		edi

	inc		ebx
	cmp 	ebx,ecx
	jne 	digitos_int_str

	pop 	EDX
	pop 	ECX
	pop 	EBX

	leave
	ret		8

;;;; OUTPUT. OUTPUT INT
;; Pode ser utilizado como int -> str
output_int:
	enter	4,0

	push	EBX
	push	ECX
	push	EDX

	mov		EDI,[EBP+8] ;edi = string vazia
	mov		EAX,[EBP+12] ;eax = número
	mov		EBX,10 ;ebx = divisor
	mov 	[EBP-4],EAX ;EBP-4 guarda o número original

	and		ECX,0 ;ecx = iterador
	cmp		[EBP-4],ECX
	jl		negativo
	jmp		passa_output

negativo:
	mov 	edx,-1
	imul 	edx

passa_output:
	and		edx,0
	idiv	ebx

	add		edx,0x30
	push 	EDX

	inc		ecx ;incrementa ecx para depois desempilhar
	cmp 	eax,0
	jne		passa_output

	and		ebx,0

	cmp		[EBP-4],EBX
	jl 		negativo_digito
	jmp		digitos

negativo_digito:
	inc 	ecx
	mov 	edx,"-"
	push 	edx

digitos:
	pop		edx
	mov 	[edi],edx
	inc		edi

	inc		ebx
	cmp 	ebx,ecx
	jne 	digitos

   	mov   	[size_of_s_input_output_int], ECX
    push 	ECX

	mov		EAX,4
	mov 	EBX,1
	mov		EDX,ECX
	mov 	ECX,[EBP+8]
	int 	80h

    push	DWORD [size_of_s_input_output_int] ; empilha o tamanho em INT
	push	size_of_s_input_output_str ; empilha onde vai guardar o tamanho em STR
	call	get_pos_int_str

	print_str_macro     newl, 1

    print_str_macro     msg_print_bytes, size_print_bytes
    print_str_macro     size_of_s_input_output_str, [size_of_s_input_output_int]
    print_str_macro     msg_bytes, size_bytes_msg
    print_str_macro     newl, 1

    pop 	EAX
	pop 	EDX
	pop 	ECX
	pop 	EBX

	leave
	ret		8

;;; INPUT_C. INPUT C
INPUT_C:

    enter   0,0

    push    ebx
    push    ecx
    push    edx

    read_str_macro      [ebp+8], 1

    print_str_macro     msg_read_bytes, size_read_bytes
    print_str_macro     n_1, 1
    print_str_macro     msg_bytes, size_bytes_msg
    print_str_macro     newl, 1

    pop    ebx
    pop    ecx
    pop    edx

    mov     eax, 1
    leave
    ret     4

;;;; OUTPUT_C, OUTPUT CHAR
OUTPUT_C:
    enter   0,0

    push    ebx
    push    ecx
    push    edx

    print_str_macro     [ebp+8], 1
    print_str_macro     newl, 1

    ; Foram imprimidos 1 bytes
    print_str_macro     msg_print_bytes, size_print_bytes
    print_str_macro     n_1, 1
    print_str_macro     msg_bytes, size_bytes_msg
    print_str_macro     newl, 1

    pop    ebx
    pop    ecx
    pop    edx

    mov     eax, 1
    leave
    ret     4

;;;  INPUT_S LABEL, x. Input string em LABEL, com ttamanho maximo x, e escreva o tamanho em byte
;; que pode ir até x
INPUT_S:

    enter    0,0

    push    ebx
    push    ecx
    push    edx

    read_str_macro [ebp+8], [ebp+12]
    
	mov		ebx, [ebp+16]
	mov		[ebx], eax	; tamnho real da string
    mov     [size_of_s_input_output_int], eax

    push	DWORD [size_of_s_input_output_int] ; empilha o tamanho em INT
	push	size_of_s_input_output_str ; empilha onde vai guardar o tamanho em STR
	call	input_s_str_to_int

    print_str_macro     msg_read_bytes, size_read_bytes
    print_str_macro     size_of_s_input_output_str, [size_of_s_input_output_int]
    print_str_macro     msg_bytes, size_bytes_msg
    print_str_macro     newl, 1

    pop    ebx
    pop    ecx
    pop    edx

    mov    eax, [size_of_s_input_output_int]

    leave
    ret     12

;; OUTPUT STRING. Ouput string e bytes escritos

OUTPUT_S:

    enter    0,0

    push    ebx
    push    ecx
    push    edx

	mov 	ebx, DWORD [ebp+12]
    mov     eax, [ebx]
    mov     [size_of_s_input_output_int], eax
    
    print_str_macro  [ebp+8], [size_of_s_input_output_int]

    push	DWORD [size_of_s_input_output_int] ; empilha o tamanho em INT
	push	size_of_s_input_output_str ; empilha onde vai guardar o tamanho em STR
	call	input_s_str_to_int

    print_str_macro     newl, 1

    ; Foram imprimidos x bytes
    print_str_macro     msg_print_bytes, size_print_bytes
    print_str_macro     size_of_s_input_output_str, [size_of_s_input_output_int]
    print_str_macro     msg_bytes, size_bytes_msg
    print_str_macro     newl, 1

    pop    ebx
    pop    ecx
    pop    edx

    mov    eax, [size_of_s_input_output_int]

    leave
    ret     8

;;;;; OUTPUT INT (INT->STR). Esse é um especial do INPUT_S e output_S

input_s_str_to_int:
	enter	4,0

	push	EBX
	push	ECX
	push	EDX

	mov		EDI,[EBP+8] ;edi = string vazia
	mov		EAX,[EBP+12] ;eax = número
	mov		EBX,10 ;ebx = divisor
	mov 	[EBP-4],EAX ;EBP-4 guarda o número original

	and		ECX,0 ;ecx = iterador
	cmp		[EBP-4],ECX
	jl		ehneg
	jmp		continue

ehneg:

	mov 	edx,-1
	imul 	edx

continue:
	and		edx,0
	idiv	ebx

	add		edx,0x30
	push 	EDX

	inc		ecx ;incrementa ecx para depois desempilhar
	cmp 	eax,0
	jne		continue

	and		ebx,0

	cmp		[EBP-4],EBX
	jl 		neg_digit
	jmp		all_digits

neg_digit:
	inc 	ecx
	mov 	edx,"-"
	push 	edx

all_digits:
	pop		edx
	mov 	[edi],edx
	inc		edi

	inc		ebx
	cmp 	ebx,ecx
	jne 	all_digits

	pop 	EDX
	pop 	ECX
	pop 	EBX

	leave
	ret		8