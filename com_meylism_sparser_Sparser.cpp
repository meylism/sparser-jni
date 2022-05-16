//
// Created by Meylis Matiyev on 2022. 05. 15..
//
#include "com_meylism_sparser_Sparser.h"
#include "jni.h"
#include "sparser.h"
#include <iostream>

void fill_from_javastr(const char *arr[], JNIEnv *env, jobjectArray predicates_java, jsize size) {

  for (int i=0; i<size; i++) {
    jstring str = (jstring) env->GetObjectArrayElement(predicates_java, i);
    arr[i] = env->GetStringUTFChars(str, 0);
  }

}
 

JNIEXPORT jlong JNICALL Java_com_meylism_sparser_Sparser_filter
        (JNIEnv *env, jobject this_java, jstring line_java, jint line_length_java, 
        jobjectArray predicates_java, jint nof_predicates_java) {

    const char *line = env->GetStringUTFChars(line_java, 0);
    
    jsize npreds = env->GetArrayLength(predicates_java);
    const char *predicates[npreds];
    fill_from_javastr(predicates, env, predicates_java, npreds);

    ascii_rawfilters_t d = decompose(predicates, npreds);

    std::cout << d.lens;

    env->ReleaseStringUTFChars(line_java, line);
    return 0;
}

JNIEXPORT void JNICALL Java_com_meylism_sparser_Sparser_init
        (JNIEnv *env, jclass class_java) {
          printf("Sparser has been initialized");
}

