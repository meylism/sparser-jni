//
// Created by Meylis Matiyev on 2022. 05. 15..
//
#include "com_meylism_sparser_Sparser.h"
#include "jni.h"
#include "sparser.h"

// Cache for the parse method. As for the JNIEnv pointer, it's only valid in the thread associated
// with it, so it shouldn't be cached.
static JavaVM *jvm;
// static jclass parseMethodClass = NULL;
static jobject parseMethodThis = NULL;
static jmethodID parseMethodId = NULL;


static ascii_rawfilters raw_filters;
static sparser_query_t *query;

struct callback_data {
	long count;
};

static struct callback_data cdata;

void fill_from_javastr(const char *arr[], JNIEnv *env, jobjectArray predicates_java, jsize size) {

  for (int i=0; i<size; i++) {
    jstring str = (jstring) env->GetObjectArrayElement(predicates_java, i);
    arr[i] = env->GetStringUTFChars(str, 0);
  }

}

int parse_callback(const char *line, void *query) {
  SPARSER_DBG("Parser entered\n");
  // SPARSER_DBG("Method id: %s\n", parseMethodClass);
  if (!query) return false;
	struct callback_data *data = (struct callback_data *)query;
  char *newline = (char *)strchr(line, '\n');
  // Last one?
  if (!newline) {
    SPARSER_DBG("last line\n");
    newline = (char *)(line + strlen(line) + 1);
  }
  char tmp = *newline;
  *newline = '\0';

  JNIEnv *env;
  jint status = jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
  SPARSER_DBG("got the env: %i\n", status);
  if (status == JNI_EDETACHED) {
    SPARSER_DBG("the thread is detached: %i\n", status);
    jint jvm_local = jvm->AttachCurrentThread((void **)&env, NULL);
    SPARSER_DBG("attached the thread: %s\n", jvm_local);
  }


  SPARSER_DBG("line: %s\n", line);
  jstring str_java = env->NewStringUTF(line);
  if (str_java == NULL)
    SPARSER_DBG("couldn't allocate string\n");
  SPARSER_DBG("string loc: %d\n", str_java);
  SPARSER_DBG("before call in method\n");
  SPARSER_DBG("Method id: %d\n", parseMethodId);
  SPARSER_DBG("This java : %s\n", parseMethodThis);
	int passed = env->CallIntMethod(parseMethodThis, parseMethodId, str_java);
	
  if (passed) {
		data->count++;
	}
  SPARSER_DBG("Parser done: %i\n", passed);

  env->DeleteLocalRef(str_java);
  // env->DeleteLocalRef(env);
  SPARSER_DBG("after freeing loc: %d\n", str_java);
  *newline = tmp;
	return passed;
}


