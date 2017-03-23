;////////////////////////////////////////////////////////////////////////////////////////////////////////
;// Part of Injectable Generic Camera System
;// Copyright(c) 2017, Frans Bouma
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

;---------------------------------------------------------------
; Public definitions so the linker knows which names are present in this file
PUBLIC cameraWriteInterceptor1
PUBLIC cameraWriteInterceptor2
PUBLIC fovWriteInterceptor
;---------------------------------------------------------------

;---------------------------------------------------------------
; Externs which are used and set by the system. Read / write these
; values in asm to communicate with the system
EXTERN g_cameraEnabled: byte
;---------------------------------------------------------------

;---------------------------------------------------------------
; Own externs, defined in InterceptorHelper.cpp
EXTERN _cameraWriteInterception1Continue: qword
EXTERN _cameraWriteInterception2Continue: qword
EXTERN _fovWriteInterceptionContinue: qword
EXTERN _cameraCoordsAddress: qword
EXTERN _cameraQuaternionAddress: qword

;---------------------------------------------------------------
; Scratch pad
; FOV WRITE: 
;LordsOfTheFallen.exe+D2E5F46 - 0F2E 40 18            - ucomiss xmm0,[rax+18]
;LordsOfTheFallen.exe+D2E5F4A - 74 1C                 - je LordsOfTheFallen.exe+D2E5F68
;LordsOfTheFallen.exe+D2E5F4C - F3 0F11 40 18         - movss [rax+18],xmm0										<< FOV WRITE. Intercept here
;LordsOfTheFallen.exe+D2E5F51 - C6 80 4C010000 01     - mov byte ptr [rax+0000014C],01 { 1 }
;LordsOfTheFallen.exe+D2E5F58 - 66 C7 80 4E010000 0101 - mov word ptr [rax+0000014E],0101 { 257 }
;LordsOfTheFallen.exe+D2E5F61 - C6 80 50010000 01     - mov byte ptr [rax+00000150],01 { 1 }
;LordsOfTheFallen.exe+D2E5F68 - 48 8B 49 58           - mov rcx,[rcx+58]
;


;---------------------------------------------------------------
.code

; No need for a camera address interceptor, as the addresses are hard-coded and the game won't be updated. 

cameraWriteInterceptor1 PROC
;LordsOfTheFallen.exe+D58411A - F3 0F11 49 10         - movss [rcx+10],xmm1						<< INTERCEPT HERE
;LordsOfTheFallen.exe+D58411F - 8B 42 04              - mov eax,[rdx+04]
;LordsOfTheFallen.exe+D584122 - 89 41 14              - mov [rcx+14],eax
;LordsOfTheFallen.exe+D584125 - 8B 42 08              - mov eax,[rdx+08]
;LordsOfTheFallen.exe+D584128 - B2 15                 - mov dl,15
;LordsOfTheFallen.exe+D58412A - 89 41 18              - mov [rcx+18],eax
;LordsOfTheFallen.exe+D58412D - E8 BE010000           - call LordsOfTheFallen.exe+D5842F0		<< CONTINUE HERE

	; check for our coord target address. This code is used by multiple target structures
	cmp qword ptr rcx, [_cameraCoordsAddress]
	jne originalCode									; code operates on other struct than the camera struct, leave it.
	cmp byte ptr [g_cameraEnabled], 1					; check if the user enabled the camera. If so, just skip the write statements, otherwise just execute the original code.
	je noCameraWrite									; our own camera is enabled, just skip the writes
	; as we don't know what the registers are used for futher, our 'disabled' part just doesn't write to memory.
originalCode:
	movss dword ptr [rcx+10h],xmm1	
	mov eax, [rdx+04h]
	mov [rcx+14h],eax
	mov eax,[rdx+08h]
	mov dl,15h
	mov [rcx+18h],eax
	jmp exit
noCameraWrite:
	mov eax, [rdx+04h]
	mov eax,[rdx+08h]
	mov dl,15h
