#!/bin/bash

sshd_run=1
app_run=1
app_path=.
app_name=nair_server
app_params=nairserver.conf

if [[ "$sshd_run" == 1 ]]; then
	/usr/sbin/sshd
fi

if [[ "$app_run" == 1 ]]; then
	$app_path/$app_name -f $app_params
fi
	
trap cleanup 1 2 3 8 9 15
function cleanup(){	
	pids=`ps -A | grep -E "($app_name)|sshd" | awk '{print $1}'`
	if [[ "$pids" != "" ]]; then
		kill $pids >/dev/null 2>/dev/null
		echo "Kill App..."
	fi
	app_run=($app_run-1)
}

app_code=0
while(($app_run>0))
do
    	echo "Runing..."
    	pids=`ps -A | grep $app_name | awk '{print $1}'`
    	if [[ "$pids" == "" ]]; then
		echo "App Is Died..."
		if [[ "$sshd_run" == 1 ]]; then
			sshdpids=`ps -A | grep sshd | awk '{print $1}'`
			if [[ "$sshdpids" != "" ]]; then
				kill $sshdpids >/dev/null 2>/dev/null
			fi
		fi
    		app_code=9
		break
    	fi
    	sleep 1
done

echo "Stoped..."

exit $app_code
