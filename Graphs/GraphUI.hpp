#ifndef _GRAPH_UI_
#define _GRAPH_UI_

#include"Graph.hpp"
#include"NegativeGraphCycle.hpp"
#include <string>       
#include <iostream>     
#include <sstream>      

template <typename T>
int graphUI() {
  
  string option, line;
  int distance;
  bool digraph = false;
  
  cin >> option;
  if(!option.compare("digraph"))
    digraph = true;
  Graph<T> g(digraph);
  
  while(true) {
    
    std::stringstream stream;
    cin >> option;
    
    if(!option.compare("av")) {
      getline(std::cin, line);
      stream << line;
      T vtx(stream);
      if(g.addVtx(vtx))
        cout << "av " << vtx << " OK\n";
      else
        cout << "av " << vtx << " NOK\n";
    }
    else if(!option.compare("rv")) {
      getline(std::cin, line);
      stream << line;
      T vtx(stream);
      if(g.rmvVtx(vtx))
        cout << "rv " << vtx << " OK\n";
      else
        cout << "rv " << vtx << " NOK\n";
    }
    else if(!option.compare("ae")) {
        string from, to;
//         int distance;
        
        getline(std::cin, line);
        stream << line;
        
        T vtxf(stream);
        T vtxt(stream);
        stream >> distance;
        
        if(g.addEdg(vtxf, vtxt, distance))
             cout << "ae " << vtxf << " " << vtxt << " OK\n";
        else
             cout << "ae " << vtxf << " " << vtxt << " NOK\n";
        
    }
    else if(!option.compare("re")) {
        string from, to;
        
        getline(std::cin, line);
        stream << line;
        
        T vtxf(stream);
        T vtxt(stream);
        
        if(g.rmvEdg(vtxf, vtxt))
             cout << "ae " << vtxf << " " << vtxt << " OK\n";
        else
             cout << "ae " << vtxf << " " << vtxt << " NOK\n";
    }
    else if(!option.compare("dot")) {
      
    }
    else if(!option.compare("bfs")) {
        getline(std::cin, line);
        stream << line;
        T vtx(stream);
        list<T> bfslist = g.bfs(vtx);
        
        typename list<T>::iterator it_l = bfslist.begin();
        typename list<T>::iterator ite_l = bfslist.end();
      
        cout << "\n----- BFS Traversal -----\n";
        cout << (*it_l);
        it_l++;
        for(; it_l!=ite_l; it_l++){
            cout << " -> " << (*it_l);
        }
      
      cout << "\n-------------------------\n";
    }
    else if(!option.compare("dfs")) {
        getline(std::cin, line);
        stream << line;
        T vtx(stream);
        list<T> dfslist = g.dfs(vtx);
        
        typename list<T>::iterator it_l = dfslist.begin();
        typename list<T>::iterator ite_l = dfslist.end();
      
      cout << "\n----- DFS Traversal -----\n";
      cout << (*it_l);
        it_l++;
        for(; it_l!=ite_l; it_l++){
            cout << " -> " << (*it_l);
        }
      cout << "\n-------------------------\n";
    }
    else if(!option.compare("dijkstra")) {
      getline(std::cin, line);
      stream << line;
      T from(stream);
      T to(stream);
      list<T> dijlist;

      cout << "Dijkstra (" << from << " - " << to <<"): ";
      dijlist = g.dijkstra(from, to);
      if(!dijlist.empty()){
        typename list<T>::iterator it_l = dijlist.begin();
        typename list<T>::iterator ite_l = dijlist.end();
        
        cout << (*it_l);
        it_l++;
        
        for(; it_l!=ite_l; it_l++){
            cout << ", " << (*it_l);
        }
      }
      cout << endl;
    }
    else if(!option.compare("bellman-ford")) {
      getline(std::cin, line);
      stream << line;
      T from(stream);
      T to(stream);
      list<T> belford_list;
      
      cout << "Bellman-Ford (" << from << " - " << to <<"): ";
       try{
            belford_list = g.bellman_ford(from, to);
       }catch (const NegativeGraphCycle &ex){cout << ex.what();}
      
    
      if(!belford_list.empty()){
          typename list<T>::iterator it_l = belford_list.begin();
          typename list<T>::iterator ite_l = belford_list.end();
        
          cout << (*it_l);
          it_l++;
        
          for(; it_l!=ite_l; it_l++){
            cout << ", " << (*it_l);
          }
       }else{cerr << "FROM UI LIST EMPTY" << endl;}
       cout << endl;
      
    }
    else if(!option.compare("mst")) {
        list<Edge<T>> mstlist = g.mst();
        int sum=0;
        
        typename list<Edge<T>>::iterator it = mstlist.begin();
        typename list<Edge<T>>::iterator ite = mstlist.end();
        
       cout << "\n--- Min Spanning Tree ---\n";
       for(; it!=ite; it++){
           cout << (*it) << endl;
           sum += (*it).dist;
        }
       
       cout << "MST Cost: " << sum << endl;
    }
    else if(!option.compare("q")) {
      cerr << "bye bye...\n";
      return 0;
    }
    else if(!option.compare("#")) {
      string line;
      getline(cin,line);
      cerr << "Skipping line: " << line << endl;
    }
    else {
      cout << "INPUT ERROR\n";
      return -1;
    }
  }
  return -1;  
}

#endif
