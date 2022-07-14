#include "attribute.hpp"

Attribute::Attribute(vector<vector<int>> node2hyperedge, vector<vector<int>> hyperedge2node, string dataname){
    // All attributes do not include zero
    vector<int> vec;
    vector<int>::iterator it;
    int number_of_nodes = (int)node2hyperedge.size();
    int number_of_hedges = (int)hyperedge2node.size();

    // degree
    cout << "Get Degree Distribution" << endl;
    unordered_map<int, long long> degree;
    string degree_name = "./results/answer/" + dataname + "_degree.txt";
    if( file_exist(degree_name) ){
        // read file
        ifstream degFile(degree_name.c_str());
        string line;
        while (getline(degFile, line)){
            vector<string> tokens = split(line, ',');
            assert((int)tokens.size() == 2);
            int deg = stoi(tokens[0]);
            long long freq = stoll(tokens[1]);
            degree[deg] = freq;
        }
    }
    else{
        for (int v = 0; v < number_of_nodes ; v++){
            int deg = (int)node2hyperedge[v].size();
            if(deg == 0){
                continue;
            }
            degree[deg]++;
        }
        // output file
        ofstream resultFile(degree_name.c_str());
        for (auto item : degree){
            resultFile << to_string(item.first) << "," << to_string(item.second) << endl;
        }
        resultFile.close();
    }
    this->attr_list["degree"] = degree;
    degree.clear();
    
    // size
    cout << "Get Size Distribution" << endl;
    unordered_map<int, long long> size;
    string size_name = "./results/answer/" + dataname + "_size.txt";
    if( file_exist(size_name) ){
        // read file
        ifstream sizeFile(size_name.c_str());
        string line;
        while (getline(sizeFile, line)){
            vector<string> tokens = split(line, ',');
            assert( (int)tokens.size() == 2);
            int sz = stoi(tokens[0]);
            long long freq = stoll(tokens[1]);
            size[sz] = freq;
        }
    }
    else{
        for (int h = 0; h < number_of_hedges ; h++){
            int hsize = (int)hyperedge2node[h].size();
            size[hsize]++;
        }
        // output file
        ofstream resultFile(size_name.c_str());
        for (auto item : size){
            resultFile << to_string(item.first) << "," << to_string(item.second) << endl;
        }
        resultFile.close();
    }
    this->attr_list["size"] = size;
    size.clear();

    // For Pair
    unordered_map<string, long long> tmp;
    unordered_map<int, long long> intersect;
    // intersect
    cout << "Get Intersect Distribution" << endl;
    string its_name = "./results/answer/" + dataname + "_intersect.txt";
    if( file_exist(its_name) ){
        // read file
        ifstream itsFile(its_name.c_str());
        string line;
        while (getline(itsFile, line)){
            vector<string> tokens = split(line, ',');
            assert( (int)tokens.size() == 2);
            int its = stoi(tokens[0]);
            long long freq = stoll(tokens[1]);
            intersect[its] = freq;
        }
    }
    else{
        for (int ha = 0; ha < number_of_hedges ; ha++){
            int hsize = (int)hyperedge2node[ha].size();
            for (int i = 0 ; i < hsize ; i++){
                int v = hyperedge2node[ha][i];
                int deg = (int)node2hyperedge[v].size();
                for (int j = 0 ; j < deg; j++){
                    int hb = node2hyperedge[v][j];
                    if (ha < hb){
                        string key = to_string(ha) + "_" + to_string(hb);
                        tmp[key]++;
                    }
                }
            }
        }
        for (auto item : tmp){
            intersect[item.second]++;
        }
        tmp.clear();
        // output file
        ofstream resultFile(its_name.c_str());
        for (auto item : intersect){
            resultFile << to_string(item.first) << "," << to_string(item.second) << endl;
        }
        resultFile.close();
    }
    this->attr_list["intersect"] = intersect;
    intersect.clear();

    // pairdeg
    cout << "Get PairDegree Distribution" << endl;
    unordered_map<int, long long> pairdeg;
    string pd_name = "./results/answer/" + dataname + "_pairdeg.txt";
    if( file_exist(pd_name) ){
        // read file
        ifstream pdFile(pd_name.c_str());
        string line;
        while (getline(pdFile, line)){
            vector<string> tokens = split(line, ',');
            assert( (int)tokens.size() == 2);
            int pd = stoi(tokens[0]);
            long long freq = stoll(tokens[1]);
            pairdeg[pd] = freq;
        }
    }
    else{
        for(int h = 0 ; h < number_of_hedges ; h++){
            int hsize = (int)hyperedge2node[h].size();
            for(int i = 0 ; i < hsize ; i++){
                for(int j = (i + 1) ; j < hsize ; j++){
                    int va = hyperedge2node[h][i];
                    int vb = hyperedge2node[h][j];
                    string key;
                    if (va > vb){
                        key = to_string(vb) + "_" + to_string(va);
                    }
                    else{
                        key = to_string(va) + "_" + to_string(vb);
                    }
                    tmp[key]++;
                }
            }
        }
        for(auto elem : tmp){
            pairdeg[elem.second]++;
        }
        tmp.clear();
        // output file
        ofstream resultFile(pd_name.c_str());
        for (auto item : pairdeg){
            resultFile << to_string(item.first) << "," << to_string(item.second) << endl;
        }
        resultFile.close();
    }
    this->attr_list["pairdeg"] = pairdeg;
    pairdeg.clear();
}

