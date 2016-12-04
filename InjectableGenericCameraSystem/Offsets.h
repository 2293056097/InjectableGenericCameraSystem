#pragma once

// Offsets for camera intercept code. 
// TODO: Add AOB scanning to replace these offsets.
#define MATRIX_ADDRESS_INTERCEPT_START_OFFSET		0x41C5B9A		// example from Hitman 2016 (v1.7)
#define MATRIX_ADDRESS_INTERCEPT_CONTINUE_OFFSET	0x41C5BA8		// example from Hitman 2016 (v1.7)

#define	MATRIX_WRITE_INTERCEPT1_START_OFFSET		0x41C9150		// example from Hitman 2016 (v1.7)
#define MATRIX_WRITE_INTERCEPT1_CONTINUE_OFFSET		0x41C9171		// example from Hitman 2016 (v1.7)
#define	MATRIX_WRITE_INTERCEPT2_START_OFFSET		0x41C5B8A		// example from Hitman 2016 (v1.7)
#define MATRIX_WRITE_INTERCEPT2_CONTINUE_OFFSET		0x41C5B9A		// example from Hitman 2016 (v1.7)
#define	MATRIX_WRITE_INTERCEPT3_START_OFFSET		0x41C5BA9		// example from Hitman 2016 (v1.7)
#define MATRIX_WRITE_INTERCEPT3_CONTINUE_OFFSET		0x41C5BB8		// example from Hitman 2016 (v1.7)
#define	MATRIX_WRITE_INTERCEPT4_START_OFFSET		0x41C5BC7		// example from Hitman 2016 (v1.7)
#define MATRIX_WRITE_INTERCEPT4_CONTINUE_OFFSET		0x41C5BF5		// example from Hitman 2016 (v1.7)

#define MATRIX_START_IN_CAMERA_STRUCT_OFFSET		0x60			// example from Hitman 2016 (v1.7)