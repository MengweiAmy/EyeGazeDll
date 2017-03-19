#include "stdafx.h"
#include "eyegaze_jni_EyeGazeJNI.h"
#include "EyeGazeControlJNI.h"
#include <stdio.h>
#include <iostream>

using namespace std;

CEyeGazeControlJNI eyeGaze;

_stEgData stLogGazepoint[5000];
int i = 0;

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
	/*
		retrieve data after calibrate
	*/
	//eyeGaze.startMonitorData();
	return 0;
}


void __stdcall OnDownloadFinished(const char* pURL, bool bOK)
{
	cout << "OnDownloadFinished, URL:" << pURL << "    status:" << bOK << endl;
}

/*
 * Class:     eyegaze_jni_EyeGazeJNI
 * Method:    getEyeGazeData
 * Signature: ()Leyegaze/jni/EyeGazeData;
 */
JNIEXPORT jobject JNICALL Java_eyegaze_jni_EyeGazeJNI_getEyeGazeData
  (JNIEnv *env, jobject) {
	cout << "log from JNI cpp: Try to get data" << endl;
	cout << "log from JNI cpp: Call EyeGaze start monitor function" << endl;
	
	// try to find the object class
	jclass jclseyeData = env->FindClass("eyegaze/jni/EyeGazeData");  

	// try to find the call back class and callback method
	jclass jclsretur_eyeData = env->FindClass("eyegaze/jni/EyeGazeRetrieveData");  
	jmethodID jmgazeData = env->GetStaticMethodID(jclsretur_eyeData, "receivegazedaata", "(Leyegaze/jni/EyeGazeData;)V");
	if (jmgazeData == nullptr) {
		cerr << "ERROR: method it receivegazedaata not found !" << endl;
	}
	else {
		cout << "Class MyTest found" << endl;
		while (i < 36000) {
			jmethodID jeyeData = env->GetMethodID(jclseyeData, "<init>", "()V");
			jfieldID jgazeVectorFound = env->GetFieldID(jclseyeData, "gazeVectorFound", "Z");
			jfieldID jigaze = env->GetFieldID(jclseyeData, "iIGaze", "I");
			jfieldID jjgaze = env->GetFieldID(jclseyeData, "iJGaze", "I");
			jfieldID jpupilRadius = env->GetFieldID(jclseyeData, "pupilRadiusMm", "F");
			jfieldID jfXEyeballOffsetMm = env->GetFieldID(jclseyeData, "fXEyeballOffsetMm", "F");
			jfieldID jfYEyeballOffsetMm = env->GetFieldID(jclseyeData, "fYEyeballOffsetMm", "F");
			jfieldID jfoucsRangeOffsetMm = env->GetFieldID(jclseyeData, "foucsRangeOffsetMm", "F");
			jfieldID jfLengExtOffsetMm = env->GetFieldID(jclseyeData, "fLengExtOffsetMm", "F");

			//_stEgData data = eyeGaze.getEyeData();

			jobject jobjEyeData = env->NewObject(jclseyeData, jeyeData);
			//env->SetIntField(jobjEyeData, jigaze, data.iIGaze);
			//env->SetIntField(jobjEyeData, jjgaze, data.iJGaze);
			//env->SetFloatField(jobjEyeData, jpupilRadius, data.fPupilRadiusMm);
			//env->SetFloatField(jobjEyeData, jfXEyeballOffsetMm, data.fXEyeballOffsetMm);
			//env->SetFloatField(jobjEyeData, jfYEyeballOffsetMm, data.fYEyeballOffsetMm);
			//env->SetFloatField(jobjEyeData, jfoucsRangeOffsetMm, data.fFocusRangeOffsetMm);
			//env->SetFloatField(jobjEyeData, jfLengExtOffsetMm, data.fFocusRangeOffsetMm);

			env->SetIntField(jobjEyeData, jigaze, 100);
			env->SetIntField(jobjEyeData, jjgaze, 500);
			env->SetFloatField(jobjEyeData, jpupilRadius, 3.16F);
			env->SetFloatField(jobjEyeData, jfXEyeballOffsetMm, -1.6F);
			env->SetFloatField(jobjEyeData, jfYEyeballOffsetMm, 11.1F);
			env->SetFloatField(jobjEyeData, jfoucsRangeOffsetMm, 683.3F);
			env->SetFloatField(jobjEyeData, jfLengExtOffsetMm, 0.0F);

			env->CallStaticVoidMethod(jclsretur_eyeData, jmgazeData, jobjEyeData);
			cout << "log from JNI cpp: Return object to Java" << endl;
		}
		cout << endl;
	}
	return NULL;
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

