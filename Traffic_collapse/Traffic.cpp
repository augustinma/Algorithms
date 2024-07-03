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
using Capacity = unsigned;
using Connection = std::tuple<Place, Place, Capacity>;

struct Map {
  std::vector<Place> places;
  std::vector<Connection> connections;
};

#endif
using namespace std;

int binary_find(std::vector<pair<size_t, string>>& v, size_t data) {
    auto it = std::lower_bound(v.begin(), v.end(), data, [](auto a, auto b)
                                                          {
                                                            return a.first < b;
                                                          });
    if (it == v.end() || (*it).first != data) {
        return -1;
    } else {
        std::size_t index = std::distance(v.begin(), it);
        return index;
    }   
}

int bfs(size_t s, size_t t, vector<int>& parent, const std::vector<set<size_t>>& adj, vector<vector<int>>& capacity, set<size_t>& blocked){
    blocked.clear();
    fill(parent.begin(), parent.end(), -1);
    parent.at(s) = -2;
    queue<pair<int, int>> q;
    q.push({s, INT16_MAX});
    blocked.insert(s);
    while (q.empty()==false) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj.at(cur)) {
            if (parent.at(next) == -1 && capacity.at(cur).at(next)>0) {
                parent.at(next) = cur;
                int new_flow = min(flow, capacity.at(cur).at(next));
                if ((size_t)next == t)
                    return new_flow;
                q.push({next, new_flow});
                blocked.insert(next);
            }
        }
    }

    return 0;
}

Capacity fordFulkerson(const std::vector<set<size_t>>& adj_list, const vector<vector<int>>& mat_cap, size_t s, size_t t, set<size_t>& blocked){
  vector<vector<int>> rGraph = mat_cap;
  int flow = 0;
    vector<int> parent(adj_list.size());
    int new_flow;

    while ((new_flow = (bfs(s, t, parent, adj_list, rGraph, blocked)))) {
        flow += new_flow;
        int cur = t;
        while ((size_t)cur != s) {
            int prev = parent.at(cur);
            rGraph.at(prev).at(cur) -= new_flow;
            rGraph.at(cur).at(prev) -= new_flow;
            cur = prev;
        }
    }
    
    return flow;
}

std::pair<Capacity, std::set<Place>> critical_streets(const Map& map){
  std::vector<pair<size_t, string>> lookup_table;
  for(auto x : map.places){
    lookup_table.push_back({hash<Place>{}(x), x});
  }
  sort(lookup_table.begin(), lookup_table.end(), [](auto a, auto b)
                                                  {
                                                    return a.first < b.first;
                                                  });
  std::vector<set<size_t>> adj_list(lookup_table.size());
  vector<vector<int>> mat_cap(adj_list.size(), vector<int>(adj_list.size(),0));

  for(auto x : map.connections){
    auto pl1 = hash<Place>{}(get<0>(x));
    auto pl2 = hash<Place>{}(get<1>(x));
    if (pl1 != pl2 && get<2>(x)!=0 ){
      // not a self-loop
      size_t ind_pl1 = binary_find(lookup_table, pl1);
      size_t ind_pl2 = binary_find(lookup_table, pl2);
      if (adj_list.at(ind_pl1).find(ind_pl2) == adj_list.at(ind_pl1).end()){
        adj_list.at(ind_pl1).insert(ind_pl2);
        adj_list.at(ind_pl2).insert(ind_pl1);
        mat_cap.at(ind_pl1).at(ind_pl2)=get<2>(x);
        mat_cap.at(ind_pl2).at(ind_pl1)=get<2>(x);
      }
      else{
        mat_cap.at(ind_pl1).at(ind_pl2)+=get<2>(x);
        mat_cap.at(ind_pl2).at(ind_pl1)+=get<2>(x);
      }
    }
  }

  set<size_t> global_blocked;
  Capacity global_min=UINT16_MAX;
  {
    size_t s=0;
    for(size_t t=1; t<adj_list.size(); t++){
      set<size_t> blocked;
      Capacity min = fordFulkerson(adj_list, mat_cap, s, t, blocked);
      if(min<global_min){
        global_min=min;
        global_blocked = blocked;
      }
    }
  }
  std::pair<Capacity, std::set<Place>> result;
  result.first=global_min;
  for(auto x : global_blocked){
    result.second.insert(lookup_table.at(x).second);
  }
  return result;
}

