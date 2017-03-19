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

struct _stEgData stRawEyeGazeData[36000];
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
	stEgControl.fHorzPixPerMm = 10.246F;
	stEgControl.iSamplePerSec = 60;
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

void CEyeGazeControlJNI::startMonitorData()
{
	int i = 0;
	while (stEgControl.bTrackingActive == TRUE)
	{
		cout << "log from Gaze control cpp: Enter the monitor data" << endl;
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
			stRawEyeGazeData[i].bGazeVectorFound = stEgControl.pstEgData->bGazeVectorFound;

			/* 	Originally assume the point is not associated with a fixation. 		 */
			//stRawGazepoint[i].iFixIndex = -1;


			/* 	If the gazepoint was tracked this sample, 									 */
			if (stEgControl.pstEgData->bGazeVectorFound == TRUE)
			{
				cout << "log from Gaze control cpp: Getting eye data and prepare to pass to Java" << endl;
				/* 		Record the raw gazepoint data:												 */
				stRawEyeGazeData[i].iIGaze = stEgControl.pstEgData->iIGaze - iWindowHorzOffset;
				stRawEyeGazeData[i].iJGaze = stEgControl.pstEgData->iJGaze - iWindowVertOffset;
				stRawEyeGazeData[i].fPupilRadiusMm = stEgControl.pstEgData->fPupilRadiusMm * 2;
				stRawEyeGazeData[i].fXEyeballOffsetMm = stEgControl.pstEgData->fXEyeballOffsetMm;
				stRawEyeGazeData[i].fYEyeballOffsetMm = stEgControl.pstEgData->fYEyeballOffsetMm;
				stRawEyeGazeData[i].fFocusRangeOffsetMm = stEgControl.pstEgData->fFocusRangeOffsetMm;
				stRawEyeGazeData[i].fFocusRangeImageTime = stEgControl.pstEgData->fFocusRangeImageTime;
				cout << "log from Gaze control cpp: fPupilRadiusMm" << stEgControl.pstEgData->fPupilRadiusMm * 2 << endl;
				cout << "log from Gaze control cpp: iIGaze" << stEgControl.pstEgData->iJGaze << endl;
				cout << "log from Gaze control cpp:Current data size is: "<< i << endl;
			}
			/* 	Otherwise, if the gazepoint was not tracked this sample, 				 */
			else
			{
				/* 		Record zeros:																		 */
				stRawEyeGazeData[i].iIGaze = 0;
				stRawEyeGazeData[i].iJGaze = 0;
				stRawEyeGazeData[i].fPupilRadiusMm = 0.0F;
				stRawEyeGazeData[i].fXEyeballOffsetMm = 0.0F;
				stRawEyeGazeData[i].fYEyeballOffsetMm = 0.0F;
				stRawEyeGazeData[i].fFocusRangeOffsetMm = 0.0F;
				stRawEyeGazeData[i].fFocusRangeImageTime = stEgControl.pstEgData->fFocusRangeImageTime;
			}
			/*----------------------- End Data Collection Loop -------------------------*/

			/* Write the trace data file. 															 */
			//WriteTraceDataFile(TEXT("trace.dat"));
			i++;
		}
	}
}

