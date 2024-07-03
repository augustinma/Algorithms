#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

using Place = size_t;

struct Map {
  size_t places;
  Place start, end;
  std::vector<std::pair<Place, Place>> connections;
  std::vector<std::vector<Place>> items;
};

template < typename F, typename S >
struct std::hash<std::pair<F, S>> {
  std::size_t operator () (const std::pair<F, S> &p) const noexcept {
    // something like boost::combine would be much better
    return std::hash<F>()(p.first) ^ (std::hash<S>()(p.second) << 1);
  }
};

#endif



class Graph{
  public:
  std::vector<std::pair<std::vector<bool>, std::vector<Place>>> g;
  Graph(const Map &map){
    g.resize(map.places);
    for (size_t i=0; i<map.connections.size(); i++){
      g.at(map.connections.at(i).first).second.push_back(map.connections.at(i).second);
      g.at(map.connections.at(i).second).second.push_back(map.connections.at(i).first);
    }
    for(size_t i=0; i<map.items.size(); i++){
      for (size_t j = 0; j < map.items.at(i).size(); j++)
      {
        g.at(map.items.at(i).at(j)).first.resize(map.items.size(), false);
        g.at(map.items.at(i).at(j)).first.at(i) = true;
      }
    }
  }
};

// struct Dist{
//   std::vector<std::vector<int>> mDist;
//   Dist(size_t numItems, size_t numVertices){
//     u_int16_t size = 1<<numItems;
//     mDist.resize(size);
//     for (size_t i = 0; i < mDist.size(); i++)
//     {
//       mDist.at(i).resize(numVertices, INT32_MAX);
//     }
    
//   }
// };

// struct Visited{
//   std::vector<std::vector<bool>> mVisited;
//   Visited(size_t numItems, size_t numVertices){
//     u_int16_t size = 1<<numItems;
//     mVisited.resize(size);
//     for (size_t i = 0; i < mVisited.size(); i++)
//     {
//       mVisited.at(i).resize(numVertices, false);
//     }
    
//   }
// };

// struct Pred{
//   std::vector<std::vector<int>> mPred;
//   Pred(size_t numItems, size_t numVertices){
//     u_int16_t size = 1<<numItems;
//     mPred.resize(size);
//     for (size_t i = 0; i < mPred.size(); i++)
//     {
//       mPred.at(i).resize(numVertices, -1);
//     }
    
//   }
// };

u_int16_t boolVecToint(const std::vector<bool> &itemsInVector){
  u_int16_t res =0;
  for(size_t i=0; i<itemsInVector.size(); i++){
    if(itemsInVector.at(i)==true){
      u_int16_t tmp =1<<i;
      res += tmp;
    }
  }
  return res;
}

bool BFS(Graph &adj, const Map &map, Place s, Place t, std::vector<std::vector<bool>> &visited, std::vector<std::vector<std::pair<u_int16_t, int>>> &pred, u_int16_t fullmask)
{
  // std::vector<std::vector<bool>> visited;
  // std::vector<std::vector<int>> pred;
  // std::vector<std::vector<int>> dist;

  

  size_t numItems = map.items.size();

  // u_int16_t fullmask =0;
  // for(size_t i=0; i<numItems; i++){
  //     u_int16_t tmp =1<<i;
  //     fullmask += tmp;
  // }

  u_int16_t size = 1<<numItems;
  visited.resize(size);
  pred.resize(size);
  // dist.resize(size);
  for (size_t i = 0; i < visited.size(); i++)
  {
    visited.at(i).resize(map.places, false);
    pred.at(i).resize(map.places, std::make_pair(UINT16_MAX, -1));
    // dist.at(i).resize(map.places, INT32_MAX);
  }

  // Create a queue for BFS
  std::list<std::pair<uint16_t, Place>> queue;

  // Mark the current node as visited and enqueue it
  visited[boolVecToint(adj.g.at(s).first)][s] = true;
  // dist[boolVecToint(adj.g.at(s).first)][s] = 0;
  queue.push_back(std::make_pair(boolVecToint(adj.g.at(s).first), s));
  uint16_t startmask = boolVecToint(adj.g.at(s).first);
  if((startmask == fullmask) && (s == t)){
    return true;
  }

  while(!queue.empty())
  {
      // Dequeue a vertex from queue and print it
      size_t u = queue.front().second;
      uint16_t mask = queue.front().first;
      // std::cout << s << " ";
      
      queue.pop_front();

      // Get all adjacent vertices of the dequeued
      // vertex s. If a adjacent has not been visited,
      // then mark it visited and enqueue it
      for (auto adjecent: adj.g.at(u).second)
      {
        u_int16_t neighbour_mask = mask | boolVecToint(adj.g.at(adjecent).first);
          if (!visited[neighbour_mask][adjecent])
          {
              visited[neighbour_mask][adjecent] = true;
              // dist[neighbour_mask][adjecent] = dist[mask][u]+1;
              pred[neighbour_mask][adjecent] = std::make_pair(mask, u);
              queue.push_back(std::make_pair(neighbour_mask, adjecent));
              
              if ((neighbour_mask == fullmask) && (adjecent == t)){
                return true;
              }
          }
      }
  }
  return false;
}

