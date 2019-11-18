#!/bin/bash

# Print the heading
awk 'BEGIN { printf "%-10s%-10s%-10s%-10s\n", "PID", "USER", "RSS", "COMMAND"; }'

for process in `ls -vd /proc/[0-9]*`; do
    # Retrieve the pid from the directory name
    pid=${process/\/proc\//}
    
    # Retrieve the user from the owner of the directory
	user=`stat -c %U $process`
	
	# Retrieve the rss
	rss=`grep 'VmRSS' $process'/status' | sed -e 's/.*\t *//' | sed -e 's/[Kk]B//'`
	
	# Check if the rss is empty
	if [ -z "$rss" ];then
	    rss=0
	fi
    
    # Retrieve the command
	cmd=`cat $process"/cmdline" | tr '\0' ' '`
	
	# Check if the command is empty
	if [ -z "$cmd" ];then
	    cmd=`grep 'Name' $process'/status' | sed -e 's/.*\t *//'`
	    cmd=[$cmd]
	fi
	
	# Print the data
	awk -v pid="$pid" -v user="$user" -v rss="$rss" -v cmd="$cmd" 'BEGIN { printf "%-10s%-10s%-10s%-10s\n", pid, user, rss, cmd }'
done
