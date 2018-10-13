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
#pragma once
#include "stdafx.h"
#include "Gamepad.h"
#include "GameConstants.h"
#include "Defaults.h"
#include "CDataFile.h"
#include "Utils.h"
#include <atomic>

extern "C" BYTE g_cameraEnabled;
extern "C" BYTE g_gamePaused;

namespace IGCS
{
	struct Settings
	{
		// settings written to config file
		bool invertY;
		float fastMovementMultiplier;
		float slowMovementMultiplier;
		float movementUpMultiplier;
		float movementSpeed;
		float rotationSpeed;
		float fovChangeSpeed;
		int cameraControlDevice;		// 0==keyboard/mouse, 1 == gamepad, 2 == both, see Defaults.h
		bool allowCameraMovementWhenMenuIsUp;

		// settings not persisted to config file.
		// add settings to edit here.
		float resolutionScale;			// 0.5-4.0
		int todHour;					// 0-23
		int todMinute;					// 0-59

		void loadFromFile()
		{
			CDataFile iniFile;
			if (!iniFile.Load(IGCS_SETTINGS_INI_FILENAME))
			{
				// doesn't exist
				return;
			}
			invertY = iniFile.GetBool("invertY", "CameraSettings");
			allowCameraMovementWhenMenuIsUp = iniFile.GetBool("allowCameraMovementWhenMenuIsUp", "CameraSettings");
			fastMovementMultiplier = Utils::clamp(iniFile.GetFloat("fastMovementMultiplier", "CameraSettings"), 0.0f, FASTER_MULTIPLIER);
			slowMovementMultiplier = Utils::clamp(iniFile.GetFloat("slowMovementMultiplier", "CameraSettings"), 0.0f, SLOWER_MULTIPLIER);
			movementUpMultiplier = Utils::clamp(iniFile.GetFloat("movementUpMultiplier", "CameraSettings"), 0.0f, DEFAULT_UP_MOVEMENT_MULTIPLIER);
			movementSpeed = Utils::clamp(iniFile.GetFloat("movementSpeed", "CameraSettings"), 0.0f, DEFAULT_MOVEMENT_SPEED);
			rotationSpeed = Utils::clamp(iniFile.GetFloat("rotationSpeed", "CameraSettings"), 0.0f, DEFAULT_ROTATION_SPEED);
			fovChangeSpeed = Utils::clamp(iniFile.GetFloat("fovChangeSpeed", "CameraSettings"), 0.0f, DEFAULT_FOV_SPEED);
			cameraControlDevice = Utils::clamp(iniFile.GetInt("cameraControlDevice", "CameraSettings"), 0, DEVICE_ID_ALL, DEVICE_ID_ALL);
		}


		void saveToFile()
		{
			CDataFile iniFile;
			iniFile.SetBool("invertY", invertY, "", "CameraSettings");
			iniFile.SetBool("allowCameraMovementWhenMenuIsUp", allowCameraMovementWhenMenuIsUp, "", "CameraSettings");
			iniFile.SetFloat("fastMovementMultiplier", fastMovementMultiplier, "", "CameraSettings");
			iniFile.SetFloat("slowMovementMultiplier", slowMovementMultiplier, "", "CameraSettings");
			iniFile.SetFloat("movementUpMultiplier", movementUpMultiplier, "", "CameraSettings");
			iniFile.SetFloat("movementSpeed", movementSpeed, "", "CameraSettings");
			iniFile.SetFloat("rotationSpeed", rotationSpeed, "", "CameraSettings");
			iniFile.SetFloat("fovChangeSpeed", fovChangeSpeed, "", "CameraSettings");
			iniFile.SetInt("cameraControlDevice", cameraControlDevice, "", "CameraSettings");
			iniFile.SetFileName(IGCS_SETTINGS_INI_FILENAME);
			iniFile.Save();
		}


		void init(bool persistedOnly)
		{
			invertY = CONTROLLER_Y_INVERT;
			fastMovementMultiplier = FASTER_MULTIPLIER;
			slowMovementMultiplier = SLOWER_MULTIPLIER;
			movementUpMultiplier = DEFAULT_UP_MOVEMENT_MULTIPLIER;
			movementSpeed = DEFAULT_MOVEMENT_SPEED;
			rotationSpeed = DEFAULT_ROTATION_SPEED;
			fovChangeSpeed = DEFAULT_FOV_SPEED;
			cameraControlDevice = DEVICE_ID_ALL;
			allowCameraMovementWhenMenuIsUp = false;

			if (!persistedOnly)
			{
				resolutionScale = 1.0f;
				todHour = 12;
				todMinute = 0;
			}
		}
	};


	class Globals
	{
	public:
		Globals();
		~Globals();

		static Globals& instance();

		void saveSettingsIfRequired(float delta);
		void markSettingsDirty();

		bool inputBlocked() const { return _inputBlocked; }
		void inputBlocked(bool value) { _inputBlocked = value; }
		bool systemActive() const { return _systemActive; }
		void systemActive(bool value) { _systemActive = value; }
		HWND mainWindowHandle() const { return _mainWindowHandle; }
		void mainWindowHandle(HWND handle) { _mainWindowHandle = handle; }
		Gamepad& gamePad() { return _gamePad; }
		Settings& settings() { return _settings; }
		bool keyboardMouseControlCamera() const { return _settings.cameraControlDevice == DEVICE_ID_KEYBOARD_MOUSE || _settings.cameraControlDevice == DEVICE_ID_ALL; }
		bool controllerControlsCamera() const { return _settings.cameraControlDevice == DEVICE_ID_GAMEPAD || _settings.cameraControlDevice == DEVICE_ID_ALL; }

	private:
		bool _inputBlocked = false;
		atomic_bool _systemActive = false;
		Gamepad _gamePad;
		HWND _mainWindowHandle;
		Settings _settings;
		float _settingsDirtyTimer=0.0f;			// when settings are marked dirty, this is set with a value > 0 and decremented each frame. If 0, settings are saved. In seconds.
	};
}