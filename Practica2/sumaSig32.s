# Suma.s:	Sumar los elementos de una lsita llamando a funcion, pasando argumentos mediante registros
# Retorna:	codigo retorno 0, comprobar suma en %eax mediante gbd/ddd

#SECCION DE DATOS (.data, variables globales inicializadas)

.section .data
	.macro linea	
			.int -1,-1,-1,-1
		#	.int 1,-2,1,-2
		#	.int 1,2,-3,-4
		#	.int 0x7FFFFFFF,0x7FFFFFFF,0x7FFFFFFF,0x7FFFFFFF
		#	.int 0x80000000,0x80000000,0x80000000,0x80000000
		#	.int 0x04000000,0x04000000,0x04000000,0x04000000
		#	.int 0x08000000,0x08000000,0x08000000,0x08000000
		#	.int 0xFC000000,0xFC000000,0xFC000000,0xFC000000
		#	.int 0xF0000000,0xE0000000,0xE0000000,0xD0000000
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

	mov  $lista, %ebx			#Ubicamos la lista
	mov  longlista, %ecx		#Guardamos el tamaño de la lista
	call suma					#Llamada a la funcion suma	
	mov  %edi, resultado+4		#Metemos los resultados de suma en resultado para mostrarlo
	mov  %ebp, resultado

	#Metemos el resultado en orden inverso y el formato
	push resultado+4
	push resultado
	push resultado+4
	push resultado
	push $formato
	call printf					#Llamada a print para mostrar
	add  $20, %esp				#Regresamos la pila a su posicion inicial (hacemos 3 push 4*5)

	mov  $1, %eax				#Datos para la salida
	mov  $0, %ebx		
	int  $0x80					#Llamada a exit


suma:
	push %edx					#Guardamos el valor que contenga
	mov $0, %eax				#Ponemos a 0 los registros a usar
	mov $0, %edx
	mov $0, %esi 				#Indice del bucle
	mov $0, %ebp				#Primer registro (derecha)
	mov $0, %edi				#Segundo registro (izquierda)

bucle:
	mov (%ebx,%esi,4), %eax		#Cogemos el primer valor
	cdq							#Extension del signo de EAX a EDX (Importante no usarlos para nada mas)
	add %eax,%ebp				#Realizamos la suma
	adc %edx,%edi

	inc %esi					#Incremento del indice del bucle
	cmp %esi,%ecx				#¿Fin del bucle?
	jne bucle
	
	pop %edx					#Devuelve EDX a su estado original
	ret
