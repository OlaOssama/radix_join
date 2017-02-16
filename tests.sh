PROGRAM="./release/radix_join" 
OUTPUT_FILE=out.csv
rsize=10000000

echo "threads,partitions,left cardinality,right cardinality,random range max,total time,partitioning time,join time,selectivity" > out.csv

for threads in 16
do
	for max_rand in 512000
	do
		for lsize in 128000 512000 1000000 3000000
		do
			for radix_bits in 1 2 3 4 5 6 7 8
			do
				max_rand="$lsize"
				rsize="$lsize"
				"$PROGRAM" "$threads" "$radix_bits" "$lsize" "$rsize" "$max_rand" >> "$OUTPUT_FILE"
			done
		done
	done
done