_stEgData CEyeGazeControlJNI::getEyeData()
{/* Collect the data samples.																 */
 /* 	Stop collecting data if the user requests it.								 */
 /* 	Track the user gazepoint.
 */

	_stEgData stRawEyeGazeData;
	EgGetData(&stEgControl);

	/* 	Record whether the gaze was found.												 */
	stRawEyeGazeData.bGazeVectorFound = stEgControl.pstEgData->bGazeVectorFound;

	/* 	Originally assume the point is not associated with a fixation. 		 */
	//stRawGazepoint[i].iFixIndex = -1;


	/* 	If the gazepoint was tracked this sample, 									 */
	if (stEgControl.pstEgData->bGazeVectorFound == TRUE)
	{
		cout << "log from Gaze control cpp: Getting eye data and prepare to pass to Java" << endl;
		/* 		Record the raw gazepoint data:												 */
		stRawEyeGazeData.iIGaze = stEgControl.pstEgData->iIGaze - iWindowHorzOffset;
		stRawEyeGazeData.iJGaze = stEgControl.pstEgData->iJGaze - iWindowVertOffset;
		stRawEyeGazeData.fPupilRadiusMm = stEgControl.pstEgData->fPupilRadiusMm * 2;
		stRawEyeGazeData.fXEyeballOffsetMm = stEgControl.pstEgData->fXEyeballOffsetMm;
		stRawEyeGazeData.fYEyeballOffsetMm = stEgControl.pstEgData->fYEyeballOffsetMm;
		stRawEyeGazeData.fFocusRangeOffsetMm = stEgControl.pstEgData->fFocusRangeOffsetMm;
		stRawEyeGazeData.fFocusRangeImageTime = stEgControl.pstEgData->fFocusRangeImageTime;
		cout << "log from Gaze control cpp: fPupilRadiusMm" << stEgControl.pstEgData->fPupilRadiusMm * 2 << endl;
		cout << "log from Gaze control cpp: iIGaze" << stEgControl.pstEgData->iJGaze << endl;
	}
	/* 	Otherwise, if the gazepoint was not tracked this sample, 				 */
	else
	{
		/* 		Record zeros:																		 */
		stRawEyeGazeData.iIGaze = 0;
		stRawEyeGazeData.iJGaze = 0;
		stRawEyeGazeData.fPupilRadiusMm = 0.0F;
		stRawEyeGazeData.fXEyeballOffsetMm = 0.0F;
		stRawEyeGazeData.fYEyeballOffsetMm = 0.0F;
		stRawEyeGazeData.fFocusRangeOffsetMm = 0.0F;
		stRawEyeGazeData.fFocusRangeImageTime = stEgControl.pstEgData->fFocusRangeImageTime;
	}
	return stRawEyeGazeData;
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

/*
 Return the configuration data defined in the beginning
*/
_stEgControl CEyeGazeControlJNI::getControlData()
{
	return stEgControl;
}

int CEyeGazeControlJNI::egDetectFixtion(_stEgData *rawdata,int size)
{
	int uiStartCameraFieldCount; /* camera field count at which the gaze-      */
	float fGazeDeviationThreshPix; /* gaze deviation threshold (pixels)        */

	int iEyeMotionState=0; /* DETECT FIXATION RETURN VALUE:				 */
						 /*	MOVING				  0						 */
						 /*		   The eye was in motion iMinFixSamples ago       */
						 /*	FIXATING 			  1						 */
						 /*		   The eye was fixating	 iMinFixSamples ago       */
						 /*	FIXATION_COMPLETED    2						 */
						 /*			A completed fixation has just been detected;  */
                         /*           the fixation ended iMinFixSamples ago.           */
	/*Fixation OUTPUT parameters*/
	 
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
	int iFixStartSample;
	stEgControl.fHorzPixPerMm = 10.246F;
	stEgControl.iSamplePerSec = 60;
	/* Set the fixation-detection control parameter arguments.						 */

	/*The minimum number of gaze samples that can be considered as a fixation*/
	iMinFixSamples = int(fMinFixMs * stEgControl.iSamplePerSec / 1000.0F);
	
	/*Distance that a gazepoint may vary from the average fixation point and still be considered as a part of fixation*/
	/*Current value is 65; */
	/*============Will be modified based on the performance===================*/
	fGazeDeviationThreshPix = fGazeDeviationThreshMm *
		stEgControl.fHorzPixPerMm;


	/*Initialize the fixtion detection function*/
	iLastFixCollected = -1;
	InitFixation(iMinFixSamples);

	for (int i = 0; i < size; i++)
	{
		_stEgData dat = rawdata[i];
		/* 	If this is the first data sample in the test run,							 */
		if (i == 0)
		{
			/* 		Save the starting camera field count.										 */
			uiStartCameraFieldCount = dat.ulCameraFieldCount;
		}

		/*- - - - - - - - - - - - Store Gazepoint Trace - - - - - - - - - - - - - - */
		/* 	Record whether the gaze was found.												 */
		stRawGazepoint[i].bGazeTracked = dat.bGazeVectorFound;

		/* 	Originally assume the point is not associated with a fixation. 		 */
		stRawGazepoint[i].iFixIndex = -1;

		/*If the gazepoint was tracked this sample, */
		if (stRawGazepoint[i].bGazeTracked == TRUE)
		{
			/* 		Record the raw gazepoint data:												 */
			stRawGazepoint[i].iXGazeWindowPix = dat.iIGaze - iWindowHorzOffset;
			stRawGazepoint[i].iYGazeWindowPix = dat.iJGaze - iWindowVertOffset;
			stRawGazepoint[i].fPupilDiamMm = dat.fPupilRadiusMm * 2;
			stRawGazepoint[i].fXEyeballMm = dat.fXEyeballOffsetMm;
			stRawGazepoint[i].fYEyeballMm = dat.fYEyeballOffsetMm;
			stRawGazepoint[i].fFocusOffsetMm = dat.fLensExtOffsetMm;
			stRawGazepoint[i].fFocusRangeMm = dat.fFocusRangeOffsetMm;
		}

		/* - - - - - - - - - - - - - Process Fixations - - - - - - - - - - - - - - -*/
		/* 	Check for fixations. 																 */
		iEyeMotionState = DetectFixation(
			stRawGazepoint[i].bGazeTracked,
			(float)stRawGazepoint[i].iXGazeWindowPix,
			(float)stRawGazepoint[i].iYGazeWindowPix,
			fGazeDeviationThreshPix,
			iMinFixSamples,

			&bGazepointFoundDelayed,
			&fXGazeDelayed, &fYGazeDelayed,
			&fGazeDeviationDelayed,

			&fXFixDelayed, &fYFixDelayed,
			&iSaccadeDurationDelayed,
			&iFixDurationDelayed);

		/*  If a completed fixation has just been detected,  */
		if (iEyeMotionState == FIXATION_COMPLETED)
		{
			cout << i << "First detect fixation " << iEyeMotionState << endl;
			/* 	Compute the starting sample for the fixation. */
			iFixStartSample = i - (iFixDurationDelayed - 1)
				- iMinFixSamples;
			/* 	Store the fixation in the temporary fixation holding array. */

			AddFixation(&iLastFixCollected, iFixStartSample,
				fXFixDelayed, fYFixDelayed,
				iSaccadeDurationDelayed, iFixDurationDelayed);
			/* 	Tag all the raw gazepoints associated with this fixation. */
			for (int ii = iFixStartSample;
				ii < iFixStartSample + iFixDurationDelayed; ii++)
			{
				stRawGazepoint[ii].iFixIndex = iLastFixCollected;
				cout << "ii" <<ii << endl;
			}
		}
		/*If the current gaze point is moving,then set the fixation index to -1*/
		if (iEyeMotionState == MOVING) 
		{
			stRawGazepoint[i].iFixIndex = -1;
		}
		iLastSampleCollected = i;
	}

	/* Write the trace data file. 															 */
	WriteTraceDataFile(TEXT("fixation.dat"));
	return iEyeMotionState;
}

void CEyeGazeControlJNI::DownloadFile(const char * pURL, DownloadCallback callback)
{
	cout << "downloading: " << pURL << "" << endl;
	callback(pURL, true);
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
	FILE *fp_fixation_file; 			  /* fixation index data output file					 */
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

	/* - - - - - - - - - - - Write Raw Gazepoint Trace - - - - - - - - - - - - -*/
	/* Write the header data for the columns. 											 */
	/*fwprintf(fp_trace_file,
		TEXT("samp  Eye     Gazepoint  Pupil   Eyeball-Position  Focus   Fix\n")); */
	fwprintf(fp_trace_file,
		TEXT("sampindx  EyeFound   XPos   YPos   PupilDiam   EyeX   EyeY  EyeZ  FoucsRange  FixIndx\n"));
	/*fwprintf(fp_trace_file,
		TEXT("     (t/f)  (pix)  (pix)  (mm)   (mm)  (mm)  (mm)   (mm)      \n\n"));*/

	/* Print the raw data for the gazepoint sample. 									 */
	for (i = 0; i <= iLastSampleCollected; i++)
	{
		fwprintf(fp_trace_file, TEXT("%6i %10i %8i %6i %6.2f %6.1f %5.1f %5.1f %7.1f %4i\n"),
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

	/* Open the trace file for writing. 													 */
	_wfopen_s(&fp_fixation_file, TEXT("fixtionIndex.dat"), TEXT("w"));
	/* - - - - - - - - - - - - Write Fixation Trace - - - - - - - - - - - - - - */
	/* Write the header data for the columns. 											 */
	/* fwprintf(fp_trace_file,
		TEXT(" fix    Fixation     Sac   Fix   Fix \n"));*/
	fwprintf(fp_fixation_file,
		TEXT("fixindx    X     Y     SacDur  FixDur  FixStart\n"));
	/* fwprintf(fp_trace_file,
		TEXT("      (pix)  (pix)  (cnt) (cnt)  Samp\n\n"));*/

	/* Print the fixation data points.														 */
	for (i = 0; i <= iLastFixCollected; i++)
	{
		fwprintf(fp_fixation_file, TEXT("%3i %6i %6i %5i %6i %6i\n"),
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
	/* Close the trace data file. 															 */
	fclose(fp_fixation_file);
}
