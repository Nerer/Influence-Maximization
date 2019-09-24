#!/bin/csh
setenv TBBROOT "/newNAS/Share/yjc/proj/proj/project/graph_comb_opt/code/s2v_im/maxcut_lib" #
setenv tbb_bin "/newNAS/Share/yjc/proj/proj/project/graph_comb_opt/code/s2v_im/maxcut_lib" #
if (! $?CPATH) then #
    setenv CPATH "${TBBROOT}/include" #
else #
    setenv CPATH "${TBBROOT}/include:$CPATH" #
endif #
if (! $?LIBRARY_PATH) then #
    setenv LIBRARY_PATH "${tbb_bin}" #
else #
    setenv LIBRARY_PATH "${tbb_bin}:$LIBRARY_PATH" #
endif #
if (! $?LD_LIBRARY_PATH) then #
    setenv LD_LIBRARY_PATH "${tbb_bin}" #
else #
    setenv LD_LIBRARY_PATH "${tbb_bin}:$LD_LIBRARY_PATH" #
endif #
 #