/*
 * Class:     com_meylism_sparser_Sparser
 * Method:    decompose
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_meylism_sparser_Sparser_decompose
  (JNIEnv *env, jobject this_java, jobjectArray preds_java) {
    SPARSER_DBG("Decompose started\n");
    jsize npreds = env->GetArrayLength(preds_java);
    const char *predicates[npreds];
    fill_from_javastr(predicates, env, preds_java, npreds);

    raw_filters = decompose(predicates, npreds);
    SPARSER_DBG("Decompose done\n");
  }

/*
 * Class:     com_meylism_sparser_Sparser
 * Method:    calibrate
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_meylism_sparser_Sparser_calibrate
  (JNIEnv *env, jobject this_java, jstring text_java) {
    SPARSER_DBG("Calibration started\n");
    jboolean is_copy;
    char *text = (char*) env->GetStringUTFChars(text_java, &is_copy);
    if (is_copy == JNI_FALSE)
      SPARSER_DBG("Direct pointer is got\n");

    //  if (parseMethodClass == NULL) {
    //   jclass parseMethodClassLocal = env->GetObjectClass(this_java);
    //   parseMethodClass = (jclass) env->NewGlobalRef(parseMethodClassLocal);
    //   env->DeleteLocalRef(parseMethodClassLocal);
    //   SPARSER_DBG("getting the class\n");
    // }

    // if (parseMethodThis == NULL) {
      parseMethodThis = env->NewGlobalRef(this_java); 
    // } 
    
    // if (parseMethodId == NULL) {
      jclass parseMethodClassLocal = env->GetObjectClass(this_java);
      jmethodID parseMethodIdLocal = env->GetMethodID((jclass) parseMethodClassLocal, "parse", "(Ljava/lang/String;)I");
      parseMethodId = (jmethodID) env->NewGlobalRef((jobject) parseMethodIdLocal);
      // env->DeleteLocalRef(parseMethodIdLocal);
      env->DeleteLocalRef(parseMethodClassLocal);
      SPARSER_DBG("getting the id: %i\n", parseMethodId);
    // }

    // if (jvm == NULL) {
      jint jmv_local = env->GetJavaVM(&jvm);
      assert (jmv_local == JNI_OK);
      SPARSER_DBG("got the jvm: %i\n", jmv_local);
    // }

    SPARSER_DBG("Method id: %d\n", parseMethodId);
    // SPARSER_DBG("Method id: %s\n", parseMethodClass);

    cdata.count = 0;

    //
    // int passed = env->CallIntMethod(this_java, parseMethodId, "{\"name\":\"Meylis\", \"surname\": \"Matiyev\"}\n{\"name\":\"Elon\", \"surname\": \"Matiyev\"}");
    // SPARSER_DBG("method result: %d\n", passed);

    
    
    query = sparser_calibrate(text, strlen(text), '\n', &raw_filters, parse_callback, &cdata);


    SPARSER_DBG("Calibration done\n");
  }

/*
 * Class:     com_meylism_sparser_Sparser
 * Method:    filter
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_meylism_sparser_Sparser_filter
  (JNIEnv *env, jobject this_java, jstring text_java) {
    SPARSER_DBG("Filter started\n");
    SPARSER_DBG("Method id: %d\n", parseMethodId);
    // SPARSER_DBG("Method id: %s\n", parseMethodClass);
    jboolean is_copy;
    char *text = (char*) env->GetStringUTFChars(text_java, &is_copy);
    jsize text_size = env->GetStringLength(text_java);
    if (is_copy == JNI_FALSE)
      SPARSER_DBG("Direct pointer is got\n");

    sparser_stats_t *stats = sparser_search(text, (long) text_size, '\n', query, parse_callback, &cdata);
    int sparser_passed = stats->sparser_passed;
    SPARSER_DBG("Filter done\n");

    // free

    env->ReleaseStringUTFChars(text_java, text);

    // env->DeleteGlobalRef((jobject) parseMethodClass);
    env->DeleteGlobalRef(parseMethodThis);
    env->DeleteGlobalRef((jobject) parseMethodId);

    free_ascii_rawfilters(&raw_filters);
    free(stats);

    return sparser_passed;
  }

 

// JNIEXPORT jlong JNICALL Java_com_meylism_sparser_Sparser_filter
//         (JNIEnv *env, jobject this_java, jstring line_java, jint line_length_java, 
//         jobjectArray predicates_java, jint nof_predicates_java) {

//     const char *line = env->GetStringUTFChars(line_java, 0);
    
//     jsize npreds = env->GetArrayLength(predicates_java);
//     const char *predicates[npreds];
//     fill_from_javastr(predicates, env, predicates_java, npreds);

//     ascii_rawfilters_t d = decompose(predicates, npreds);

//     std::cout << d.lens;

//     env->ReleaseStringUTFChars(line_java, line);
//     return 0;
// }

// JNIEXPORT void JNICALL Java_com_meylism_sparser_Sparser_init
//         (JNIEnv *env, jclass class_java) {
//           printf("Sparser has been initialized");
// }

