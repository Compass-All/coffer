if [ $# -ne 2 ]
then
    echo "Usage: $0 [payload] [repeat]"
    exit 1
fi

/prog/host/reset_coffer
repeat=$2
for i in `seq 1 $repeat`
do
	taskset 0xe /prog/host/test_eapp $1
done

# /root/dump_timer.sh 1 $repeat `date -u +%Y%m%d_%H%M%S`.log
/root/dump_timer.sh 1 $repeat `basename $1`_`date -u +%Y%m%d_%H%M%S`.log

