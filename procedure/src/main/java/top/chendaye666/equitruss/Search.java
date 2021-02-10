package top.chendaye666.equitruss;

import org.neo4j.graphdb.*;
import org.neo4j.procedure.Context;
import org.neo4j.procedure.Description;
import org.neo4j.procedure.Name;
import org.neo4j.procedure.Procedure;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Map;
import java.util.stream.Stream;
import top.chendaye666.equitruss.streams.*;
import top.chendaye666.equitruss.util.DataUtils;

/**
 * https://neo4j.com/docs/java-reference/current/extending-neo4j/procedures-and-functions/procedures/
 * https://neo4j.com/docs/cypher-manual/4.1/clauses/call/
 * https://neo4j.com/docs/graph-data-science/current/algorithms/louvain/#algorithms-louvain-examples-stream
 */
public class Search {
    @Context
    public GraphDatabaseService db;
    public Search(){}


    /**
     * 社区搜索存储过程(只读权限)
     * @param node
     * @param k_value
     * @param attr_count
     * @param selection
     * @return
     */
    @Procedure(value = "top.chendaye666.equitruss.search")
    @Description("Get community by equitruss.")
    public Stream<Equitruss> search(@Name("node") Node node, @Name("k_value") long k_value, @Name("attr_count") long attr_count, @Name("selection") long selection) {
        ArrayList<Equitruss> res = new ArrayList<>();
        String ans = generateGraph(node, k_value, attr_count, selection);
        System.out.println("ans="+ans);
        if (ans == null) return res.stream();
        String[] split = ans.split(":");
        if (split.length != 6) return res.stream();
        String[] communities = split[5].split("@"); //todo: 多个结果,取第一个
        final String[] cm = communities[0].split("#");
        // 遍历所有社区节点
        try(Transaction tx = db.beginTx()) {
            Result result = tx.execute("match (p:Author) where id(p) in [" + cm[0] + "] return p");
            while (result.hasNext()){
                Map<String,Object> row = result.next();
                for ( Map.Entry<String,Object> column : row.entrySet() ){
                    Node author = (Node) column.getValue();
                    res.add(new Equitruss(author.getId(), (String) author.getProperty("authorId"), (String) author.getProperty("name"), (long) communities.length, cm[0], cm[1], split[5]));
                }
            }
        }
        return res.stream();
    }

    /**
     * 按行展示所有community
     * @param node
     * @param k_value
     * @param attr_count
     * @param selection
     * @return
     */
    @Procedure(value = "top.chendaye666.equitruss.searchall")
    @Description("Get community by equitruss.")
    public Stream<Equitrussall> searchall(@Name("node") Node node, @Name("k_value") long k_value, @Name("attr_count") long attr_count, @Name("selection") long selection) {
        ArrayList<Equitrussall> res = new ArrayList<>();
        String ans = generateGraph(node, k_value, attr_count, selection);
        if (ans == null) res.stream();
        ArrayList<ArrayList<long[]>> ansList = DataUtils.parseCommunityString(ans);
        if (ansList.size() == 0) res.stream();
        for (ArrayList<long[]> list : ansList)
            res.add(new Equitrussall(DataUtils.longArrayToLongList(list.get(0)), DataUtils.longArrayToLongList(list.get(1)), DataUtils.longArrayToLongList(list.get(2))));
        return res.stream();
    }