Attribute::Attribute(vector<vector<int>> node2hyperedge, vector<vector<int>> hyperedge2node, set<int> hset, vector<int> &node_degree, string eval_opt){
    vector<int> _hset; // set -> vector
    vector<int> _nodeset;

    for (auto h: hset){
        _hset.push_back(h);
        for (auto v: hyperedge2node[h]){
            if (find(_nodeset.begin(), _nodeset.end(), v) == _nodeset.end()){
                _nodeset.push_back(v);
            }
        }
    }
    sort(_hset.begin(), _hset.end());

    // degree
    unordered_map<int, long long> degree;
    unordered_map<int, double> degree_rw;
    for(auto h : hset){
        int hsize = (int)hyperedge2node[h].size();
        for(int i = 0 ; i < hsize ; i++){
            int v = hyperedge2node[h][i];
            node_degree[v]++;
        }
    }
    for (auto v : _nodeset){
        int d = node_degree[v];
        if (d != 0){
            degree[d]++;
            degree_rw[d]=degree_rw[d]+1.0/d;
        }
        assert(d <= (int)hyperedge2node.size());
    }
    this->attr_list["degree"] = degree;
    this->attr_list_rw["degree"] = degree_rw;
    degree.clear();
    degree_rw.clear();
    if (eval_opt.compare("avg") == 0){
        // size
        unordered_map<int, long long> size;
        unordered_map<int, double> size_rw;
        for (auto h : _hset){
            int hsize = (int)hyperedge2node[h].size();
            size[hsize]++;
            size_rw[hsize]=size_rw[hsize]+1.0/hsize;
        }
        this->attr_list["size"] = size;
        this->attr_list_rw["size"] = size_rw;

        size.clear();
        size_rw.clear();

        // For Pair
        unordered_map<string, long long> tmp;
         unordered_map<string, double> tmp_rw;
        
        // intersect
        unordered_map<int, long long> intersect;
        unordered_map<int, double> intersect_rw;
        for (int ha_idx = 0; ha_idx < (int)_hset.size() ; ha_idx++){
            int ha = _hset[ha_idx];
            int hsize = (int)hyperedge2node[ha].size();
            for (int i = 0 ; i < hsize ; i++){
                int v = hyperedge2node[ha][i];
                int deg = (int)node2hyperedge[v].size();
                for (int j = 0 ; j < deg; j++){
                    int hb = node2hyperedge[v][j];
                    int hsize_2=(int)hyperedge2node[hb].size();
                    if ( (hset.find(hb) != hset.end()) && (ha < hb)){
                        string key = to_string(ha) + "_" + to_string(hb);
                        tmp[key]++;
                        tmp_rw[key]=tmp_rw[key]+1.0/(hsize*hsize_2);
                    }
                }
            }
        }
        for (auto item : tmp){
            intersect[item.second]++;
            intersect_rw[item.second]=tmp_rw[item.first]+intersect_rw[item.second];
        }
        tmp.clear();
         tmp_rw.clear();
        this->attr_list["intersect"] = intersect;
        this->attr_list_rw["intersect"] = intersect_rw;
        intersect.clear();
        intersect_rw.clear();

        // pairdeg
        unordered_map<int, long long> pairdeg;
        unordered_map<int, double> pairdeg_rw;
        for(int h_idx = 0 ; h_idx < (int)_hset.size() ; h_idx++){
            int h = _hset[h_idx];
            int hsize = (int)hyperedge2node[h].size();
            for(int i = 0 ; i < hsize ; i++){
                for(int j = (i + 1) ; j < hsize ; j++){
                    int va = hyperedge2node[h][i];
                    int vb = hyperedge2node[h][j];
                    string key;
                    if (va > vb){
                        key = to_string(vb) + "_" + to_string(va);
                    }
                    else{
                        key = to_string(va) + "_" + to_string(vb);
                    }
                    tmp[key]++;
                    tmp_rw[key]=tmp_rw[key]+1/hsize;
                }
            }
        }
        for(auto elem : tmp){
            pairdeg[elem.second]++;
            pairdeg_rw[elem.second]=pairdeg_rw[elem.second]+tmp_rw[elem.first];
        }
        tmp.clear();
        tmp_rw.clear();
        this->attr_list["pairdeg"] = pairdeg;
        this->attr_list_rw["pairdeg"] = pairdeg_rw;
        pairdeg.clear();
        pairdeg_rw.clear();
    }
}

