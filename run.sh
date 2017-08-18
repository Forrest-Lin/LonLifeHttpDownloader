IP=""
PORT=""
get_ip () {
	IP=$(ifconfig | grep 'inet' | grep -v 'inet6' | grep -v '127.0.0.1' | awk '{print $2}')
}

get_port() {
	PORT=$(cat etc/server.json |grep $IP -A 1 | grep 'port' |awk '{print $2}')
}

get_ip
get_port
if [ -z $PORT ]
then
	echo "Begin start scheduler"
	cd src/scheduler && ./scheduler
	cd -
else
	echo "Begin start server: $IP"
	cd src/main && ./server $IP $PORT
	if [ $? != 0 ]
	then 
		sh cleanmsg.sh
	fi
	cd -
fi


