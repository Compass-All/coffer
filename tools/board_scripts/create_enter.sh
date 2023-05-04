#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 [number] [size]"
	exit 0
fi

NUM=$1
size=$2

first=`/prog/host/get_eid_count`
end=`expr $first + $NUM - 1`

echo $first $end

OUTPUT=log-create.txt

echo "create enclaves: $first - $end" > $OUTPUT
for i in `seq $first $end`
do
	nohup taskset 0xe /prog/host/create_enclave >> $OUTPUT && nohup taskset 0xe /prog/host/enter_enclave $i /prog/enclave/stress-ng/stress-ng --memrate 1 --memrate-wr 20000 --memrate-rd 10000 --memrate-bytes $size & >> $OUTPUT &
done