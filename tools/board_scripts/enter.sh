#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Usage: $0 [eapp] [first] [end]"
	exit 0
fi

PAYLOAD=$1
first=$2
end=$3

if test ! -f $PAYLOAD
then
	echo "Error: Eapp not found"
	exit 1
fi

OUTPUT=log-enter.txt
echo "enter enclaves: $PAYLOAD, $first - $end" > $OUTPUT
for i in `seq $first $end`
do
	nohup taskset 0xe /prog/host/enter_enclave $i $PAYLOAD >> $OUTPUT &
done