/*Biti Polyxeni 2582*/

#ifndef _GRAPH_HPP_ 
#define _GRAPH_HPP_

#include <list>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <algorithm>
#include "NegativeGraphCycle.hpp"


using namespace std;

template<typename T>
struct Edge {
  T from;
  T to;
  int dist;
  Edge(T f, T t, int d): from(f), to(t), dist(d) {
  }
  bool operator<(const Edge<T>& e) const;
  bool operator>(const Edge<T>& e) const;
  template<typename U>
  friend std::ostream& operator<<(std::ostream& out, const Edge<U>& e);
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Edge<T>& e) {
  out << e.from << " -- " << e.to << " (" << e.dist << ")";
  return out;
}

template <typename T>
class Graph {
  struct Vertex{
    int visited = 0;
    T vtx_info;
    int vtx_num = 0; //every vertex has a number assigned
    vector<Edge<T>> vertex_edge; //every vertex has an edge<T> vector in which I save all the edges tha start from this vertex
  };
  vector<Vertex> vertex_table; //the main graph is a Vertex vector
  bool directed;
  
public:
  Graph(bool isDirectedGraph = true, int capacity = 2);
  bool contains(const T& info);
  bool addVtx(const T& info);
  bool rmvVtx(const T& info);
  bool addEdg(const T& from, const T& to, int distance);
  bool rmvEdg(const T& from, const T& to);
  list<T> dfs(const T& info) const;
  list<T> bfs(const T& info) const;
  list<Edge<T>> mst();
  
  void print2DotFile(const char *filename) const;
  list<T> dijkstra(const T& from, const T& to);
  list<T> bellman_ford(const T& from, const T& to);
  
private:
    list<T> dfsrecur(vector<Vertex> v, list<T> dfslist, Vertex vtx) const;
    list<Edge<T>> sort_list(list<Edge<T>> unsort);
    int minDistance(int dist[], bool sptSet[]);
    list<int> printPath(int parent[], int j, list<int> dj);
    void printSolution(int dist[], int n, int parent[], int from); 
    
    
};

template <typename T>
Graph<T>::Graph(bool isDirectedGraph, int capacity){
    vertex_table.reserve(capacity);
    
    directed = isDirectedGraph;
}

template <typename T>
bool Graph<T>::contains(const T& info){
    typename vector<Vertex>::iterator it = vertex_table.begin();
    typename vector<Vertex>::iterator ite = vertex_table.end();
    
    for(; it!=ite; it++){
        if((*it).vtx_info == info){return true;}
    }
    return false;
}

template <typename T> 
bool Graph<T>::addVtx(const T& info){
    
    if(Graph<T>::contains(info)){return false;}
    Vertex new_entry;
    
    new_entry.vtx_info = info;
    if(vertex_table.size()>0){new_entry.vtx_num = vertex_table[vertex_table.size() - 1].vtx_num + 1;} //assign a number to the new vertex
    vertex_table.push_back(new_entry); //add the new vertex to the graph
    
    if(Graph<T>::contains(info)){return true;}
    return false;
}

template <typename T>
bool Graph<T>::rmvVtx(const T& info){
    typename vector<Vertex>::iterator it = vertex_table.begin();
    typename vector<Vertex>::iterator ite = vertex_table.end();

    
    if(!(Graph<T>::contains(info))){return false;}
    
    for(; it!=ite; it++){
        if((*it).vtx_info == info){
           vertex_table.erase(it);
        }
    }
   
     if(!(Graph<T>::contains(info))){return true;}
     return false;
}

