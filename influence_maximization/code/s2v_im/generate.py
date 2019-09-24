import networkx as nx
g = nx.barabasi_albert_graph(n = 1000, m = 4)

with open('node1000data.txt', 'w') as fw:
    fw.write("%d %d\n"%(g.number_of_nodes(), g.number_of_edges()))
    for u, v in g.edges():
        fw.write("%d %d\n"%(u, v))


