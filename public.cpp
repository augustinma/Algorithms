#ifndef __PROGTEST__
#include <cassert>
#include <cstdint>
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
#include <random>

#include <stdio.h>
#include <sys/resource.h>

template < typename F, typename S >
struct std::hash<std::pair<F, S>> {
  std::size_t operator () (const std::pair<F, S> &p) const noexcept {
    // something like boost::combine would be much better
    return std::hash<F>()(p.first) ^ (std::hash<S>()(p.second) << 1);
  }
};

// For exposition only. In the tests Place will not
// be a string but some other type. This type will always
// be comparable, hashable and it will have default and
// copy constructors.
using Place = std::string;
using Connection = std::pair<Place, Place>;

struct Map {
  std::vector<Place> places;
  std::vector<Connection> connections;
};

#endif

struct TrafficNetworkTester {
  explicit TrafficNetworkTester(const Map&);
  std::map<size_t, size_t> index;
  std::vector<std::set<size_t>> new_dsts;
  std::vector<long long int> scc;
  // Count how many areas exist in the network
  // after adding conns.
  // Note that conns may introduce new places.
  unsigned count_areas(const std::vector<Connection>& conns) const;
};



void transverse(const std::vector<std::set<size_t>>& dsts, std::vector<std::set<size_t>>& transverse_graph){
  for(size_t i=0; i<dsts.size(); i++){
    for (auto j=dsts.at(i).begin(); j!=dsts.at(i).end(); j++){
      transverse_graph.at(*j).insert(i);
    }
  }
}

// void dfs(int v, std::stack<size_t>& z, std::vector<uint8_t>& explored, const std::vector<std::set<size_t>>& transverse_graph){
//   if(explored.at(v)!=0){
//     return;
//   }
//   explored.at(v)=1;
//   for(auto i=transverse_graph.at(v).begin(); i!=transverse_graph.at(v).end(); i++){
//     dfs((*i), z, explored, transverse_graph);
//   }
//   explored.at(v)=2;
//   z.push(v);
// }
struct for_dfs
{
  u_int8_t color;
  size_t pi;
  size_t d;
  size_t right;
  size_t index;
};

void dfs(size_t v, std::deque<size_t>& z, std::vector<for_dfs>& str_dfs, const std::vector<std::set<size_t>>& dsts){
  using namespace std;
  
  u_int8_t step;
  size_t index;

  for(size_t i=0; i<dsts.size(); i++){
    str_dfs.at(i).color=0;
    str_dfs.at(i).pi=SIZE_MAX;
  }
  size_t time=0;
  stack<tuple<size_t, u_int8_t, size_t>> s;
  for(size_t u=0; u<dsts.size(); u++){
    if(str_dfs.at(u).color==0){
      // open;
      step =1;
      index=0;
      while(true){
        bool flag = false;
        if (step==1){
          str_dfs.at(u).color=1;
          time=time+1;
          str_dfs.at(u).d=time;
          step =2;
        }
        if (step==2){
          size_t i=0;
          for(auto v=dsts.at(u).begin(); v!=dsts.at(u).end(); v++){
            if (i>= index && str_dfs.at(*v).color==0){
              str_dfs.at(*v).pi = u;
              s.push({u, 2, i+1});
              u=*v;
              step =1;
              index=0;
              flag =true;
              break;
            }
            i++;
          }
          if (flag == true){
            continue;
          }
          step =3;
        }
        if (step ==3){
          str_dfs.at(u).color=2;
          z.push_back(u);
          time = time+1;
          str_dfs.at(u).right = time;
          if(s.empty()==true){
            break;
          }
          else {
            auto tmp = s.top();
            u = get<0>(tmp);
            step = get<1>(tmp);
            index = get<2>(tmp);
            s.pop();
          }
        }
      }
    }
  }

  cout << flush;

  // stack<size_t> s;
  // s.push(v);
  // explored.at(v)=1;
  // while(s.empty() == false){
  //   auto top = s.top();
  //   if(explored.at(top)<2){
  //     explored.at(top)=2;
  //   } else{
  //     s.pop();
  //     z.push_back(top);
  //     explored.at(top)=3;
  //     continue;
  //   }
  //   for(auto i=dsts.at(top).begin(); i!=dsts.at(top).end(); i++){
  //     if(explored.at(*i)<1){
  //       s.push(*i);
  //       explored.at(*i)=1;
  //     }
  //   }
  // }
}


