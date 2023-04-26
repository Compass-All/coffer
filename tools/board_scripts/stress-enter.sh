#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Usage: $0 [mem_size] [first] [end]"
	exit 0
fi

size=$1
first=$2
end=$3

OUTPUT=log-enter.txt
echo "enter stress-ng enclaves: $size, $first - $end" > $OUTPUT
for i in `seq $first $end`
do
	nohup taskset 0xe /prog/host/enter_enclave $i /prog/enclave/stress-ng/stress-ng --memrate 1 --memrate-wr 20000 --memrate-rd 10000 --memrate-bytes $size & >> $OUTPUT &
done