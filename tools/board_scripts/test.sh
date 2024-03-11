if [ $# -ne 1 ]
then
    echo "Usage: $0 [repeat]"
    exit 1
fi

repeat=$1
RV8=/prog/enclave/rv8
# STRESSNG=/prog/enclave/stress-ng
STRESSNG="/prog/enclave/stress-ng/stress-ng --vm 1 --vm-bytes 2g --vm-ops 65536 --vm-keep --vm-madvise normal --vm-method all --metrics-brief"
for file in $RV8/*; do
     benchmarkFile=`basename $file`
     if [ "$benchmarkFile" != "rv8_qsort" ] && [ "$benchmarkFile" != "rv8_norx" ] && [ "$benchmarkFile" != "rv8_aes" ] && [ "$benchmarkFile" != "rv8_miniz" ]; then
        echo "reset_coffer"
        /prog/host/reset_coffer
        echo "$benchmarkFile"
       for i in `seq 1 $repeat`
       do
 	  echo "taskset $file"
 	  taskset 0xe /prog/host/test_eapp $file
       done
       echo "/root/dump_timer.sh 1 $repeat `basename $file`.log" 
       /root/dump_timer.sh 1 $repeat $benchmarkFile.log
     fi
done

echo "reset_coffer"
/prog/host/reset_coffer
echo "stress-ng"
for i in `seq 1 $repeat`
do
    echo "taskset stress-ng"
    taskset 0xe /prog/host/test_eapp $STRESSNG
done
