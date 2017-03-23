// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EYEGAZECONTROLJNI_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EYEGAZECONTROLJNI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#include "EgWin.h"

#ifdef EYEGAZECONTROLJNI_EXPORTS
#define EYEGAZECONTROLJNI_API __declspec(dllexport)
#else
#define EYEGAZECONTROLJNI_API __declspec(dllimport)
#endif

typedef void(__stdcall *DownloadCallback)(const char* pURL, bool bOK);

struct _stRawGazepoint        /* data for a single raw gazepoint, i.e. the  */
							  /*   gazepoint for a single camera sample 	 */
{
	int	bGazeTracked;			/* gaze-tracked flag (true or false)			 */
	int	iXGazeWindowPix;		/* gaze coordinate within window (pixels) 	 */
	int	iYGazeWindowPix;		/*  "                                         */
	float fPupilDiamMm;        /* pupil diameter (mm)                        */
	float fXEyeballMm;         /* x and y eyeball offsets with respect to    */
	float fYEyeballMm;         /*   the camera axis (mm)                     */
							   /*   x positive: eye moved to user's right    */
							   /*   y positive: eye moved up 					 */
	float fFocusOffsetMm;      /* distance of eye from focus plane (mm)      */
							   /*   positive: eye farther from camera 		 */
	float fFocusRangeMm;       /* range from the camera-sensor plane to      */
							   /*   the focus plane (mm)							 */
	int   iFixIndex;           /* index of the fixation that this point is   */
							   /*   in, -1 if not part of a fixation			 */
};

// This class is exported from the EyeGazeControlJNI.dll
class EYEGAZECONTROLJNI_API CEyeGazeControlJNI {
public:
	CEyeGazeControlJNI(void);
	int initDevice();
	int shutDownDevice();
	void calibrateDevice();
	void startMonitorData();
	_stEgData getEyeData();
	int startLogging();
	int stopLogging();
	_stEgControl getControlData();
	int egDetectFixtion(_stEgData *rawdata,int size);
	int egDisplayEyeImages();
	// TODO: add your methods here.
private:
	void AddFixation(int *iLastFixCollected, int iFixStartSample,
		float fXFix, float fYFix,
		int iSaccadeDuration, int iFixDuration);
	void WriteTraceDataFile(wchar_t *pchTraceDataFileName);
};

extern EYEGAZECONTROLJNI_API int nEyeGazeControlJNI;

EYEGAZECONTROLJNI_API int fnEyeGazeControlJNI(void);