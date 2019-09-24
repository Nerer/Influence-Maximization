import sys
import numpy as np
import networkx as nx
import cPickle as cp
import random
import ctypes
import os
import time
from tqdm import tqdm

sys.path.append( '%s/maxcut_lib' % os.path.dirname(os.path.realpath(__file__)) )
from maxcut_lib import MaxcutLib


def runIC(G, S, p=.1):
    ''' Runs independent cascade model.
    Input: G -- networkx graph object
    S -- initial set of vertices
    p -- propagation probability
    Output: T -- resulted influenced set of vertices (including S)
    '''
    from copy import deepcopy
    from random import random
    T = [] # copy already selected nodes
    for i in range(len(S)):
        if (S[i] > 0):
            T.append(i)
    # ugly C++ version
    i = 0
    while i < len(T):
        for v in G[T[i]]:  # for neighbors of a selected node
            if v not in T:  # if it wasn't selected yet  # count the number of edges between two nodes
                if random() <= 1 - (1 - p):  # if at least one of edges propagate influence
                    T.append(v)
        i += 1

    # neat pythonic version
    # legitimate version with dynamically changing list: http://stackoverflow.com/a/15725492/2069858
    # for u in T: # T may increase size during iterations
    #     for v in G[u]: # check whether new node v is influenced by chosen node u
    #         w = G[u][v]['weight']
    #         if v not in T and random() < 1 - (1-p)**w:
    #             T.append(v)
    return len(T)


def find_model_file(opt):
    max_n = int(opt['max_n'])
    min_n = int(opt['min_n'])
    log_file = '%s/log-%d-%d.txt' % (opt['save_dir'], min_n, max_n)

    best_r = -10000000
    best_it = -1
    with open(log_file, 'r') as f:
        for line in f:
            if 'average' in line:
                line = line.split(' ')
                it = int(line[1].strip())
                r = float(line[-1].strip())
                if r > best_r:
                    best_r = r
                    best_it = it
    assert best_it >= 0
    print 'using iter=', best_it, 'with r=', best_r
    return '%s/nrange_%d_%d_iter_%d.model' % (opt['save_dir'], min_n, max_n, best_it)

def old_pkl(n_test):
    with open(opt['data_test'], 'rb') as f:
        d = cp.load(f)
    for i in range(n_test):
        yield d['g_list'][i]

def seq_pkl(n_test):
    with open(opt['data_test'], 'rb') as f:
        for i in range(n_test):
            yield cp.load(f)

if __name__ == '__main__':
    api = MaxcutLib(sys.argv)
    
    opt = {}
    for i in range(1, len(sys.argv), 2):
        opt[sys.argv[i][1:]] = sys.argv[i + 1]
    print opt
    model_file = find_model_file(opt)
    assert model_file is not None
    print 'loading', model_file
    sys.stdout.flush()
    print('ok')

    api.LoadModel(model_file)

    #test_name = opt['data_test'].split('/')[-1]
    #result_file = '%s/test-%s-gnn-%s-%s.csv' % (opt['save_dir'], test_name, opt['min_n'], opt['max_n'])

    #n_test = 1000
    #frac = 0.0
    #f = open(opt['data_test'], 'rb')
    '''
    with open(result_file, 'w') as f_out:
        print 'testing'
        sys.stdout.flush()
        for i in tqdm(range(n_test)):
            g = cp.load(f)
            api.InsertGraph(g, is_test=True)
            t1 = time.time()
            val, sol = api.GetSol(i, nx.number_of_nodes(g), float(opt['ratio_seeds']))
            t2 = time.time()
            f_out.write('%.8f,' % val)
            f_out.write('%d' % sol[0])
            for i in range(sol[0]):
                f_out.write(' %d' % sol[i + 1])
            f_out.write(',%.6f\n' % (t2 - t1))
            frac += val
    '''


    g = nx.Graph()
    for rat in [0.05]:
        with open('node1000data.txt') as f:
            n, m = f.readline().split()
            for line in f:
                u, v = map(int, line.split())
                g.add_edge(u, v)
            #g = nx.barabasi_albert_graph(n = 5000, m = 4)
            #print('ok1')
            api.InsertGraph(g, is_test=True)
            #print('ok2')

            val, sol = api.GetSol(0, nx.number_of_nodes(g), rat)
            influence = 0
            for t in range(20):
                influence = influence + runIC(g, sol, 0.1)
            print influence / 20