void dfs_graph(std::deque<size_t>& z, std::vector<uint8_t>& explored, const std::vector<std::set<size_t>>& transverse_graph){
  using namespace std;
  // stack<size_t> rec_stack;
  // for(long unsigned int j=0; j<explored.size();j++){
  //   if(explored.at(j)==0){
  //     rec_stack.push(j);
  //     while(rec_stack.empty()==false){
  //       size_t rec_stack_top = rec_stack.top();
  //       rec_stack.pop();
  //       if(explored.at(rec_stack_top)==0){
  //         explored.at(rec_stack_top) = explored.at(rec_stack_top)+1;
  //         auto i= transverse_graph.at(rec_stack_top).begin();
  //         if(i==transverse_graph.at(rec_stack_top).end()){
  //           z.push_back(rec_stack_top);
  //         }
  //         while(i!=transverse_graph.at(rec_stack_top).end()){
  //           rec_stack.push(*i);
  //           i++;
  //           if(i==transverse_graph.at(rec_stack_top).end()){
  //             z.push_back(rec_stack_top);
  //           }
  //         }
  //       } 
  //     }
  //   }
  // }
  // for(long unsigned int i=0; i<explored.size();i++){
  //   if(explored.at(i)==0){
  vector<for_dfs> struct_graph;
  struct_graph.resize(transverse_graph.size());
  dfs(0, z, struct_graph, transverse_graph);
    // }
  // }
}

void dfs_graph(size_t vert, size_t orig, std::vector<for_dfs>& str_dfs, std::vector<long long int>& scc, const std::vector<std::set<size_t>>& dsts, std::set<size_t>& unique_scc,
                u_int8_t& step, size_t& index, size_t& time, std::stack<std::tuple<size_t, u_int8_t, size_t>>& s){
  using namespace std;

  size_t u=vert;
  // for(size_t u=0; u<dsts.size(); u++){
    if(str_dfs.at(u).color==0){
      // open;
      if (scc.at(u)==-1){
        scc.at(u)=orig;
        unique_scc.insert(orig);
      }
      step =1;
      index=0;
      while(true){
        bool flag = false;
        if (step==1){
          str_dfs.at(u).color=1;
          if (scc.at(u)==-1){
            scc.at(u)=orig;
            unique_scc.insert(orig);
          }
          time=time+1;
          str_dfs.at(u).d=time;
          step =2;
        }
        if (step==2){
          size_t i=0;
          for(auto v=dsts.at(u).begin(); v!=dsts.at(u).end(); v++){
            if (i>= index && str_dfs.at(*v).color==0){
              str_dfs.at(*v).pi = u;
              s.push({u, 2, i+1});
              u=*v;
              step =1;
              index=0;
              flag = true;
              break;
            }
            i++;
          }
          if (flag==true){
            continue;
          }
          step =3;
        }
        if (step ==3){
          str_dfs.at(u).color=2;
          // z.push_back(u);
          time = time+1;
          str_dfs.at(u).right = time;
          if(s.empty()==true){
            break;
          }
          else {
            auto tmp = s.top();
            u = get<0>(tmp);
            step = get<1>(tmp);
            index = get<2>(tmp);
            s.pop();
          }
        }
      }
    }
  // }

  cout << flush;



  

  // if(state.at(v)!=0){
  //   return;
  // }
  // if(scc.at(v)==-1){
  //   state.at(v)=1;
  //   scc.at(v)=orig;
  //   unique_scc.insert(orig);
  //   for(auto i=dsts.at(v).begin(); i!=dsts.at(v).end(); i++){
  //     dfs_graph((*i), orig, state, scc, dsts, unique_scc);
  //   }
  //   state.at(v)=2;
  // }
}

