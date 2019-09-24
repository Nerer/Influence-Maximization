#include "simulator.h"
#include "graph.h"
#include "nstep_replay_mem.h"
#include "i_env.h"
#include "maxcut_env.h"
#include "nn_api.h"
#include "config.h"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <fstream>
#include <set>
std::vector<IEnv*> Simulator::env_list;
std::vector< std::shared_ptr<Graph> > Simulator::g_list;
std::vector< std::vector<int>* > Simulator::covered;
std::vector< std::vector<double>* > Simulator::pred;

std::default_random_engine Simulator::generator;
std::uniform_real_distribution<double> Simulator::distribution(0.0,1.0);

void Simulator::Init(int num_env)
{
    env_list.resize(num_env);
    g_list.resize(num_env);
    covered.resize(num_env);
    pred.resize(num_env);
    for (int i = 0; i < num_env; ++i)
    {
        g_list[i] = nullptr;
        pred[i] = new std::vector<double>(cfg::max_n);
    }
}

void Simulator::run_simulator(int num_seq, double eps, double ratio_s)
{
    int num_env = env_list.size();
    int n = 0;
    while (n < num_seq)
    {
        //std::cerr << std::flush << "n = " << n << " num_seq = " << num_seq << std::endl;
        for (int i = 0; i < num_env; ++i)
        {
            //std::cerr << std::flush << "i = " << i  << " num_env = " << num_env << std::endl;
            //std::cerr << "graph = " << env_list[i]->graph  << " terminal = " << env_list[i]->isTerminal() << std::endl;

            if (!env_list[i]->graph || env_list[i]->isTerminal(ratio_s))
            {   
                if (env_list[i]->graph && env_list[i]->isTerminal(ratio_s))
                {
                    //std::cerr << "oh shoot^2 " << "num_node = " << env_list[i]->graph->num_nodes << std::endl;

                    n++;
                    NStepReplayMem::Add(env_list[i]);
                }
                //std::cerr << std::flush<< "oh shoot" << std::endl;
                env_list[i]->s0(GSetTrain.Sample());

                g_list[i] = env_list[i]->graph;
                covered[i] = &(env_list[i]->action_list);
            }
        }

        if (n >= num_seq)
            break;            

        bool random = false;
        if (distribution(generator) >= eps)
            Predict(g_list, covered, pred);
        else        
            random = true;
        //std::cerr << std::flush << "random = " << random  << std::endl;

        int a_t;
        for (int i = 0; i < num_env; ++i)
        {
            if (random)
                a_t = env_list[i]->randomAction();
            else 
                a_t = arg_max(env_list[i]->graph->num_nodes, pred[i]->data(), env_list[i]->seed_set);
            env_list[i]->step(a_t);
            //std::cerr << std::flush<< "a_t = " << a_t  << std::endl;
        }
    }
    //std::cerr << std::flush<< "finish" << std::endl;

}

int arg_max(int n, const double* scores, const std::set<int> cset)
{
    int pos = -1; 
    double best = -10000000;
    for (int i = 0; i < n; ++i)
        if ((pos == -1 || scores[i] > best) && cset.count(i) == 0)
        {
            pos = i;
            best = scores[i];
        }
    assert(pos >= 0);
    return pos;
}

double max(int n, const double* scores)
{
    int pos = -1; 
    double best = -10000000;
    for (int i = 0; i < n; ++i)
        if (pos == -1 || scores[i] > best)
        {
            pos = i;
            best = scores[i];
        }
    assert(pos >= 0);
    return best;
}
