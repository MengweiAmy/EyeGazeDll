/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class eyegaze_jni_EyeGazeJNI */

#ifndef _Included_eyegaze_jni_EyeGazeJNI
#define _Included_eyegaze_jni_EyeGazeJNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     eyegaze_jni_EyeGazeJNI
 * Method:    EyeGazeInit
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_EyeGazeInit
  (JNIEnv *, jobject, jint);

/*
 * Class:     eyegaze_jni_EyeGazeJNI
 * Method:    EyeGazeShutDown
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_EyeGazeShutDown
  (JNIEnv *, jobject);

/*
 * Class:     eyegaze_jni_EyeGazeJNI
 * Method:    Calibrate
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_Calibrate
  (JNIEnv *, jobject);

/*
 * Class:     eyegaze_jni_EyeGazeJNI
 * Method:    getEyeGazeData
 * Signature: ()[Leyegaze/jni/EyeGazeData;
 */
JNIEXPORT jobjectArray JNICALL Java_eyegaze_jni_EyeGazeJNI_getEyeGazeData
  (JNIEnv *, jobject);

/*
 * Class:     eyegaze_jni_EyeGazeJNI
 * Method:    EyeGazeLogStart
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_EyeGazeLogStart
  (JNIEnv *, jobject);

/*
 * Class:     eyegaze_jni_EyeGazeJNI
 * Method:    EyeGazeLogStop
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_eyegaze_jni_EyeGazeJNI_EyeGazeLogStop
  (JNIEnv *, jobject);

/*
 * Class:     eyegaze_jni_EyeGazeJNI
 * Method:    verifyFixtion
 * Signature: (Leyegaze/jni/EyeGazeData;)Leyegaze/jni/FixtionData;
 */
JNIEXPORT jobject JNICALL Java_eyegaze_jni_EyeGazeJNI_verifyFixtion
  (JNIEnv *, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif
