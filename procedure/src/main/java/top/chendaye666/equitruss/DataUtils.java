package top.chendaye666.equitruss;

import org.neo4j.graphdb.*;

import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

public class DataUtils {
    public static String[] mutilStepGenerate(GraphDatabaseService db, String query){
        String[] resp = new String[3];
        try {
            FileOutputStream[] fs = ReadWriteTxtUtils.initFile(resp);
            // (没有缓存数据)处理生成数据
            if(fs[0] != null) {
                depthMutil(db, query, fs[0], fs[1]);
                ReadWriteTxtUtils.endFile(resp, fs);
                return resp;
            }
        }catch(IOException e) {
            e.printStackTrace();
        }
        return resp;
    }

    public static void depthMutil(GraphDatabaseService db, String query, FileOutputStream nodeOutputStream, FileOutputStream relationshipOutputStream) throws IOException {
        ArrayList<Long> relationships = new ArrayList<>();
        ArrayList<Long> nodes = new ArrayList<>();
        try (Transaction tx = db.beginTx() ) {
            Result result = tx.execute(query);
            while (result.hasNext()){
                Map<String,Object> row = result.next();
                // 所有 node
                for ( Map.Entry<String,Object> column : row.entrySet() ){
                    Node author = (Node) column.getValue();
                    dealNode(author, nodeOutputStream, nodes); // 处理node
                    // node 直接相连的边
                    for (Relationship relationship : author.getRelationships(Direction.BOTH, RelationshipTypes.Article)){
                        dealRelationShip(relationship, relationshipOutputStream, relationships);
                        // 边的另一个节点
                        Node otherNode = relationship.getOtherNode(author);
                        dealNode(otherNode, nodeOutputStream, nodes); // 处理node
                        // 另一个节点的边
                        for (Relationship r : otherNode.getRelationships(Direction.BOTH, RelationshipTypes.Article)){
                            dealRelationShip(r, relationshipOutputStream, relationships);
                        }
                    }
                }
            }
            tx.commit();
        }
    }


    /**
     * 获取指定节点一步之内的所有邻边和邻节点
     * @param db
     * @param query
     * @return
     */
    public static String[] communityGenerate(GraphDatabaseService db, String query, int node_id){
        String[] resp = new String[3];
        try {
            FileOutputStream[] fs = ReadWriteTxtUtils.initFile(resp, node_id);
            // (没有缓存数据)处理生成数据
            if(fs != null) {
                // 处理数据
                depthOne(db, query, fs[0], fs[1]);
                ReadWriteTxtUtils.endFile(resp, fs);
                return resp;
            }
        }catch(IOException e) {
            e.printStackTrace();
        }
        return resp;
    }
    //todo: 生成equitruss查询所需要的数据 ArrayList
    public static void _depthOne(GraphDatabaseService db, String query, FileOutputStream nodeOutputStream, FileOutputStream relationshipOutputStream) throws IOException {
        ArrayList<Long> relationships = new ArrayList<>();
        ArrayList<Long> nodes = new ArrayList<>();
        try (Transaction tx = db.beginTx() ) {
            // match res=(p:Author)-[r1:Article]-(p1:Author) where id(p)="+node_id+" return p,p1
            Result result = tx.execute(query);
            // p的所有邻接点
            while (result.hasNext()){
                Map<String,Object> row = result.next();
                // 所有 node
                for ( Map.Entry<String,Object> column : row.entrySet() ){
                    Node author = (Node) column.getValue();
                    //todo： 处理node, 耗费时间的点（考虑写入图中）
                    dealNode(author, nodeOutputStream, nodes);
                    // node 直接相连的边
                    for (Relationship relationship : author.getRelationships(Direction.BOTH, RelationshipTypes.Article)){
                        // 当前节点的边
                        dealRelationShip(relationship, relationshipOutputStream, relationships);
                        // 边的另一个节点
                        Node otherNode = relationship.getOtherNode(author);
                        dealNode(otherNode, nodeOutputStream, nodes); // 处理node
                        // 另一个节点的边
                        for (Relationship r : otherNode.getRelationships(Direction.BOTH, RelationshipTypes.Article)){
                            dealRelationShip(r, relationshipOutputStream, relationships);
                        }
                    }
                }
            }
            tx.commit();
        }
    }

