# Suma.s:	Sumar los elementos de una lsita llamando a funcion, pasando argumentos mediante registros
# Retorna:	codigo retorno 0, comprobar suma en %eax mediante gbd/ddd

#SECCION DE DATOS (.data, variables globales inicializadas)

.section .data
	.macro linea	
		#	.int 1,1,1,1
		#	.int 2,2,2,2
		#	.int 1,2,3,4
			.int -1,-1,-1,-1
		#	.int 0xffffffff,0xffffffff,0xffffffff,0xffffffff
		#	.int 0x08000000,0x08000000,0x08000000,0x08000000
		#	.int 0x10000000,0x20000000,0x40000000,0x80000000
	.endm
lista:.irpc i,12345678
		linea
	.endr
	longlista:	.int (.-lista)/4
	resultado:	.quad -1

formato:
	.ascii "suma=%lld hex=%llx  \n \0"
	
.section .text
main: .global main
#_start:	.global _start

	mov  $lista, %ebx
	mov  longlista, %ecx
	call suma
	mov  %esi, resultado+4
	mov  %eax, resultado


	push resultado+4
	push resultado
	push resultado+4
	push resultado
	push $formato
	call printf
	add  $12, %esp

	mov  $1, %eax
	mov  $0, %ebx
	int  $0x80

suma:
	push %edx
	mov $0, %eax
	mov $0, %edx
	mov $0, %esi
bucle:
	add (%ebx,%esi,4), %eax
	adc $0,%edx	
	cdq

	inc %esi
	cmp %esi,%ecx
	jne bucle
	
	mov %edx,%esi
	pop %edx
	ret
