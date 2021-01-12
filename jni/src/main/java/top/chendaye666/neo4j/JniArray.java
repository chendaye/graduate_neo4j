package top.chendaye666.neo4j;

import java.util.ArrayList;

/**
 * 测试数组
 */
public class JniArray {
    static {
        System.loadLibrary("Neo4jTest");
    }

    /**
     * 测试字符串
     * @param str
     * @return
     */
    public String testString(String str){
        return testNativeString(str);
    }

    /**
     * 测试一维数组
     * @param array
     * @return
     */
    public int[] testArray(int[] array){
        return testNativeArray(array);
    }

    /**
     * 测试ArrayList
     * @param list
     * @return
     */
    public ArrayList<int[]> testArrayListInt(ArrayList<int[]> list){
        return testNativeArrayListInt(list);
    }

    /**
     * 测试二维数组+
     * @param array
     * @return
     */
    public int[][] testArray2D(int[][] array){
        return testNativeArray2D(array);
    }


    private native String testNativeString(String str);
    private native int[] testNativeArray(int[] array);
    private native int[][] testNativeArray2D(int[][] array);
    private native ArrayList<int[]> testNativeArrayListInt(ArrayList<int[]> list);
}
