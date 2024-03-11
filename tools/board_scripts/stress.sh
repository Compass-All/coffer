if [ $# -ne 2 ]
then
    echo "Usage: $0 [mem_size GB] [repeat]"
    exit 1
fi

repeat=$2
mem_size=$1
# /prog/enclave/stress-ng/stress-ng --vm 1 --vm-bytes 32m --vm-ops 65536 --vm-keep --vm-madvise normal --vm-method all --metrics-brief
STRESSNG="/prog/enclave/stress-ng/stress-ng --vm 1 --vm-bytes ${mem_size}g --vm-ops 65536 --vm-keep --vm-madvise normal --vm-method all --metrics-brief"

echo "reset_coffer"
/prog/host/reset_coffer
echo "stress-ng"
echo $STRESSNG
for i in `seq 1 $repeat`
do
    echo "taskset stress-ng"
    taskset 0xe /prog/host/test_eapp $STRESSNG
done
/root/dump_timer.sh 1 $repeat stress-ng-vm_${mem_size}g.log
