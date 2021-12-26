#!/bin/bash
make

passCounter=0
totalCounter=0

testFolder="my_tests"
if [[ $# -gt 0 ]]; then
	testFolder=$1
fi

for File in "$testFolder"/*.cmm
do
	(( totalCounter=$totalCounter+1 ))
	File=$(echo ${File%.*})
	FileCmm="$File".cmm
	FileMyO="$File"_my.out
	FileOrO="$File".out
	./part2 <"$FileCmm" >"$FileMyO"
	diffRes=$(diff "$FileOrO" "$FileMyO")
	if [[ -z $diffRes ]]; then #diff is empty == files are eqaul
		echo "Same: $File"
		(( passCounter=$passCounter+1 ))
	else
		echo "--- Different: $FileOrO != $FileMyO"
		echo "$diffRes"
	fi
done
echo "#######################"
echo "Summary: Passed $passCounter/$totalCounter"

#make clean
