#!/bin/bash

if [ $# -ne 3 ]
then
    echo "Usage: $0 [eid_start] [eid_end] [output file]"
    exit 1
fi

eid_start=$1
eid_end=$2
out=$3

echo "" > $out

MAX=2048
for i in `seq $eid_start $eid_end`
do
    /prog/host/dump_timer $i >> $out
done