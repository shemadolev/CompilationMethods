#!/bin/bash
flex part1.lex
gcc -ll lex.yy.c

dirName=$1
passCounter=0
totalCounter=0

for File in "$dirName"/*.cmm
do
	(( totalCounter=$totalCounter+1 ))
	File=$(echo ${File%.*})
	./a.out <"$File.cmm" >"$File_my.tokens"
	diffRes=$(diff "$File.tokens" "$File_my.tokens")
	if [[ -z $diffRes ]]; then #diff is empty == files are eqaul
		echo "Same: $File"
		(( passCounter=$passCounter+1 ))
	else
		echo "--- Different: $File.tokens != $File_my.tokens"
		echo "$diffRes"
	fi
done
echo "#######################"
echo "Summary: Passed $passCounter/$totalCounter"

