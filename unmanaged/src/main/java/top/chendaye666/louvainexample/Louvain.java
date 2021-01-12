package top.chendaye666.louvainexample;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Random;

public class Louvain implements Cloneable{
    int n; // 结点个数
    int m; // 边数
    int cluster[]; // 结点i属于哪个簇
    Edge edge[]; // 邻接表
    int head[]; // 头结点下标
    int top; // 已用E的个数
    double resolution; // 1/2m 全局不变
    double node_weight[]; // 结点的权值
    double totalEdgeWeight; // 总边权值
    double[] cluster_weight; // 簇的权值
    double eps = 1e-14; // 误差

    int global_n; // 最初始的n
    int global_cluster[]; // 最后的结果，i属于哪个簇
    Edge[] new_edge;   //新的邻接表
    int[] new_head;
    int new_top = 0;
    final int iteration_time = 3; // 最大迭代次数

    Edge global_edge[];   //全局初始的临接表  只保存一次，永久不变，不参与后期运算
    int global_head[];
    int global_top=0;

    //todo: 节点 u的 邻接链表 （u的所有邻边 和 对应权重）
    void addEdge(int u, int v, double weight) {
        if(edge[top]==null)
            edge[top]=new Edge(); //todo: edge[] 保存图中的所有边（同一条边保存 2条，方向相反）
        //todo: 边 v-next 头插法
        edge[top].v = v;
        edge[top].weight = weight;
        edge[top].next = head[u];
        //todo: 与 节点 u 相关的点 是 第 top条边
        head[u] = top++;  //todo: head[u] 中存的 是 edge的索引；相当于指针 edge[head[u]] = 节点u的第一条边
    }

    void addNewEdge(int u, int v, double weight) {
        if(new_edge[new_top]==null)
            new_edge[new_top]=new Edge();
        new_edge[new_top].v = v;
        new_edge[new_top].weight = weight;
        new_edge[new_top].next = new_head[u];
        new_head[u] = new_top++;
    }

    void addGlobalEdge(int u, int v, double weight) {
        if(global_edge[global_top]==null)
            global_edge[global_top]=new Edge();
        global_edge[global_top].v = v;
        global_edge[global_top].weight = weight;
        global_edge[global_top].next = global_head[u];
        global_head[u] = global_top++;
    }

    void init(String filePath) {
        try {
            String encoding = "UTF-8";
            File file = new File(filePath);
            if (file.isFile() && file.exists()) { // 判断文件是否存在
                InputStreamReader read = new InputStreamReader(new FileInputStream(file), encoding);// 考虑到编码格式
                BufferedReader bufferedReader = new BufferedReader(read);
                String lineTxt = null;
                lineTxt = bufferedReader.readLine();

                ////// 预处理部分
                String cur2[] = lineTxt.split(" ");
                global_n = Integer.parseInt(cur2[0]); //todo: node 数 最初始的n
                n = global_n;
                m = Integer.parseInt(cur2[1]); //todo: 边数
                m *= 2; //todo: 度数
                edge = new Edge[m]; //todo: 邻接表 一条边 两个端点 算2条边
                head = new int[n]; //todo: 头结点
                for (int i = 0; i < n; i++)
                    head[i] = -1;
                top = 0; //todo: 已用E的个数

                global_edge=new Edge[m]; //todo:全局初始的临接表  只保存一次，永久不变，不参与后期运算
                global_head = new int[n]; //todo:全局初始的临接表  头结点
                for(int i=0;i<n;i++)
                    global_head[i]=-1;
                global_top=0;

                //todo:答案
                global_cluster = new int[n]; //todo: 最后的结果，i属于哪个簇
                for (int i = 0; i < global_n; i++) //todo: 节点i属于第 i 簇
                    global_cluster[i] = i;

                node_weight = new double[n]; //todo:结点的权值
                totalEdgeWeight = 0.0; //todo:总边权值

                //todo: 读每一条边
                while ((lineTxt = bufferedReader.readLine()) != null) {
                    String cur[] = lineTxt.split(" ");
                    int u = Integer.parseInt(cur[0]); //todo： 节点u
                    int v = Integer.parseInt(cur[1]); //todo: 节点v
                    double curw; //todo: 节点 u-v 边的权重
                    if (cur.length > 2) {
                        curw = Double.parseDouble(cur[2]);
                    } else {
                        curw = 1.0;
                    }
                    //todo: 记录哪些点 有 哪些边
                    addEdge(u, v, curw); //todo: 新增边 v->u 并在 head[u] 中
                    addEdge(v, u, curw); //todo: 新增边 u->v
                    addGlobalEdge(u,v,curw);
                    addGlobalEdge(v,u,curw);
                    //todo: 整个图的权重增加 2 * curw
                    totalEdgeWeight += 2 * curw;
                    //todo: 节点u、v的，边权重分别增加 curw
                    node_weight[u] += curw;
                    if (u != v) {
                        node_weight[v] += curw;
                    }
                }
                //todo: 1/2m 全局不变（m = 边*边的权重之和）
                resolution = 1 / totalEdgeWeight;
                read.close();
            } else {
                System.out.println("找不到指定的文件");
            }
        } catch (Exception e) {
            System.out.println("读取文件内容出错");
            e.printStackTrace();
        }
    }

