#!/bin/bash

cleanup() {
	echo -e "\nPID: $$. Interrupt ping test: $(date)" | tee -a log_ping.txt
	exit
}

trap cleanup SIGINT

hosts=(
	"192.168.1.101"
	"192.168.2.101"
	"192.168.3.101"
	"192.168.4.101"
)

if [[ $# -eq 1 ]]; then
	duration=$(( $1 * 60 ))
	ping_option="-w $duration"
	echo -e "Ping during $1 min = $duration sec" | tee log_ping.txt
else
	ping_option=""
	echo -e "Infinite ping, kill -2 -$$ to stop (or ctrl+c if fg)" | tee log_ping.txt
fi

echo -e "\nPID: $$. Start ping test: $(date)\n" | tee -a log_ping.txt

for host in "${hosts[@]}"; do
	ping "$host" -q $ping_option &>> log_ping.txt &
done

wait

echo -e "\nPID: $$. Finish ping test: $(date)" | tee -a log_ping.txt