Attribute::Attribute(Attribute *attr){
    unordered_map<int, long long> degree;
    unordered_map<int, long long> size;
    unordered_map<int, long long> intersect;
    unordered_map<int, long long> pairdeg;

    for (auto dist : attr->attr_list){
        string dist_name = dist.first;
        if (dist_name.compare("degree") == 0){
            assert((int)attr->attr_list.size() > 0);
            for(auto d : attr->attr_list["degree"]){
                degree[d.first] = d.second;
            }
            this->attr_list["degree"] = degree;
            degree.clear();
        }
        else if (dist_name.compare("size") == 0){
            for(auto d : attr->attr_list["size"]){
                size[d.first] = d.second;
            }
            this->attr_list["size"] = size;
            size.clear();
        }
        else if (dist_name.compare("intersect") == 0){
            for(auto d : attr->attr_list["intersect"]){
                intersect[d.first] = d.second;
            }
            this->attr_list["intersect"] = intersect;
            intersect.clear();
        }
        else if (dist_name.compare("pairdeg") == 0){
            for(auto d : attr->attr_list["pairdeg"]){
                pairdeg[d.first] = d.second;
            }
            this->attr_list["pairdeg"] = pairdeg;
            pairdeg.clear();
        }
    }
}

void Attribute::set_attribute(vector<vector<int>> node2hyperedge, vector<vector<int>> hyperedge2node, vector<int> hset, vector<int> &node_degree, string eval_opt){
    vector<int> _nodeset;
    fill(node_degree.begin(), node_degree.end(), 0);

    for (auto h: hset){
        for (auto v: hyperedge2node[h]){
            if (find(_nodeset.begin(), _nodeset.end(), v) == _nodeset.end()){
                _nodeset.push_back(v);
            }
        }
    }

    for (auto dist : attr_list){
        auto &d = dist.second;
        d.clear();
    }
    
    unordered_map<int, long long> degree;
    unordered_map<int, double> degree_rw;
    for(auto h : hset){
        int hsize = (int)hyperedge2node[h].size();
        for(int i = 0 ; i < hsize ; i++){
            int v = hyperedge2node[h][i];
            node_degree[v]++;
        }
    }
    for (auto v : _nodeset){
        int d = node_degree[v];
        if (d != 0){
            degree[d]++;
            degree_rw[d]=degree_rw[d]+1.0/d;
        }
        assert(d <= (int)hyperedge2node.size());
    }
    this->attr_list["degree"] = degree;
    this->attr_list_rw["degree"] = degree_rw;
    degree.clear();
    degree_rw.clear();

    if (eval_opt.compare("avg") == 0){
        // size
        unordered_map<int, long long> size;
        unordered_map<int, double> size_rw;
        for (auto h : hset){
            int hsize = (int)hyperedge2node[h].size();
            size[hsize]++;
            size_rw[hsize]=size_rw[hsize]+1.0/hsize;
        }
        this->attr_list["size"] = size;
        this->attr_list_rw["size"] = size_rw;

        size.clear();
        size_rw.clear();

        // For Pair
        unordered_map<string, long long> tmp;
         unordered_map<string, double> tmp_rw;
        
        // intersect
        unordered_map<int, long long> intersect;
        unordered_map<int, double> intersect_rw;
        for (int ha_idx = 0; ha_idx < (int)hset.size() ; ha_idx++){
            int ha = hset[ha_idx];
            int hsize = (int)hyperedge2node[ha].size();
            for (int i = 0 ; i < hsize ; i++){
                int v = hyperedge2node[ha][i];
                int deg = (int)node2hyperedge[v].size();
                for (int j = 0 ; j < deg; j++){
                    int hb = node2hyperedge[v][j];
                    int hsize_2=(int)hyperedge2node[hb].size();
                     auto it = find(hset.begin(), hset.end(), hb);
                   if ( (it != hset.end()) && (ha < hb)){
                        string key = to_string(ha) + "_" + to_string(hb);
                        tmp[key]++;
                        tmp_rw[key]=tmp_rw[key]+1.0/(hsize+hsize_2);
                    }
                }
            }
        }
        for (auto item : tmp){
            intersect[item.second]++;
            intersect_rw[item.second]=tmp_rw[item.first]+intersect_rw[item.second];
        }
        tmp.clear();
         tmp_rw.clear();
        this->attr_list["intersect"] = intersect;
        this->attr_list_rw["intersect"] = intersect_rw;
        intersect.clear();
        intersect_rw.clear();

        // pairdeg
        unordered_map<int, long long> pairdeg;
        unordered_map<int, double> pairdeg_rw;
        for(int h_idx = 0 ; h_idx < (int)hset.size() ; h_idx++){
            int h = hset[h_idx];
            int hsize = (int)hyperedge2node[h].size();
            for(int i = 0 ; i < hsize ; i++){
                for(int j = (i + 1) ; j < hsize ; j++){
                    int va = hyperedge2node[h][i];
                    int vb = hyperedge2node[h][j];
                    string key;
                    if (va > vb){
                        key = to_string(vb) + "_" + to_string(va);
                    }
                    else{
                        key = to_string(va) + "_" + to_string(vb);
                    }
                    tmp[key]++;
                    tmp_rw[key]=tmp_rw[key]+1/hsize;
                }
            }
        }
        for(auto elem : tmp){
            pairdeg[elem.second]++;
            pairdeg_rw[elem.second]=pairdeg[elem.second]+tmp_rw[elem.first];
        }
        tmp.clear();
        tmp_rw.clear();
        this->attr_list["pairdeg"] = pairdeg;
        this->attr_list_rw["pairdeg"] = pairdeg_rw;
        pairdeg.clear();
        pairdeg_rw.clear();
    }
}

