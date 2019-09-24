#ifndef I_ENV_H
#define I_ENV_H

#include <vector>
#include <set>

#include "graph.h"

class IEnv
{
public:

    IEnv(double _norm, double _ratio_seeds, double _prob) : norm(_norm), ratio_seeds(_ratio_seeds), prob(_prob), graph(nullptr) {}

    virtual void s0(std::shared_ptr<Graph> _g) = 0;

    virtual double step(int a) = 0;

    virtual int randomAction() = 0;

    virtual bool isTerminal(double ratio_s = -1.) = 0;
    
    double norm, ratio_seeds, prob;
    std::shared_ptr<Graph> graph;
    
    std::vector< std::vector<int> > state_seq;
    std::vector<int> act_seq, action_list;
    std::vector<double> reward_seq, sum_rewards;
    std::set<int> cut_set;
    std::set<int> seed_set;

};

#endif