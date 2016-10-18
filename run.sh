#!/bin/bash
#run.sh
arg=`pwd`
if [ $# -eq 1 ]
	then
		arg="$arg/src/policy_simulator $1"
		eval $arg
fi
if [ $# -lt 1 ] || [ $# -gt 1 ]
	then 
		arg="$arg/src/policy_simulator $arg/test_cases/test.dat"
		eval $arg
fi