template <typename T>
bool Graph<T>::addEdg(const T& from, const T& to, int cost){
    if(!contains(from) || !contains(to)){return false;}
    
    typename vector<Vertex>::iterator it_v = vertex_table.begin();
    typename vector<Vertex>::iterator ite_v = vertex_table.end();
    
    for(; it_v!=ite_v; it_v++){
        if((*it_v).vtx_info == from){break;}
    }
    
    typename vector<Edge<T>>::iterator it_e = (*it_v).vertex_edge.begin();
    typename vector<Edge<T>>::iterator ite_e = (*it_v).vertex_edge.end();
    for(; it_e!=ite_e; it_e++){
        if((*it_e).to == to){return false;}
    }
    Edge<T> new_edge(from, to, cost); //create the new edge
    
    (*it_v).vertex_edge.push_back(new_edge); //add the new edge to the vertex it starts from

    if(directed==false){ //if the graph is not directed add the edge to the vertex it ends to
        it_v = vertex_table.begin();
        for(; it_v!=ite_v; it_v++){
            if((*it_v).vtx_info == to){break;}
        }
        it_e = (*it_v).vertex_edge.begin(); 
        ite_e = (*it_v).vertex_edge.end();
        
        for(; it_e!=ite_e; it_e++){
            if((*it_e).to == from){return false;}
        }
        Edge<T> new_edge(to, from, cost);

        (*it_v).vertex_edge.push_back(new_edge);
        
    }
    return true;
}

template <typename T>
bool Graph<T>::rmvEdg(const T& from, const T& to){
    if(!contains(from) || !contains(to)){return false;}
    
    typename vector<Vertex>::iterator it_v = vertex_table.begin();
    typename vector<Vertex>::iterator ite_v = vertex_table.end();
    
    for(; it_v!=ite_v; it_v++){
        if((*it_v).vtx_info == from){break;}
    }
//     int edge_tb_size = (int)(*it_v).vertex_edge.size();
    typename vector<Edge<T>>::iterator it_e = (*it_v).vertex_edge.begin();
    typename vector<Edge<T>>::iterator ite_e = (*it_v).vertex_edge.end();
    
    for(; it_e!=ite_e; it_e++){
        if((*it_e).to == to){
            (*it_v).vertex_edge.erase(it_e);
            break;
        }
    }
    if(it_e == ite_e){return false;}
    
    if(directed==false){
        it_v = vertex_table.begin();
        
        for(; it_v!=ite_v; it_v++){
            if((*it_v).vtx_info == to){break;}
        }
        it_e = (*it_v).vertex_edge.begin();
        ite_e = (*it_v).vertex_edge.end();
        
        for(; it_e!=ite_e; it_e++){
            if((*it_e).to == from){
                (*it_v).vertex_edge.erase(it_e);
                break;
            }
        }
        if(it_e == ite_e){return false;}
    }
    return true;
}

template <typename T>
list<T> Graph<T>::dfsrecur(vector<Vertex> v, list<T> dfs_list, Vertex ver) const{
    typename vector<Vertex>::iterator it_v = v.begin();
    typename vector<Vertex>::iterator ite_v = v.end();
    
    for(; it_v!=ite_v; it_v++){ //mark vertex as visited
        if((*it_v).vtx_info == ver.vtx_info){
            (*it_v).visited = 1;
        }
    }
    
    dfs_list.push_back(ver.vtx_info); //add visited vertex to the final list
    
    typename vector<Edge<T>>::iterator it_e = ver.vertex_edge.begin();
    typename vector<Edge<T>>::iterator ite_e = ver.vertex_edge.end();
    
  //check vertex's edges to find the next unvisited vertex
    it_v = v.begin();
    for(; it_v!=ite_v; it_v++){
        it_e = ver.vertex_edge.begin();
        for(; it_e!=ite_e; it_e++){
            if((*it_e).to == (*it_v).vtx_info && (*it_v).visited==0 && !(std::find(dfs_list.begin(), dfs_list.end(), (*it_v).vtx_info)!= dfs_list.end())){
                dfs_list = dfsrecur(v, dfs_list, (*it_v));
            }
        }
    }
    return dfs_list;
}

template <typename T>
list<T> Graph<T>::dfs(const T& info) const{
    typename vector<Vertex>::const_iterator it_v = vertex_table.begin();
    typename vector<Vertex>::const_iterator ite_v = vertex_table.end();
    list<T> dfs_list;
    vector<Vertex> res;
    
    for(; it_v!=ite_v; it_v++){ 
        res.push_back((*it_v)); //the res vector is a helper vector I use to check if a vertex is visited
        
    }
    
    typename vector<Vertex>::iterator res_it = res.begin();
    typename vector<Vertex>::iterator res_ite = res.end();
    
    for(; res_it!=res_ite; res_it++){
        (*res_it).visited = 0; //initialize all vertexes as non visited
    }
    
    
    it_v = vertex_table.begin();
    res_it = res.begin();
    
    for(; it_v!=ite_v; it_v++, res_it++){
        if((*it_v).vtx_info == info && (*res_it).visited == 0){ //start the dfs from given vertex
           dfs_list =  dfsrecur(res, dfs_list, (*res_it)); //the recursive dfs function 
        }
    }
    return dfs_list;
}

