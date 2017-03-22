////////////////////////////////////////////////////////////////////////////////////////////////////////
// Part of Injectable Generic Camera System
// Copyright(c) 2017, Frans Bouma
// All rights reserved.
// https://github.com/FransBouma/InjectableGenericCameraSystem
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
//
//  * Redistributions of source code must retain the above copyright notice, this
//	  list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and / or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "InterceptorHelper.h"
#include "GameConstants.h"
#include "GameImageHooker.h"
#include <map>
#include "Console.h"

using namespace std;

//--------------------------------------------------------------------------------------------------------------------------------
// external asm functions
extern "C" {
	void cameraWriteInterceptor1();
	void cameraWriteInterceptor2();
}

// external addresses used in asm.
extern "C" {
	// the continue address for continuing execution after camera write interception. 
	LPBYTE _cameraWriteInterception1Continue = nullptr;
	// the continue address for continuing execution after camera write interception. 
	LPBYTE _cameraWriteInterception2Continue = nullptr;
}


namespace IGCS::GameSpecific::InterceptorHelper
{
	void initializeAOBBlocks(LPBYTE hostImageAddress, DWORD hostImageSize, map<string, AOBBlock*> &aobBlocks)
	{
		aobBlocks[CAMERA_WRITE_INTERCEPT1_KEY] = new AOBBlock(CAMERA_WRITE_INTERCEPT1_KEY, "F3 0F 11 49 10 8B 42 04 89 41 14 8B 42 08 B2 15 89 41 18", 1);
		aobBlocks[CAMERA_WRITE_INTERCEPT2_KEY] = new AOBBlock(CAMERA_WRITE_INTERCEPT2_KEY, "41 89 40 0C 8B 02 41 89 00 8B 42 04 41 89 40 04 8B 42 08 B2 15 41 89 40 08 ", 1);

		map<string, AOBBlock*>::iterator it;
		bool result = true;
		for (it = aobBlocks.begin(); it != aobBlocks.end(); it++)
		{
			result &= it->second->scan(hostImageAddress, hostImageSize);
		}
		if (result)
		{
			Console::WriteLine("All interception offsets found.");
		}
		else
		{
			Console::WriteError("One or more interception offsets weren't found: tools aren't compatible with this game's version.");
		}
	}

	void setCameraWriteInterceptorHooks(map<string, AOBBlock*> &aobBlocks)
	{
		GameImageHooker::setHook(aobBlocks[CAMERA_WRITE_INTERCEPT1_KEY], 0x13, &_cameraWriteInterception1Continue, &cameraWriteInterceptor1);
		GameImageHooker::setHook(aobBlocks[CAMERA_WRITE_INTERCEPT2_KEY], 0x19, &_cameraWriteInterception1Continue, &cameraWriteInterceptor1);
	}
}
