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
	push	EBP
	mov		EBP,ESP

	push	EBX
	push	ECX
	push	EDX

	mov		EDI,[EBP+12] ;edi guarda o ponteiro para a string
	mov		ECX,[EBP+8] ;edx vai guardar o tamanho da string
	sub		ECX,1

	mov 	EBX,10 ;multiplicador

	push 	EDX
	push 	EBX
	mov 	DL,[EDI] ;valor do primeiro caractere da string
	cmp 	EDX,'-'
	jne 	positivo

	pop 	EBX
	pop 	EDX
	inc		EDI ;pula -
	sub 	ECX,1 ;loop pula 1 posição, já que em 0 tem -
	mov 	EAX,-1 ;sinaliza que é negativo
	push 	EAX
	jmp 	continua

positivo:
	pop 	EBX
	pop 	EDX
	mov 	EAX,1 ;sinaliza que é positivo
	push 	EAX

continua:
	and 	EAX,0 ;soma começa zerada

passa:
	push	EDX ;edx é alterado com mul, então precisa salvar o tamanho da string
	imul 	EBX
	add 	AL,[EDI]
	sub 	AL,0x30
	pop 	EDX
	inc		EDI
	loop 	passa
	pop 	EBX
	imul 	EBX ;dá o sinal ao número

	pop 	EDX
	pop 	ECX
	pop 	EBX

	pop 	EBP
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
	jmp		passa

negativo:
	;inc 	ecx
	;mov 	edx,"-"
	;push 	edx
	mov 	edx,-1
	imul 	edx

passa:
	and		edx,0
	idiv	ebx

	add		edx,0x30
	push 	EDX

	inc		ecx ;incrementa ecx para depois desempilhar
	cmp 	eax,0
	jne		passa

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
    
    mov    [size_of_s_input_output_int], eax

    push	DWORD [size_of_s_input_output_int] ; empilha o tamanho em INT
	push	size_of_s_input_output_str ; empilha onde vai guardar o tamanho em STR
	call	output_int

    print_str_macro     msg_read_bytes, size_read_bytes
    print_str_macro     size_of_s_input_output_str, [size_of_s_input_output_int]
    print_str_macro     msg_bytes, size_bytes_msg
    print_str_macro     newl, 1

    pop    ebx
    pop    ecx
    pop    edx

    mov    eax, [size_of_s_input_output_int]

    leave
    ret     8

;; OUTPUT STRING. Ouput string e bytes escritos
OUTPUT_S:

    enter    0,0

    push    ebx
    push    ecx
    push    edx

    mov     eax, [ebp+8]
    call    strlen
    mov    [size_of_s_input_output_int], eax
    
    print_str_macro     [ebp+8], [size_of_s_input_output_int]

    push	DWORD [size_of_s_input_output_int] ; empilha o tamanho em INT
	push	size_of_s_input_output_str ; empilha onde vai guardar o tamanho em STR
	call	output_int

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

;;; STR_LEN

strlen:
	push 	ebx
	mov 	ebx, eax
loop:
	cmp 	byte [eax], 0
	jz		fim		
	inc		eax
	jmp		loop

fim:
	sub		eax, ebx
	pop 	ebx
	ret