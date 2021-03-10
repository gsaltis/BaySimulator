./createBayConfig.sh $1
./slcan_remove
./slcan_add
export a=`ifconfig eth0 | head -2 | tail -1 | awk '{print $2;}' | awk 'BEGIN{FS="."} {print $4;}' | awk 'BEGIN{FS=""} { print $3;}'`
export b=`echo BayConfig.txt`
echo $b
./baysim -D $b -w Web -v