    //todo: 一个结点一个簇
    void init_cluster() {
        cluster = new int[n];
        for (int i = 0; i < n; i++) {
            cluster[i] = i; //todo: node_id => community_id 节点所属的社区
        }
    }

    //todo: 尝试将i加入某个簇
    boolean try_move_i(int i) {
        double[] edgeWeightPerCluster = new double[n];
        //todo: 遍历节点 j 的所有邻居节点（社区）；
        for (int j = head[i]; j != -1; j = edge[j].next) {
            int l = cluster[edge[j].v]; //todo： l是 节点 i的邻节点  edge[j].v 所在簇的编号
            //todo: 节点 i 每个邻节点所在的社区， 新增的可能 边权
            edgeWeightPerCluster[l] += edge[j].weight;
        }
        int bestCluster = -1; //todo： 最优的簇号下标(先默认是自己)
        double maxx_deltaQ = 0.0; //todo： 增量的最大值

        //todo: 社区 i 减去 节点 i的边权重
        boolean[] vis = new boolean[n];
        //todo: 节点 i 所在的 的社区的边权和 - 节点 i 的权值
        cluster_weight[cluster[i]] -= node_weight[i]; //todo: 结点的权值,所有邻边的权值和

        //todo: 遍历 i的所有 邻节点，将 i 加入其中之一
        for (int j = head[i]; j != -1; j = edge[j].next) {
            int l = cluster[edge[j].v]; //todo: l代表領接点的簇号
            if (vis[l]) //todo: 一个領接簇只判断一次
                continue;
            vis[l] = true;

            //todo: 如果把节点 i 加入 其邻节点 edge[i].v 所在的社区 l， 社区 l 会新增边权 edge[j].weight
            //todo： 记录在 edgeWeightPerCluster[l] 中
            double cur_deltaQ = edgeWeightPerCluster[l];
            //todo: deltaQ 只与 i 和 社区 l 有关。 i 是一个节点 社区l也是一个节点
//            cur_deltaQ = cur_deltaQ - node_weight[i] * cluster_weight[l] * resolution;
            cur_deltaQ = cur_deltaQ*resolution/2 - node_weight[i] * cluster_weight[l] * resolution*resolution/2;
            //todo:找到最大的 deltaQ l
            if (cur_deltaQ > maxx_deltaQ) {
                bestCluster = l;
                maxx_deltaQ = cur_deltaQ;
            }
            edgeWeightPerCluster[l] = 0;
        }
        //todo： 如果尝试完所有的 邻节点社区之后，deltaQ变化不是足够大，就让 节点 i 不动
        if (maxx_deltaQ < eps) {
            bestCluster = cluster[i];
        }
        //todo: 节点 i 移动入 社区 bestClister
        cluster_weight[bestCluster] += node_weight[i];
        if (bestCluster != cluster[i]) { // i成功移动了
            cluster[i] = bestCluster;
            return true;
        }
        return false;
    }

