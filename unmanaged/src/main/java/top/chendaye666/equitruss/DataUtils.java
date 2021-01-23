package top.chendaye666.equitruss;

import org.neo4j.graphdb.*;

import javax.ws.rs.PathParam;
import javax.ws.rs.core.Response;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
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

    public static void depthOne(GraphDatabaseService db, String query, FileOutputStream nodeOutputStream, FileOutputStream relationshipOutputStream) throws IOException {
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

    public static void dealNode(Node node, FileOutputStream nodeOutputStream, ArrayList<Long> nodes) throws IOException {
        if (!nodes.contains(node.getId())) {
            String endStr = ReadWriteTxtUtils.parseNode(node);
            nodeOutputStream.write(endStr.getBytes());
            nodes.add(node.getId());
        }
    }

    public static void dealRelationShip(Relationship relationship, FileOutputStream relationshipOutputStream, ArrayList<Long> relationships) throws IOException {
        if (!relationships.contains(relationship.getId())){
            relationships.add(relationship.getId());
            String r = relationship.getStartNode().getId()+"\t"+relationship.getEndNode().getId()+"\n";
            relationshipOutputStream.write(r.getBytes());
        }
    }

    // vertex	query_k	time	result_community_count	query_attribute_set	communities_set
    public static ArrayList<int[]> parseCommunity(String commubity){
        String[] result = commubity.split(":");
        ArrayList<int[]> list = new ArrayList<>();
        list.add(strToIntArray(result[4])); // 全部的属性集合
        if (result.length == 6){
            String[] split = result[5].split("#");
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
        String commubity = "1:" +
                "15:" +
                "0.826373:" +
                "1:" +
                "1,2,3,4,5,6,12,14,17,18:" +
                "1,54347,110775,381840,384015,490642,490648,492815,547069,599205,708217,737579,762478,845949,979500,1006881,1009145,1033820,1069769,1115209,1278104,1278590,1359585,1495279,1633037,1794206,1821467,1875810,1877848,1986787,2284301,2474620#1,2,3,4,5,6,12,14,17,18";
        commubity = "4:3:0.000023:0:1,2:1,54347,110775,381840#1,2";

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