    /**
     * todo: 按时间筛选文章 or 作者:不直接用最近几年数据构造图，来进行社区搜索的原因：
     *      为了用历史数据尽可能完整的展示作者之间的合作关系。（用最近的数据构图也不存在技术困难）
     * @param node
     * @param k_value
     * @param attr_count
     * @param selection
     * @param year  某年之后有合作关系的
     * @return
     */
    @Procedure(value = "top.chendaye666.equitruss.time")
    @Description("Get community by equitruss.")
    public Stream<Equitrusstime> time(@Name("node") Node node, @Name("k_value") long k_value, @Name("attr_count") long attr_count, @Name("selection") long selection, @Name("year") long year) {
        ArrayList<Equitrusstime> res = new ArrayList<>();
        // 查询的作者的文章
        HashSet<String> ownArticles = new HashSet<>();
        String own = (String) node.getProperty("articles");
        if (own == null || own.length() == 0) return res.stream();
        String[] ownSplit = own.split("@");
        for (String s : ownSplit){
            String[] sSplit = s.split("#");
            ownArticles.add(sSplit[0]);
        }
        String ans = generateGraph(node, k_value, attr_count, selection);
//        String ans = "881893:4:0.042062:2:1,2,3,4:1,2,3,4,5#1,2,3,4@8,9,10,11#1,2,3,4";
        if (ans == null) return res.stream();
        ArrayList<ArrayList<long[]>> ansList = DataUtils.parseCommunityString(ans);
        if (ansList.size() == 0) return res.stream();
        try (Transaction tx = db.beginTx()){
            //todo: 每一个社区
            for (ArrayList<long[]> c : ansList){
                if (c.size() == 0) continue;
                HashSet<String> articleYearList = new HashSet<>(); // 符合年限要求的文章
                HashSet<Long> authorYearList = new HashSet<>(); // 符合年限要求的作者
                authorYearList.add(node.getId()); // 当前节点要加进来
                // 遍历每一个社区
                for (long nid : c.get(0)){
                    String query = "match (p:Author) where id(p)= "+nid+" return p";
                    Result result = tx.execute(query);
                    while (result.hasNext()){
                        Map<String,Object> row = result.next();
                        for ( Map.Entry<String,Object> column : row.entrySet() ){
                            Node author = (Node) column.getValue();
//                            authorYearList.add(author.getId()); // 社区节点
                            String articles = (String) author.getProperty("articles");
                            if (articles == null || articles.length() == 0) continue;
                            String[] articleArray = articles.split("@");
                            for (String article :  articleArray){
                                if (article == null || article.length() == 0) continue;
                                String[] articleInfo = article.split("#");
                                if (articleInfo.length < 4) continue;
                                String articleId = articleInfo[0];
                                // 年份必须是数字
                                if (!DataUtils.isNumber(articleInfo[3]) || articleInfo[3].length() < 4) continue; // 年份信息不全的跳过
                                long articleYear = Integer.parseInt(articleInfo[3].substring(0, 4));
                                // 文章发布时间要符合，同时，文章是和查询点合作的
                                if (articleYear >= year && ownArticles.contains(articleId)){
                                    authorYearList.add(author.getId()); // 社区节点
                                    articleYearList.add(articleId);
                                }
                            }
                        }
                    }
                }
                if (authorYearList.size() > 0 && articleYearList.size() > 0)
                    res.add(new Equitrusstime(new ArrayList<>(authorYearList), new ArrayList<>(articleYearList)));
            }
            tx.commit();
        }
        return res.stream();
    }

    /**
     * equitruss 生成子图，并执行算法
     * @param node
     * @param k_value
     * @param attr_count
     * @param selection
     * @return
     */
    public String generateGraph(Node node, long k_value, long attr_count, long selection){
        if (node == null) return null;
        int node_id = (int) node.getId(); // 查询的节点id
        // 获取数据
        String query = "match res=(p:Author)-[r1:Article]-(p1:Author) where id(p)="+node_id+" return p1";
        String[] path = DataUtils.communityGenerate(db, query, node_id);
        if (path == null) return null;
        //调用 Jni： relationship.txt node.txt vertex  resultPath query_k attr_cnt algo_type
        JniUtil jni = new JniUtil();
        String ans = jni.query(path[0], path[1], node_id, path[2], (int)k_value, (int)attr_count, (int)selection);
//        String ans = "881893:4:0.042062:2:1,2,3,4:15,190868,522503,637353,664837,743606,881893#1,2,3,4@419175,533944,881893,1457212#1,2,3,4";
        return ans;
    }

}
