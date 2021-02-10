package top.chendaye666.simrank;

/**
 * 中心度算法
 * /Users/lengo/Library/Java/Extensions
 */
public class JniUtil {
    static {
        System.loadLibrary("Simrank");
    }

    public String query(String configPath, int nodeId){
        return getRank(configPath, nodeId);
    }


    private native String getRank(String configPath, int nodeId);


}
