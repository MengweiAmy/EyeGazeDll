// EyeGazeControlJNI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EyeGazeControlJNI.h"
#include "EgWin.h"
#include <windows.h>
#include <iostream>
#include <time.h>
#include <string>
#include "fixfunc.h"

using namespace std;

#define EG_BUFFER_LeN  60;
#define MAX_GAZEPOINTS 36000;
#define	MAX_FIXATIONS	 3600;

struct _stFixPoint            /* data for a single gaze fixation point      */
{
	int iXFixPix;              /* x fixation coordinate (screen pixels)      */
	int iYFixPix;              /* y fixation coordinate (screen pixels)      */
	int iSaccadeDurCnt;        /* duration of the saccade preceeding the     */
							   /*   fixation (camera samples)					 */
	int iFixDurCnt;            /* duration of the fixation (camera samples)  */
	int iFixStartSamp;         /* camera sample at which the fixation        */
							   /*   started											 */
};

/****************************************************************************/
/* GLOBAL VARIABLES																			 */

struct _stRawGazepoint stRawGazepoint[36000];
/* array of raw gazepoint structures		 */
struct _stFixPoint stFixPoint[3600];

static struct _stEgControl stEgControl;
struct _stEgData stEgData;

float fMinFixMs = 100.0f;
float iMinFixSamples = 0;
float fGazeDeviationThreshMm = 6.35F;

float fCollectionTimeSeconds = 60.0F; /* length of time to collect gazepoint  */  //what
																				  /*   data (seconds)								 */
int	iSamplesToCollect = 36000;			/* maximum number of gazepoint data 		 */
								/*   samples to collect 						 */
int	iLastSampleCollected;		/* index of the last gazepoint data sample */
								/*   actually collected during the test	 */
								/*   run */
int iLastFixCollected;
int iWindowHorzOffset = 0;
int iWindowVertOffset = 0;

int iScreenWidthPix;
int iScreenHeightPix;

// This is an example of an exported variable
EYEGAZECONTROLJNI_API int nEyeGazeControlJNI=0;

