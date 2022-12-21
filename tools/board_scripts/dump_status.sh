#!/bin/bash

ALIVE_COUNT=`/prog/host/get_alive_count`

echo "Alive:" $ALIVE_COUNT

MAX=2048
for i in `seq 1 $MAX`
do
	status=`/prog/host/get_enc_status $i`
    if [ $status -ne 0 ]
    then
	    echo -e $i ":\t" $status
    fi
done