#!/bin/bash
make

dirName=$1
passCounter=0
totalCounter=0

for File in "$dirName"/*.cmm
do
	(( totalCounter=$totalCounter+1 ))
	File=$(echo ${File%.*})
	FileCmm="$File".cmm
	FileMyO="$File"_my.tokens
	FileOrO="$File".tokens
	./part1 <"$FileCmm" >"$FileMyO"
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