template <typename T>
list<T> Graph<T>::bfs(const T& info) const{
    typename vector<Vertex>::const_iterator it_v = vertex_table.begin();
    typename vector<Vertex>::const_iterator ite_v = vertex_table.end();
    
    list<T> bfs_queue, bfs_list;
    vector<Vertex> res; //helper vector to mark visited vertexes
    Vertex v_info;
    
    for(; it_v!=ite_v; it_v++){
        res.push_back((*it_v));
    }
    typename vector<Vertex>::iterator res_it = res.begin();
    typename vector<Vertex>::iterator res_ite = res.end();
    
    for(; res_it!=res_ite; res_it++){
        (*res_it).visited = 0;
    }
    
    it_v = vertex_table.begin();
    res_it = res.begin();
    
    for(; it_v!=ite_v; it_v++,res_it++){
        if((*it_v).vtx_info == info){(*res_it).visited=1; break;}} //visit the vertex given as a start
    bfs_queue.push_back((*res_it).vtx_info);
    
    while(!bfs_queue.empty()){ 
        
        v_info.vtx_info = bfs_queue.front(); //get the first vertex from the priority queue
        bfs_list.push_back(v_info.vtx_info); //put it in the final bfs list
        bfs_queue.pop_front(); //remove it from the queue
        
        res_it = res.begin();
        
        for(; res_it!=res_ite; res_it++){
            if((*res_it).vtx_info == v_info.vtx_info){break;}
        }
        v_info = (*res_it);
        
        typename vector<Edge<T>>::iterator beg_edge = v_info.vertex_edge.begin();
        typename vector<Edge<T>>::iterator end_edge = v_info.vertex_edge.end();
        
        res_it = res.begin();
        
        for(; res_it!=res_ite; res_it++){
            beg_edge = v_info.vertex_edge.begin();
            for(; beg_edge!=end_edge; beg_edge++){ //add to the queue the non visited vertexes that share an edge with the vertex that had just been visited
                if((*res_it).vtx_info==(*beg_edge).to && (*res_it).visited == 0 && !(std::find(bfs_list.begin(), bfs_list.end(), (*res_it).vtx_info)!= bfs_list.end())){
                    (*res_it).visited = 1;
                    bfs_queue.push_back((*res_it).vtx_info);
                    break;
                }
            }
        }
    }

    return bfs_list;
}

template <typename T>
list<Edge<T>> Graph<T>::sort_list(list<Edge<T>> unsort){
    list<Edge<T>> sorted;
    int mincost = __INT_MAX__;
    int count=0;
    int num_from, num_to, from_index, to_index;
    Edge<T> minedge(T(),T(),0);
    int list_size = unsort.size();
    
    typename list<Edge<T>>::iterator it = unsort.begin();
    typename list<Edge<T>>::iterator ite = unsort.end();
    
    typename vector<Vertex>::iterator it_v = vertex_table.begin();
    typename vector<Vertex>::iterator ite_v = vertex_table.end();
    
    while((unsigned)sorted.size()<(unsigned)list_size){
        it_v = vertex_table.begin();
        it = unsort.begin();
        count = 0;
        
        for(; it!=ite; it++){
            if((*it).dist < mincost){
                mincost = (*it).dist;
                minedge = (*it);
            }
        }
        it = unsort.begin();
        for(; it!=ite; it++){
            if((*it).from == minedge.from && (*it).to == minedge.to){unsort.erase(it); break;}
        }
        
        for(; it_v!=ite_v; it_v++){
            if((*it_v).vtx_info == minedge.from){num_from = (*it_v).vtx_num; from_index = count;}
            else if((*it_v).vtx_info == minedge.to){num_to = (*it_v).vtx_num; to_index = count;}
            count++;
        }
        if(num_from > num_to){
            minedge.from = vertex_table[to_index].vtx_info;
            minedge.to = vertex_table[from_index].vtx_info;
        }
        sorted.push_back(minedge);
        mincost = __INT_MAX__;
    }
    return sorted;
}

