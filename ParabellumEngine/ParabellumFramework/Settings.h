#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// ----------------------------------------------------------------------------------------------------
//		Memory
// ----------------------------------------------------------------------------------------------------

#define PF_MEM_HEAP_SIZE 10000000000


// ----------------------------------------------------------------------------------------------------
//		Profiler - Profile.h
// ----------------------------------------------------------------------------------------------------

#define PF_PROFILER_MAX_INSTANCES 8
#define PF_PROFILER_MAX_NODES 32
#define PF_PROFILER_MAX_INFOSTACK 4096


// Debug
	
	#define DEBUG_SETUP_DEBUGGING							// enable debugging

	#define DEBUG_SHOW_DETAILS								// ebable verbose modes

	#define DEBUG_MEMORY									// show details about memory

	#define DEBUG_LOG_OUTPUT

	#define DEBUG_LOG_BUFFER_SIZE 1024

	#define DEBUG_LOGLEVEL_ERROR

	#define DEBUG_LOGLEVEL_1 DEBUG_LOGLEVEL_ERROR
	#define DEBUG_LOGLEVEL_2
	#define DEBUG_LOGLEVEL_3

	// debugging graphics device - L1 - only error ; L2 - extended ; L3 - verbose
	#define DEBUG_GRAPHICSDEVICE_L1

	#ifdef DEBUG_GRAPHICSDEVICE_L1

		//#define DEBUG_GRAPHICSDEVICE_L2
		
		#ifdef DEBUG_GRAPHICSDEVICE_L2
			//#define DEBUG_GRAPHICSDEVICE_L3
		#endif

	#endif

	
	// Track debug data for graphics device (fps counter, triangles count, shader set count etc.)
	#define DEBUG_GRAPHICSDEVICE_DEBUGDATA_ENABLED


	// debugging input device - L1 - only error ; L2 - extended ; L3 - verbose
	#define DEBUG_INPUTDEVICE_L1

		#ifdef DEBUG_INPUTDEVICE_L1

			#define DEBUG_INPUTDEVICE_L2

			#ifdef DEBUG_INPUTDEVICE_L2
			#define DEBUG_INPUTDEVICE_L3
			#endif

		#endif

// Profiler

	#define PF_PROFILER_ENABLED

	#define PF_PROFILER_MEMALLOC

// ------------------------------------------------------------------------------------
// Graphics
// ------------------------------------------------------------------------------------



	// Model

		// numer of unique ModelParts which can be loaded
		#define PF_MODEL_MAXPARTS	4096

		#define PF_MODEL_MAXOBJECTS	16384


		#define PF_GO1					EBYTENO_1	
		#define PF_GO					EBYTENO_2	
		#define PF_GO_FULLSCREEN		EBYTENO_3		// set fullscreen mode
		#define PF_GO_VSYNC				EBYTENO_4		// enable VSYNC
		#define PF_TWOGPU				EBYTENO_5		// use two GPUs	


		#define PF_GRAPHICS_MODE_PARALLEL				// GraphicsDevice will run rendering in multi CPU threads


#endif