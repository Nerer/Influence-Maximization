#!/bin/bash
export TBBROOT="/newNAS/Share/yjc/proj/proj/project/graph_comb_opt/code/s2v_im/maxcut_lib" #
tbb_bin="/newNAS/Share/yjc/proj/proj/project/graph_comb_opt/code/s2v_im/maxcut_lib" #
if [ -z "$CPATH" ]; then #
    export CPATH="${TBBROOT}/include" #
else #
    export CPATH="${TBBROOT}/include:$CPATH" #
fi #
if [ -z "$LIBRARY_PATH" ]; then #
    export LIBRARY_PATH="${tbb_bin}" #
else #
    export LIBRARY_PATH="${tbb_bin}:$LIBRARY_PATH" #
fi #
if [ -z "$LD_LIBRARY_PATH" ]; then #
    export LD_LIBRARY_PATH="${tbb_bin}" #
else #
    export LD_LIBRARY_PATH="${tbb_bin}:$LD_LIBRARY_PATH" #
fi #
 #