/*function that finds the mst
 * put all visited edges in a queue
 * and select the edge with the smallest distance
 */
template <typename T>
list<Edge<T>> Graph<T>::mst(){
    list<Edge<T>> mst_list;
    list<T> visited;
    vector<Edge<T>> edge_queue;
    Edge<T> minedge(T(),T(),0);
    int minval = __INT_MAX__;
    Vertex next;
    
    if(directed==true){return mst_list;}
    
    typename vector<Vertex>::iterator it_v = vertex_table.begin();
    typename vector<Vertex>::iterator ite_v = vertex_table.end();
    
    typename vector<Edge<T>>::iterator it_e = (*it_v).vertex_edge.begin();
    typename vector<Edge<T>>::iterator ite_e = (*it_v).vertex_edge.end();
    
    for(; it_e!=ite_e; it_e++){ //starting from the 1st vertex tha has been added to the graph
        if((*it_e).dist < minval){
            minval = (*it_e).dist; //find it's edge with the smallest distance
            minedge = (*it_e);
        }
        edge_queue.push_back((*it_e));
    }
    visited.push_back(vertex_table[0].vtx_info); //add the first vertex to the visited vertexes
    mst_list.push_back(minedge);
    next.vtx_info = minedge.to; //the next vertex to visit is the one at the end of the smallest edge
    
    typename vector<Edge<T>>::iterator eq_it = edge_queue.begin();
    typename vector<Edge<T>>::iterator eq_ite = edge_queue.end();
    
    for(; eq_it!=eq_ite; eq_it++){
        if((*eq_it).from == minedge.from && (*eq_it).to == minedge.to){edge_queue.erase(eq_it); break;} //after selecting the smallest edge from the queue erase it
    }
    
   //loop tha continues until all vertexes have been visited
   //basically does the same thing as the above but the vertex tha is being visited
   //is the one give from the smallest edge selected
    while(1){
        
        it_v = vertex_table.begin();
        for(; it_v!=ite_v; it_v++){
            if((*it_v).vtx_info == next.vtx_info){break;}
        }
        visited.push_back((*it_v).vtx_info);
        if(visited.size()==vertex_table.size()){break;}
        
        typename vector<Edge<T>>::iterator it_e = (*it_v).vertex_edge.begin();
        typename vector<Edge<T>>::iterator ite_e = (*it_v).vertex_edge.end();
        
        for(; it_e!=ite_e; it_e++){
            if(!(std::find(visited.begin(), visited.end(), (*it_e).to)!=visited.end())){
                edge_queue.push_back((*it_e));
            }
        }
        
        eq_it = edge_queue.begin();
        eq_ite = edge_queue.end();
        
        minval = __INT_MAX__;
        for(; eq_it!=eq_ite; eq_it++){
            if((*eq_it).dist < minval && !(std::find(visited.begin(), visited.end(), (*eq_it).to)!=visited.end())){
                minval = (*eq_it).dist;
                minedge = (*eq_it);
            }
        }
        mst_list.push_back(minedge);
        next.vtx_info = minedge.to;
        
        
        eq_it = edge_queue.begin();
        eq_ite = edge_queue.end();
        for(; eq_it!=eq_ite; eq_it++){
            if((*eq_it).from == minedge.from && (*eq_it).to == minedge.to){edge_queue.erase(eq_it); break;}
        }
    }
    
    mst_list = sort_list(mst_list);
    
    return mst_list;
}

template <typename T>


/*function used by dijkstra's algorithm
 * find the vertex with the min distance from source
 * that has not been visited
 */
int Graph<T>::minDistance(int dist[],  
                bool visited[]) 
{ 
      
    int min = __INT_MAX__, min_index; 
  
    for (int v = 0; v < (int)vertex_table.size(); v++) 
        if (visited[v] == false && dist[v] <= min) 
            min = dist[v], min_index = v; 
  
    return min_index; 
}

