PROGRAM="./release/radix_join" 
OUTPUT_FILE=out.csv
rsize=10000000

echo "threads,partitions,left cardinality,right cardinality,random range max,total time,partitioning time,join time,selectivity" > out.csv

for threads in 16
do
	for max_rand in 512000
	do
		for lsize in 64000 128000 512000 1000000 3000000
		do
			for radix_bits in 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
			do
				max_rand="$lsize"
				rsize="$lsize"
				"$PROGRAM" "$threads" "$radix_bits" "$lsize" "$rsize" "$max_rand" >> "$OUTPUT_FILE"
			done
		done
	done
done
