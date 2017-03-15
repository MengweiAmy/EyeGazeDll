#include "stdafx.h"
#include "EyeGazeJNI.h"
#include "EyeGazeControlJNI.h"
#include <stdio.h>
#include <iostream>

using namespace std;

CEyeGazeControlJNI eyeGaze;



/*
* Class:     EyeGazeJNI
* Method:    EyeGazeInit
* Signature: (I)I
*/
JNIEXPORT jint JNICALL Java_EyeGazeJNI_EyeGazeInit
(JNIEnv *env, jobject, jint controltype)
{
	jint result = eyeGaze.initDevice();
	return result;
}

/*
* Class:     EyeGazeJNI
* Method:    EyeGazeShutDown
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_EyeGazeJNI_EyeGazeShutDown
(JNIEnv *env, jobject) {
	jint result = eyeGaze.shutDownDevice();
	return result;
}

/*
* Class:     EyeGazeJNI
* Method:    Calibrate
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_EyeGazeJNI_Calibrate
(JNIEnv *env, jobject) {
	eyeGaze.calibrateDevice();
	return 0;
}

/*
* Class:     EyeGazeJNI
* Method:    getEyeGazeData
* Signature: ()Ldata/EyeGazeData;
*/
JNIEXPORT jobject JNICALL Java_EyeGazeJNI_getEyeGazeData
(JNIEnv *env, jobject obj) {

	jclass eyeData = env->FindClass("data/EyeGazeData");
	_stEgData data= eyeGaze.getEyeData();
	cout << data.bGazeVectorFound << endl;

	jmethodID jeyeData = env->GetMethodID(eyeData, "<init>", "()V");
	jfieldID jgazeVectorFound = env->GetFieldID(eyeData, "gazeVectorFound", "Z");
	jfieldID jigaze = env->GetFieldID(eyeData, "iIGaze", "I");
	jfieldID jjgaze = env->GetFieldID(eyeData, "iJGaze", "I");
	jfieldID jpupilRadius = env->GetFieldID(eyeData, "pupilRadiusMm", "F");
	jfieldID jfXEyeballOffsetMm = env->GetFieldID(eyeData, "fXEyeballOffsetMm", "F");
	jfieldID jfYEyeballOffsetMm = env->GetFieldID(eyeData, "fYEyeballOffsetMm", "F");
	jfieldID jfoucsRangeOffsetMm = env->GetFieldID(eyeData, "foucsRangeOffsetMm", "F");
	jfieldID jfLengExtOffsetMm = env->GetFieldID(eyeData, "fLengExtOffsetMm", "F");

	jobject jobjEyeData = env->NewObject(eyeData, jeyeData);
	env->SetIntField(jobjEyeData, jigaze, data.iIGaze);
	env->SetIntField(jobjEyeData, jjgaze, data.iJGaze);
	env->SetFloatField(jobjEyeData, jpupilRadius, data.fPupilRadiusMm);
	env->SetFloatField(jobjEyeData, jfXEyeballOffsetMm, data.fXEyeballOffsetMm);
	env->SetFloatField(jobjEyeData, jfYEyeballOffsetMm, data.fYEyeballOffsetMm);
	env->SetFloatField(jobjEyeData, jfoucsRangeOffsetMm, data.fFocusRangeOffsetMm);
	env->SetFloatField(jobjEyeData, jfLengExtOffsetMm, data.fLensExtOffsetMm);

	return jobjEyeData;
}

/*
* Class:     EyeGazeJNI
* Method:    EyeGazeLogStart
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_EyeGazeJNI_EyeGazeLogStart
(JNIEnv *env, jobject) {
	jint re = eyeGaze.startLogging();
	return re;
}

/*
* Class:     EyeGazeJNI
* Method:    EyeGazeLogStop
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_EyeGazeJNI_EyeGazeLogStop
(JNIEnv *env, jobject) {
	jint re = eyeGaze.stopLogging();
	return re;
}