void algo_scc(const std::vector<std::set<size_t>>& dsts, std::vector<long long int>& scc, std::set<size_t>& unique_scc){
  using namespace std;
  vector<std::set<size_t>> transverse_graph;
  transverse_graph.resize(dsts.size());
  transverse(dsts, transverse_graph);
  deque<size_t> z;
  scc.resize(dsts.size(), -1);
  vector<u_int8_t> explored;
  explored.resize(transverse_graph.size(), 0);
  if(transverse_graph.size()>0){
    dfs_graph(z, explored, transverse_graph);
  }
  transverse_graph.clear();
  vector<u_int8_t>state1;
  state1.resize(dsts.size(), 0);
  vector<for_dfs> str_dfs;
  str_dfs.resize(dsts.size());
  for(size_t i=0; i<dsts.size(); i++){
    str_dfs.at(i).color=0;
    str_dfs.at(i).pi=SIZE_MAX;
  }
  u_int8_t step;
  size_t index;

  size_t time=0;
  stack<tuple<size_t, u_int8_t, size_t>> s;
  

  while(z.empty()==false){
    size_t top = z.back();
    z.pop_back();
    if(scc.at(top)==-1){
      dfs_graph(top, top, str_dfs, scc, dsts, unique_scc, step, index, time, s);
    }
  }
}

void change_dsts(std::vector<std::set<size_t>>& dsts, std::vector<long long int>& scc, std::set<size_t>& unique_scc){
  for(size_t i=0; i<dsts.size(); i++){
    for(auto j=dsts.at(i).begin(); j!=dsts.at(i).end(); j++){
      if(scc.at(i)!=scc.at(*j)){
        dsts.at(scc.at(i)).insert(scc.at(*j));
      }
    }
  }
}

size_t binary_search(size_t key, std::vector<size_t>& arr){
  size_t l=0;
  size_t r=arr.size()-1;
  while(l<=r){
    size_t m = (l+r)/2;
    if(arr.at(m)<key){
      l=m+1;
    } 
    else if (arr.at(m)>key){
      r=m-1;
    }
    else{
      return m;
    }
  }
  return SIZE_MAX;
}

void remove_non_unique_dsts(std::vector<std::set<size_t>>& dsts, std::vector<long long int>& scc, std::set<size_t>& unique_scc, std::vector<std::set<size_t>>& new_dsts){
  std::vector<size_t> new_index;
  new_dsts.resize(unique_scc.size());
  new_index.resize(unique_scc.size());
  size_t k=0;
  for(auto i=unique_scc.begin(); i!=unique_scc.end(); i++){
    new_index.at(k)=*i;
    k++;
  }
  k=0;
  for(auto i=unique_scc.begin(); i!=unique_scc.end(); i++){
    for(auto j=dsts.at(*i).begin(); j!=dsts.at(*i).end(); j++){
      if(unique_scc.find(*j)!=unique_scc.end()){
        new_dsts.at(k).insert(binary_search(*j, new_index));
      }
    }
    k++;
  }
  for(auto i=scc.begin(); i!=scc.end(); i++){
    // create new array of scc;
    (*i)=binary_search((*i), new_index);
  }
}

TrafficNetworkTester::TrafficNetworkTester(const Map& in_graph){
  using namespace std;
  std::vector<std::set<size_t>> dsts;
  for(auto i=in_graph.places.begin(); i!=in_graph.places.end(); i++){
    auto hash_val = hash<Place>{}(*i);
    index.insert({hash_val, index.size()});
  }
  
  for(auto i=in_graph.connections.begin(); i!=in_graph.connections.end(); i++){
    auto hash_val1 = hash<Place>{}((*i).first);
    auto iter1 = index.find(hash_val1);
    auto hash_val2 = hash<Place>{}((*i).second);
    auto iter2 = index.find(hash_val2);
    if (iter1==index.end()){ 
      index.insert({hash_val1, index.size()});
      iter1 = index.find(hash_val1);
    }
    if (iter2==index.end()){
      index.insert({hash_val2, index.size()});
      iter2 = index.find(hash_val2);
    }
    dsts.resize(index.size());
    if ((*iter1).second!=(*iter2).second){
      dsts.at((*iter1).second).insert((*iter2).second);
    }
  }
  // dsts.at(4).insert(2);
  set<size_t> unique_scc;
  algo_scc(dsts, scc, unique_scc);
  change_dsts(dsts, scc, unique_scc);
  remove_non_unique_dsts(dsts, scc, unique_scc, new_dsts);
  // cout << flush;
}

