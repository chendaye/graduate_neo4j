package top.chendaye666.httpjni;

import org.apache.commons.lang3.ArrayUtils;
import org.neo4j.graphdb.*;

import java.util.*;

public  class DataProcess {
    private HashMap<Long, HashSet<Long>> nodeNeighbors = new HashMap<>(); // 每一个node的邻居节点
    private HashMap<Long, HashMap<Long, Double>> nodeNeighborsWeights = new HashMap<>(); // 邻边的权重
    private long graphWeightSum = 0; // 图的权重和

    public HashMap<Long, HashSet<Long>> getNodeNeighbors() {
        return nodeNeighbors;
    }

    public void setNodeNeighbors(HashMap<Long, HashSet<Long>> nodeNeighbors) {
        this.nodeNeighbors = nodeNeighbors;
    }

    public HashMap<Long, HashMap<Long, Double>> getNodeNeighborsWeights() {
        return nodeNeighborsWeights;
    }

    public void setNodeNeighborsWeights(HashMap<Long, HashMap<Long, Double>> nodeNeighborsWeights) {
        this.nodeNeighborsWeights = nodeNeighborsWeights;
    }
    
    public long getgraphWeightSum(){
        return this.graphWeightSum;
    }

    /**
     * 获取全部节点，并收集 邻居节点和邻边权重
     * @param db
     * @return
     */
    public long[] getPhysicians(GraphDatabaseService db){
        ArrayList<Long> physicianList;
        try(Transaction tx = db.beginTx()){
            // 取node
            ResourceIterator<Node> physicians = tx.findNodes(Labels.Physician);
            // 处理 node
            physicianList = iteratorNode(physicians);
            tx.commit();
        }
        return ArrayUtils.toPrimitive(physicianList.toArray(new Long[physicianList.size()]));
    }
    private ArrayList<Long> iteratorNode(ResourceIterator<Node> physicians){
        ArrayList<Long> physicianList = new ArrayList<>();
        while (physicians.hasNext()){
            // 当前节点
            Node physician = physicians.next();
            physicianList.add(physician.getId()); // 所有获取到的节点
            //todo:处理节点
            relationshipProcess(physician);
        }
        return physicianList;
    }

    /**
     * 通过cypher查询获取节点
     * @param db
     * @param query
     * @return
     */
    public long[]   getPhysicians(GraphDatabaseService db, String query){
        ArrayList<Long> physicianList;
        try(Transaction tx = db.beginTx()){
            // 取node
            Result execute = tx.execute(query);
            // 处理node
            physicianList = iteratorNode(execute);
            tx.commit();
        }

        // 复制原数组
        return ArrayUtils.toPrimitive(physicianList.toArray(new Long[physicianList.size()]));
    }

    private ArrayList<Long> iteratorNode(Result execute){
        ArrayList<Long> physicianList = new ArrayList<>();
        while (execute.hasNext()){
            Map<String,Object> row = execute.next();
            for ( Map.Entry<String,Object> column : row.entrySet() ){
                Node physician = (Node) column.getValue();
                // 图中所有节点
                physicianList.add(physician.getId());
                // 处理边
                relationshipProcess(physician);
            }
        }
        return physicianList;
    }

    /**
     * 处理给定节点的 邻居节点 和 边
     * @param physician
     */
    private void relationshipProcess(Node physician){
        // HashSet 节点和边去重
        HashSet<Long> neighbors = new HashSet<>();
        HashMap<Long, Double> neighborWeights = new HashMap<>(); // 权重要转换一下
        for (Relationship relationship : physician.getRelationships(Direction.BOTH, RelationshipTypes.SHARE_MEMBER)){
            Node otherNode = relationship.getOtherNode(physician);
            // 当前节点的邻居节点 <neighbor_node_id>
            neighbors.add(otherNode.getId());
            // 当前边权重 <relationship_id, patients>
            long weight = (long)relationship.getProperty("patients", 0.0);
            // 因为 是有向图，出边和入边都算一个 度
            if (neighborWeights.containsKey(otherNode.getId())){
                neighborWeights.put(otherNode.getId(), (double)weight + neighborWeights.get(otherNode.getId()));
            }else{
                neighborWeights.put(otherNode.getId(), (double)weight);
            }
            // 图的权重和
            graphWeightSum += (long)relationship.getProperty("patients", 0);
        }
        // 保存每一个 node 的所有邻居 和 每一条邻边的权，
        nodeNeighbors.put(physician.getId(), neighbors); // <node_id, <neighbor_node_id>>
        nodeNeighborsWeights.put(physician.getId(), neighborWeights); // <node_id, <relationship_id, patients>>
    }


}
