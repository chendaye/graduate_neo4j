/*************************************************************************
    > File Name: EffiEVD.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 22 Mar 2015 02:33:49 PM CST
 ************************************************************************/
#ifndef __SIMMATSVD_H__
#define __SIMMATSVD_H__

#include "SimMat.h"


class SimMatSVD : public SimMat {
public:
    SimMatSVD(double decayfactor, int *orig_graph_src, int *orig_graph_dst, int rank, int mvid, bool isInit,
              char *inpath) :
            SimMat(decayfactor, orig_graph_src, orig_graph_dst, rank, mvid, isInit, inpath) {
        sprintf(Lpath, "%s/%s/index/simmat/svd/%s.L", BASE_PATH, graphName, graphName);
        sprintf(Rpath, "%s/%s/index/simmat/svd/%s.R", BASE_PATH, graphName, graphName);
        sprintf(l_path, "%s/%s/index/simmat/svd/%s.l_", BASE_PATH, graphName, graphName);
        sprintf(r_path, "%s/%s/index/simmat/svd/%s.r_", BASE_PATH, graphName, graphName);
        sprintf(l_devpath, "%s/%s/index/simmat/svd/%s.l_dev", BASE_PATH, graphName, graphName);
        sprintf(r_devpath, "%s/%s/index/simmat/svd/%s.r_dev", BASE_PATH, graphName, graphName);
    }

    ~SimMatSVD() {}

    void initialize();

    void run(int qv, int k);
};

#endif
