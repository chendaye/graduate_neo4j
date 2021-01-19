package top.chendaye666.equitruss;

import org.apache.commons.lang3.RandomStringUtils;
import org.neo4j.graphdb.Node;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class readWriteTxtUtils {
    /**使用FileOutputStream来写入txt文件
     * @param txtPath txt文件路径
     * @param content 需要写入的文本
     */
    public static void writeTxt(String txtPath,String content){
        FileOutputStream fileOutputStream = null;
        File file = new File(txtPath);
        try {
            if(file.exists()){
                //判断文件是否存在，如果不存在就新建一个txt
                file.createNewFile();
            }
            fileOutputStream = new FileOutputStream(file);
            fileOutputStream.write(content.getBytes());
            fileOutputStream.flush();
            fileOutputStream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 判断文件是否存在，不存在就新建
     * @param path
     */
    public static void fileExist(String path){
        File file = new File(path);
        try {
            if (!file.exists()) file.createNewFile();
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    /**
     * 删除文件
     * @param path
     */
    public static void fileDel(String path){
        File file = new File(path);
        if (file.exists()) file.delete();
    }

    /**
     * 生成指定长度的随机字符串
     * @param len
     * @return
     */
    public  static String  generateStr(int len){
        String str = RandomStringUtils.randomAlphanumeric(len);
        return str.toLowerCase();
    }

    /**
     * 解析node信息， 5:AI:1@2:CV:1@4:DM:4
     * @param node
     * @return
     */
    public static String parseNode(Node node){
//        String authorId = (String)node.getProperty("authorId");
        String words = (String) node.getProperty("words");
        String[] split = words.split("@");
        String index = "";
        for (String str : split){
            String[] word = str.split(":");
            index += word[0]+",";
        }
        index = index.substring(0, index.length() - 1);
        return node.getId()+"\t"+index+"\n";
    }
}
