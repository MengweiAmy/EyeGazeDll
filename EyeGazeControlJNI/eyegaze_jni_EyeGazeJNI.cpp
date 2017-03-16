#include "stdafx.h"
#include "eyegaze_jni_EyeGazeJNI.h"
#include "EyeGazeControlJNI.h"
#include <stdio.h>
#include <iostream>

using namespace std;

CEyeGazeControlJNI eyeGaze;

_stEgData stLogGazepoint[5000];

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
* Method:    VerifyFixation
* Signature: ([Leyegaze/jni/EyeGazeData;)I
*/
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_VerifyFixation
(JNIEnv *env, jobject, jobjectArray eye_gazeData)
{
	/*TODO Analysis eyegaze data to get fixtion information
	*/
	cout << "enter jni function successfully......." << endl;
	if (eye_gazeData != NULL)
	{
		int size = env->GetArrayLength(eye_gazeData);
		//cout << "eye_gazeData size:" <<size << endl;
		//int fixtionResult = eyeGaze.egDetectFixtion(stRawGazepoint);
		for (int i = 0; i < size; i++)
		{
			jobject job_egData = (jobject)env->GetObjectArrayElement(eye_gazeData, i);
			if (job_egData != NULL)
			{
				jclass jcls_egdata = env->GetObjectClass(job_egData);
				if (i == 0)
				{
					//jfieldID jfid_cameraFil = env->GetFieldID(jcls_egdata, "cameraFieldCount", "L");
					/* 		Save the starting camera field count.										 */
					//stLogGazepoint[i].ulCameraFieldCount = env->GetIntField(job_egData, jfid_cameraFil);
				}

				/* Get gaze vector found parameter*/
				jfieldID jfid_gazeFound = env->GetFieldID(jcls_egdata, "gazeVectorFound", "I");
				int iGazeFound = env->GetIntField(job_egData, jfid_gazeFound);
				stLogGazepoint[i].bGazeVectorFound = iGazeFound;

				/* Get iIGaze parameter*/
				jfieldID jfid_iXgaze = env->GetFieldID(jcls_egdata, "iIGaze", "I");
				int iXGazePix = env->GetIntField(job_egData, jfid_iXgaze);
				stLogGazepoint[i].iIGaze = iXGazePix;

				/* Get iJGaze parameter*/
				jfieldID jfid_iJgaze = env->GetFieldID(jcls_egdata, "iJGaze", "I");
				int iJGazePix = env->GetIntField(job_egData, jfid_iJgaze);
				stLogGazepoint[i].iJGaze = iJGazePix;

				/* Get pupilRadiusMm parameter*/
				jfieldID jfid_pupilradius = env->GetFieldID(jcls_egdata, "pupilRadiusMm", "F");
				float fpupilradiu = env->GetFloatField(job_egData, jfid_pupilradius);
				stLogGazepoint[i].fPupilRadiusMm = fpupilradiu;

				/* Get fXEyeballOffsetMm parameter*/
				jfieldID jfid_fxEyeballoff = env->GetFieldID(jcls_egdata, "fXEyeballOffsetMm", "F");
				float fXEyeBall = env->GetFloatField(job_egData, jfid_fxEyeballoff);
				stLogGazepoint[i].fXEyeballOffsetMm = fXEyeBall;

				/* Get fYEyeballOffsetMm parameter*/
				jfieldID jfid_fyEyeballoff = env->GetFieldID(jcls_egdata, "fYEyeballOffsetMm", "F");
				float fYEyeBall = env->GetFloatField(job_egData, jfid_fyEyeballoff);
				stLogGazepoint[i].fYEyeballOffsetMm = fYEyeBall;

				/* Get fLengExtOffsetMm parameter*/
				jfieldID jfid_fLengExtOffset = env->GetFieldID(jcls_egdata, "fLengExtOffsetMm", "F");
				float fYLengExt = env->GetFloatField(job_egData, jfid_fLengExtOffset);
				stLogGazepoint[i].fLensExtOffsetMm = fYLengExt;

				/* Get foucsRangeOffsetMm parameter*/
				jfieldID jfid_fLengRangOffset = env->GetFieldID(jcls_egdata, "foucsRangeOffsetMm", "F");
				float fYRangeoff = env->GetFloatField(job_egData, jfid_fLengRangOffset);
				stLogGazepoint[i].fFocusRangeOffsetMm = fYRangeoff;

				//cout << i << "get rawpoint data bGazeVectorFound" << stRawGazepoint[i].bGazeVectorFound << endl;
				//cout << i << "get rawpoint data iIGaze" << stRawGazepoint[i].iIGaze << endl;
				//cout << i << "get rawpoint data iJGaze" << stRawGazepoint[i].iJGaze << endl;
				//cout << i << "get rawpoint data fPupilRadiusMm" << stRawGazepoint[i].fPupilRadiusMm << endl;
				//cout << i << "get rawpoint data fXEyeBall" << stRawGazepoint[i].fXEyeballOffsetMm << endl;
				//cout << i << "get rawpoint data fYEyeBall" << stRawGazepoint[i].fYEyeballOffsetMm << endl;
			}
		}
		int fixtionResult = eyeGaze.egDetectFixtion(stLogGazepoint,size);
		//delete[] stRawGazepoint;
		return fixtionResult;
	}
	return 0;
}

