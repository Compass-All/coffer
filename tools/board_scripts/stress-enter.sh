#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 [mem_size] [first] [end]"
	exit 0
fi

first=$1
end=$2

OUTPUT=log-enter.txt
echo "enter stress-ng enclaves: $size, $first - $end" > $OUTPUT
for i in `seq $first $end`
do
	# nohup taskset 0xe /prog/host/enter_enclave $i /prog/enclave/stress-ng/stress-ng --matrix 1 --matrix-ops 1 --matrix-method prod --matrix-size 512 --matrix-yx & >> $OUTPUT &
	# nohup taskset 0xe /prog/host/enter_enclave $i /prog/enclave/stress-ng/stress-ng --matrix 1 --matrix-ops 1 --matrix-method negate --matrix-size 2900 --matrix-yx & >> $OUTPUT &
	# nohup taskset 0xe /prog/host/enter_enclave $i /prog/enclave/stress-ng/stress-ng --vm 1 --vm-ops 10000 --vm-bytes 102m --vm-keep --vm-method read64 & >> $OUTPUT &
	nohup taskset 0xe /prog/host/enter_enclave $i /prog/enclave/stress-ng/stress-ng --vm-addr 1 --vm-ops 100000 --vm-addr-method bitposn & >> $OUTPUT &
	# nohup taskset 0xe /prog/host/enter_enclave $i /prog/enclave/stress-ng/stress-ng --qsort 1 --qsort-size 262144 & >> $OUTPUT &
	# nohup taskset 0xe /prog/host/enter_enclave $i /prog/enclave/stress-ng/stress-ng --memrate 1 --memrate-wr 20000 --memrate-rd 10000 --memrate-bytes $size & >> $OUTPUT &
done