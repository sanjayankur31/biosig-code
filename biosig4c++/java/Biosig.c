#include <stdio.h>
#include <biosig.h>
#include "Biosig.h"
/* Header for class Biosig */

#ifndef _Included_biosig
#define _Included_biosig
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     Biosig
 * Method:    hdr2ascii
 * Signature: ()V
 */
JNIEXPORT jstring JNICALL Java_Biosig_hdr2ascii (JNIEnv *env, jobject, jstring filename) {
     HDR_STRUCT *hdr = sopen((const char*)env->GetStringUTFChars(filename, 0),"r", NULL);
     char *json=NULL;
     asprintf_hdr2json(&json,hdr);
     sclose(hdr);
     destructHDR(hdr);
     printf("hdr2ascii: Hello World! \n%s\n", json);
     return env->NewStringUTF(json);
}

/*
 * Class:     Biosig
 * Method:    isTTY
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_Biosig_isTTY (JNIEnv *env, jclass) {
     printf("isTTY: Hello World!\n");

}
/*
 * Class:     Biosig
 * Method:    getTTYName
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_Biosig_getTTYName (JNIEnv *env, jclass) {
     printf("getTTYName: Hello World!\n");
}

#ifdef __cplusplus
}
#endif
#endif
