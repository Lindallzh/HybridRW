#include <fstream>
#include <unordered_map>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

#include "utils.hpp"

#ifndef ATTRIBUTE_HPP
#define ATTRIBUTE_HPP

using namespace std;

class Attribute {
public:
    map<string, unordered_map<int,long long>> attr_list;

    map<string, unordered_map<int,double>> attr_list_rw;

    Attribute(vector<vector<int>> node2hyperedge, vector<vector<int>> hyperedge2node, string dataname);
    Attribute(vector<vector<int>> node2hyperedge, vector<vector<int>> hyperedge2node, set<int> hset, vector<int> &node_degree, string eval_opt);
    Attribute(Attribute *attr);
    
    bool operator==(Attribute& r) const;
    void to_textfile(string output_dir);
    void set_attribute(vector<vector<int>> node2hyperedge, vector<vector<int>> hyperedge2node, vector<int> hset, vector<int> &node_degree, string eval_opt);
    void set_attribute_rw(vector<vector<int>> node2hyperedge, vector<vector<int>> hyperedge2node, vector<int> hset, map<int,int> add_size,vector<int> &node_degree, vector<float> &node_degree_size,string eval_opt,vector<int> nodeset, map<int,int> add_node_size);
    void set_attribute_deg_rwrw(vector<vector<int>> node2hyperedge, vector<int> nodeset, map<int,int> add_node_size);
    ~Attribute(){
        for (auto &item : attr_list){
            auto &m = item.second;
            m.clear();
        }
        attr_list.clear();

        for (auto &item : attr_list_rw){
            auto &m = item.second;
            m.clear();
        }
        attr_list_rw.clear();
    }

};
#endif