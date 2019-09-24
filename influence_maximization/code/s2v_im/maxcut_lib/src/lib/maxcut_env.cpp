#include "maxcut_env.h"
#include "graph.h"
#include <cassert>
#include <random>
#include <iostream>
MaxcutEnv::MaxcutEnv(double _norm, double _ratio_seeds, double _prob) : IEnv(_norm, _ratio_seeds, _prob)
{

}

void MaxcutEnv::s0(std::shared_ptr<Graph> _g)
{
    graph = _g;
    cut_set.clear();
    seed_set.clear();
    action_list.clear();
    cutWeight = 0;
    num_seeds = 0;
    state_seq.clear();
    act_seq.clear();
    reward_seq.clear();
    sum_rewards.clear();
    T.clear();
}

double MaxcutEnv::step(int a)
{
    assert(graph);
    assert(seed_set.count(a) == 0);
    assert(a >= 0 && a < graph->num_nodes);

   // std::cerr << std::flush << "Action is " << a << std::endl;
    state_seq.push_back(action_list);
    act_seq.push_back(a);

    cut_set.insert(a);
    seed_set.insert(a);
    action_list.push_back(a);

    num_seeds += 1;
    double old_cutWeight = (double)T.size();
  //  std::cerr << std::flush << "Old Influence is  " << old_cutWeight << std::endl;

    /*for (auto& neigh : graph->adj_list[a])
        if (cut_set.count(neigh.first) == 0)
            cutWeight += neigh.second;
        else
            cutWeight -= neigh.second;
    */
    T.push_back(a);
    for (int i = (int)T.size() - 1;  i < (int)T.size(); i++) {
        for (auto& neigh: graph->adj_list[T[i]]) {
            if (cut_set.count(neigh.first) == 0) {
              if ((rand() % 1001 * 1.0) / 1000 <= prob) {
                cut_set.insert(neigh.first);
                T.push_back(neigh.first);
              }
            }
        }
    }

    cutWeight = (double)T.size();
    double r_t = getReward(old_cutWeight);
    reward_seq.push_back(r_t);
    sum_rewards.push_back(r_t);  

    return r_t;
}

int MaxcutEnv::randomAction()
{
    assert(graph);
    avail_list.clear();

    for (int i = 0; i < graph->num_nodes; ++i)
        if (cut_set.count(i) == 0)
            avail_list.push_back(i);
    
    assert(avail_list.size());
    int idx = rand() % avail_list.size();
    return avail_list[idx];
}

bool MaxcutEnv::isTerminal(double ratio_s)
{
    assert(graph);
  //  std::cerr << std::flush << "num_seeds = " << num_seeds << " num_nodes = " << graph->num_nodes << " ratio = " << ratio_seeds << std::endl;
    if (ratio_s > 0) {
        return num_seeds >= ratio_s * graph->num_nodes;
    }
    return num_seeds >= ratio_seeds * graph->num_nodes;
    //return ((int)cut_set.size() + 1 >= graph->num_nodes);
}

double MaxcutEnv::getReward(double old_cutWeight)
{
    return (cutWeight - old_cutWeight) / norm;
}