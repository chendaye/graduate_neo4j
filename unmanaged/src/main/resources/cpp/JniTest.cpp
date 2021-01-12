#include "top_chendaye666_httpjni_JniUtil.h"
#include <iostream>
#include <stdio.h>


/*
 * Class:     top_chendaye666_jni_JniUtil
 * Method:    getNativeNum
 * Signature: (I)I
 */
  JNIEXPORT jint JNICALL Java_top_chendaye666_httpjni_JniUtil_getNativeNum
    (JNIEnv *, jobject, jint nums){
        return nums + 1;
      }
