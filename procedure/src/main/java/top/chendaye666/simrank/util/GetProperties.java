package top.chendaye666.simrank.util;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Properties;

public class GetProperties {
    public  String config = "src/main/java/top/chendaye666/simrank/simrank.properties";
    public  Properties props = null;
    private GetProperties() throws IOException {
        props = new Properties();
        BufferedReader bufferedReader = new BufferedReader(new FileReader(config));
        props.load(bufferedReader);
    }


    /**
     * 获取实例
     */
    public static GetProperties getInstance() {
        return StaticSingletonHolder.instance;
    }

    /**
     * 一个私有的静态内部类，用于初始化一个静态final实例
     */
    private static class StaticSingletonHolder {
        private static GetProperties instance = null;
        static {
            try {
                instance = new GetProperties();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public String getConfig(String key){
        return props.getProperty(key);
    }
}
