#include "stdafx.h"
#include "eyegaze_jni_EyeGazeJNI.h"
#include "EyeGazeControlJNI.h"
#include <stdio.h>
#include <iostream>

#define _CRT_SECURE_NO_DEPRECATE

using namespace std;

CEyeGazeControlJNI eyeGaze;

_stEgData stLogGazepoint[36000];
_stClickData stClickData[2000];
int i = 0;// calulate the size of eyegaze data when receive gaze data

//The sign that stop call EgGetData function
bool isStopCollectData = FALSE;

void jstringTostring(JNIEnv* env, jstring jstr, string &strData)
{
	char* cArray = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		cArray = (char*)malloc(alen + 1);
		memcpy(cArray, ba, alen);
		cArray[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	strData = cArray;
	if (alen > 0)
	{
		free(cArray);
	}
	env->DeleteLocalRef(strencode);
	env->DeleteLocalRef(clsstring);
}

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
	//delete[] stLogGazepoint;
	//delete[] stClickData;
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
		while (i < 36000) {

			jmethodID jeyeData = env->GetMethodID(jclseyeData, "<init>", "()V");
			jfieldID jgazeVectorFound = env->GetFieldID(jclseyeData, "gazeVectorFound", "I");
			jfieldID jigaze = env->GetFieldID(jclseyeData, "iIGaze", "I");
			jfieldID jjgaze = env->GetFieldID(jclseyeData, "iJGaze", "I");
			jfieldID jpupilRadius = env->GetFieldID(jclseyeData, "pupilRadiusMm", "F");
			jfieldID jfXEyeballOffsetMm = env->GetFieldID(jclseyeData, "fXEyeballOffsetMm", "F");
			jfieldID jfYEyeballOffsetMm = env->GetFieldID(jclseyeData, "fYEyeballOffsetMm", "F");
			jfieldID jfoucsRangeOffsetMm = env->GetFieldID(jclseyeData, "foucsRangeOffsetMm", "F");
			jfieldID jfLengExtOffsetMm = env->GetFieldID(jclseyeData, "fLengExtOffsetMm", "F");
			jfieldID jdgazeTime = env->GetFieldID(jclseyeData, "gazeTimeSec", "D");
			jfieldID jdReportTime = env->GetFieldID(jclseyeData, "reportTimeSec", "D");

			_stEgData data = eyeGaze.getEyeData();

			jobject jobjEyeData = env->NewObject(jclseyeData, jeyeData);
			env->SetIntField(jobjEyeData, jgazeVectorFound, data.bGazeVectorFound);
			env->SetIntField(jobjEyeData, jigaze, data.iIGaze);
			env->SetIntField(jobjEyeData, jjgaze, data.iJGaze);
			env->SetFloatField(jobjEyeData, jpupilRadius, data.fPupilRadiusMm);
			env->SetFloatField(jobjEyeData, jfXEyeballOffsetMm, data.fXEyeballOffsetMm);
			env->SetFloatField(jobjEyeData, jfYEyeballOffsetMm, data.fYEyeballOffsetMm);
			env->SetFloatField(jobjEyeData, jfoucsRangeOffsetMm, data.fFocusRangeOffsetMm);
			env->SetFloatField(jobjEyeData, jfLengExtOffsetMm, data.fLensExtOffsetMm);
			env->SetDoubleField(jobjEyeData, jdgazeTime, data.dGazeTimeSec);
			env->SetDoubleField(jobjEyeData, jdReportTime, data.dReportTimeSec);

			env->CallStaticVoidMethod(jclsretur_eyeData, jmgazeData, jobjEyeData);
			if (isStopCollectData == TRUE)
			{
				cout << "log from JNI cpp: Data collection stopped" << endl;
				break;
			}
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

				jfieldID jfid_dgazetime = env->GetFieldID(jcls_egdata, "gazeTimeSec", "D");
				double dgazeTime = env->GetDoubleField(job_egData, jfid_dgazetime);
				stLogGazepoint[i].dGazeTimeSec = dgazeTime;

				jfieldID jfid_dreportTime = env->GetFieldID(jcls_egdata, "reportTimeSec", "D");
				double dreportTime = env->GetDoubleField(job_egData, jfid_dreportTime);
				stLogGazepoint[i].dReportTimeSec = dreportTime;
			}
		}
		int fixtionResult = eyeGaze.egDetectFixtion(stLogGazepoint,size);
		//delete[] stRawGazepoint;
		return fixtionResult;
	}
	return 0;
}

