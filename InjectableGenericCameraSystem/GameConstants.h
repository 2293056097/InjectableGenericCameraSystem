#pragma once

// Mandatory constants to define for a game
#define GAME_NAME									"Hitman 2016"
#define CAMERA_VERSION								"1.0 (2016-12-10)"
#define CAMERA_CREDITS								"Otis_Inf, Jim2Point0, One3rd"
#define INITIAL_PITCH_RADIANS						(-90.0f * XM_PI) / 180.f	// World has Z up and Y out of the screen, so rotate around X (pitch) -90 degrees.
#define INITIAL_YAW_RADIANS							0.0f
#define INITIAL_ROLL_RADIANS						0.0f
// End Mandatory constants

// Offsets for camera intercept code. Used in interceptor.
#define CAMERA_ADDRESS_INTERCEPT_START_OFFSET		0x41C5B9A		// example from Hitman 2016 (v1.7)
#define CAMERA_ADDRESS_INTERCEPT_CONTINUE_OFFSET	0x41C5BA8		// example from Hitman 2016 (v1.7)
#define CAMERA2_ADDRESS_INTERCEPT_START_OFFSET		0x44307B3
#define CAMERA2_ADDRESS_INTERCEPT_CONTINUE_OFFSET	0x44307C3

#define	CAMERA_WRITE_INTERCEPT1_START_OFFSET		0x41C5B8A		// example from Hitman 2016 (v1.7)
#define CAMERA_WRITE_INTERCEPT1_CONTINUE_OFFSET		0x41C5B9A		// example from Hitman 2016 (v1.7)
#define	CAMERA_WRITE_INTERCEPT2_START_OFFSET		0x41C5BA9		// example from Hitman 2016 (v1.7)
#define CAMERA_WRITE_INTERCEPT2_CONTINUE_OFFSET		0x41C5BB8		// example from Hitman 2016 (v1.7)
#define	CAMERA_WRITE_INTERCEPT3_START_OFFSET		0x41C5BC7		// example from Hitman 2016 (v1.7)
#define CAMERA_WRITE_INTERCEPT3_CONTINUE_OFFSET		0x41C5BF5		// example from Hitman 2016 (v1.7)

#define GAMESPEED_ADDRESS_INTERCEPT_START_OFFSET	0x41C897B		// example from Hitman 2016 (v1.7)
#define GAMESPEED_ADDRESS_INTERCEPT_CONTINUE_OFFSET 0x41C898B		// example from Hitman 2016 (v1.7)

#define FOV_WRITE_INTERCEPT1_START_OFFSET			0x44E4887		// example from Hitman 2016 (v1.7)
#define FOV_WRITE_INTERCEPT2_START_OFFSET			0x44E48A9		// example from Hitman 2016 (v1.7)

// Indices in the structures read by interceptors 
#define LOOK_QUATERNION_IN_CAMERA_STRUCT_OFFSET		0x80			// example from Hitman 2016 (v1.7)
#define CAMERA_COORDS_IN_CAMERA_STRUCT_OFFSET		0x90			// example from Hitman 2016 (v1.7)
#define FOV_IN_CAMERA_STRUCT_OFFSET					0x17C			// example from Hitman 2016 (v1.7)
#define LOOK_QUATERNION_IN_CAMERA2_STRUCT_OFFSET	0x8				// example from Hitman 2016 (v1.7)
#define CAMERA_COORDS_IN_CAMERA2_STRUCT_OFFSET		0x18			// example from Hitman 2016 (v1.7)
#define GAMESPEED_IN_STRUCT_OFFSET					0x48			// example from Hitman 2016 (v1.7)
#define MENU_TIMESTOP_OFFSET_IN_IMAGE				0x2662448		// example from Hitman 2016 (v1.7)