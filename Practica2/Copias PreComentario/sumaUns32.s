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

#Sección de formato para la salida por pantalla
formato:
	.ascii "suma=%llu hex=%llx  \n \0"
	
.section .text
main: .global main
#_start:	.global _start

	mov  $lista, %ebx	#Ubicamos la lista
	mov  longlista, %ecx	#Guardamos el tamaño de la lista
	call suma		#Llamada a la funcion suma	
	mov  %esi, resultado+4	#Metemos los resultados de suma en resultado para mostrarlo
	mov  %eax, resultado

				#Metemos el resultado en orden inverso y el formato
	push resultado+4
	push resultado
	push resultado+4
	push resultado
	push $formato
	call printf		#Llamada a print para mostrar
	add  $20, %esp		#Regresamos la pila a su posicion inicial (hacemos 3 push 4*5)

	mov  $1, %eax		#Datos para la salida
	mov  $0, %ebx		
	int  $0x80		#Llamada a exit

suma:
	push %edx		#Guardamos lo anterior en la pila
	mov $0, %eax		#Limpieza de registros
	mov $0, %edx
	mov $0, %esi
bucle:
	add (%ebx,%edx,4), %eax	#Sumamos
	adc $0,%esi		#Suma de acarreo
	inc %edx		#Incrementar indice
	cmp %edx,%ecx		#Comparamos si estamos al final del bucle para saltar o no
	jne bucle

	pop %edx		#Regresamos al estado anterior de la pila.
	ret
