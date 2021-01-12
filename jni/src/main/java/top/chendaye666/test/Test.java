package top.chendaye666.test;

public class Test {
    public static void main(String[] args) {
        JniTest jniTest = new JniTest();
//        jniTest.hello();
//        System.out.println(jniTest.setArgBoolean(false));
//
//        int n = 123;
//        byte b = (byte)n;
//        System.out.println(jniTest.setArgByte(b));
//        System.out.println(jniTest.setArgChar('A'));
//        System.out.println(jniTest.setArgShort((short) 1));
//        System.out.println(jniTest.setArgDouble(1.2));

        int[] array = new int[]{1, 2, 3};
        int[] ints = jniTest.setArgArray3(array);
        for (int i : ints){
            System.out.println(i);
        }
    }
}
