# include <random>
# include <queue>
# include "hset.hpp"

#ifndef ALGORITHMRW_HPP
#define ALGORITHMRW_HPP
using namespace std;

class Algorithm_RW{
public:
    HyperGraph *graph;
    double restart;
    string eval_opt, algo_opt, outputdir;
    int givenmaxlength;
    vector<vector<int>> node2node;
    vector<int> htable;
    vector<bool> check;

    Algorithm_RW(string eval_opt, string algo_opt, string outputdir, HyperGraph *graph, double restart, int givenmaxlength);
    HSet* run(double target_portion);
    void walk(int seed_node, int max_length, set<int> &pool, int remain);
    //Three random-walk based sampling methods
    
    HSet* run_he_node(double target_portion);
    HSet* run_he_node_nonback(double target_portion);
    HSet* run_he_node_skipTriangle(double target_portion);
};
#endif