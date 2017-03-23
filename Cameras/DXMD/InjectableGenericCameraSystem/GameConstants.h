//////////////////////////	//////////////////////////////////////////////////////////////////////////////
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

namespace IGCS::GameSpecific
{
	// Mandatory constants to define for a game
	#define GAME_NAME									"Deus Ex:Mankind Divided 1.17"
	#define CAMERA_VERSION								"1.0.3"
	#define CAMERA_CREDITS								"Otis_Inf. Special thanks to: K-putt, Jim2Point0"
	#define GAME_WINDOW_TITLE							"Deus Ex: Mankind Divided"
	#define INITIAL_PITCH_RADIANS						(90.0f * XM_PI) / 180.f	// World has Z up and Y into the screen, so rotate around X (pitch) 90 degrees.
	#define INITIAL_YAW_RADIANS							0.0f	// around Y axis	(out of the screen)
	#define INITIAL_ROLL_RADIANS						0.0f	// around Z axis	(up)
	#define CONTROLLER_Y_INVERT							false
	#define FASTER_MULTIPLIER							10.0f
	#define SLOWER_MULTIPLIER							0.1f
	#define MOUSE_SPEED_CORRECTION						0.2f	// to correct for the mouse-deltas related to normal rotation.
	#define DEFAULT_MOVEMENT_SPEED						0.025f
	#define DEFAULT_ROTATION_SPEED						0.015f
	#define DEFAULT_FOV_RADIANS							54.54f
	#define DEFAULT_FOV_SPEED							0.1f
	#define DEFAULT_UP_MOVEMENT_MULTIPLIER				0.7f
	#define DEFAULT_GAME_SPEED							1.0f
	#define DEFAULT_MIN_GAME_SPEED						0.001f
	#define DEFAULT_MAX_GAME_SPEED						10.0f
	// End Mandatory constants

	// AOB Keys for interceptor's AOB scanner
	#define CAMERA_ADDRESS_INTERCEPT_KEY				"AOB_CAMERA_ADDRESS_INTERCEPT"
	#define CAMERA_WRITE_INTERCEPT_KEY					"AOB_CAMERA_WRITE_INTERCEPT"
	#define GAMESPEED_ADDRESS_INTERCEPT_KEY				"AOB_GAMESPEED_ADDRESS_INTERCEPT"
	#define HUD_TOGGLE_ADDRESS_INTERCEPT_KEY			"AOB_HUD_TOGGLE_ADDRESS_INTERCEPT"
	#define FOV_WRITE_ADDRESS_INTERCEPT_KEY				"AOB_FOV_WRITE_ADDRESS_INTERCEPT"

	// Indices in the structures read by interceptors 
	#define CAMERA_COORDS_IN_CAMERA_STRUCT_OFFSET		0x74
	#define LOOK_DATA_IN_CAMERA_STRUCT_OFFSET			0x50		
	#define FOV_IN_CAMERA_STRUCT_OFFSET					0x13C		
	#define GAMESPEED_IN_STRUCT_OFFSET					0x60
	#define HUD_TOGGLE_IN_STRUCT_OFFSET					0x24
	#define TIMESTOP_VALUE_IN_STRUCT_OFFSET				0xE8
}