#include "stdafx.h"
#include "eyegaze_jni_EyeGazeJNI.h"
#include "EyeGazeControlJNI.h"
#include <stdio.h>
#include <iostream>

using namespace std;

CEyeGazeControlJNI eyeGaze;

float fMinFixtionMs = 100.0F;
float fGazeDeviationThreshPix;
int   iMinFixSamples;
float fGazeDeviatThreshPix = 6.35F;

int bGazepointFoundDelayed;

/*
* Class:     eyegaze_jni_EyeGazeJNI
* Method:    EyeGazeInit
* Signature: (I)I
*/
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_EyeGazeInit
(JNIEnv *env, jobject, jint controltype)
{
	jint result = eyeGaze.initDevice();
	return result;
}

/*
* Class:     eyegaze_jni_EyeGazeJNI
* Method:    EyeGazeShutDown
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_EyeGazeShutDown
(JNIEnv *env, jobject)
{
	jint result = eyeGaze.shutDownDevice();
	return result;
}

/*
* Class:     eyegaze_jni_EyeGazeJNI
* Method:    Calibrate
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_Calibrate
(JNIEnv *, jobject)
{
	eyeGaze.calibrateDevice();
	return 0;
}

/*
* Class:     eyegaze_jni_EyeGazeJNI
* Method:    getEyeGazeData
* Signature: ()[Leyegaze/jni/EyeGazeData;
*/
JNIEXPORT jobjectArray JNICALL Java_eyegaze_jni_EyeGazeJNI_getEyeGazeData
(JNIEnv *env, jobject)
{
	int num = 36000;
	jclass eyeData = env->FindClass("eyegaze/jni/EyeGazeData");
	_stRawGazepoint* data = eyeGaze.getEyeData();

	jmethodID jeyeData = env->GetMethodID(eyeData, "<init>", "()V");
	jfieldID jgazeVectorFound = env->GetFieldID(eyeData, "gazeVectorFound", "Z");
	jfieldID jigaze = env->GetFieldID(eyeData, "iIGaze", "I");
	jfieldID jjgaze = env->GetFieldID(eyeData, "iJGaze", "I");
	jfieldID jpupilRadius = env->GetFieldID(eyeData, "pupilRadiusMm", "F");
	jfieldID jfXEyeballOffsetMm = env->GetFieldID(eyeData, "fXEyeballOffsetMm", "F");
	jfieldID jfYEyeballOffsetMm = env->GetFieldID(eyeData, "fYEyeballOffsetMm", "F");
	jfieldID jfoucsRangeOffsetMm = env->GetFieldID(eyeData, "foucsRangeOffsetMm", "F");
	jfieldID jfLengExtOffsetMm = env->GetFieldID(eyeData, "fLengExtOffsetMm", "F");
	jobjectArray jobjectEyeData = env->NewObjectArray(num, eyeData, 0);

	int arraySize = sizeof(data) / sizeof(data[0]);
	for (int i = 0; i < arraySize; i++)
	{
		jobject jobjEyeData = env->NewObject(eyeData, jeyeData);
		env->SetIntField(jobjEyeData, jigaze, data[i].iXGazeWindowPix);
		env->SetIntField(jobjEyeData, jjgaze,data[i].iYGazeWindowPix);
		env->SetFloatField(jobjEyeData, jpupilRadius,data[i].fPupilDiamMm);
		env->SetFloatField(jobjEyeData, jfXEyeballOffsetMm, data[i].fXEyeballMm);
		env->SetFloatField(jobjEyeData, jfYEyeballOffsetMm, data[i].fYEyeballMm);
		env->SetFloatField(jobjEyeData, jfoucsRangeOffsetMm, data[i].fFocusRangeMm);
		env->SetFloatField(jobjEyeData, jfLengExtOffsetMm, data[i].fFocusOffsetMm);

		env->SetObjectArrayElement(jobjectEyeData, i, jobjEyeData);
		env->DeleteLocalRef(jobjEyeData);
	}

	return jobjectEyeData;
}

