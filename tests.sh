PROGRAM="./release/radix_join" 
OUTPUT_FILE=out.csv
rsize=10000000

if [ ! -e "$OUTPUT_FILE" ]; then
	echo "threads,partitions,left cardinality,right cardinality,random range max,total time,partitioning time,join time,selectivity" > out.csv
fi

for threads in 16
do
	for max_rand in 1000000
	do
		for lsize in 64000 128000 512000 1000000
		do
			for radix_bits in 4 6 8 10 12 14
			do
				rsize="$lsize"
				"$PROGRAM" $threads $radix_bits $lsize $rsize $max_rand >> out.csv
			done
		done
	done
done
