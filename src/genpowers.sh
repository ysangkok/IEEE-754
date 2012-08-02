#!/bin/sh
#for i in `seq 0 1087`; do echo char two$i\[\] = \"$(echo "2 $i ^ p"  | dc | tr -d '\\' | tr -d '\n' )\"\;; done
for i in `seq 0 512`; do echo char two$i\[\] = \"$(echo "((2^1088-2^1087)/512)*$i" | bc | tr -d '\\' | tr -d '\n' )\"\;; done
#for i in `seq 0 1087`; do echo char sum$i\[\] = \"$(echo "2^$i+2^1087"  | bc | tr -d '\\' | tr -d '\n' )\"\;; done
for i in `seq 0 512`; do echo char sum$i\[\] = \"$(echo "2^1087+((2^1088-2^1087)/512)*$i"  | bc | tr -d '\\' | tr -d '\n' )\"\;; done

echo "char* twos[] = {";
for i in `seq 0 512`; do
	echo "two$i,"
done
echo "};";
echo "char* sums[] = {";
for i in `seq 0 512`; do
	echo "sum$i,"
done
echo "};";
