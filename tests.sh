PROGRAM="./release/radix_join" 
OUTPUT_FILE=out.csv
rsize=10000000

if [ ! -e "$OUTPUT_FILE" ]; then
	echo "threads,partitions,left cardinality,right cardinality,random range max,total time,partitioning time,join time" > out.csv
fi

for threads in 16
do
	for max_rand in 128000
	do
		for lsize in 128000 256000 512000 1000000
		do
			for radix_bits in 4 8 12 16
			do
				rsize="$lsize"
				"$PROGRAM" $threads $radix_bits $lsize $rsize $max_rand >> out.csv
			done
		done
	done
done