/*
* Class:     eyegaze_jni_EyeGazeJNI
* Method:    EyeGazeLogStart
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_EyeGazeLogStart
(JNIEnv *env, jobject)
{
	jint re = eyeGaze.startLogging();
	return re;
}


/*
* Class:     eyegaze_jni_EyeGazeJNI
* Method:    EyeGazeLogStop
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_EyeGazeLogStop
(JNIEnv *env, jobject)
{
	jint re = eyeGaze.stopLogging();
	return re;
}

/*
* Class:     eyegaze_jni_EyeGazeJNI
* Method:    verifyFixtion
* Signature: (Leyegaze/jni/EyeGazeData;)Leyegaze/jni/FixtionData;
*/
JNIEXPORT jobject JNICALL Java_eyegaze_jni_EyeGazeJNI_verifyFixtion
(JNIEnv *env, jobject, jobject eg_data) {

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

	/* Set the fixation-detection control parameter arguments.						 */
	iMinFixSamples = int(fMinFixtionMs * eyeGaze.getControlData().iSamplePerSec / 1000.0F);
	cout << "iMinFixSamples," << iMinFixSamples << endl;
	fGazeDeviatThreshPix = fGazeDeviatThreshPix *
		eyeGaze.getControlData().fHorzPixPerMm;
	cout << "fGazeDeviationThreshPix" << fGazeDeviationThreshPix << endl;

	jclass jclseg_data = env->GetObjectClass(eg_data);

	jfieldID jbGazeTracked = env->GetFieldID(jclseg_data, "gazeVectorFound", "I");
	jint iGazeTracked = env->GetIntField(eg_data, jbGazeTracked);

	jfieldID jixGaze = env->GetFieldID(jclseg_data, "iIGaze", "I");
	jint iXGaze = env->GetIntField(eg_data, jixGaze);

	jfieldID jiyGaze = env->GetFieldID(jclseg_data, "iJGaze", "I");
	jint iYGaze = env->GetIntField(eg_data, jiyGaze);

	/* 		Keep the DetectFixation() function up to date.							 */
	int iEyeMotionState = eyeGaze.egDetectFixtion(
		iGazeTracked,
		(float)iXGaze,
		(float)iYGaze,
		fGazeDeviationThreshPix,
		iMinFixSamples,&bGazepointFoundDelayed,
		&fXGazeDelayed, &fYGazeDelayed,
		&fGazeDeviationDelayed,

		&fXFixDelayed, &fYFixDelayed,
		&iSaccadeDurationDelayed,
		&iFixDurationDelayed);

	jclass fixData = env->FindClass("eyegaze/jni/FixtionData");

	jmethodID jOFixData = env->GetMethodID(fixData, "<init>", "()V");
	jfieldID jgazeVectorFound = env->GetFieldID(fixData, "gazePointFoundDelayed", "I");
	jfieldID jigaze = env->GetFieldID(fixData, "xGazeDelayed", "F");
	jfieldID jjgaze = env->GetFieldID(fixData, "yGazeDelayed", "F");
	jfieldID jgazeDeviationDel = env->GetFieldID(fixData, "gazeDeviationDelayed", "F");
	jfieldID jxFixDelayed = env->GetFieldID(fixData, "xFixDelayed", "F");
	jfieldID jyFixDelayed = env->GetFieldID(fixData, "yFixDelayed", "F");
	jfieldID jiSaccadeDurationDelayed = env->GetFieldID(fixData, "iSaccadeDurationDelayed", "I");
	jfieldID jiFixDurationDelayed = env->GetFieldID(fixData, "iFixDurationDelayed", "I");

	jobject jobjFixData = env->NewObject(fixData, jOFixData);
	env->SetIntField(jobjFixData, jgazeVectorFound, bGazepointFoundDelayed);
	env->SetFloatField(jobjFixData, jigaze, fXGazeDelayed);
	env->SetFloatField(jobjFixData, jjgaze, fYGazeDelayed);
	env->SetFloatField(jobjFixData, jgazeDeviationDel, fGazeDeviationDelayed);
	env->SetFloatField(jobjFixData, jxFixDelayed, fXFixDelayed);
	env->SetFloatField(jobjFixData, jyFixDelayed, fYFixDelayed);
	env->SetIntField(jobjFixData, jiSaccadeDurationDelayed, iSaccadeDurationDelayed);
	env->SetIntField(jobjFixData, jiFixDurationDelayed, iFixDurationDelayed);

	return jobjFixData;
}

