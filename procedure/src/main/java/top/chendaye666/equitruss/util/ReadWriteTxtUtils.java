package top.chendaye666.equitruss.util;

import org.apache.commons.lang3.RandomStringUtils;
import org.neo4j.graphdb.Node;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class ReadWriteTxtUtils {
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
    public static boolean fileExist(String path){
        File file = new File(path);
        boolean flag = true;
        try {
            if (!file.exists()) {
                file.createNewFile();
                flag = false;
            }
        }catch (IOException e){
            e.printStackTrace();
        }
        return flag;
    }

    /**
     * 判断文件夹是否存在
     * @param path
     */
    public static boolean dictExist(String path){
        File folder = new File(path);
        boolean flag = true;
        if (!folder.exists() && !folder.isDirectory()) {
            folder.mkdirs();
            flag = false;
        }
        return flag;
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
     * 删除文件夹
     * @param path
     */
    public static void folderDel(String path){
        File file = new File(path);
        deleteAll(file);
    }

    public static void deleteAll(File file) {
        if (!file.exists()) return;
        if (file.isFile()) {
                file.delete();
        } else {
            for (File f : file.listFiles()) {
                deleteAll(f); // 递归删除每一个文件
            }
            file.delete(); // 删除文件夹
        }
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
//        String words = (String) node.getProperty("words");
//        String[] split = words.split("@");
//        String index = "";
//        for (String str : split){
//            String[] word = str.split(":");
//            index += word[0]+",";
//        }
//        index = index.substring(0, index.length() - 1);
        String index = (String)node.getProperty("attribute");
        return node.getId()+"\t"+index+"\n";
    }

    public static boolean isLinux() {
        return System.getProperty("os.name").toLowerCase().contains("linux");
    }

    public static boolean isWindows() {
        return System.getProperty("os.name").toLowerCase().contains("windows");
    }

    public static String JudgeSystem() {
        if (isLinux()) {
            return "linux";
        } else if (isWindows()) {
            return "windows";
        } else {
            return "other system";
        }
    }

    public static String path(){
        if (isLinux()) {
            return "/tmp";
        } else if (isWindows()) {
            return "E:\\code";
        } else {
            return "/tmp";
        }
    }

    /**
     *  随机文件名
     * @param resp
     * @return
     * @throws FileNotFoundException
     */
    public static FileOutputStream[] initFile(String[] resp) throws FileNotFoundException {
        // 路径
        String random = ReadWriteTxtUtils.generateStr(5);
        String resPath =  ReadWriteTxtUtils.path();
        String nodePath = resPath+"/node_"+ random+".txt";
        String relationshipPath = resPath+"/relationship_"+ random+".txt";
        String resultPath = resPath+"/result_"+ random+"/";
        // 创建文件
        ReadWriteTxtUtils.fileExist(nodePath);
        ReadWriteTxtUtils.fileExist(relationshipPath);
        ReadWriteTxtUtils.dictExist(resultPath);
        // 文件描述符
        final File nodeFile = new File(nodePath);
        FileOutputStream nodeOutputStream = new FileOutputStream(nodeFile);
        final File relationshipFile = new File(relationshipPath);
        FileOutputStream relationshipOutputStream = new FileOutputStream(relationshipFile);
        // resp
        resp[0] = relationshipPath;
        resp[1] = nodePath;
        resp[2] = resultPath;

        FileOutputStream[] fs = new FileOutputStream[2];
        fs[0] = nodeOutputStream;
        fs[1] = relationshipOutputStream;
        return fs;
    }

    /**
     * 初始化文件：用node_id 初始化
     * @param resp
     * @param node_id
     * @return
     * @throws FileNotFoundException
     */
    public static FileOutputStream[] initFile(String[] resp , int node_id) throws FileNotFoundException {
        // 路径
        String random = String.valueOf(node_id);
        String resPath =  ReadWriteTxtUtils.path();
        String nodePath = resPath+"/node_"+ random+".txt";
        String relationshipPath = resPath+"/relationship_"+ random+".txt";
        String resultPath = resPath+"/result_"+ random+"/";
        // resp
        resp[0] = relationshipPath;
        resp[1] = nodePath;
        resp[2] = resultPath;
        // 创建文件
        FileOutputStream[] fs = new FileOutputStream[2];
        if(ReadWriteTxtUtils.fileExist(nodePath) &&
                ReadWriteTxtUtils.fileExist(relationshipPath) &&
                ReadWriteTxtUtils.dictExist(resultPath)){
            return null;
        }
        // 文件描述符
        final File nodeFile = new File(nodePath);
        FileOutputStream nodeOutputStream = new FileOutputStream(nodeFile);
        final File relationshipFile = new File(relationshipPath);
        FileOutputStream relationshipOutputStream = new FileOutputStream(relationshipFile);
        fs[0] = nodeOutputStream;
        fs[1] = relationshipOutputStream;
        return fs;
    }

    public static void endFile(String[] resp, FileOutputStream[] fs) throws IOException {
        fs[0].flush();
        fs[0].close();
        fs[1].flush();
        fs[1].close();
        // 删除旧文件
//        fileDel(resp[0]);
//        fileDel(resp[1]);
//        folderDel(resp[2]);
    }

}
