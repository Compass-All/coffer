echo 0 > /proc/sys/kernel/hung_task_panic

echo 0 > /proc/sys/kernel/softlockup_panic

echo 0 > /proc/sys/kernel/watchdog

dmesg -n 1

taskset 0x1 /bin/bash
