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
    // 节点文件路径
    const char *node_path_point = env->GetStringUTFChars(node_path, nullptr);
    if (node_path_point == nullptr) return nullptr;
    // 查询结果(索引)目录
    const char *result_path_point = env->GetStringUTFChars(result_path, nullptr);
    if (result_path_point == nullptr) return nullptr;

    char *s1 = relationship_path_point;

    //TODO: 社区搜索
    string community = algorithm_neo4j(relationship_path_point, node_path_point, vertex, relationship_path_point, query_k, attr_cnt, algo_type);
    // 删除引用
    env->ReleaseStringUTFChars(relationship_path, relationship_path_point);
    env->ReleaseStringUTFChars(node_path, node_path_point);
    env->ReleaseStringUTFChars(result_path, result_path_point);
    // 自动转为 Unicode
    return env->NewStringUTF(community.data());
  }

int main(int argc, char **argv)
{
    /**
     * todo:
     *  - java读取一个子图， 生成 node-attr  relationship 文件
     *  - jni 调用 equitruss 搜索社区，结果返回给 java调用程序
     *  - java拿到结果，更新数据库（unmanaged or  extention）
     */
    //todo: vertex query_k time result_community_count query_attribute_set communities_set
    string res = algorithm_neo4j(argv[1], argv[2], stoi(argv[3]), argv[4], stoi(argv[5]), stoi(argv[6]), stoi(argv[7]));
    string example = "304565:4:0.045001:1:72,84,92:304565,946299,1647914,1883367#84,92";
    cout << res <<endl;
    return 0;
}
