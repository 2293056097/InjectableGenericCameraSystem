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
#include "CameraManipulator.h"
#include "GameConstants.h"
#include "Console.h"
#include "InterceptorHelper.h"

using namespace DirectX;
using namespace std;

extern "C" {
	LPBYTE g_cameraStructAddress = nullptr;
	LPBYTE g_cinematicsCameraStructAddress = nullptr;
}

namespace IGCS::GameSpecific::CameraManipulator
{
	static DWORD _originalLookData[3];	// game uses 3 shorts, aligned on dword addresses. 
	static float _originalCoordsData[3];
	static DWORD _originalCinematicsLookData[3];	// game uses 3 shorts, aligned on dword addresses. 
	static float _originalCinematicsCoordsData[3];

	// Resets the FOV to the default
	void resetFoV()
	{
		float* fovInMemory = reinterpret_cast<float*>(g_cameraStructAddress + FOV_IN_CAMERA_STRUCT_OFFSET);
		float* defaultFov = reinterpret_cast<float*>(g_cameraStructAddress + FOV_DEFAULT_IN_CAMERA_STRUCT_OFFSET);
		*fovInMemory = *defaultFov;
	}


	// changes the FoV with the specified amount
	void changeFoV(float amount)
	{
		float* fovInMemory = reinterpret_cast<float*>(g_cameraStructAddress + FOV_IN_CAMERA_STRUCT_OFFSET);
		*fovInMemory += amount;
	}


	XMFLOAT3 getCurrentCameraCoords(bool mainCamera)
	{
		float* coordsInMemory = reinterpret_cast<float*>((mainCamera ? g_cameraStructAddress : g_cinematicsCameraStructAddress) + CAMERA_COORDS_IN_CAMERA_STRUCT_OFFSET);
		XMFLOAT3 currentCoords = XMFLOAT3(coordsInMemory);
		return currentCoords;
	}
	

	// newCoords are the new coordinates for the camera in worldspace. Angles given are the rotation angles stored in the camera struct
	void writeNewCameraValuesToGameData(XMFLOAT3 newCoords, float pitch, float yaw, float roll, bool mainCamera)
	{
		// this camera is stored as 3 coords (floats) and 3 angles (ushorts, aligned on dword addresses). 
		float* coordsInMemory = reinterpret_cast<float*>((mainCamera ? g_cameraStructAddress : g_cinematicsCameraStructAddress) + CAMERA_COORDS_IN_CAMERA_STRUCT_OFFSET);
		coordsInMemory[0] = newCoords.x;
		coordsInMemory[1] = newCoords.y;
		coordsInMemory[2] = newCoords.z;

		// angles in the game are a ushort. Angles in our camera are 0-2PI. So 2PI is 0x65535. They're aligned on DWORD boundaries. 
		DWORD* anglesInMemory = (DWORD*)((mainCamera ? g_cameraStructAddress : g_cinematicsCameraStructAddress) + LOOK_DATA_IN_CAMERA_STRUCT_OFFSET);
		anglesInMemory[0] = DWORD(((pitch / XM_2PI) * 65535.0f) + 0.5) & 0x0000FFFF;
		anglesInMemory[1] = DWORD(((yaw / XM_2PI) * 65535.0f) + 0.5) & 0x0000FFFF;
		anglesInMemory[2] = DWORD(((roll / XM_2PI) * 65535.0f) + 0.5) & 0x0000FFFF;
	}


	// Waits for the interceptor to pick up the camera struct address. Should only return if address is found 
	void waitForCameraStructAddresses(LPBYTE hostImageAddress)
	{
		Console::WriteLine("Waiting for camera struct interception...");
		while (nullptr == g_cameraStructAddress)
		{
			Sleep(100);
		}
		Console::WriteLine("Camera found.");

#ifdef _DEBUG
		cout << "Camera struct address: " << hex << (void*)g_cameraStructAddress << endl;
#endif
	}


	// should restore the camera values in the camera structures to the cached values. This assures the free camera is always enabled at the original camera location.
	void restoreOriginalCameraValues()
	{
		DWORD* lookInMemory = (DWORD*)(g_cameraStructAddress + LOOK_DATA_IN_CAMERA_STRUCT_OFFSET);
		float* coordsInMemory = reinterpret_cast<float*>(g_cameraStructAddress + CAMERA_COORDS_IN_CAMERA_STRUCT_OFFSET);
		memcpy(lookInMemory, _originalLookData, 3 * sizeof(DWORD));
		memcpy(coordsInMemory, _originalCoordsData, 3 * sizeof(float));

		lookInMemory = (DWORD*)(g_cinematicsCameraStructAddress + LOOK_DATA_IN_CAMERA_STRUCT_OFFSET);
		coordsInMemory = reinterpret_cast<float*>(g_cinematicsCameraStructAddress + CAMERA_COORDS_IN_CAMERA_STRUCT_OFFSET);
		memcpy(lookInMemory, _originalCinematicsLookData, 3 * sizeof(DWORD));
		memcpy(coordsInMemory, _originalCinematicsCoordsData, 3 * sizeof(float));
	}


	void cacheOriginalCameraValues()
	{
		DWORD* lookInMemory = (DWORD*)(g_cameraStructAddress + LOOK_DATA_IN_CAMERA_STRUCT_OFFSET);
		float* coordsInMemory = reinterpret_cast<float*>(g_cameraStructAddress + CAMERA_COORDS_IN_CAMERA_STRUCT_OFFSET);
		memcpy(_originalLookData, lookInMemory, 3 * sizeof(DWORD));
		memcpy(_originalCoordsData, coordsInMemory, 3 * sizeof(float));

		lookInMemory = (DWORD*)(g_cinematicsCameraStructAddress + LOOK_DATA_IN_CAMERA_STRUCT_OFFSET);
		coordsInMemory = reinterpret_cast<float*>(g_cinematicsCameraStructAddress + CAMERA_COORDS_IN_CAMERA_STRUCT_OFFSET);
		memcpy(_originalCinematicsLookData, lookInMemory, 3 * sizeof(DWORD));
		memcpy(_originalCinematicsCoordsData, coordsInMemory, 3 * sizeof(float));
	}
}