    //todo: 生成equitruss查询所需要的数据 HashSet
    public static void depthOne(GraphDatabaseService db, String query, FileOutputStream nodeOutputStream, FileOutputStream relationshipOutputStream) throws IOException {
        HashSet<Long> relationships = new HashSet<>();
        HashSet<Long> nodes = new HashSet<>();
        try (Transaction tx = db.beginTx() ) {
            // match res=(p:Author)-[r1:Article]-(p1:Author) where id(p)="+node_id+" return p,p1
            Result result = tx.execute(query);
            // p的所有邻接点
            while (result.hasNext()){
                Map<String,Object> row = result.next();
                // 所有 node
                for ( Map.Entry<String,Object> column : row.entrySet() ){
                    Node author = (Node) column.getValue();
                    //todo： 处理node, 耗费时间的点（考虑写入图中）
                    dealNode(author, nodeOutputStream, nodes);
                    // node 直接相连的边
                    for (Relationship relationship : author.getRelationships(Direction.BOTH, RelationshipTypes.Article)){
                        // 当前节点的边
                        dealRelationShip(relationship, relationshipOutputStream, relationships);
                        // 边的另一个节点
                        Node otherNode = relationship.getOtherNode(author);
                        dealNode(otherNode, nodeOutputStream, nodes); // 处理node
                        // 另一个节点的边
                        for (Relationship r : otherNode.getRelationships(Direction.BOTH, RelationshipTypes.Article)){
                            dealRelationShip(r, relationshipOutputStream, relationships);
                        }
                    }
                }
            }
            tx.commit();
        }
    }


    // ArrayList
    public static void dealNode(Node node, FileOutputStream nodeOutputStream, ArrayList<Long> nodes) throws IOException {
        if (!nodes.contains(node.getId())) {
            String endStr = ReadWriteTxtUtils.parseNode(node);
            nodeOutputStream.write(endStr.getBytes());
            nodes.add(node.getId());
        }
    }
    // HashSet (底层是HashMap)
    public static void dealNode(Node node, FileOutputStream nodeOutputStream, HashSet<Long> nodes) throws IOException {
        if (nodes.add(node.getId())) {
            String endStr = ReadWriteTxtUtils.parseNode(node);
            nodeOutputStream.write(endStr.getBytes());
        }
    }
    // ArrayList
    public static void dealRelationShip(Relationship relationship, FileOutputStream relationshipOutputStream, ArrayList<Long> relationships) throws IOException {
        if (!relationships.contains(relationship.getId())){
            relationships.add(relationship.getId());
            String r = relationship.getStartNode().getId()+"\t"+relationship.getEndNode().getId()+"\n";
            relationshipOutputStream.write(r.getBytes());
        }
    }
    // HashSet
    public static void dealRelationShip(Relationship relationship, FileOutputStream relationshipOutputStream, HashSet<Long> relationships) throws IOException {
        if (relationships.add(relationship.getId())){
            String r = relationship.getStartNode().getId()+"\t"+relationship.getEndNode().getId()+"\n";
            relationshipOutputStream.write(r.getBytes());
        }
    }

    /*
     * vertex	query_k	time	result_community_count	query_attribute_set	communities_set
     * 881893:4:0.042062:2:1,2,3,4:15,190868,522503,637353,664837,743606,881893#1,2,3,4@419175,533944,881893,1457212#1,2,3,4
     *
     * result[0]= 881893 vertex
     * result[1]= 4 query_k
     * result[2]= 0.042062  time
     * result[3]= 2 result_community_count
     * result[4]= 1,2,3,4 query_attribute_set
     * result[5]= 15,190868,522503,637353,664837,743606,881893#1,2,3,4@419175,533944,881893,1457212#1,2,3,4 communities_set
     */
    public static ArrayList<int[]> parseCommunity(String commubity){
        System.out.println("search result="+commubity);
        String[] result = commubity.split(":");
        ArrayList<int[]> list = new ArrayList<>();
        list.add(strToIntArray(result[4])); // 全部的属性集合
        if (result.length == 6){
            //todo： 可能查询到多个社区
            String[] communities = result[5].split("@");
            //TODO: 处理多个社区
            String[] split = communities[0].split("#");
            list.add(strToIntArray(split[1])); // 公共的属性集合
            list.add(strToIntArray(split[0])); // 社区节点集合
        }
       return list;
    }

    public static int[] strToIntArray(String str){
        String[] split = str.split(",");
        int[] ans = new int[split.length];
        int i = 0;
        for (String s : split){
            ans[i++] = Integer.parseInt(s);
        }
        return ans;
    }

    public static void main(String[] args) {
        String  commubity = "" +
                "881893" +
                ":4" +
                ":0.042062" +
                ":2" +
                ":1,2,3,4" +
                ":15,190868,522503,637353,664837,743606,881893#1,2,3,4@419175,533944,881893,1457212#1,2,3,4";

        ArrayList<int[]> ints = parseCommunity(commubity);
        for (int[] arr : ints){
            String s = "";
            for (int i : arr){
                s += i+" ";
            }
            System.out.println(s);
        }
    }
}
