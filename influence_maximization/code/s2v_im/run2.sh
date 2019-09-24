#!/bin/bash

g_type=barabasi_albert

result_root=results/dqn-$g_type

# max belief propagation iteration
max_bp_iter=3

# embedding size
embed_dim=64

# gpu card id
dev_id=1

# max batch size for training/testing
batch_size=128

net_type=NewQNet
avg_global=0

# set reg_hidden=0 to make a linear regression
reg_hidden=32

# learning rate
learning_rate=0.0001

# init weights with rand normal(0, w_scale)
w_scale=0.01

# nstep
n_step=1

min_n=80
max_n=100

ratio_seeds=0.1
prob=0.1

num_env=1

mem_size=500000

max_iter=100000

# folder to save the trained model
save_dir=$result_root/ntype-$net_type-embed-$embed_dim-nbp-$max_bp_iter-rh-$reg_hidden-ag-$avg_global-nodes-$max_n

if [ ! -e $save_dir ];
then
    mkdir -p $save_dir
fi

python main.py \
    -net_type $net_type \
    -dev_id $dev_id \
    -avg_global $avg_global \
    -n_step $n_step \
    -min_n $min_n \
    -max_n $max_n \
    -ratio_seeds $ratio_seeds \
    -prob $prob \
    -num_env $num_env \
    -max_iter $max_iter \
    -mem_size $mem_size \
    -g_type $g_type \
    -learning_rate $learning_rate \
    -max_bp_iter $max_bp_iter \
    -net_type $net_type \
    -max_iter $max_iter \
    -save_dir $save_dir \
    -embed_dim $embed_dim \
    -batch_size $batch_size \
    -reg_hidden $reg_hidden \
    -momentum 0.9 \
    -l2 0.00 \
    -w_scale $w_scale \
    2>&1 | tee $save_dir/log-$min_n-${max_n}.txt
