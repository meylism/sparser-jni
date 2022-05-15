//
// Created by Meylis Matiyev on 2022. 05. 15..
//
#include "com_meylism_sparser_Sparser.h"
#include "jni.h"

JNIEXPORT jlong JNICALL Java_com_meylism_sparser_Sparser_filter
        (JNIEnv *env, jobject obj_java, jstring line_java, jstring predicate_java) {
    const char *line = env->GetStringUTFChars(line_java, 0);
    const char *predicate = env->GetStringUTFChars(predicate_java, 0);

    printf("Line: %s\n", line);
    printf("Predicate: %s\n", predicate);

    env->ReleaseStringUTFChars(line_java, line);
    env->ReleaseStringUTFChars(predicate_java, predicate);
    return 0;
}

JNIEXPORT void JNICALL Java_com_meylism_sparser_Sparser_init
        (JNIEnv *env, jclass class_java) {
          printf("Sparser has been initialized");
}