exit:
	jmp qword ptr [_cameraWriteInterception1Continue]	; jmp back into the original game code, which is the location after the original statements above.
cameraWriteInterceptor1 ENDP


cameraWriteInterceptor2 PROC
;LordsOfTheFallen.exe+D5841BC - 41 89 40 0C           - mov [r8+0C],eax			<< WRITE QW      <<< INTERCEPT HERE
;LordsOfTheFallen.exe+D5841C0 - 8B 02                 - mov eax,[rdx]
;LordsOfTheFallen.exe+D5841C2 - 41 89 00              - mov [r8],eax			<< WRITE QX
;LordsOfTheFallen.exe+D5841C5 - 8B 42 04              - mov eax,[rdx+04]
;LordsOfTheFallen.exe+D5841C8 - 41 89 40 04           - mov [r8+04],eax			<< WRITE QY
;LordsOfTheFallen.exe+D5841CC - 8B 42 08              - mov eax,[rdx+08]
;LordsOfTheFallen.exe+D5841CF - B2 15                 - mov dl,15 { 21 }
;LordsOfTheFallen.exe+D5841D1 - 41 89 40 08           - mov [r8+08],eax			<< WRITE QZ
;LordsOfTheFallen.exe+D5841D5 - E8 16010000           - call LordsOfTheFallen.exe+D5842F0		<<< CONTINUE HERE

	; check for our quaternion target address. This code is used by multiple target structures
	cmp qword ptr r8, [_cameraQuaternionAddress]
	jne originalCode									; code operates on other struct than the camera struct, leave it.
	cmp byte ptr [g_cameraEnabled], 1					; check if the user enabled the camera. If so, just skip the write statements, otherwise just execute the original code.
	je noCameraWrite									; our own camera is enabled, just skip the writes
	; as we don't know what the registers are used for futher, our 'disabled' part just doesn't write to memory.
originalCode:
	mov [r8+0Ch],eax	
	mov eax,[rdx]
	mov [r8],eax	
	mov eax,[rdx+04h]
	mov [r8+04h],eax	
	mov eax,[rdx+08h]
	mov dl,15h
	mov [r8+08h],eax	
	jmp exit
noCameraWrite:
	mov eax,[rdx]
	mov eax,[rdx+04h]
	mov eax,[rdx+08h]
	mov dl,15h
exit:
	jmp qword ptr [_cameraWriteInterception2Continue]	; jmp back into the original game code, which is the location after the original statements above.
cameraWriteInterceptor2 ENDP


fovWriteInterceptor PROC
;LordsOfTheFallen.exe+D2E5F4C - F3 0F11 40 18         - movss [rax+18],xmm0										<< FOV WRITE. Intercept here
;LordsOfTheFallen.exe+D2E5F51 - C6 80 4C010000 01     - mov byte ptr [rax+0000014C],01 
;LordsOfTheFallen.exe+D2E5F58 - 66 C7 80 4E010000 0101 - mov word ptr [rax+0000014E],0101
;LordsOfTheFallen.exe+D2E5F61 - C6 80 50010000 01     - mov byte ptr [rax+00000150],01
;LordsOfTheFallen.exe+D2E5F68 - 48 8B 49 58           - mov rcx,[rcx+58]									    <<< CONTINUE HERE

	cmp byte ptr [g_cameraEnabled], 1					; check if the user enabled the camera. If so, just skip the write statements, otherwise just execute the original code.
	je continue											; our own camera is enabled, just skip the writes
originalCode:	
	movss dword ptr [rax+18h],xmm0		
continue:
	mov byte ptr [rax+0000014Ch],01h
	mov word ptr [rax+0000014Eh],0101h
	mov byte ptr [rax+00000150h],01h
exit:
	jmp qword ptr [_fovWriteInterceptionContinue]	; jmp back into the original game code, which is the location after the original statements above.
fovWriteInterceptor ENDP

END