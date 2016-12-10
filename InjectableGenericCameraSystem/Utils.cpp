#include "stdafx.h"
#include "Utils.h"

// Sets a jmp qword ptr [address] statement at hostImageAddress + startOffset.
void SetHook(LPBYTE hostImageAddress, DWORD startOffset, DWORD continueOffset, LPBYTE* interceptionContinue, void* asmFunction)
{
	LPBYTE startOfHookAddress = hostImageAddress + startOffset;
	*interceptionContinue = hostImageAddress + continueOffset;
	// now write bytes of jmp qword ptr [address], which is jmp qword ptr 0 offset.
	memcpy(startOfHookAddress, jmpFarInstructionBytes, sizeof(jmpFarInstructionBytes));
#ifdef _WIN64
	// now write the address. Do this with a recast of the pointer to an __int64 pointer to avoid endianmess.
	__int64* interceptorAddressDestination = (__int64*)(startOfHookAddress + 6);
	interceptorAddressDestination[0] = (__int64)asmFunction;
#else	// x86
	// now write the address. Do this with a recast of the pointer to a DWORD pointer to avoid endianmess.
	LPDWORD interceptorAddressDestination = (LPDWORD)(startOfHookAddress + 6);
	interceptorAddressDestination[0] = (LPDWORD)asmFunction;
#endif
}

// Writes NOP opcodes to a range of memory.
void NopRange(LPBYTE startAddress, int length)
{
	for (int i = 0; i < length; i++)
	{
		startAddress[i] = 0x90;
	}
}