unsigned TrafficNetworkTester::count_areas(const std::vector<Connection>& conns) const{
  using namespace std;
  vector<set<size_t>> tmp_dsts = new_dsts;
  map<size_t, size_t> tmp_index;
  for(auto i=conns.begin(); i!=conns.end(); i++){
    auto hash_val1 = hash<Place>{}((*i).first);
    auto iter11 = index.find(hash_val1);
    auto iter12 = tmp_index.find(hash_val1);

    auto hash_val2 = hash<Place>{}((*i).second);
    auto iter21 = index.find(hash_val2);
    auto iter22 = tmp_index.find(hash_val2);
    if((iter11!=index.end() && iter21!=index.end() )){
      if(scc.at((*iter11).second)!=scc.at((*iter21).second)){
        tmp_dsts.at(scc.at((*iter11).second)).insert(scc.at((*iter21).second));
      }
    } else if (iter11!=index.end()){
      if(iter22==tmp_index.end()){  
        tmp_index.insert({hash_val2, tmp_dsts.size()});
        tmp_dsts.push_back({});
      }
      tmp_dsts.at(scc.at((*iter11).second)).insert(tmp_index.at(hash_val2));
    } else if(iter21!=index.end()){
      if(iter12==tmp_index.end()){
        tmp_index.insert({hash_val1, tmp_dsts.size()});
        tmp_dsts.push_back({});
      }
      tmp_dsts.at(tmp_index.at(hash_val1)).insert(scc.at((*iter21).second));
    } else {
      if(iter12==tmp_index.end()){
        tmp_index.insert({hash_val1, tmp_dsts.size()});
        tmp_dsts.push_back({});
      }
      if(iter22==tmp_index.end()){
        tmp_index.insert({hash_val2, tmp_dsts.size()});
        tmp_dsts.push_back({});
      }
      tmp_dsts.at(tmp_index.at(hash_val1)).insert(tmp_index.at(hash_val2));
    }

    // if (iter1==tmp_index.end()){ 
    //   tmp_index.insert({hash_val1, tmp_index.size()});
      
    //   iter1 = tmp_index.find(hash_val1);;
    // }
    // if (iter2==tmp_index.end()){
    //   tmp_index.insert({hash_val2, tmp_index.size()});
      
    //   iter2 = tmp_index.find(hash_val2);
    // }
    // tmp_dsts.resize(tmp_index.size());
    // tmp_dsts.at((*iter1).second).insert((*iter2).second);
  }
  std::vector<long long int> tmp_scc;
  // tmp_scc.resize(tmp_index.size());
  std::set<size_t> tmp_unique_scc;
  algo_scc(tmp_dsts, tmp_scc, tmp_unique_scc);

  unsigned result = tmp_unique_scc.size();
  // cout << result << " " << flush;
  return result;
}

#ifndef __PROGTEST__

using Test = std::pair<Map, std::vector<std::pair<unsigned, std::vector<Connection>>>>;

