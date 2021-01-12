package top.chendaye666.httpjni;

public class JniUtil {
    static {
        System.loadLibrary("Unmanaged");
    }
    public int getNum(int n){
        return getNativeNum(n);
    }
    private native int getNativeNum(int n);

    public static void main(String[] args) {
        JniUtil jniUtil = new JniUtil();

        System.out.println(jniUtil.getNativeNum(1));
    }
}
