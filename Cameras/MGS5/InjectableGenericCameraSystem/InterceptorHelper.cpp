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
#include "OverlayConsole.h"

using namespace std;

//--------------------------------------------------------------------------------------------------------------------------------
// external asm functions
extern "C" {
	void cameraStructInterceptor();
	void cameraCutsceneStructInterceptor();
	void cameraCutsceneWrite1Interceptor();
	void fovWriteInterceptor();
	void gamespeedWriteInterceptor();
}

// external addresses used in asm.
extern "C" {
	LPBYTE _cameraStructInterceptionContinue = nullptr;
	LPBYTE _cameraCutsceneStructInterceptionContinue = nullptr;
	LPBYTE _cameraCutsceneWrite1InterceptionContinue = nullptr;
	LPBYTE _fovWriteInterceptionContinue = nullptr;
	LPBYTE _gamespeedInterceptionContinue = nullptr;
}


namespace IGCS::GameSpecific::InterceptorHelper
{
	void initializeAOBBlocks(LPBYTE hostImageAddress, DWORD hostImageSize, map<string, AOBBlock*> &aobBlocks)
	{
		aobBlocks[CAMERA_ADDRESS_INTERCEPT_KEY] = new AOBBlock(CAMERA_ADDRESS_INTERCEPT_KEY, "0F 28 02 0F 29 81 F0 00 00 00 0F 28 4A 10 0F 29 89 00 01 00 00 C3", 1);	
		aobBlocks[CAMERA_CUTSCENE_ADDRESS_INTERCEPT_KEY] = new AOBBlock(CAMERA_CUTSCENE_ADDRESS_INTERCEPT_KEY, "0F 29 88 20 01 00 00 0F 28 44 24 40 0F 29 80 30 01 00 00 41 8B 46 60", 1);
		aobBlocks[CAMERA_CUTSCENE_WRITE1_INTERCEPT_KEY] = new AOBBlock(CAMERA_CUTSCENE_WRITE1_INTERCEPT_KEY, "0F 29 80 20 01 00 00 0F 29 88 30 01 00 00 48 8B 81 80 00 00 00", 1);
		aobBlocks[FOV_WRITE_INTERCEPT_KEY] = new AOBBlock(FOV_WRITE_INTERCEPT_KEY, "F3 0F 59 8E B8 05 00 00 F3 0F 58 C8 F3 0F 11 49 0C 8B 86 24 02 00 00", 1);
		aobBlocks[GAMESPEED_WRITE_INTERCEPT_KEY] = new AOBBlock(GAMESPEED_WRITE_INTERCEPT_KEY, "F3 0F 11 7F 08 F3 0F 11 77 0C 48 8B 47 28 48 85 C0", 1);

		map<string, AOBBlock*>::iterator it;
		bool result = true;
		for (it = aobBlocks.begin(); it != aobBlocks.end(); it++)
		{
			result &= it->second->scan(hostImageAddress, hostImageSize);
		}
		if (result)
		{
			OverlayConsole::instance().logLine("All interception offsets found.");
		}
		else
		{
			OverlayConsole::instance().logError("One or more interception offsets weren't found: tools aren't compatible with this game's version.");
		}
	}


	void setCameraStructInterceptorHook(map<string, AOBBlock*> &aobBlocks)
	{
		GameImageHooker::setHook(aobBlocks[CAMERA_ADDRESS_INTERCEPT_KEY], 0x15, &_cameraStructInterceptionContinue, &cameraStructInterceptor);
	}


	void setPostCameraStructHooks(map<string, AOBBlock*> &aobBlocks, LPBYTE hostImageAddress)
	{
		GameImageHooker::setHook(aobBlocks[CAMERA_CUTSCENE_ADDRESS_INTERCEPT_KEY], 0x13, &_cameraCutsceneStructInterceptionContinue, &cameraCutsceneStructInterceptor);
		GameImageHooker::setHook(aobBlocks[CAMERA_CUTSCENE_WRITE1_INTERCEPT_KEY], 0x15, &_cameraCutsceneWrite1InterceptionContinue, &cameraCutsceneWrite1Interceptor);
		GameImageHooker::setHook(aobBlocks[FOV_WRITE_INTERCEPT_KEY], 0x11, &_fovWriteInterceptionContinue, &fovWriteInterceptor);
		GameImageHooker::setHook(aobBlocks[GAMESPEED_WRITE_INTERCEPT_KEY], 0x0E, &_gamespeedInterceptionContinue, &gamespeedWriteInterceptor);
	}
}
