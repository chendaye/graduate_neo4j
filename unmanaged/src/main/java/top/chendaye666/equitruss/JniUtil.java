package top.chendaye666.equitruss;

/**
 * 调用社区搜索算法
 */
public class JniUtil {
    static {
        System.loadLibrary("Equitruss");
    }

    public String query(String relationshipPath, String nodePath, int vertexId,
                        String resultPath, int queryK, int attrCnt, int algoType){
        return getNativeQuery(relationshipPath, nodePath, vertexId, resultPath, queryK, attrCnt, algoType);
    }

    /**
     * 调用社区搜索算法，查找属性和结构都稠密的社区
     * @param relationshipPath
     * @param nodePath
     * @param vertexId
     * @param resultPath
     * @param queryK
     * @param attrCnt
     * @param algoType
     * @return
     */
    private native String getNativeQuery(String relationshipPath, String nodePath, int vertexId,
                                         String resultPath, int queryK, int attrCnt, int algoType);


}
