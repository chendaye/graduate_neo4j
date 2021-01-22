#include "algo.h"
using namespace std;

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
