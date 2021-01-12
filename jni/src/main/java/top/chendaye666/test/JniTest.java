package top.chendaye666.test;

public class JniTest {
    static {
        System.loadLibrary("JniTest");
    }

    public native void hello();

    /**
     * 传递参数，并且接收返回参数
     * @param bArg
     * @return
     */
    public boolean setArgBoolean(boolean bArg){
        boolean ans = nativeSetArgBoolean(bArg);
        return ans;
    }

    public byte setArgByte(byte bArg){
        return nativeSetArgByte(bArg);
    }

    public char setArgChar(char c){
        return nativeSetArgChar(c);
    }

    public short setArgShort(short s){
        return nativeSetArgShort(s);
    }

    public int setArgInt(int i){
        return nativeSetArgInt(i);
    }

    public double setArgDouble(double d){
        return nativeSetArgDouble(d);
    }

    public float setArgFloat(float f){
        return nativeSetArgFloat(f);
    }

    public long setArgLong(long l){
        return nativeSetArgLong(l);
    }


    public int[] setArgArray(int[] array){
        return nativeSetArgArray(array);
    }
    public int[] setArgArray2(int[] array){
        nativeSetArgArray2(array);
        return array;
    }

    public int[] setArgArray3(int[] array){
        System.out.println("nativeSetArgArray3");
        return nativeSetArgArray3(array);
    }

    /**
     * native 方法 用 private； 调用的时候在封装一层
     * 向底层传递 boolean 型参数，并返回 boolean 型返回值
     * @param bArg
     * @return
     */
    private native boolean nativeSetArgBoolean(boolean bArg);

    /**
     * 向底层传递 byte 型参数，并返回 byte 型返回值
     * @param bArg
     * @return
     */
    private native byte nativeSetArgByte(byte bArg);

    /**
     * 向底层传递 char 型参数，并返回 char 型返回值
     * @param cArg
     * @return
     */
    private native char nativeSetArgChar(char cArg);

    /**
     * 向底层传递 short 型参数，并返回 short 型返回值
     * @param sArg
     * @return
     */
    private native short nativeSetArgShort(short sArg);

    /**
     * 向底层传递 int 型参数，并返回 int 型返回值
     * @param iArg
     * @return
     */
    private native int nativeSetArgInt(int iArg);

    /**
     * 向底层传递 long 型参数，并返回 long 型返回值
     * @param lArg
     * @return
     */
    private native long nativeSetArgLong(long lArg);

    /**
     * 向底层传递 float 型参数，并返回 float 型返回值
     * @param fArg
     * @return
     */
    private native float nativeSetArgFloat(float fArg);

    /**
     * 向底层传递 float 型参数，并返回 float 型返回值
     * @param dArg
     * @return
     */
    private native double nativeSetArgDouble(double dArg);


    /**
     * Java通过JNI传递给C/C++，经过处理后，再复制到Java数组并返回
     * @param array
     * @return
     */
    private native int[] nativeSetArgArray(int[] array);

    /**
     * 在C中直接操作元素，然后把C数组复制到Java数组中，并更新Java数组
     * @param array
     * @return
     */
    private native void nativeSetArgArray2(int[] array);

    /**
     * 在C代码中新建Java数组，然后把C中数组的元素复制到Java数组中在返回给Java；
     * @param array
     */
    private native int[] nativeSetArgArray3(int[] array);



}
