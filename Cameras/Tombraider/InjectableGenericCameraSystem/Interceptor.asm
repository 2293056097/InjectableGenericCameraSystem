;////////////////////////////////////////////////////////////////////////////////////////////////////////
;// Part of Injectable Generic Camera System
;// Copyright(c) 2016, Frans Bouma
;// All rights reserved.
;// https://github.com/FransBouma/InjectableGenericCameraSystem
;//
;// Redistribution and use in source and binary forms, with or without
;// modification, are permitted provided that the following conditions are met :
;//
;//  * Redistributions of source code must retain the above copyright notice, this
;//	  list of conditions and the following disclaimer.
;//
;//  * Redistributions in binary form must reproduce the above copyright notice,
;//    this list of conditions and the following disclaimer in the documentation
;//    and / or other materials provided with the distribution.
;//
;// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
;// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
;// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;////////////////////////////////////////////////////////////////////////////////////////////////////////
;---------------------------------------------------------------
; Game specific asm file to intercept execution flow to obtain addresses, prevent writes etc.
;---------------------------------------------------------------

.model flat,C
.stack 4096

;---------------------------------------------------------------
; Public definitions so the linker knows which names are present in this file
;---------------------------------------------------------------

;---------------------------------------------------------------
; Externs defined in Core.cpp, which are used and set by the system. Read / write these
; values in asm to communicate with the system
EXTERN _cameraStructAddress: dword
EXTERN _gamespeedStructAddress: dword
EXTERN _cameraEnabled: byte
EXTERN _timeStopped: byte
;---------------------------------------------------------------

;---------------------------------------------------------------
; Own externs, defined in InterceptorHelper.cpp
EXTERN _cameraStructInterceptionContinue: dword
EXTERN _cameraWriteInterceptionContinue1: dword
EXTERN _cameraWriteInterceptionContinue2: dword
EXTERN _fovWriteInterceptionContinue1: dword
EXTERN _fovWriteInterceptionContinue2: dword
EXTERN _gamespeedInterceptionContinue: dword


.code 
cameraAddressInterceptor PROC
	; Game jmps to this location due to the hook set in C function SetCameraStructInterceptorHook
	mov [_cameraStructAddress], esi						; intercept address of camera struct
exit:
	mov [esi-60h],eax									; original statement
	mov ecx,[edi+04h]
	jmp [_cameraStructInterceptionContinue]				; jmp back into the original game code, which is the location after the original statements above.
cameraAddressInterceptor ENDP


gamespeedAddressInterceptor PROC
	mov [_gamespeedStructAddress], esi
	cmp byte ptr [_timeStopped], 1
	je exit
originalCode:
	fstp dword ptr [esi+00000104h]
exit:
	jmp [_gamespeedInterceptionContinue]
gamespeedAddressInterceptor ENDP


fovWriteInterceptor1 PROC
	; Game jmps to this location due to the hook set in C function SetFoVWriteInterceptorHooks. 
	cmp dword ptr esi, [_cameraStructAddress]
	jne originalCode
	cmp byte ptr [_cameraEnabled], 1					; check if the user enabled the camera. If so, just skip the write statements, otherwise just execute the original code.
	je exit												; our own camera is enabled, just skip the writes
originalCode:
	fstp dword ptr [esi-78h]							; original statement
exit:
	push ebx											; original statement
	mov ecx,edi											; original statement
	jmp [_fovWriteInterceptionContinue1]				; jmp back into the original game code which is the location after the original statements above.
fovWriteInterceptor1 ENDP


fovWriteInterceptor2 PROC
	; Game jmps to this location due to the hook set in C function SetFoVWriteInterceptorHooks. 
	cmp byte ptr [_cameraEnabled], 1					; check if the user enabled the camera. If so, just skip the write statements, otherwise just execute the original code.
	je exit												; our own camera is enabled, just skip the writes
originalCode:
	fld dword ptr [eax+ecx+38h]							; original statement
	lea eax,[eax+ecx+10h]								; original statement
exit:
	jmp [_fovWriteInterceptionContinue2]				; jmp back into the original game code which is the location after the original statements above.
fovWriteInterceptor2 ENDP


;-------------------------------------------------------------------
; Camera values write interceptions. For each block of statements in the game's exe which write to the camera values, we intercept them and execute them if our
; own camera is disabled, otherwise we skip them. Each block checks whether the current struct pointer is the camera struct. If it's not the camera struct, it will
; execute the code regardless of whether our camera is enabled.

cameraWriteInterceptor1 PROC
	; Game jmps to this location due to the hook set in C function SetCameraWriteInterceptorHooks. 
	cmp byte ptr [_cameraEnabled], 1					; check if the user enabled the camera. If so, just skip the write statements, otherwise just execute the original code.
	je exit										; our own camera is enabled, just skip the writes
originalCode:
	movaps [esi+090h],xmm0
exit:
	jmp [_cameraWriteInterceptionContinue1]				; jmp back into the original game code which is the location after the original statements above.
cameraWriteInterceptor1 ENDP

cameraWriteInterceptor2 PROC
	; Game jmps to this location due to the hook set in C function SetCameraWriteInterceptorHooks. 
	cmp dword ptr esi, [_cameraStructAddress]
	jne originalCode
	; The first write is in the camera address interception, these are the rest of the writes.
	cmp byte ptr [_cameraEnabled], 1					; check if the user enabled the camera. If so, just skip the write statements, otherwise just execute the original code.
	je exit												; our own camera is enabled, just skip the writes
originalCode:
	mov [esi-30h],eax
	mov ecx,[esi+000003E4h]
	mov [esi-2Ch],ecx
	mov edx,[esi+000003E8h]
	mov [esi-28h],edx
exit:
	jmp [_cameraWriteInterceptionContinue2]	; jmp back into the original game code which is the location after the original statements above.
cameraWriteInterceptor2 ENDP


_TEXT ENDS

END