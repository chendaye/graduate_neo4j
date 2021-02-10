package top.chendaye666.simrank.util;

import org.neo4j.graphdb.*;
import top.chendaye666.equitruss.graph.RelationshipTypes;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Map;
import java.util.regex.Pattern;

public class DataUtils {
    /**
     * 获取指定节点一步之内的所有邻边和邻节点
     * @param db
     * @param query
     * @return
     */
    public static int[] communityGenerate(GraphDatabaseService db, String query){
        int[] ans = null;
        try {
            String simrankPath = "/tmp/simrank/AL/";
            String dataPath = "/tmp/simrank/AL/AL.data";
            ReadWriteTxtUtils.dictExist(simrankPath);
            ReadWriteTxtUtils.folderClear(simrankPath);
            ReadWriteTxtUtils.fileExist(dataPath);
            final File nodeFile = new File(dataPath);
            FileOutputStream nodeOutputStream = new FileOutputStream(nodeFile);
            // (没有缓存数据)处理生成数据
            if(nodeOutputStream != null) {
                // 处理数据
                ans = depthOne(db, query, nodeOutputStream);
                nodeOutputStream.flush();
                nodeOutputStream.close();
            }
        }catch(IOException e) {
            e.printStackTrace();
        }
        return ans;
    }

    public static int[] depthOne(GraphDatabaseService db, String query, FileOutputStream relationshipOutputStream) throws IOException {
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
                    nodes.add(author.getId());
                    // node 直接相连的边
                    for (Relationship relationship : author.getRelationships(Direction.BOTH, RelationshipTypes.Article)){
                        // 当前节点的边
                        dealRelationShip(relationship, relationshipOutputStream, relationships);
                        // 边的另一个节点
                        Node otherNode = relationship.getOtherNode(author);
                        nodes.add(otherNode.getId());
                        // 另一个节点的边
//                        for (Relationship r : otherNode.getRelationships(Direction.BOTH, RelationshipTypes.Article)){
//                            dealRelationShip(r, relationshipOutputStream, relationships);
//                        }
                    }
                }
            }
            tx.commit();
        }
        int[] ans = new int[2];
        ans[0] = nodes.size();
        ans[1] = relationships.size();
        return ans;
    }



    // HashSet
    public static void dealRelationShip(Relationship relationship, FileOutputStream relationshipOutputStream, HashSet<Long> relationships) throws IOException {
        if (relationships.add(relationship.getId())){
            String r = relationship.getStartNode().getId()+"\t"+relationship.getEndNode().getId()+"\n";
            relationshipOutputStream.write(r.getBytes());
        }
    }

    // 解析 simrank 结果
    public static ArrayList<String[]> parseRank(String ans){
        ArrayList<String[]> res = new ArrayList<>();
        if (ans.length() == 0) return res;
        String[] split = ans.split("@");
        for (String s : split){
            res.add(s.split(","));
        }
        return res;
    }


}