void Attribute::set_attribute_deg_rwrw(vector<vector<int>> node2hyperedge, vector<int> nodeset, map<int,int> add_node_size)
{
     unordered_map<int, double> degree_rw;
    for(auto h :nodeset){
        int hsize = (int)node2hyperedge[h].size();
        int as=add_node_size[h];
        degree_rw[hsize]=degree_rw[hsize]+1.0*as/hsize;
    }
    this->attr_list_rw["degree"] = degree_rw;
}

void Attribute::set_attribute_rw(vector<vector<int>> node2hyperedge, vector<vector<int>> hyperedge2node, vector<int> hset, map<int,int> add_size,vector<int> &node_degree, vector<float> &node_degree_size,string eval_opt,vector<int> nodeset, map<int,int> add_node_size){
    vector<int> _nodeset;
     unordered_map<int, long long> degree;
     
    fill(node_degree.begin(), node_degree.end(), 0);
    fill(node_degree_size.begin(), node_degree_size.end(), 0.000);

    for (auto h: hset){
        for (auto v: hyperedge2node[h]){
            if (find(_nodeset.begin(), _nodeset.end(), v) == _nodeset.end()){
                _nodeset.push_back(v);
            }
        }
    }

    for (auto dist : attr_list){
        auto &d = dist.second;
        d.clear();
    }
    
    unordered_map<int, double> degree_rw;
    for(auto h :nodeset){
        int hsize = (int)node2hyperedge[h].size();
        int as=add_node_size[h];
        degree_rw[hsize]=degree_rw[hsize]+1.0*as/hsize;
        degree[hsize]=degree[hsize]+1;
    }
    this->attr_list_rw["degree"] = degree_rw;
    //this->attr_list["degree"] = degree;
  /*  unordered_map<int, long long> degree;
    unordered_map<int, double> degree_rw;
    for(auto h : hset){
        int hsize = (int)hyperedge2node[h].size();
        int as=add_size[h];
        for(int i = 0 ; i < hsize ; i++){
            int v = hyperedge2node[h][i];
            node_degree[v]++;
            node_degree_size[v]=node_degree_size[v]+1.0*as/hsize;
        }
    }
    for (auto v : _nodeset){
        int d = node_degree[v];
        if (d != 0){
            degree[d]++;
            //degree_rw[d]=degree_rw[d]+1.0/d;
            degree_rw[d]=degree_rw[d]+node_degree_size[v];
          // degree_rw[d]=degree_rw[d]+1.0;
        }
        assert(d <= (int)hyperedge2node.size());
    }
    this->attr_list["degree"] = degree;
    this->attr_list_rw["degree"] = degree_rw;
    degree.clear();
    degree_rw.clear();
*/
    if (eval_opt.compare("avg") == 0){
        // size
        unordered_map<int, long long> size;
        unordered_map<int, double> size_rw;
        for (auto h : hset){
            int hsize = (int)hyperedge2node[h].size();
            int as=add_size[h];
            size[hsize]++;
            size_rw[hsize]=size_rw[hsize]+1.0*as/hsize;
          // size_rw[hsize]=size_rw[hsize]+1.0/hsize;
        }
        this->attr_list["size"] = size;
        this->attr_list_rw["size"] = size_rw;

        size.clear();
        size_rw.clear();

        // For Pair
        unordered_map<string, long long> tmp;
         unordered_map<string, double> tmp_rw;
        
        // intersect
        unordered_map<int, long long> intersect;
        unordered_map<int, double> intersect_rw;
        for (int ha_idx = 0; ha_idx < (int)hset.size() ; ha_idx++){
            int ha = hset[ha_idx];
            int hsize = (int)hyperedge2node[ha].size();
            int as=add_size[ha];
            for (int i = 0 ; i < hsize ; i++){
                int v = hyperedge2node[ha][i];
                int deg = (int)node2hyperedge[v].size();
                for (int j = 0 ; j < deg; j++){
                    int hb = node2hyperedge[v][j];
                    int hsize_2=(int)hyperedge2node[hb].size();
                    int as_2=add_size[hb];
                     auto it = find(hset.begin(), hset.end(), hb);
                   if ( (it != hset.end()) && (ha < hb)){
                        string key = to_string(ha) + "_" + to_string(hb);
                        tmp[key]++;
                        tmp_rw[key]=tmp_rw[key]+(1.0*as*as_2)/(hsize*hsize_2);
                    }
                }
            }
        }
        for (auto item : tmp){
            intersect[item.second]++;
            intersect_rw[item.second]=tmp_rw[item.first]+intersect_rw[item.second];
        }
        tmp.clear();
         tmp_rw.clear();
        this->attr_list["intersect"] = intersect;
        this->attr_list_rw["intersect"] = intersect_rw;
        intersect.clear();
        intersect_rw.clear();

        // pairdeg
        unordered_map<int, long long> pairdeg;
        unordered_map<int, double> pairdeg_rw;
        for(int h_idx = 0 ; h_idx < (int)hset.size() ; h_idx++){
            int h = hset[h_idx];
            int hsize = (int)hyperedge2node[h].size();
            int as=add_size[h];
            for(int i = 0 ; i < hsize ; i++){
                for(int j = (i + 1) ; j < hsize ; j++){
                    int va = hyperedge2node[h][i];
                    int vb = hyperedge2node[h][j];
                    string key;
                    if (va > vb){
                        key = to_string(vb) + "_" + to_string(va);
                    }
                    else{
                        key = to_string(va) + "_" + to_string(vb);
                    }
                    tmp[key]++;
                    tmp_rw[key]=tmp_rw[key]+1.0*as/hsize;
                }
            }
        }
        for(auto elem : tmp){
            pairdeg[elem.second]++;
            pairdeg_rw[elem.second]= pairdeg_rw[elem.second]+tmp_rw[elem.first];
        }
        tmp.clear();
        tmp_rw.clear();
        this->attr_list["pairdeg"] = pairdeg;
        this->attr_list_rw["pairdeg"] = pairdeg_rw;
        pairdeg.clear();
        pairdeg_rw.clear();
    }
}


//修改to_textfile
void Attribute::to_textfile(string output_dir){
    for (auto a: attr_list_rw){
        string writeFile  = output_dir + a.first + "_dist_skiphrw.txt";
        ofstream resultFile(writeFile.c_str());
        resultFile << a.first << ",value" << endl;
        double sum_val = 0.0;
        for (auto d: a.second){
            sum_val += d.second;
        }
        for (auto d: a.second){
            resultFile << to_string(d.first) << "," << to_string(d.second / sum_val) << endl;
        }
        resultFile.close();
    }
}

bool Attribute::operator==(Attribute& attr_p) const{
    for (auto dist : this->attr_list){
        string dist_name = dist.first;
        for (auto d: dist.second){
            int k = d.first;
            int v = d.second;
            if (v != attr_p.attr_list[dist_name][k]) return false;
        }
        for (auto d: attr_p.attr_list[dist_name]){
            int k = d.first;
            int v = d.second;
            if (dist.second[k] != v) return false;
        }
    }
    return true;
}