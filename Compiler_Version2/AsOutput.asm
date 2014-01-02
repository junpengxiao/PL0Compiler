.386
.model flat,stdcall
option casemap:none

;Include resource
include msvcrt.inc
include kernel32.inc
includelib kernel32.lib
includelib msvcrt.lib

;Define C-type IO
printf PROTO C:ptr sbyte, :vararg
scanf PROTO C:ptr sbyte, :vararg

.data
$IOChar db "%c",0
$IOInt db "%d",0
$Return db 10,13,0

.code

@main0 proc

;Subroutine Prologue
	push ebp
	mov ebp,esp
	sub esp,96
	push edi
	push esi

;Subroutine Body
	lea eax, dword ptr [ebp-4]
	push eax
	push offset $IOInt
	call scanf
	mov dword ptr [ebp-8], 0
	mov eax,[ebp-4]
	sub eax, 1
	mov [ebp-56], eax
@ttemp1:
	mov eax,[ebp-8]
	cmp eax,[ebp-56]
	jg @ttemp2
	lea eax, dword ptr [ebp-12]
	push eax
	push offset $IOInt
	call scanf
	mov eax, dword ptr [ebp-8]
	mov dword ptr [ebp-72], eax
	mov eax, dword ptr [ebp-12]
	mov esi,ebp
	sub esi,16
	mov edi,[ebp-72]
	shl edi,2
	sub esi,edi
	mov dword ptr [esi], eax
	mov eax,[ebp-8]
	add eax, 1
	mov [ebp-8], eax
	jmp @ttemp1
@ttemp2:
	mov dword ptr [ebp-8], 0
	mov eax,[ebp-4]
	sub eax, 1
	mov [ebp-76], eax
@ttemp6:
	mov eax,[ebp-8]
	cmp eax,[ebp-76]
	jg @ttemp7
	mov eax, dword ptr [ebp-8]
	mov dword ptr [ebp-92], eax
	mov esi,ebp
	sub esi,16
	mov edi,[ebp-92]
	shl edi,2
	sub esi,edi
	mov eax,[esi]
	push eax
	push offset $IOInt
	call printf
	push offset $Return
	call printf
	mov eax,[ebp-8]
	add eax, 1
	mov [ebp-8], eax
	jmp @ttemp6
@ttemp7:
	mov eax, [ebp-96]

;Subroutine Epilogue
	pop esi
	pop edi
	mov esp,ebp
	pop ebp
	ret
@main0 endp

start:

;Subroutine Prologue
push ebp
mov ebp,esp
sub esp,0
push edi
push esi

;Subroutine Body
call @main0
call ExitProcess
end start

