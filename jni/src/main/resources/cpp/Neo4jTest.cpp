#include "top_chendaye666_neo4j_JniArray.h"
#include <iostream>
#include <stdio.h>


/*
 * 测试字符串
 * Class:     top_chendaye666_neo4j_JniArray
 * Method:    testNativeString
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_top_chendaye666_neo4j_JniArray_testNativeString
  (JNIEnv *env, jobject obj, jstring str){
    // 从java的内存中把字符串拷贝出来  在native使用 Java的编码 Unicode， C++编码 UTF-8
    const char *strFromJava = env->GetStringUTFChars(str, nullptr);
    if (strFromJava == nullptr){
        return nullptr;
    }
    // 定义c++字符串
    char buf[128] = {0};
    strcpy(buf, strFromJava);
    strcat(buf, "拼接字符串！");
    // 删除引用
    env->ReleaseStringUTFChars(str, strFromJava);
    // 自动转为 Unicode
    return env->NewStringUTF(buf);
  }

/*
* Class:     top_chendaye666_neo4j_JniArray
* Method:    testNativeArray
* Signature: ([I)[I
*/
JNIEXPORT jintArray JNICALL Java_top_chendaye666_neo4j_JniArray_testNativeArray
(JNIEnv *env, jobject obj, jintArray array){
  //todo: 一维数组求和
  printf("创建新数组方式，一维数组求和（一）");
  jint sum = 0;
  jint len = env->GetArrayLength(array);
  // 重新申请数组空间
  jint *buf = (jint *)malloc(len*sizeof(jint));
  memset(buf, 0, len*sizeof(jint));
  for(jint i = 0; i < len; i++){
      sum += buf[i];
  }
  // 释放空间
  free(buf);
  printf("一维数组的和%d \n", sum);


  //todo: 直接引用的方式
  printf("直接引用原数组方式，修改");
  //此种方式比较危险,GetIntArrayElements会直接获取数组元素指针,是可以直接对该数组元素进行修改的.
  jint *parr = env->GetIntArrayElements(array, nullptr); // 直接引用原数组
  if (parr == nullptr){
      return nullptr;
  }
  for (int i = 0; i < len; ++i) {
    parr[i] += i;
  }
  //有Get,一般就有Release; 删除引用
  env->ReleaseIntArrayElements(array, parr, 0);
  return array;
}

/*
* Class:     top_chendaye666_neo4j_JniArray
* Method:    testNativeArray2D
* Signature: ([[I)[[I
*/
JNIEXPORT jobjectArray JNICALL Java_top_chendaye666_neo4j_JniArray_testNativeArray2D
(JNIEnv *env, jobject obj, jobjectArray array){
    //todo: 使用二维数组 求和
    printf("使用二维数组 求和");
    jarray arr;
    jint rows = env->GetArrayLength(array);
    printf("rows：%d \n", (int)rows);

    jint sum = 0;
    for(jint i = 0; i < rows; i++){
        // 取内层数组
        arr = (jintArray)env->GetObjectArrayElement(array, i);
        // 内层数组列数
        jint cols = env->GetArrayLength(arr);
        // 每从jvm 里面取数据都要转化一些才能 被 c++使用
        jint* colData = env->GetIntArrayElements((jintArray)arr, nullptr);
        for (jint j = 0; j < cols; j++){
            sum += colData[j];
        }
        // 删除引用
        env->ReleaseIntArrayElements((jintArray)arr, colData, 0);
    }
    printf("二维数组的和：%d \n", sum);

    //todo: 返回二维数组
    printf("返回二维数组");
    jobjectArray arrObj;
    jclass intArrayObj = env->FindClass("[I"); //  [ 表示数组  I 表示 int； [I 表示 int[]
    arrObj = env->NewObjectArray(10*10, intArrayObj, nullptr);
    jint tmp[10];
    for (jint i = 0; i < 10; ++i) {
        jintArray colArr = env->NewIntArray(10);
        for (jint j = 0; j < 10; j++){
            tmp[j] = sum + i + j;
        }
        // 设置一列的值
        env->SetIntArrayRegion(colArr, 0, 10, tmp);
        // 设置二维数组的值
        env->SetObjectArrayElement(arrObj, i, colArr);
        // 删除引用
        env->DeleteLocalRef(colArr);
    }
    return arrObj;
  }


/*
* Class:     top_chendaye666_neo4j_JniArray
* Method:    testNativeArrayListInt
* Signature: (Ljava/util/ArrayList;)Ljava/util/ArrayList;
*/
JNIEXPORT jobject JNICALL Java_top_chendaye666_neo4j_JniArray_testNativeArrayListInt
        (JNIEnv *env, jobject obj, jobject list){
    // 获取class类 通过反射处理类
//    jclass arrayListClass = env->FindClass("java.util.ArrayList");
    jclass arrayListClass = env->GetObjectClass(list);
    // 获取方法
    jmethodID methodSize = env->GetMethodID(arrayListClass, "size", "()I");
    // ArrayList.get(Object)
    jmethodID methodGet = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");
    if (methodGet == nullptr || methodSize == nullptr){
        return nullptr;
    }
    // 调用 ArrayList size 方法
    jint size = env->CallIntMethod(list, methodSize);
    
    // 遍历 list
    for (jint i = 0; i < size; ++i) {
        // 调用 get 方法 获取 ArrayList 里面的 一个对象元素
        jintArray array = (jintArray)env->CallObjectMethod(list, methodGet, i);
        // 把获取到的数组转化一下
        jint *data = env->GetIntArrayElements((jintArray)array, nullptr);
        jint len = env->GetArrayLength(array);
        for (jint j = 0; j < len; ++j) {
            data[j] += 1;
        }
        // 删除引用
        env->ReleaseIntArrayElements((jintArray)array, data, 0);
    }
    return list;

}

