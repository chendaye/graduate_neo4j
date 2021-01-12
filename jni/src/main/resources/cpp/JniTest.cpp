#include "top_chendaye666_test_JniTest.h"
#include <iostream>
#include <stdio.h>

JNIEXPORT void JNICALL Java_top_chendaye666_test_JniTest_hello
(JNIEnv *, jobject) {
    printf("Hello Jin");
}

/*
 * Class:     top_chendaye666_test_JniTest
 * Method:    nativeSetArgBoolean
 * Signature: (Z)Z
 */
JNIEXPORT jboolean JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgBoolean
  (JNIEnv *, jobject obj, jboolean bArg){
      jboolean ret = !bArg; // 对传入的参数 bArg 取反
      return ret;
  }

/*
 * Class:     top_chendaye666_test_JniTest
 * Method:    nativeSetArgByte
 * Signature: (B)B
 */
JNIEXPORT jbyte JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgByte
  (JNIEnv *, jobject obj, jbyte bArg){
      jbyte ret = bArg << 2; // 对传入的 bArg 左移两位（即乘 2 ）
      return ret;
  }

/*
 * Class:     top_chendaye666_test_JniTest
 * Method:    nativeSetArgChar
 * Signature: (C)C
 */
JNIEXPORT jchar JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgChar
  (JNIEnv *, jobject, jchar cArg){
      jchar ret = cArg + 1; // 对传入的 cArg 加 1，即其下一个字符
      return ret;
  }

/*
 * Class:     top_chendaye666_test_JniTest
 * Method:    nativeSetArgShort
 * Signature: (S)S
 */
JNIEXPORT jshort JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgShort
  (JNIEnv *, jobject, jshort sArg){
      jshort ret = sArg + 2; // 对传入的参数 sArg 加 2
      return ret;
  }

/*
 * Class:     top_chendaye666_test_JniTest
 * Method:    nativeSetArgInt
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgInt
  (JNIEnv *, jobject, jint iArg){
        jint ret;
        if (iArg < 10) {// 如果传入的参数 iArg 小于 10，则返回成功，否则返回失败
            ret = 0;
        } else {
            ret = -1;
        }
        return ret;
  }

/*
 * Class:     top_chendaye666_test_JniTest
 * Method:    nativeSetArgLong
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgLong
  (JNIEnv *, jobject, jlong lArg){
      jlong ret = lArg + 1 * 1000 * 1000; // 对传入的 lArg 做运算
      return ret;
  }

/*
 * Class:     top_chendaye666_test_JniTest
 * Method:    nativeSetArgFloat
 * Signature: (F)F
 */
JNIEXPORT jfloat JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgFloat
  (JNIEnv *, jobject, jfloat fArg){
      jfloat ret = fArg + 1.0f; // 对传入的 fArg 做运算
      return ret;
  }

/*
 * Class:     top_chendaye666_test_JniTest
 * Method:    nativeSetArgDouble
 * Signature: (D)D
 */
JNIEXPORT jdouble JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgDouble
  (JNIEnv *, jobject, jdouble dArg){
      jdouble ret = dArg + 2.0; // 对传入的 dArg 做运算
      return ret;
  }


  /*
 * Class:     top_chendaye666_test_JniTest
 * Method:    nativeSetArgArray
 * Signature: ([I)[I
 */
JNIEXPORT jintArray JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgArray
  (JNIEnv *env, jobject, jintArray javaArr){
    //获取Java数组长度
    int length = (*env).GetArrayLength(javaArr);

    //根据Java数组创建C数组，也就是把Java数组转换成C数组
//     jint* (*GetIntArrayElements)(JNIEnv*, jintArray, jboolean*);
    jint* arrp =(*env).GetIntArrayElements(javaArr,0);

    //把数组元素值加10处理
    int i;
    for(i =0 ; i<length;i++){
      *(arrp+i) +=10;
    }
    //将C数组种的元素拷贝到Java数组中
    (*env).SetIntArrayRegion(javaArr,0,length,arrp);

    return javaArr;
  }

  /*
   * Class:     top_chendaye666_test_JniTest
   * Method:    nativeSetArgArray2
   * Signature: ([I)V
   */
  JNIEXPORT void JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgArray2
    (JNIEnv *env, jobject, jintArray javaArr){
        //获取Java数组长度
        int length = (*env).GetArrayLength(javaArr);

        //根据Java数组创建C数组，也就是把Java数组转换成C数组
        // jint* (*GetIntArrayElements)(JNIEnv*, jintArray, jboolean*);
        int* arrp =(*env).GetIntArrayElements(javaArr,0);

        //把数组元素值加10处理
        int i;
        for(i =0 ; i<length;i++){
        *(arrp+i) +=10;
        }
        //将C数组种的元素拷贝到Java数组中
        (*env).SetIntArrayRegion(javaArr,0,length,arrp);
    }

  /*
   * Class:     top_chendaye666_test_JniTest
   * Method:    nativeSetArgArray3
   * Signature: ([I)V
   */
  JNIEXPORT jintArray JNICALL Java_top_chendaye666_test_JniTest_nativeSetArgArray3
    (JNIEnv *env, jobject, jintArray javaArr){
        //获取Java数组长度
        int length = (*env).GetArrayLength(javaArr);

        //根据Java数组创建C数组，也就是把Java数组转换成C数组
        // jint* (*GetIntArrayElements)(JNIEnv*, jintArray, jboolean*);
        int* arrp =(*env).GetIntArrayElements(javaArr,0);
        //新建一个Java数组
        jintArray newArr = (*env).NewIntArray(length);

        //把数组元素值加10处理
        int i;
        for(i =0 ; i<length;i++){
        *(arrp+i) +=10;
        }
        //将C数组种的元素拷贝到Java数组中
        (*env).SetIntArrayRegion(newArr,0,length,arrp);

        return newArr;
    }