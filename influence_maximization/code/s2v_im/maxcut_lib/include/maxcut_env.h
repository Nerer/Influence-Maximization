#ifndef MAXCUT_ENV_H
#define MAXCUT_ENV_H

#include "i_env.h"

class MaxcutEnv : public IEnv
{
public:

    MaxcutEnv(double _norm, double _ratio_seeds, double _prob);

    virtual void s0(std::shared_ptr<Graph>  _g) override;

    virtual double step(int a) override;

    virtual int randomAction() override;

    virtual bool isTerminal(double ratio_s = -1.) override;

    double getReward(double oldCutWeight);

    double cutWeight;
    int num_seeds;
    std::vector<int> avail_list;
    std::vector<int> T;
};

#endif