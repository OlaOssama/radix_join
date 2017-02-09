PROGRAM="./release/radix_join" 
OUTPUT_FILE=out.csv
rsize=10000

if [ ! -e "$OUTPUT_FILE" ]; then
	echo "threads,partitions,left cardinality,right cardinality,random range max,total time,partitioning time,join time" > out.csv
fi

for partitions in 1 2 4
do
	for max_rand in 10000 
	do
		for threads in 1 2 4
		do
			for lsize in 10000
			do
				"$PROGRAM" $threads $(($threads*$partitions)) $lsize $rsize $max_rand >> out.csv
			done
		done
	done
done
