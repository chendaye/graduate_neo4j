/*************************************************************************
    > File Name: EffiEVD.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 22 Mar 2015 02:33:49 PM CST
 ************************************************************************/
#ifndef __SIMMATEVD_H__
#define __SIMMATEVD_H__
#include "SimMat.h"
class SimMatEVD: public SimMat{
	public:
	SimMatEVD(double decayfactor,int* orig_graph_src,int* orig_graph_dst, int rank, int mvid, bool isInit,char* inpath):
		SimMat(decayfactor,orig_graph_src,orig_graph_dst,rank, mvid, isInit,inpath){
		sprintf(Lpath,"%s/%s/index/simmat/evd/%s.L",BASE_PATH.data(), graphName,graphName);
        	sprintf(Rpath,"%s/%s/index/simmat/evd/%s.R",BASE_PATH.data(), graphName,graphName);
        	sprintf(l_path,"%s/%s/index/simmat/evd/%s.l_",BASE_PATH.data(), graphName,graphName);
		sprintf(r_path,"%s/%s/index/simmat/evd/%s.r_",BASE_PATH.data(), graphName,graphName);
        	sprintf(l_devpath,"%s/%s/index/simmat/evd/%s.l_dev",BASE_PATH.data(), graphName,graphName);
     	   	sprintf(r_devpath,"%s/%s/index/simmat/evd/%s.r_dev",BASE_PATH.data(), graphName,graphName);
		}
		
		~SimMatEVD(){ }
		void initialize();
		void run(int qv, int k);
};
#endif
