#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 [number]"
	exit 0
fi

NUM=$1

first=`/prog/host/get_eid_count`
end=`expr $first + $NUM - 1`

echo $first $end

OUTPUT=log-create.txt

echo "create enclaves: $first - $end" > $OUTPUT
for i in `seq $first $end`
do
	nohup taskset 0xe /prog/host/create_enclave >> $OUTPUT && taskset 0xe /prog/host/enter_enclave $i /prog/enclave/rv8/rv8_qsort >> $OUTPUT &
done