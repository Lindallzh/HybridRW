# include "algorithmRW.hpp"
struct pairhash
{
    public:
      template <typename T, typename U>
      std::size_t operator()(const std::pair<T,U> &x) const
      {
          return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
      }
};

Algorithm_RW::Algorithm_RW(string eval_opt, string algo_opt, string outputdir, HyperGraph *graph, double restart, int maxlength){
    this->eval_opt = eval_opt;
    this->algo_opt = algo_opt;
    this->outputdir = outputdir;
    this->graph = graph;
    this->restart = restart;
    this->givenmaxlength = maxlength;
    this->node2node.resize(graph->number_of_nodes);

    if (algo_opt.compare(0, 4, "rw_c") == 0){
        for(int va = 0 ; va < graph->number_of_nodes ; va++){
            for(auto h : graph->node2hyperedge[va]){
                int hsize = (int)graph->hyperedge2node[h].size();
                for(int i = 0 ;  i < hsize; i++){
                    int vb = graph->hyperedge2node[h][i];
                    if(va != vb){
                        this->node2node[va].push_back(vb);
                        this->node2node[vb].push_back(va);
                    }
                }
            }
        }
    }
}

void Algorithm_RW::walk(int seed, int max_length, set<int> &pool, int remain){
    default_random_engine e;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, 1);
    queue<int> q;
    vector<bool> visited(graph->number_of_nodes, false);
    
    int current_remain = remain;
    q.push(seed);
    int step = 0;
    while ((step < max_length) && (q.empty() == false)){
        int current = q.front();
        q.pop();
        if(!check[current]){
            check[current] = true;
            int deg = (int)graph->node2hyperedge[current].size();
            for (int hidx = 0 ; hidx < deg ; hidx++){
                int h = graph->node2hyperedge[current][hidx];
                htable[h]--;
                if (htable[h] == 0){
                    pool.insert(h);
                    current_remain--;
                    if (current_remain == 0){
                        break;
                    }
                }
            }
        }
        if (current_remain == 0){
            break;
        }
        double random_double = dist(gen);
        if(random_double < restart){
            q.push(seed);
        }
        else if ((int)node2node[current].size() != 0){
            int deg = (int)node2node[current].size();
            std::uniform_int_distribution<> dist2(0, deg-1);
            int random_index = dist2(gen);
            int next_node = node2node[current][random_index];
            q.push(next_node);
        }
        step++;
    }
}
//weighted-random walk
HSet* Algorithm_RW::run(double target_portion){
    int target_size = int(floor(graph->number_of_hedges * target_portion));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_node(0, graph->number_of_nodes-1);

    int max_length = givenmaxlength;
    if (givenmaxlength == -1){
        max_length = 100 * graph->number_of_nodes;            
    }
    else if (givenmaxlength == 1){
        max_length = graph->number_of_nodes;
    }

    auto start = chrono::steady_clock::now();
    
    set<int> initial_state;
    HSet *sampled = new HSet(initial_state, graph, eval_opt);
    htable.clear();
    check.clear();
    htable.resize(graph->number_of_hedges);
    for (int h = 0 ; h < graph->number_of_hedges ; h++){
        htable[h] = (int)graph->hyperedge2node[h].size();
    }
    check.resize(graph->number_of_nodes);
    
    //vector<int> pool;
    set<int> add;
    int remain;
    while((int)add.size() < target_size){
        cout << to_string((int)add.size()) << " / " << target_size << endl;
        string logfname = outputdir + "log.txt";
        ofstream logFile(logfname.c_str());
        logFile << to_string((int)add.size()) << "/" << to_string(target_size) << endl;
        logFile.close();

        int seed;
        seed = dist_node(gen);
        remain = target_size - (int)add.size();
        walk(seed, max_length, add, remain);
        unordered_map<pair<unsigned int, unsigned int>, vector<unsigned int>,pairhash> visited_path;
    }
    auto end = chrono::steady_clock::now();
    sampled->change_state(add, "+");
    sampled->timespent = std::chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time : " << to_string(sampled->timespent.count()) << endl;
    /*sampled->calculate_distribution(graph, eval_opt);
    sampled->evaluate(graph->attr);

    cout << "[ Result ]" << endl;
    for(auto e: sampled->evaluation){
        cout << e.first << ";\t";
    }
    cout << endl;
    for(auto e: sampled->evaluation){
        cout << to_string(e.second) << ";\t";
    }
    cout << endl;

    string writeFile = outputdir + "result.txt";
    ofstream resultFile(writeFile.c_str());
    resultFile << "[ Result ]" << endl;
    for(auto e: sampled->evaluation){
        resultFile << e.first << " : " << to_string(e.second) << endl;
    }
    resultFile << "Time : " << to_string(sampled->timespent.count()) << endl;
    resultFile.close();*/

    return sampled;
}
HSet* Algorithm_RW::run_he_node_nonback(double target_portion){
    int target_size = int(floor(graph->number_of_hedges * target_portion));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_node(0, graph->number_of_nodes-1);

    int max_length = givenmaxlength;
    if (givenmaxlength == -1){
        max_length = 100 * graph->number_of_nodes;            
    }
    else if (givenmaxlength == 1){
        max_length = graph->number_of_nodes;
    }

    auto start = chrono::steady_clock::now();
    
    set<int> initial_state;
    HSet *sampled = new HSet(initial_state, graph, eval_opt);
    htable.clear();
    check.clear();
    htable.resize(graph->number_of_hedges);
    for (int h = 0 ; h < graph->number_of_hedges ; h++){
        htable[h] = (int)graph->hyperedge2node[h].size();
    }
    check.resize(graph->number_of_nodes);

    
    vector<int> pool;
    set<int> add;
    map<int,int> add_size;

    set<int> add_node;
    map<int,int> add_node_size;
    int remain;
    int seed;
    seed = dist_node(gen);
    int current=seed;


      int he_deg = (int)graph->node2hyperedge[current].size();
       std::uniform_int_distribution<> distHe(0, he_deg-1);
       int random_he_index = distHe(gen);
       int h = graph->node2hyperedge[current][random_he_index];
       add.insert(h);
       int previous=h;

    if ((int)graph->hyperedge2node[h].size() != 0){
        int deg = (int)graph->hyperedge2node[h].size();
        std::uniform_int_distribution<> dist2(0, deg-1);
        int random_index = dist2(gen);
        current =(int)graph->hyperedge2node[h][random_index];     
      }
     else 
     {
          current= dist_node(gen);
     }
    int cnt_query=0;
    while((int)add.size() < target_size){
		cnt_query++;
     //cout << to_string((int)add.size()) << " / " << target_size << endl;
        //string logfname = outputdir + "log.txt";
       // ofstream logFile(logfname.c_str());
       // logFile << to_string((int)add.size()) << "/" << to_string(target_size) << endl;
       // logFile.close();
    //cout <<  current << endl;
    int vn=add_node.size();
       add_node.insert(current);
       int vm=add_node.size();
       if(vn!=vm)
       {
           add_node_size.insert(make_pair(current,1));
       }
       else
       {
           add_node_size[current]++;
       }

    he_deg = (int)graph->node2hyperedge[current].size();
    // cout <<  he_deg  << endl;
    //int h=0;
    //int random_he_index=0;
    if(he_deg<=2)
    {  
       current= dist_node(gen);
      // h = graph->node2hyperedge[current][0];

    }
    else
    {
        // int unsampled_nodeHe[he_deg-1]={0};
         int unsampled_nodeHe[he_deg-1];
             int j=0;
         int max[he_deg];
         max[0]=0;
         
             for(int t=0;t<he_deg;t++)
             {
                 if( graph->node2hyperedge[current][t]!=previous)
                 {
                     unsampled_nodeHe[j]=graph->node2hyperedge[current][t];
                     //cout <<  unsampled_nodeHe[j] << endl;
                     max[j+1]=max[j]+graph->hyperedge2node[unsampled_nodeHe[j]].size();
                     j++;
                 }
             }
          int total_m=max[he_deg];
          //std::uniform_int_distribution<> distHe(0, j-1);
          std::uniform_int_distribution<> distHe(0, total_m);
          int random_he_index_1 = distHe(gen);
          int sum=0;
          for(int t=0;t<he_deg;t++)
          {
           // sum=sum+max[t];
            if(max[t+1]>=random_he_index_1)
            {
                h = unsampled_nodeHe[t]; 
                break;
            }
          }
    //add.insert(h);
     int m=add.size();
                add.insert(h);
                int n=add.size();
                if(m!=n)
                {
                    add_size.insert(make_pair(h,1));
                }
                else
                {
                    add_size[h]++;
                }
    previous=h; 
 if ((int)graph->hyperedge2node[h].size() >2){
        int deg = (int)graph->hyperedge2node[h].size();
        std::uniform_int_distribution<> dist2(0, deg-1);
        int random_index = dist2(gen);
        current = graph->hyperedge2node[h][random_index];

    }
    else 
     {
          current= dist_node(gen);
     }

    }
    }
    auto end = chrono::steady_clock::now();
    sampled->change_state(add, "+");
    sampled->timespent = std::chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time : " << to_string(sampled->timespent.count()) << endl;
    cout << "Query : " << cnt_query << endl;
    //sampled->calculate_distribution(graph, eval_opt);
    // sampled->calculate_distribution_rw(graph, add_size, eval_opt);
   /* sampled->calculate_distribution_node_rw(graph, add_node, add_node_size,add_size, eval_opt);
    sampled->evaluate_rw(graph->attr);

    cout << "[ Result ]" << endl;
    for(auto e: sampled->evaluation){
        cout << e.first << ";\t";
    }
    cout << endl;
    for(auto e: sampled->evaluation){
        cout << to_string(e.second) << ";\t";
    }
    cout << endl;

    string writeFile = outputdir + "result.txt";
    ofstream resultFile(writeFile.c_str());
    resultFile << "[ Result ]" << endl;
    for(auto e: sampled->evaluation){
        resultFile << e.first << " : " << to_string(e.second) << endl;
    }
    resultFile << "Time : " << to_string(sampled->timespent.count()) << endl;
    resultFile.close();*/

    return sampled;
}
//common neighbor 



//skip Triangles
file:///D:/paper/%E5%8D%9A%E5%90%8E%E8%AE%BA%E6%96%87/ICDE-2023/source%20code/HybridRWHG/src/algorithmRW.cpp
file:///D:/paper/%E5%8D%9A%E5%90%8E%E8%AE%BA%E6%96%87/ICDE-2023/source%20code/HybridRWHG/src/algorithmRW.cpp


