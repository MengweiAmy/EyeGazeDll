// EyeGazeControlJNI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EyeGazeControlJNI.h"
#include "EgWin.h"
#include <windows.h>
#include <iostream>
#include <time.h>
#include <string>

using namespace std;

#define EG_BUFFER_LeN  60;
#define	MAX_GAZEPOINTS 36000

static struct _stEgControl stEgControl;
struct _stEgData stEgData[MAX_GAZEPOINTS];

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
	//Allow the application program to get up to 1 second behind
	stEgControl.iNDataSetsInRingBuffer = EG_BUFFER_LeN;
	stEgControl.bTrackingActive = FALSE;
	//stEgControl.iScreenWidthPix = iScreenWidthPix;
	//stEgControl.iScreenHeightPix = iScreenheightPix;
	stEgControl.iVisionSelect = 0;
	stEgControl.iCommType = EG_COMM_TYPE_LOCAL;
	int result = EgInit(&stEgControl);
	stEgControl.bTrackingActive = TRUE;
	return result;
}

int CEyeGazeControlJNI::shutDownDevice()
{
	int result = EgExit(&stEgControl);
	return result;
}

void CEyeGazeControlJNI::calibrateDevice()
{
	EgCalibrate2(&stEgControl, EG_CALIBRATE_NONDISABILITY_APP);
	return;
}

_stEgData* CEyeGazeControlJNI::getEyeData()
{
	while (stEgControl.bTrackingActive) {
		for (int i = 0; i < MAX_GAZEPOINTS; i++) {
			stEgData[i].bGazeVectorFound = stEgControl.pstEgData->bGazeVectorFound;
			stEgData[i].dAppMarkTimeSec = stEgControl.pstEgData->dAppMarkTimeSec;
			stEgData[i].dGazeTimeSec = stEgControl.pstEgData->dGazeTimeSec;
			stEgData[i].dReportTimeSec = stEgControl.pstEgData->dReportTimeSec;
			stEgData[i].fFocusRangeImageTime = stEgControl.pstEgData->fFocusRangeImageTime;
			stEgData[i].fFocusRangeOffsetMm = stEgControl.pstEgData->fFocusRangeOffsetMm;
			stEgData[i].fLensExtOffsetMm = stEgControl.pstEgData->fLensExtOffsetMm;
			stEgData[i].fPupilRadiusMm = stEgControl.pstEgData->fPupilRadiusMm;
		}
	}
	return stEgData;
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
	int openFile = EgLogFileOpen(&stEgControl, "trace.bat", "a");
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