template <typename T>

/* function used by dijkstra and bellman_ford
 * puts all vertexes that form the path from source to destination
 * in a list
 */
list<int> Graph<T>::printPath(int parent[], int j, list<int> dij) 
{ 
      
    if (parent[j] == - 1 || parent[j]==j) 
        return dij; 
  
    dij = printPath(parent, parent[j], dij); 
  
    dij.push_back(j);
    
     return dij;
} 

template <typename T>
list<T> Graph<T>::dijkstra(const T& from, const T& to){
    list<T> final;
    list<int> dij_list;
    int Graph_Array [vertex_table.size()][vertex_table.size()];
    int to_num, from_num;
    int distance[vertex_table.size()], parent[(int)vertex_table.size()];
    bool included_vtx[vertex_table.size()];
    
    
    for(int i=0; i<(int)vertex_table.size(); i++){
        for(int j=0; j<(int)vertex_table.size(); j++){
            Graph_Array[i][j] = __INT_MAX__;
        }
    }
    
    typename vector<Vertex>::iterator it_vertex = vertex_table.begin();
    typename vector<Vertex>::iterator it_vertex_end = vertex_table.end();
    
    //create an 2-dimension array that holds the distances from one vertex to another
    for(; it_vertex!=it_vertex_end; it_vertex++){
        typename vector<Edge<T>>::iterator it_edge = (*it_vertex).vertex_edge.begin();
        typename vector<Edge<T>>::iterator it_edge_end = (*it_vertex).vertex_edge.end();
        
        from_num = (*it_vertex).vtx_num;
        
        for(; it_edge!=it_edge_end; it_edge++){
           typename vector<Vertex>::iterator helper_it = vertex_table.begin();
           for(; helper_it!=it_vertex_end; helper_it++){
               if((*it_edge).to == (*helper_it).vtx_info){to_num = (*helper_it).vtx_num; break;}
            }
            Graph_Array[from_num][to_num] = (*it_edge).dist;
        }
    }
    
    it_vertex = vertex_table.begin();
    for(; it_vertex!=it_vertex_end; it_vertex++){
        if((*it_vertex).vtx_info == from){from_num = (*it_vertex).vtx_num;}
        else if((*it_vertex).vtx_info == to){to_num = (*it_vertex).vtx_num;}
    }
    
    for(int i=0; i<(int)vertex_table.size(); i++){
        //parent has the previously visited vertex which means that it shows the previous vertex in the requested path
        parent[i]=i;
        //distance from source vertex
        distance[i] = __INT_MAX__;
        //vertexes included in the path
        included_vtx[i] = false;
    }
    //source's parent and distance from itself
    parent[from_num]=-1;
    distance[from_num]=0;
    
    
    //find shortest path from source vertex to all other vertexes
    for(int count = 0; count < (int)vertex_table.size()-1; count++){
        int u = minDistance(distance, included_vtx);
        included_vtx[u]=true;
        
        for(int k=0; k<(int)vertex_table.size(); k++){
             if (!included_vtx[k] && (Graph_Array[u][k]!=__INT_MAX__) && (distance[u] + Graph_Array[u][k] < distance[k])) {
                
                parent[k]=u;
                distance[k] = distance[u] + Graph_Array[u][k];
            }
        }
    }
    //return the list with the path
    dij_list = printPath(parent, to_num, dij_list);
    
     if(!dij_list.empty() && parent[dij_list.front()]!=from_num){ return final;}
    
    if(!dij_list.empty()){dij_list.push_front(from_num);}
    
    typename list<int>::iterator it = dij_list.begin();
    typename list<int>::iterator ite = dij_list.end();
    
    //the list at first has the numbers assigned to the vertexes
    //here I create the final list by converting the numbers
    //to the vertex <T> info
    if(!dij_list.empty()){
        for(; it!=ite; it++){
            it_vertex = vertex_table.begin();
            for(; it_vertex!=it_vertex_end; it_vertex++){
                if((*it_vertex).vtx_num == (*it)){
                    final.push_back((*it_vertex).vtx_info);
                    break;
                }
            }
        }
    }
    return final;
}

