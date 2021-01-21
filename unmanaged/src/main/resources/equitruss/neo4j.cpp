#include "algo.h"
#include "top_chendaye666_equitruss_JniUtil.h"
using namespace std;

/*
 * Class:     top_chendaye666_equitruss_JniUtil
 * Method:    getNativeQuery
 * Signature: (Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;III)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_top_chendaye666_equitruss_JniUtil_getNativeQuery
  (JNIEnv *env, jobject obj, jstring relationship_path, jstring node_path, jint vertex,
   jstring result_path, jint query_k, jint attr_cnt, jint algo_type){
    // 边文件路径
    const char *relationship_path_point = env->GetStringUTFChars(relationship_path, nullptr);
    if (relationship_path_point == nullptr) return nullptr;
    char relationship[128] = {0}; // 复制字符串
    strcpy(relationship, relationship_path_point);
    // 节点文件路径
    const char *node_path_point = env->GetStringUTFChars(node_path, nullptr);
    if (node_path_point == nullptr) return nullptr;
    char node[128] = {0};
    strcpy(node, node_path_point);
    // 查询结果(索引)目录
    const char *result_path_point = env->GetStringUTFChars(result_path, nullptr);
    if (result_path_point == nullptr) return nullptr;
    char result[128] = {0};
    strcpy(result, result_path_point);

    //TODO: 社区搜索
    string community = algorithm_neo4j(relationship, node, vertex, result, query_k, attr_cnt, algo_type);
    // 删除引用
    env->ReleaseStringUTFChars(relationship_path, relationship_path_point);
    env->ReleaseStringUTFChars(node_path, node_path_point);
    env->ReleaseStringUTFChars(result_path, result_path_point);
    // 自动转为 Unicode
    return env->NewStringUTF(community.data());
}