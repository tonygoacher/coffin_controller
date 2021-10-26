#pragma once


#define MARKSPACE_SEC "MARKSPACE"	// The markspace output section
#define MARKSPACE_ON	"ON"		// Time for marspace to be on
#define MARKSPACE_OFF	"OFF"		// Time for markspace to be off

#define TIMINGS_SEC "TIMINGS"		// Generic timings sectiion
#define TIMINGS_STROBE	"STROBE"	// Time for strobe to be on

#define BACKGROUNDSAMPLE_SEC	"BACKSAMPLES"	// Background music sample SECTION_MAP_EXECUTE_EXPLICIT
#define	BACKGROUNDSAMPLE_KEY	"SAMPLE%d"		// Key for sample

#define DOORBELL_SEC	"DOORBELL"
#define DOORBELLSAMPLE_KEY	"SAMPLE"
#define DOORBELL_NUMRANDOM_KEY	"NUMRANDOM"		// Number of random samples
#define DOORBELL_RANDOMFORMATTER_KEY	"SAMPLE%d"	// Formatter for the random sample
#define RANDOM_TIMEOUT_KEY "RANDOMTIMEOUT"		// Time to wait for a second doorbell after the initial lightening