// This is an example of an exported function.
EYEGAZECONTROLJNI_API int fnEyeGazeControlJNI(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see EyeGazeControlJNI.h for the class definition
CEyeGazeControlJNI::CEyeGazeControlJNI()
{
	//Allow the application program to get up to 1 second behind
	//stEgControl.iNDataSetsInRingBuffer = EG_BUFFER_LeN;
	//stEgControl.bTrackingActive = FALSE;
	//stEgControl.iScreenWidthPix = iScreenWidthPix;
	//stEgControl.iScreenHeightPix = iScreenheightPix;
	//stEgControl.iVisionSelect = 0;
	//stEgControl.iCommType = EG_COMM_TYPE_LOCAL;
}

int CEyeGazeControlJNI::initDevice()
{

	iScreenWidthPix = GetSystemMetrics(SM_CXSCREEN);
	iScreenHeightPix = GetSystemMetrics(SM_CYSCREEN);
	//Allow the application program to get up to 1 second behind
	stEgControl.iNDataSetsInRingBuffer = EG_BUFFER_LeN;
	stEgControl.bTrackingActive = FALSE;
	stEgControl.iScreenWidthPix = iScreenWidthPix;
	stEgControl.iScreenHeightPix = iScreenHeightPix;
	stEgControl.iVisionSelect = 0;
	stEgControl.iCommType = EG_COMM_TYPE_LOCAL;
	int result = EgInit(&stEgControl);
	stEgControl.bTrackingActive = TRUE;
	return result;
}

int CEyeGazeControlJNI::shutDownDevice()
{
	stEgControl.bTrackingActive = FALSE;
	int result = EgExit(&stEgControl);
	return result;
}

void CEyeGazeControlJNI::calibrateDevice()
{
	EgCalibrate2(&stEgControl, EG_CALIBRATE_NONDISABILITY_APP);
	return;
}

_stRawGazepoint* CEyeGazeControlJNI::getEyeData()
{/* Collect the data samples.																 */
 /* 	Stop collecting data if the user requests it.								 */
	while (stEgControl.bTrackingActive == TRUE)
	{
		int i = 0;
		/* - - - - - - - - - - - - -	Track Gazepoint - - - - - - - - - - - - - - - */
		if (i >= 36000) 
		{
			break;
		}
		else
		{
			/* 	Track the user gazepoint.															 */
			EgGetData(&stEgControl);

			/* 	Record whether the gaze was found.												 */
			stRawGazepoint[i].bGazeTracked = stEgControl.pstEgData->bGazeVectorFound;

			/* 	Originally assume the point is not associated with a fixation. 		 */
			stRawGazepoint[i].iFixIndex = -1;


			/* 	If the gazepoint was tracked this sample, 									 */
			if (stEgControl.pstEgData->bGazeVectorFound == TRUE)
			{
				/* 		Record the raw gazepoint data:												 */
				stRawGazepoint[i].iXGazeWindowPix = stEgControl.pstEgData->iIGaze - iWindowHorzOffset;
				stRawGazepoint[i].iYGazeWindowPix = stEgControl.pstEgData->iJGaze - iWindowVertOffset;
				stRawGazepoint[i].fPupilDiamMm = stEgControl.pstEgData->fPupilRadiusMm * 2;
				stRawGazepoint[i].fXEyeballMm = stEgControl.pstEgData->fXEyeballOffsetMm;
				stRawGazepoint[i].fYEyeballMm = stEgControl.pstEgData->fYEyeballOffsetMm;
				stRawGazepoint[i].fFocusOffsetMm = stEgControl.pstEgData->fFocusRangeOffsetMm;
				stRawGazepoint[i].fFocusRangeMm = stEgControl.pstEgData->fFocusRangeImageTime;
			}
			/* 	Otherwise, if the gazepoint was not tracked this sample, 				 */
			else
			{
				/* 		Record zeros:																		 */
				stRawGazepoint[i].iXGazeWindowPix = 0;
				stRawGazepoint[i].iYGazeWindowPix = 0;
				stRawGazepoint[i].fPupilDiamMm = 0.0F;
				stRawGazepoint[i].fXEyeballMm = 0.0F;
				stRawGazepoint[i].fYEyeballMm = 0.0F;
				stRawGazepoint[i].fFocusOffsetMm = 0.0F;
				stRawGazepoint[i].fFocusRangeMm = stEgControl.pstEgData->fFocusRangeImageTime;
			}
			/*----------------------- End Data Collection Loop -------------------------*/

			/* Write the trace data file. 															 */
			//WriteTraceDataFile(TEXT("trace.dat"));
			i++;
		}
	}
	return stRawGazepoint;
}
//Before start logging, should open the log file first
//Then write the column header in the log file
//Finally call the start log function
int CEyeGazeControlJNI::startLogging()
{
	//std::string filename = currentDateTime() + ".dat";
	//char *cstr = new char[filename.length() + 1];
	//strcpy(cstr, filename.c_str());
	//int openFile = EgLogFileOpen(&stEgControl, cstr, "a");
	int openFile = EgLogFileOpen(&stEgControl, "trace.dat", "w");
	if (openFile == 0) {
		EgLogWriteColumnHeader(&stEgControl);
		EgLogStart(&stEgControl);
	}
	//delete[] cstr;
	return openFile;
}

//After stop logging, close the log file
int CEyeGazeControlJNI::stopLogging()
{
	EgLogStop(&stEgControl);
	EgLogFileClose(&stEgControl);
	return 0;
}

_stEgControl CEyeGazeControlJNI::getControlData()
{
	return stEgControl;
}

int CEyeGazeControlJNI::egDetectFixtion(int bGazeTracked, float iXGazeWindowPix, 
	float iYGazeWindowPix, float fGazeDeviationThreshPix, int iMinFixSamples, 
	int *bGazepoint, float *fXGazeD, float *fYGazeD,
	float *fGazeDeviationD, float *fXFixDe, float *fYFixD, 
	int *iSaccadeDurationD, int *iFixDurationD)
{

	int   bGazepointFoundDelayed;   /* sample gazepoint-found flag,             */
									/*   iMinFixSamples ago                     */
	float fXGazeDelayed;            /* sample gazepoint coordinates,            */
	float fYGazeDelayed;            /*   iMinFixSamples ago                     */
	float fGazeDeviationDelayed;    /* deviation of the gaze from the           */
									/*   present fixation,                      */
									/*   iMinFixSamples ago                     */

									/* Fixation Data - delayed:                 */
	float fXFixDelayed;             /* fixation point as estimated              */
	float fYFixDelayed;             /*   iMinFixSamples ago                     */
	int   iSaccadeDurationDelayed;  /* duration of the saccade                  */
									/*   preceeding the preset fixation         */
									/*   (samples)                              */
	int   iFixDurationDelayed;      /* duration of the present fixation         */
									/*   (samples) */
	int iEyeMotionState = DetectFixation(bGazeTracked,iXGazeWindowPix,iYGazeWindowPix,
		fGazeDeviationThreshPix,
		iMinFixSamples,

		&bGazepointFoundDelayed,
		&fXGazeDelayed, &fYGazeDelayed,
		&fGazeDeviationDelayed,

		&fXFixDelayed, &fYFixDelayed,
		&iSaccadeDurationDelayed,
		&iFixDurationDelayed);

	*bGazepoint = bGazepointFoundDelayed;
	*fXGazeD = fXGazeDelayed;
	*fYGazeD = fYGazeDelayed;
	*fGazeDeviationD = fGazeDeviationDelayed;
	*fXFixDe = fXFixDelayed;
	*fYFixD = fYFixDelayed;
	*iSaccadeDurationD = iSaccadeDurationDelayed;
	*iFixDurationD = iFixDurationDelayed;

	return iEyeMotionState;
}

/* This function adds a fixation to the fixation data array.					 */
void CEyeGazeControlJNI::AddFixation(int * iLastFixCollected, int iFixStartSample, float fXFix, float fYFix, int iSaccadeDuration, int iFixDuration)
{
	/* If the fixation array is already full, 											 */
	if (*iLastFixCollected >= 3600 - 1)
	{
		/* 	Return without adding the fixation. 											 */
		return;
	}

	/* Increment the fixation index. 														 */
	*iLastFixCollected = *iLastFixCollected + 1;

	/* Store the fixation in the temporary holding array. 							 */
	stFixPoint[*iLastFixCollected].iXFixPix = int(fXFix);
	stFixPoint[*iLastFixCollected].iYFixPix = int(fYFix);
	stFixPoint[*iLastFixCollected].iSaccadeDurCnt = iSaccadeDuration;
	stFixPoint[*iLastFixCollected].iFixDurCnt = iFixDuration;
	stFixPoint[*iLastFixCollected].iFixStartSamp = iFixStartSample;
}

/* This function writes the trace data to the disk file. 						 */
void CEyeGazeControlJNI::WriteTraceDataFile(wchar_t * pchTraceDataFileName)
{
	FILE *fp_trace_file; 			  /* trace data output file					 */
	time_t  lTime;
	struct  tm	tmToday;
	wchar_t date_string[21];        /* string for today's date               */
	wchar_t time_string[21];        /* string for time test was run          */
	int  i;								  /* general index								 */

	/* Open the trace file for writing. 													 */
	_wfopen_s(&fp_trace_file, pchTraceDataFileName, TEXT("w"));
	if (fp_trace_file == NULL)
	{
		wprintf(TEXT("Error opening %s file for writing\n"), pchTraceDataFileName);
		return;
	}

	/* Get the present date and time.														 */
	time(&lTime);
	localtime_s(&tmToday, &lTime);

	swprintf_s(date_string, 20, TEXT("%02i/%02i/%04i"),
		tmToday.tm_mon + 1,
		tmToday.tm_mday,
		tmToday.tm_year + 1900);
	swprintf_s(time_string, 20, TEXT("%02i:%02i:%02i"),
		tmToday.tm_hour,
		tmToday.tm_min,
		tmToday.tm_sec);

	/* Write a header line for the file.													 */
	fwprintf(fp_trace_file, TEXT("Gazepoint Trace Data File,  %s  %s\n"),
		time_string, date_string);

	/* Record the scene type for which the trace was collected. 					 */
	fwprintf(fp_trace_file, TEXT("Scene Type: "));
	fwprintf(fp_trace_file, TEXT("text %i %i"),
		iScreenWidthPix, iScreenHeightPix);
	fwprintf(fp_trace_file, TEXT("\n"));

	/* - - - - - - - - - - - Write Raw Gazepoint Trace - - - - - - - - - - - - -*/
	/* Write the header data for the columns. 											 */
	fwprintf(fp_trace_file,
		TEXT("samp  Eye     Gazepoint  Pupil   Eyeball-Position  Focus   Fix\n"));
	fwprintf(fp_trace_file,
		TEXT("indx Found    X      Y    Diam     X     Y     Z   Range  Indx\n"));
	fwprintf(fp_trace_file,
		TEXT("     (t/f)  (pix)  (pix)  (mm)   (mm)  (mm)  (mm)   (mm)      \n\n"));

	/* Print the raw data for the gazepoint sample. 									 */
	for (i = 0; i <= iLastSampleCollected; i++)
	{
		fwprintf(fp_trace_file, TEXT("%3i %5i %6i %6i %6.2f %6.1f %5.1f %5.1f %7.1f %4i\n"),
			i,
			stRawGazepoint[i].bGazeTracked,
			stRawGazepoint[i].iXGazeWindowPix,
			stRawGazepoint[i].iYGazeWindowPix,
			stRawGazepoint[i].fPupilDiamMm,
			stRawGazepoint[i].fXEyeballMm,
			stRawGazepoint[i].fYEyeballMm,
			stRawGazepoint[i].fFocusOffsetMm,
			stRawGazepoint[i].fFocusRangeMm,
			stRawGazepoint[i].iFixIndex);
	}

	/* - - - - - - - - - - - - Write Fixation Trace - - - - - - - - - - - - - - */
	/* Write the header data for the columns. 											 */
	fwprintf(fp_trace_file,
		TEXT(" fix    Fixation     Sac   Fix   Fix \n"));
	fwprintf(fp_trace_file,
		TEXT("indx    X      Y     Dur   Dur  Start\n"));
	fwprintf(fp_trace_file,
		TEXT("      (pix)  (pix)  (cnt) (cnt)  Samp\n\n"));

	/* Print the fixation data points.														 */
	for (i = 0; i <= iLastFixCollected; i++)
	{
		fwprintf(fp_trace_file, TEXT("%3i %6i %6i %5i %6i %6i\n"),
			i,
			stFixPoint[i].iXFixPix,
			stFixPoint[i].iYFixPix,
			stFixPoint[i].iSaccadeDurCnt,
			stFixPoint[i].iFixDurCnt,
			stFixPoint[i].iFixStartSamp);
	}

	/*- - - - - - - - - - - - - - Close Trace File - - - - - - - - - - - - - - -*/

	/* Close the trace data file. 															 */
	fclose(fp_trace_file);
}
