#!/bin/bash

# Print the heading
# awk 'BEGIN { printf "%-10s%-10s%-10s%-10s\n", "PID", "USER", "RSS", "COMMAND"; }'

for process in /proc/[0-9]*; do
	pid=`echo $process | sed -e 's/\/proc\///'`
	userId=`grep "Uid" $process"/status" | sed -e 's/.*\t//'`
	user=`grep '.*:.*:'$userId':.*' "/etc/passwd"`
	rss=`grep "VmRSS" $process"/status" | sed -e 's/.*\t//'`
	cmd=`cat $process"/cmdline" | tr '\0' ' '`
	stat -c %U $process
	# awk -v pid="$pid" -v user="$user" -v rss="$rss" -v cmd="$cmd" 'BEGIN { printf "%-10s%-10s%-10s%-10s\n", pid, user, rss, cmd }'
done
