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

// This class is exported from the EyeGazeControlJNI.dll
class EYEGAZECONTROLJNI_API CEyeGazeControlJNI {
public:
	CEyeGazeControlJNI(void);
	int initDevice();
	int shutDownDevice();
	void calibrateDevice();
	_stEgData* getEyeData();
	int startLogging();
	int stopLogging();
	// TODO: add your methods here.
};

extern EYEGAZECONTROLJNI_API int nEyeGazeControlJNI;

EYEGAZECONTROLJNI_API int fnEyeGazeControlJNI(void);