#ifndef __PROGTEST__


using Test = std::pair<unsigned, Map>;

std::array TESTS = {

  Test{2, {{
    "Dejvicka", "Hradcanska", "Malostranska", "Staromestska", "Mustek", "Muzeum"
  }, {
    { "Dejvicka", "Hradcanska", 3 }, { "Hradcanska", "Malostranska", 2 },
    { "Malostranska", "Staromestska", 4 }, { "Staromestska", "Mustek", 5 },
    { "Mustek", "Muzeum", 3 }
  }}},
  Test{13, {{ 
    "Na Sklonku", "Poliklinika Čumpelíkova", "Šumavská", "Nové Podolí", "Vozovna Střešovice (Muzeum MHD)",
    "Florenc", "Franty Kocourka", "Cukrovar Čakovice", "Praha-Dejvice", "Pod Říhákem",
    "Sukovská", "Novoborská", "U Průhonu", "Nádraží Modřany",
  }, {
    { "Pod Říhákem", "Novoborská", 3 }, { "Nové Podolí", "Franty Kocourka", 4 },
    { "Cukrovar Čakovice", "Florenc", 10 }, { "Vozovna Střešovice (Muzeum MHD)", "Cukrovar Čakovice", 5 },
    { "U Průhonu", "Praha-Dejvice", 8 }, { "Sukovská", "Nové Podolí", 10 },
    { "Poliklinika Čumpelíkova", "Nové Podolí", 5 }, { "Florenc", "Na Sklonku", 5 },
    { "Praha-Dejvice", "Cukrovar Čakovice", 6 }, { "Franty Kocourka", "Šumavská", 10 },
    { "Florenc", "Vozovna Střešovice (Muzeum MHD)", 6 }, { "Pod Říhákem", "Florenc", 3 },
    { "Novoborská", "Šumavská", 8 }, { "Sukovská", "Šumavská", 8 },
    { "Nové Podolí", "Pod Říhákem", 10 }, { "Vozovna Střešovice (Muzeum MHD)", "Na Sklonku", 4 },
    { "Cukrovar Čakovice", "Nádraží Modřany", 9 }, { "Vozovna Střešovice (Muzeum MHD)", "Šumavská", 5 },
    { "Novoborská", "Cukrovar Čakovice", 3 }, { "Šumavská", "Na Sklonku", 5 },
    { "Poliklinika Čumpelíkova", "Vozovna Střešovice (Muzeum MHD)", 5 }, { "Nové Podolí", "Na Sklonku", 7 },
    { "Pod Říhákem", "U Průhonu", 5 }, { "Šumavská", "Sukovská", 7 },
    { "Šumavská", "Nádraží Modřany", 4 }, { "Nové Podolí", "Šumavská", 10 },
    { "Poliklinika Čumpelíkova", "Šumavská", 9 }, { "Šumavská", "Florenc", 3 },
    { "Franty Kocourka", "Nové Podolí", 3 }, { "U Průhonu", "Vozovna Střešovice (Muzeum MHD)", 5 },
    { "Praha-Dejvice", "Florenc", 3 }, { "Nové Podolí", "Praha-Dejvice", 1 },
    { "Pod Říhákem", "Florenc", 4 },
  }}},
  Test{5, {{ 
    "U Vojenské nemocnice", "Kuchyňka", "V Korytech", "Kelerka", "Vozovna Strašnice",
    "Geologická", "U Studánky", "U Jahodnice", "Hadovka", "Barrandovská",
    "K Netlukám", "Obchodní centrum Sárská", "Praha-Smíchov", "Sušická", "Moráň",
    "Praha-Bubny", "Rajská zahrada", "Strossmayerovo náměstí", "Průmstav",
  }, {
    { "U Vojenské nemocnice", "Vozovna Strašnice", 10 }, { "K Netlukám", "Obchodní centrum Sárská", 6 },
    { "Praha-Smíchov", "U Jahodnice", 8 }, { "Praha-Smíchov", "K Netlukám", 7 },
    { "Vozovna Strašnice", "Kelerka", 5 }, { "Obchodní centrum Sárská", "Geologická", 1 },
    { "K Netlukám", "Praha-Smíchov", 3 }, { "V Korytech", "Geologická", 9 },
    { "V Korytech", "Vozovna Strašnice", 1 }, { "Vozovna Strašnice", "V Korytech", 8 },
    { "U Vojenské nemocnice", "Kuchyňka", 6 }, { "Kelerka", "Geologická", 2 },
    { "Praha-Bubny", "Strossmayerovo náměstí", 4 }, { "Kuchyňka", "V Korytech", 3 },
    { "Praha-Smíchov", "Praha-Bubny", 9 }, { "Obchodní centrum Sárská", "Moráň", 1 },
    { "Kelerka", "V Korytech", 10 }, { "Kelerka", "V Korytech", 5 },
    { "Hadovka", "Rajská zahrada", 6 }, { "V Korytech", "Geologická", 7 },
    { "Sušická", "Praha-Smíchov", 2 }, { "Barrandovská", "K Netlukám", 6 },
    { "V Korytech", "Kelerka", 8 }, { "K Netlukám", "V Korytech", 6 },
    { "U Studánky", "Kuchyňka", 5 }, { "Hadovka", "Barrandovská", 6 },
    { "Praha-Bubny", "U Studánky", 4 }, { "Moráň", "K Netlukám", 4 },
    { "Strossmayerovo náměstí", "Kelerka", 7 }, { "Barrandovská", "U Jahodnice", 4 },
    { "V Korytech", "Kuchyňka", 5 }, { "Průmstav", "Praha-Smíchov", 5 },
    { "Geologická", "V Korytech", 7 }, { "Rajská zahrada", "Kuchyňka", 7 },
    { "U Jahodnice", "Kuchyňka", 1 }, { "Praha-Smíchov", "Sušická", 8 },
    { "K Netlukám", "Obchodní centrum Sárská", 3 }, { "Geologická", "Kelerka", 4 },
    { "Obchodní centrum Sárská", "K Netlukám", 2 }, { "Obchodní centrum Sárská", "K Netlukám", 6 },
    { "Hadovka", "U Studánky", 10 }, { "K Netlukám", "Sušická", 4 },
    { "Moráň", "U Vojenské nemocnice", 2 }, { "Obchodní centrum Sárská", "Praha-Smíchov", 3 },
    { "V Korytech", "U Studánky", 10 }, { "Kuchyňka", "Geologická", 6 },
    { "K Netlukám", "Moráň", 4 }, { "Sušická", "U Vojenské nemocnice", 5 },
    { "Kuchyňka", "U Vojenské nemocnice", 8 },
  }}},
  Test{0, {{ 
    "Na Lukách", "Plánická", "U Mezníku", "Bílá Hora", "Psohlavců",
    "Koupaliště Čakovice", "Volha", "Dolnopočernický hřbitov", "Studentský dům", "U Rozcestí",
    "Koleje Jižní Město", "Kusá", "Vozovna Pankrác", "Ke Koulce", "Opatov",
    "Nádraží Žvahov - V Násypu", "Na Blanici", "Nádraží Libeň", "Ořechovka", "Ke Kateřinkám",
    "Divadlo pod Palmovkou", "Přístav Radotín", "Žákovská", "Pankrác",
  }, {
    { "Koleje Jižní Město", "U Rozcestí", 7 }, { "Na Lukách", "Psohlavců", 7 },
    { "U Rozcestí", "U Mezníku", 5 }, { "Ke Koulce", "Kusá", 1 },
    { "Nádraží Žvahov - V Násypu", "Nádraží Libeň", 1 }, { "Koleje Jižní Město", "U Rozcestí", 1 },
    { "Koupaliště Čakovice", "Psohlavců", 1 }, { "Žákovská", "Pankrác", 7 },
    { "Kusá", "U Rozcestí", 10 }, { "Psohlavců", "Bílá Hora", 8 },
    { "Volha", "Na Lukách", 4 }, { "Na Lukách", "Plánická", 1 },
    { "Vozovna Pankrác", "Opatov", 7 }, { "Koupaliště Čakovice", "Bílá Hora", 1 },
    { "Ořechovka", "Na Blanici", 10 }, { "Psohlavců", "U Mezníku", 1 },
    { "U Mezníku", "Bílá Hora", 8 }, { "Divadlo pod Palmovkou", "U Rozcestí", 10 },
    { "Vozovna Pankrác", "U Mezníku", 7 }, { "Psohlavců", "Koupaliště Čakovice", 2 },
    { "Na Blanici", "Nádraží Žvahov - V Násypu", 2 }, { "Na Lukách", "Bílá Hora", 10 },
    { "Kusá", "Koleje Jižní Město", 10 }, { "Nádraží Libeň", "Ke Koulce", 2 },
    { "Opatov", "Ke Koulce", 10 }, { "Přístav Radotín", "Koupaliště Čakovice", 8 },
    { "Kusá", "Koleje Jižní Město", 5 }, { "Pankrác", "Žákovská", 2 },
    { "Vozovna Pankrác", "Koleje Jižní Město", 6 }, { "Plánická", "Na Lukách", 10 },
    { "Bílá Hora", "Koupaliště Čakovice", 10 }, { "Kusá", "Vozovna Pankrác", 7 },
    { "Koupaliště Čakovice", "Bílá Hora", 8 }, { "Psohlavců", "Koleje Jižní Město", 5 },
    { "Bílá Hora", "U Mezníku", 6 }, { "Psohlavců", "Bílá Hora", 4 },
    { "Koleje Jižní Město", "Kusá", 1 }, { "Přístav Radotín", "Opatov", 5 },
    { "Plánická", "U Mezníku", 3 }, { "Vozovna Pankrác", "Nádraží Žvahov - V Násypu", 1 },
    { "Koupaliště Čakovice", "Bílá Hora", 9 }, { "Nádraží Žvahov - V Násypu", "Opatov", 10 },
    { "Vozovna Pankrác", "Opatov", 9 }, { "Studentský dům", "Volha", 1 },
    { "Žákovská", "Pankrác", 1 }, { "U Mezníku", "Na Lukách", 9 },
    { "Ke Kateřinkám", "Koleje Jižní Město", 6 }, { "U Mezníku", "Plánická", 9 },
    { "Opatov", "Na Blanici", 9 }, { "U Mezníku", "Volha", 8 },
    { "Kusá", "Psohlavců", 2 }, { "Kusá", "Ke Koulce", 2 },
    { "Dolnopočernický hřbitov", "Bílá Hora", 4 },
  }}},
};

template < typename C >
void test(C&& tests) {
  int fail = 0, ok = 0;

  for (auto&& [ ref_s, map ] : tests) {
    auto [ s, _ ] = critical_streets(map);
    if (s == ref_s) ok++;
    else {
      fail++;
      std::cout << "Got " << s << " but expected " << ref_s << std::endl;
    }
  }

  if (fail)
    std::cout << fail << " of " << fail + ok << " tests failed!" << std::endl;
  else
    std::cout << "All " << ok << " tests succeded!" << std::endl;
}

int main() {
  test(TESTS);
}

#endif


