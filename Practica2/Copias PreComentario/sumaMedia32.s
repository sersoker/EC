# Suma.s:	Sumar los elementos de una lsita llamando a funcion, pasando argumentos mediante registros
# Retorna:	codigo retorno 0, comprobar suma en %eax mediante gbd/ddd

#SECCION DE DATOS (.data, variables globales inicializadas)

.section .data
	.macro linea	
		#	.int 1,1,1,1
		#	.int 2,2,2,2
		#	.int 1,2,3,4
		#	.int -1,-1,-1,-1
		#	.int 1,-2,1,-2
		#	.int 1,2,-3,-4
		#	.int 0x08000000,0x08000000,0x08000000,0x08000000
		#	.int 0x10000000,0x20000000,0x40000000,0x80000000
			.int 0x7FFFFFFF,0x7FFFFFFF,0x7FFFFFFF,0x7FFFFFFF
		#	.int 0xF0000000,0xE0000000,0xE0000000,0xD0000000
		#	.int 0,-1,-1,-1
		#	.int 0,-2,-1,-1
		#	.int 1,-2,-1,-1
		#	.int 32,-2,-1,1
		#	.int 64,-2,-1,1
		#	.int 80,-2,-1,1
		#	.int 95,-2,-1,1
		#	.int -31,-2,-1,1

	.endm
lista:.irpc i,12345678
		linea
	.endr
	longlista:	.int (.-lista)/4
	resultado:	.quad -1

#Sección de formato para la salida por pantalla
formato:
	.ascii "cociente=%8d \t resto=%8d  \n"
	.ascii "hex=0x%08x \t resto=0x%08x  \n \0"

.section .text
main: .global main
#_start:	.global _start

	mov  $lista, %ebx
	mov  longlista, %ecx
	call suma
	mov  %ebp,%eax
	mov  %edi,%edx
	idiv %ecx
	mov  %edx,resultado+4
	mov  %eax,resultado

	push resultado+4
	push resultado
	push resultado+4
	push resultado
	push $formato
	call printf
	add  $20, %esp
	mov  $1, %eax
	mov  $0, %ebx
	int  $0x80

suma:
	push %edx
	mov $0, %eax
	mov $0, %edx

	mov $0, %esi 	#Indice del bucle
	mov $0, %ebp	#Primer registro (derecha)
	mov $0, %edi	#Segundo registro (izquierda)

bucle:
	mov (%ebx,%esi,4), %eax
	cdq

	add %eax,%ebp
	adc %edx,%edi

	inc %esi
	cmp %esi,%ecx
	jne bucle
	
	pop %edx
	ret
