/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_meylism_sparser_Sparser */

#ifndef _Included_com_meylism_sparser_Sparser
#define _Included_com_meylism_sparser_Sparser
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_meylism_sparser_Sparser
 * Method:    decompose
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_meylism_sparser_Sparser_decompose
  (JNIEnv *, jobject, jobjectArray);

/*
 * Class:     com_meylism_sparser_Sparser
 * Method:    calibrate
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_meylism_sparser_Sparser_calibrate
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_meylism_sparser_Sparser
 * Method:    filter
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_meylism_sparser_Sparser_filter
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