template <typename T>
list<T> Graph<T>::bellman_ford(const T& from, const T& to){
    list<int> bflist;
    list<T> final;
    int distance[(int)vertex_table.size()], parent[(int)vertex_table.size()];
    int Graph_Array [vertex_table.size()][vertex_table.size()];
    int from_num, to_num, count=0;
    NegativeGraphCycle ex;
    
    for(int i=0; i<(int)vertex_table.size(); i++){
        for(int j=0; j<(int)vertex_table.size(); j++){
            Graph_Array[i][j] = __INT_MAX__;
        }
    }
    
    typename vector<Vertex>::iterator it_vertex = vertex_table.begin();
    typename vector<Vertex>::iterator it_vertex_end = vertex_table.end();
    
    //create an 2-dimension array that holds the distances from one vertex to another
    for(; it_vertex!=it_vertex_end; it_vertex++){
        typename vector<Edge<T>>::iterator it_edge = (*it_vertex).vertex_edge.begin();
        typename vector<Edge<T>>::iterator it_edge_end = (*it_vertex).vertex_edge.end();
        
        from_num = (*it_vertex).vtx_num;
        
        for(; it_edge!=it_edge_end; it_edge++){
           typename vector<Vertex>::iterator helper_it = vertex_table.begin();
           for(; helper_it!=it_vertex_end; helper_it++){
               if((*it_edge).to == (*helper_it).vtx_info){to_num = (*helper_it).vtx_num; break;}
            }
            Graph_Array[from_num][to_num] = (*it_edge).dist;
        }
    }
    
    it_vertex = vertex_table.begin();
    for(; it_vertex!=it_vertex_end; it_vertex++){
        if((*it_vertex).vtx_info == from){from_num = (*it_vertex).vtx_num;}
        else if((*it_vertex).vtx_info == to){to_num = (*it_vertex).vtx_num;}
    }
    
    for(int i=0; i<(int)vertex_table.size(); i++){
        //parent has the previously visited vertex which means that it shows the previous vertex in the requested path
        parent[i]=i;
        //distance from source vertex
        distance[i] = __INT_MAX__;
    }
    //source's parent and distance from itself
    parent[from_num]=-1;
    distance[from_num]=0;
    
    //find shortest path from source vertex to all other vertexes
    while(count < (int)vertex_table.size()){
        for(int u=0; u < (int)vertex_table.size(); u++){
            for(int v=0; v < (int)vertex_table.size(); v++){
                if(distance[u] != __INT_MAX__ && Graph_Array[u][v]!=__INT_MAX__ && distance[u]+Graph_Array[u][v] < distance[v]){
                    
                    distance[v] = distance[u] + Graph_Array[u][v];
                    parent[v] = u;
                    
                }
            }
        }
        count++;
    }
    //check for negative cycle and throw exception
    for(int i=0; i<(int)vertex_table.size(); i++){
        for(int j=0; j<(int)vertex_table.size(); j++){
            if(distance[i] != __INT_MAX__ && Graph_Array[i][j] != __INT_MAX__ && distance[i] + Graph_Array[i][j] < distance[j]){
                throw ex;
                return final;
            }
        }
    }
    
    bflist = printPath(parent, to_num, bflist);
    
     if(!bflist.empty() && parent[bflist.front()]!=from_num){return final;}
    
    if(!bflist.empty()){bflist.push_front(from_num);}
    
    typename list<int>::iterator it = bflist.begin();
    typename list<int>::iterator ite = bflist.end();
    
    //the list at first has the numbers assigned to the vertexes
    //here I create the final list by converting the numbers
    //to the vertex <T> info
    if(!bflist.empty()){
        for(; it!=ite; it++){
            it_vertex = vertex_table.begin();
            for(; it_vertex!=it_vertex_end; it_vertex++){
                if((*it_vertex).vtx_num == (*it)){
                    final.push_back((*it_vertex).vtx_info);
                    break;
                }
            }
        }
    }
    return final;
}

template <typename T>
void Graph<T>::print2DotFile(const char *filename) const{}

#endif
