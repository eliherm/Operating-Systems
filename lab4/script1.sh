#!/bin/bash

# Print the heading
awk 'BEGIN { printf "%-10s%-10s%-10s%-10s\n", "PID", "USER", "RSS", "COMMAND"; }'

for process in /proc/[0-9]*; do
	pid=`echo $process | sed -e 's/\/proc\///'`
	user=`stat -c %U $process`
	rss=`grep "VmRSS" $process"/status"`

	if [ -n "$rss" ]; then
        rss=${rss/'VmRSS:'/}
        rss=${rss/'kB'/}
	fi
	
	cmd=`cat $process"/cmdline" | tr '\0' ' '`
	awk -v pid="$pid" -v user="$user" -v rss="$rss" -v cmd="$cmd" 'BEGIN { printf "%-10s%-10s%-10s%-10s\n", pid, user, rss, cmd }'
done
