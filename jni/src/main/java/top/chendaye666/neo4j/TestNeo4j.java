package top.chendaye666.neo4j;

import java.util.ArrayList;

public class TestNeo4j {
    public static void main(String[] args) {
        JniArray jniArray = new JniArray();
//        int[] nums = new int[]{1, 2, 3};
//        int[] ans = jniArray.testArray(nums);
//        for (int i: ans){
//            System.out.println("==="+i);
//        }

//        int[][] arr = new int[3][3];
//        arr[0] = new int[]{1, 2, 3};
//        arr[1] = new int[]{4, 5, 6};
//        arr[2] = new int[]{7, 8, 9};
//        int[][] res = jniArray.testArray2D(arr);
//        for (int i = 0; i < 3; i++){
//            System.out.println(res[i][0]+"-"+res[i][1]+"-"+res[i][2]);
//            System.out.println();
//        }

        ArrayList<int[]> list = new ArrayList<>();
        list.add(new int[]{1, 2, 3});
        list.add(new int[]{4, 5, 6});
        list.add(new int[]{7, 8, 9});

        ArrayList<int[]> arrayList = jniArray.testArrayListInt(list);

        for (int[] arr : arrayList){
            System.out.println(arr[0]+"-"+arr[1]+"-"+arr[2]);
            System.out.println();
        }
    }
}