void minWalk(std::vector<std::vector<std::pair<uint16_t, int>>> &pred, Place dest, std::list<Place> &walk, u_int16_t mask){
    // std::list<Place> walk;
    Place crawl = dest;
    // u_int16_t numItems = map.items.size();
    // u_int16_t mask =0;
    // for(size_t i=0; i<numItems; i++){
    //     u_int16_t tmp =1<<i;
    //     mask += tmp;
    // }
    walk.push_front(crawl);
    while (pred[mask][crawl].second != -1) {
        walk.push_front(pred[mask][crawl].second);
        Place tmpCrawl = pred[mask][crawl].second;
        u_int16_t tmpMask = pred[mask][crawl].first;
        mask = tmpMask;
        crawl = tmpCrawl;
        if(mask == UINT16_MAX) break;
    }
}

std::list<Place> find_path(const Map &map) {
  // TODO

  Graph adj(map);
  std::vector<std::vector<bool>> visited;
  std::vector<std::vector<std::pair<u_int16_t, int>>> pred;
  // std::vector<std::vector<int>> dist;

  u_int16_t fullmask=0;
  for(size_t i=0; i<map.items.size(); i++){
      u_int16_t tmp =1<<i;
      fullmask += tmp;
  }

  bool walkExists = BFS (adj, map, map.start, map.end, visited, pred, fullmask);

  std::list<Place> walk;
  if (walkExists ==false){
    walk.clear();
  }
  else{
    minWalk(pred, map.end, walk, fullmask);
    if((walk.size()>1) && (walk.front()!=map.start)){
      walk.push_front(map.start);
    }
  }

  // std::cout << "Walk: ";
  // for(auto i: walk){
  //   std::cout << i << " "; 
  // }
  // std::cout << std::endl;

  return walk;

}

#ifndef __PROGTEST__

using TestCase = std::pair<size_t, Map>;

// Class template argument deduction exists since C++17 :-)
const std::array examples = {
  TestCase{ 1, Map{ 2, 0, 0,
    { { 0, 1 } },
    { { 0 } }
  }},
  TestCase{ 3, Map{ 2, 0, 0,
    { { 0, 1 } },
    { { 1 } }
  }},
  TestCase{ 3, Map{ 4, 0, 1,
    { { 0, 2 }, { 2, 3 }, { 0, 3 }, { 3, 1 } },
    {}
  }},
  TestCase{ 4, Map{ 4, 0, 1,
    { { 0, 2 }, { 2, 3 }, { 0, 3 }, { 3, 1 } },
    { { 2 } }
  }},
  TestCase{ 0, Map{ 4, 0, 1,
    { { 0, 2 }, { 2, 3 }, { 0, 3 }, { 3, 1 } },
    { { 2 }, {} }
  }},
  TestCase{ 7, Map{ 8, 0, 5,
    { { 0, 1 }, { 0, 2 }, { 0, 3 }, { 3, 4 }, { 3, 5 }, { 5, 6 }, { 6, 7 } },
    { { 1, 7 }, { 2, 7 }, { 4, 7 } }
  }},
    TestCase{ 0, Map{ 4, 0, 1,
    { { 0, 2 } },
    { { 2 } }
  }},
  TestCase{ 0, Map{ 4, 0, 2,
    { { 0, 2 } },
    { { 1 } }
  }},
  TestCase{ 4, Map{ 4, 3, 1,
    { { 0, 2 }, { 2, 3 }, { 0, 3 }, { 3, 1 } },
    { { 2 } }
  }},
  TestCase{ 9, Map{ 9, 0, 5,
    { { 0, 1 }, { 0, 2 }, { 0, 3 }, { 3, 4 }, { 3, 5 }, { 5, 6 }, { 6, 7 }, { 7, 8 } },
    { { 1, 7 }, { 2, 7 }, { 4, 7 }, { 8 } }
  }},
  TestCase{ 8, Map{ 13, 0, 0,
    { { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 }, { 7, 9 }, { 9, 10 }, { 10, 12 }, { 12, 11 }, { 11, 8 } },
    { { 1, 12 }, { 2, 10 }, { 6, 10 }, { 4, 10 }, { 5, 12 }, { 3, 12 } }
  }},

};

int main() {
  int fail = 0;
  for (size_t i = 0; i < examples.size(); i++) {
    auto sol = find_path(examples[i].second);
    if (sol.size() != examples[i].first) {
      std::cout << "Wrong anwer for map " << i << std::endl;
      fail++;
    }
  }

  if (fail) std::cout << "Failed " << fail << " tests" << std::endl;
  else std::cout << "All tests completed" << std::endl;

  return 0;
}

#endif