Test TESTS[] = {
  {
    { { "Dejvicka", "Hradcanska", "Malostranska", "Staromestska", "Mustek", "Muzeum" }, { // Map
      { "Dejvicka", "Hradcanska" }, { "Hradcanska", "Malostranska" },
      { "Malostranska", "Staromestska" }, { "Staromestska", "Mustek" },
      { "Mustek", "Muzeum" }
    } }, { // Queries
      { 4, { { "Mustek", "Malostranska" } } },
      { 4, { { "Malostranska", "Letnany" }, { "Letnany", "Dejvicka" } } },
      { 1, { { "Malostranska", "Letnany" }, { "Letnany", "Dejvicka" }, { "Muzeum", "Hradcanska" } } },
      { 1, { { "Muzeum", "Dejvicka" } } },
      { 6, { { "Dejvicka", "Muzeum" } } },
    }
  },
  {
    { { 
      "Na Sklonku", "Poliklinika Čumpelíkova", "Šumavská", "Nové Podolí", "Vozovna Střešovice (Muzeum MHD)",
      "Florenc", "Franty Kocourka", "Cukrovar Čakovice", "Praha-Dejvice", "Pod Říhákem",
      "Sukovská", "Novoborská", "U Průhonu", "Nádraží Modřany",
    }, {
      { "Pod Říhákem", "Novoborská" }, { "Nové Podolí", "Franty Kocourka" },
      { "Cukrovar Čakovice", "Florenc" }, { "Vozovna Střešovice (Muzeum MHD)", "Cukrovar Čakovice" },
      { "U Průhonu", "Praha-Dejvice" }, { "Sukovská", "Nové Podolí" },
      { "Poliklinika Čumpelíkova", "Nové Podolí" }, { "Florenc", "Na Sklonku" },
      { "Praha-Dejvice", "Cukrovar Čakovice" }, { "Franty Kocourka", "Šumavská" },
      { "Florenc", "Vozovna Střešovice (Muzeum MHD)" }, { "Pod Říhákem", "Florenc" },
      { "Novoborská", "Šumavská" }, { "Sukovská", "Šumavská" },
      { "Nové Podolí", "Pod Říhákem" }, { "Vozovna Střešovice (Muzeum MHD)", "Na Sklonku" },
      { "Cukrovar Čakovice", "Nádraží Modřany" }, { "Vozovna Střešovice (Muzeum MHD)", "Šumavská" },
      { "Novoborská", "Cukrovar Čakovice" }, { "Šumavská", "Na Sklonku" },
      { "Poliklinika Čumpelíkova", "Vozovna Střešovice (Muzeum MHD)" }, { "Nové Podolí", "Na Sklonku" },
      { "Pod Říhákem", "U Průhonu" }, { "Šumavská", "Sukovská" },
      { "Šumavská", "Nádraží Modřany" }, { "Nové Podolí", "Šumavská" },
      { "Poliklinika Čumpelíkova", "Šumavská" }, { "Šumavská", "Florenc" },
      { "Franty Kocourka", "Nové Podolí" }, { "U Průhonu", "Vozovna Střešovice (Muzeum MHD)" },
      { "Praha-Dejvice", "Florenc" }, { "Nové Podolí", "Praha-Dejvice" },
      { "Pod Říhákem", "Florenc" },
    } }, {
      { 4, {
      } },
      { 2, {
        { "Nádraží Modřany", "Poliklinika Čumpelíkova" },
      } },
      { 2, {
        { "Na Sklonku", "Poliklinika Čumpelíkova" },
      } },
      { 1, {
        { "Nádraží Modřany", "Poliklinika Čumpelíkova" }, { "Na Sklonku", "Poliklinika Čumpelíkova" },
      } },
      { 4, {
        { "Poliklinika Čumpelíkova", "Na Sklonku" }, // //first failed test
      } },
      { 2, {
        { "Cukrovar Čakovice", "Poliklinika Čumpelíkova" }, { "Poliklinika Čumpelíkova", "Na Sklonku" },
        { "Nádraží Modřany", "Poliklinika Čumpelíkova" },
      } },
    }
  },
  {
    { { 
      "U Vojenské nemocnice", "Kuchyňka", "V Korytech", "Kelerka", "Vozovna Strašnice",
      "Geologická", "U Studánky", "U Jahodnice", "Hadovka", "Barrandovská",
      "K Netlukám", "Obchodní centrum Sárská", "Praha-Smíchov", "Sušická", "Moráň",
      "Praha-Bubny", "Rajská zahrada", "Strossmayerovo náměstí", "Průmstav",
    }, {
      { "U Vojenské nemocnice", "Vozovna Strašnice" }, { "K Netlukám", "Obchodní centrum Sárská" },
      { "Praha-Smíchov", "U Jahodnice" }, { "Praha-Smíchov", "K Netlukám" },
      { "Vozovna Strašnice", "Kelerka" }, { "Obchodní centrum Sárská", "Geologická" },
      { "K Netlukám", "Praha-Smíchov" }, { "V Korytech", "Geologická" },
      { "V Korytech", "Vozovna Strašnice" }, { "Vozovna Strašnice", "V Korytech" },
      { "U Vojenské nemocnice", "Kuchyňka" }, { "Kelerka", "Geologická" },
      { "Praha-Bubny", "Strossmayerovo náměstí" }, { "Kuchyňka", "V Korytech" },
      { "Praha-Smíchov", "Praha-Bubny" }, { "Obchodní centrum Sárská", "Moráň" },
      { "Kelerka", "V Korytech" }, { "Kelerka", "V Korytech" },
      { "Hadovka", "Rajská zahrada" }, { "V Korytech", "Geologická" },
      { "Sušická", "Praha-Smíchov" }, { "Barrandovská", "K Netlukám" },
      { "V Korytech", "Kelerka" }, { "K Netlukám", "V Korytech" },
      { "U Studánky", "Kuchyňka" }, { "Hadovka", "Barrandovská" },
      { "Praha-Bubny", "U Studánky" }, { "Moráň", "K Netlukám" },
      { "Strossmayerovo náměstí", "Kelerka" }, { "Barrandovská", "U Jahodnice" },
      { "V Korytech", "Kuchyňka" }, { "Průmstav", "Praha-Smíchov" },
      { "Geologická", "V Korytech" }, { "Rajská zahrada", "Kuchyňka" },
      { "U Jahodnice", "Kuchyňka" }, { "Praha-Smíchov", "Sušická" },
      { "K Netlukám", "Obchodní centrum Sárská" }, { "Geologická", "Kelerka" },
      { "Obchodní centrum Sárská", "K Netlukám" }, { "Obchodní centrum Sárská", "K Netlukám" },
      { "Hadovka", "U Studánky" }, { "K Netlukám", "Sušická" },
      { "Moráň", "U Vojenské nemocnice" }, { "Obchodní centrum Sárská", "Praha-Smíchov" },
      { "V Korytech", "U Studánky" }, { "Kuchyňka", "Geologická" },
      { "K Netlukám", "Moráň" }, { "Sušická", "U Vojenské nemocnice" },
      { "Kuchyňka", "U Vojenské nemocnice" },
    } }, {
      { 9, {
      } },
      { 5, {
        { "Kuchyňka", "Kuchyňka" }, { "Strossmayerovo náměstí", "Průmstav" },
        { "Průmstav", "V Korytech" }, { "K Netlukám", "Praha-Smíchov" },
        { "Praha-Bubny", "Barrandovská" },
      } },
      { 9, {
        { "Rajská zahrada", "Strossmayerovo náměstí" }, { "Sušická", "Obchodní centrum Sárská" },
        { "Průmstav", "Strossmayerovo náměstí" }, { "Moráň", "Strossmayerovo náměstí" },
      } },
      { 5, {
        { "Kelerka", "K Netlukám" }, { "U Studánky", "Sušická" },
        { "U Studánky", "V Korytech" }, { "U Studánky", "Strossmayerovo náměstí" },
        { "Kuchyňka", "V Korytech" }, { "Průmstav", "Rajská zahrada" },
      } },
      { 5, {
        { "Vozovna Strašnice", "Obchodní centrum Sárská" }, { "Strossmayerovo náměstí", "Praha-Bubny" },
        { "U Vojenské nemocnice", "V Korytech" }, { "U Jahodnice", "U Studánky" },
        { "Rajská zahrada", "V Korytech" }, { "Obchodní centrum Sárská", "Sušická" },
      } },
      { 2, {
        { "Barrandovská", "Praha-Smíchov" }, { "Geologická", "Hadovka" },
        { "U Studánky", "Moráň" }, { "U Vojenské nemocnice", "Praha-Smíchov" },
      } },
    }
  },
  {
    { { 
      "Na Lukách", "Plánická", "U Mezníku", "Bílá Hora", "Psohlavců",
      "Koupaliště Čakovice", "Volha", "Dolnopočernický hřbitov", "Studentský dům", "U Rozcestí",
      "Koleje Jižní Město", "Kusá", "Vozovna Pankrác", "Ke Koulce", "Opatov",
      "Nádraží Žvahov - V Násypu", "Na Blanici", "Nádraží Libeň", "Ořechovka", "Ke Kateřinkám",
      "Divadlo pod Palmovkou", "Přístav Radotín", "Žákovská", "Pankrác", "Pod Děvínem",
    }, {
      { "Koleje Jižní Město", "U Rozcestí" }, { "Na Lukách", "Psohlavců" },
      { "U Rozcestí", "U Mezníku" }, { "Ke Koulce", "Kusá" },
      { "Nádraží Žvahov - V Násypu", "Nádraží Libeň" }, { "Koleje Jižní Město", "U Rozcestí" },
      { "Koupaliště Čakovice", "Psohlavců" }, { "Žákovská", "Pankrác" },
      { "Kusá", "U Rozcestí" }, { "Psohlavců", "Bílá Hora" },
      { "Volha", "Na Lukách" }, { "Na Lukách", "Plánická" },
      { "Vozovna Pankrác", "Opatov" }, { "Koupaliště Čakovice", "Bílá Hora" },
      { "Ořechovka", "Na Blanici" }, { "Psohlavců", "U Mezníku" },
      { "U Mezníku", "Bílá Hora" }, { "Divadlo pod Palmovkou", "U Rozcestí" },
      { "Vozovna Pankrác", "U Mezníku" }, { "Psohlavců", "Koupaliště Čakovice" },
      { "Na Blanici", "Nádraží Žvahov - V Násypu" }, { "Na Lukách", "Bílá Hora" },
      { "Kusá", "Koleje Jižní Město" }, { "Nádraží Libeň", "Ke Koulce" },
      { "Opatov", "Ke Koulce" }, { "Přístav Radotín", "Koupaliště Čakovice" },
      { "Kusá", "Koleje Jižní Město" }, { "Pankrác", "Žákovská" },
      { "Vozovna Pankrác", "Koleje Jižní Město" }, { "Plánická", "Na Lukách" },
      { "Bílá Hora", "Koupaliště Čakovice" }, { "Kusá", "Vozovna Pankrác" },
      { "Koupaliště Čakovice", "Bílá Hora" }, { "Psohlavců", "Koleje Jižní Město" },
      { "Bílá Hora", "U Mezníku" }, { "Psohlavců", "Bílá Hora" },
      { "Koleje Jižní Město", "Kusá" }, { "Přístav Radotín", "Opatov" },
      { "Plánická", "U Mezníku" }, { "Vozovna Pankrác", "Nádraží Žvahov - V Násypu" },
      { "Koupaliště Čakovice", "Bílá Hora" }, { "Nádraží Žvahov - V Násypu", "Opatov" },
      { "Vozovna Pankrác", "Opatov" }, { "Studentský dům", "Volha" },
      { "Žákovská", "Pankrác" }, { "U Mezníku", "Na Lukách" },
      { "Ke Kateřinkám", "Koleje Jižní Město" }, { "U Mezníku", "Plánická" },
      { "Opatov", "Na Blanici" }, { "U Mezníku", "Volha" },
      { "Kusá", "Psohlavců" }, { "Kusá", "Ke Koulce" },
      { "Dolnopočernický hřbitov", "Bílá Hora" },
    } }, {
      { 9, {
      } },
      { 8, {
        { "Pod Děvínem", "U Rozcestí" }, { "Nádraží Žvahov - V Násypu", "Pod Děvínem" },
      } },
      { 7, {
        { "Pankrác", "Pod Děvínem" }, { "U Rozcestí", "Žákovská" },
        { "Pod Děvínem", "Nádraží Žvahov - V Násypu" },
      } },
    }
  },
};

template < typename C >
void test(C&& tests, bool exact) {
  int fail = 0, ok = 0;

  for (auto&& [ map, test_cases ] : tests) {
    TrafficNetworkTester T{map};
    for (auto&& [ ans, conns ] : test_cases)
      if (exact)
        (ans == T.count_areas(conns) ? ok : fail)++;
      else
        ((ans == 1) == (T.count_areas(conns) == 1) ? ok : fail)++;
  }

  if (fail)
    std::cout << fail << " of " << fail + ok << " tests failed!" << std::endl;
  else
    std::cout << "All " << ok << " tests succeded!" << std::endl;
}

int main() {
  test(TESTS, false);
  test(TESTS, true);
}

#endif


