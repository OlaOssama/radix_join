PROGRAM="./release/radix_join" 
OUTPUT_FILE=out.csv
rsize=1000000

if [ ! -e "$OUTPUT_FILE" ]; then
	echo "threads,partitions,left cardinality,right cardinality,random range max,total time,partitioning time,join time" > out.csv
fi

for radix_bits in 4
do
	for max_rand in 1000000
	do
		for threads in 1 2 4
		do
			for lsize in 10000 1000000
			do
				"$PROGRAM" $threads $radix_bits $lsize $rsize $max_rand >> out.csv
			done
		done
	done
done