/*
* Class:     eyegaze_jni_EyeGazeJNI
* Method:    StopDataCollection
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_eyegaze_jni_EyeGazeJNI_StopDataCollection
(JNIEnv *env, jobject)
{
	isStopCollectData = TRUE;
}

/*
* Class:     eyegaze_jni_EyeGazeJNI
* Method:    EyeGazeImageDisplay
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_eyegaze_jni_EyeGazeJNI_EyeGazeImageDisplay
(JNIEnv *env, jobject)
{
	eyeGaze.egDisplayEyeImages();
}

/*
* Class:     eyegaze_jni_EyeGazeJNI
* Method:    WriteClickLog
* Signature: ([Leyegaze/jni/BtnData;)V
*/
JNIEXPORT void JNICALL Java_eyegaze_jni_EyeGazeJNI_WriteClickLog
(JNIEnv *env, jobject, jobjectArray click_arr)
{
	cout << "enter jni function successfully......." << endl;
	if (click_arr != NULL)
	{
		int size = env->GetArrayLength(click_arr);
		cout << "eye_gazeData size:" <<size << endl;
		//int fixtionResult = eyeGaze.egDetectFixtion(stRawGazepoint);
		for (int i = 0; i < size; i++)
		{
			jobject job_clickData = (jobject)env->GetObjectArrayElement(click_arr, i);
			if (job_clickData != NULL)
			{
				jclass jcls_clickData = env->GetObjectClass(job_clickData);

				stClickData[i].index = i;

				/* Get current time in millsecond parameter*/
				jfieldID jfid_time = env->GetFieldID(jcls_clickData, "time", "L");
				long time = env->GetLongField(job_clickData, jfid_time);
				stClickData[i].lTime = time;
				//cout << "time " << time << endl;

				/* Get letter of button parameter*/
				jfieldID jfidlatter = env->GetFieldID(jcls_clickData, "letter", "Ljava/lang/String;");
				jstring s_latter = (jstring)env->GetObjectField(job_clickData, jfidlatter);
				string strData;

				//convert the jstring to string, and then convert to char*
				if (s_latter != NULL)
				{
					int nLen = 0;
					jstringTostring(env, s_latter, strData);
				}
				stClickData[i].sLetter = strData;
				cout << "latter " << strData << endl;

				/* Get the click time in second parameter*/
				jfieldID jfid_seco = env->GetFieldID(jcls_clickData, "seconds", "F");
				float second = env->GetFloatField(job_clickData, jfid_seco);
				stClickData[i].fSecond = second;
				//cout << "second " << second << endl;

				/* Get button xposition parameter*/
				jfieldID jfid_xPos = env->GetFieldID(jcls_clickData, "xPosition", "I");
				jint xposi = env->GetIntField(job_clickData, jfid_xPos);
				stClickData[i].iXPosition = xposi;
				//cout << "iXPosition " << xposi << endl;

				/* Get button yposition parameter*/
				jfieldID jfid_yPos = env->GetFieldID(jcls_clickData, "yPosition", "I");
				jint yposi = env->GetIntField(job_clickData, jfid_yPos);
				stClickData[i].iYPosition = yposi;
				//cout << "iYPosition " << yposi << endl;
			}
		}
		eyeGaze.writeClickLog(stClickData, size);
	}
}