    void rebuildGraph() { //todo: 重构图
        /// 先对簇进行离散化
        int[] change = new int[n];
        int change_size=0;
        boolean vis[] = new boolean[n];
        //todo: 访问每一个节点，找到每一个节点所在社区，并做访问标记；同一个社区对应 change[] 中的一个元素
        for (int i = 0; i < n; i++) {
            if (vis[cluster[i]])
                continue;
            vis[cluster[i]] = true;
            change[change_size++]=cluster[i]; //todo: change[] 中的一个元素对应上一层图中的 一个社区
        }
        //todo: 每一个原始（社区）节点 i，在新图中都属于一个社区；不同层级的社区，像套娃一圈一圈
        int[] index = new int[n]; //todo: index[i]代表 i号簇在新图中的结点编号
        //todo: 遍历新图中的每一个节点（上一层图的一个社区）
        for (int i = 0; i < change_size; i++)
            index[change[i]] = i; //todo: <change[i]上层图的社区编号相当于本层图的节点ID， 本层图的节点编号>
        int new_n = change_size; //todo： 新图的大小
        new_edge = new Edge[m]; //todo: 新图的边 还是 m
        new_head = new int[new_n]; //todo: 新图节点
        new_top = 0;
        double new_node_weight[] = new double[new_n]; //todo: 新图 节点权和

        //todo: 初始化新图头结点
        for(int i=0;i<new_n;i++)
            new_head[i]=-1;

        //todo： 代表每个簇中的节点列表
        ArrayList<Integer>[] nodeInCluster = new ArrayList[new_n];
        for (int i = 0; i < new_n; i++)
            nodeInCluster[i] = new ArrayList<Integer>();

        //todo: cluster[i]:原始图中 节点i所在的社区 编号；index[cluster[i]]：当前图中 原始社区 cluster[i] 所在社区 编号
        for (int i = 0; i < n; i++) {
            nodeInCluster[index[cluster[i]]].add(i);
        }

        //todo： 将同一个簇的挨在一起分析。可以将visindex数组降到一维
        for (int u = 0; u < new_n; u++) {
            boolean visindex[] = new boolean[new_n]; //todo： visindex[v]代表新图中u节点到v的边在临街表是第几个（多了1，为了初始化方便）
            double delta_w[] = new double[new_n]; //todo： 边权的增量
            //todo: 遍历新图的每一个节点（包含多个原始节点）
            for (int i = 0; i < nodeInCluster[u].size(); i++) {
                int t = nodeInCluster[u].get(i); //todo: 第 u个新图节点，中包含的原始节点

                //todo: 新图中节点 u 包含的的原始节点 t 在原图中有很多邻节点 j，j在当前图中 也对应 一个节点 v；则新图中 u-v 是邻节点
                for (int k = head[t]; k != -1; k = edge[k].next) {
                    int j = edge[k].v; //todo: 原始 节点 j， j所在的原始社区 cluster[j]
                    int v = index[cluster[j]]; //todo:原始社区 cluster[j] 在当前图中 对应的节点编号
                    if (u != v) { //todo: 当前图中的节点 u v 不是同一个节点
                        if (!visindex[v]) {
                            addNewEdge(u, v, 0); //todo: 新图中 新增 u的一条邻边。 weight=0
                            visindex[v] = true;
                        }
                        //todo: 新图节点 v 的权值 增加
                        delta_w[v] += edge[k].weight;
                    }
                }
                //todo: 新图中节点 u的边权和 = 其包含的原始节点边劝和 累加
                new_node_weight[u] += node_weight[t];
            }
            //todo: 设置新图中 每条边的权值,邻接表
            for (int k = new_head[u]; k != -1; k = new_edge[k].next) {
                int v = new_edge[k].v;
                new_edge[k].weight = delta_w[v];
            }
        }

        //todo: 更新答案
        int[] new_global_cluster = new int[global_n];
        for (int i = 0; i < global_n; i++) {
            new_global_cluster[i] = index[cluster[global_cluster[i]]];
        }
        for (int i = 0; i < global_n; i++) {
            global_cluster[i] = new_global_cluster[i];
        }
        top = new_top;
        for (int i = 0; i < m; i++) {
            edge[i] = new_edge[i];
        }
        for (int i = 0; i < new_n; i++) {
            node_weight[i] = new_node_weight[i];
            head[i] = new_head[i];
        }
        n = new_n;
        init_cluster();
    }

    void print() {
        for (int i = 0; i < global_n; i++) {
            System.out.println(i + ": " + global_cluster[i]);
        }
        System.out.println("-------");
    }

    void louvain() {
        init_cluster();
        int count = 0; // 迭代次数
        boolean update_flag; // 标记是否发生过更新
        do { // 第一重循环，每次循环rebuild一次图
            //    print(); // 打印簇列表
            count++;
            cluster_weight = new double[n];
            for (int j = 0; j < n; j++) { // 生成簇的权值
                cluster_weight[cluster[j]] += node_weight[j];
            }
            int[] order = new int[n]; // 生成随机序列
            for (int i = 0; i < n; i++)
                order[i] = i;
            Random random = new Random();
            for (int i = 0; i < n; i++) {
                int j = random.nextInt(n);
                int temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
            int enum_time = 0; // 枚举次数，到n时代表所有点已经遍历过且无移动的点
            int point = 0; // 循环指针
            update_flag = false; // 是否发生过更新的标记
            do {
                int i = order[point];
                point = (point + 1) % n;
                if (try_move_i(i)) { // 对i点做尝试
                    enum_time = 0;
                    update_flag = true;
                } else {
                    enum_time++;
                }
            } while (enum_time < n);
            if (count > iteration_time || !update_flag) // 如果没更新过或者迭代次数超过指定值
                break;
            rebuildGraph(); // 重构图
        } while (true);
    }
}
