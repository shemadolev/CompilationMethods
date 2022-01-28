#!/bin/bash
make

passCounter=0
totalCounter=0

testFolder="testsuit"
if [[ $# -gt 0 ]]; then
	testFolder=$1
fi

RskList=""

for File in "$testFolder"/*.cmm
do
	# (( totalCounter=$totalCounter+1 ))
	File=$(echo ${File%.*})
	
	FileCmm="$File".cmm
	FileRsk="$File".rsk
	./rx-cc "$FileCmm" #This creates FileRsk
	RskList="$RskList $FileRsk"
done
./rx-linker "$RskList"
Prog="$testFolder.e"
mv out.e "$Prog"

for File in "$testFolder"/*.input
do
	File=$(echo ${File%.*})
	
	FileIn="$File".input
	FileExp="$File".exp
	FileMy="$File".my
	
	./rx-vm1.1 "$Prog" <"$FileIn" >"$FileMy"

	diffRes=$(diff "$FileExp" "$FileMy")
	if [[ -z $diffRes ]]; then #diff is empty == files are eqaul
		echo "$File: Same"
	else
		echo "$File: --- Different: $FileExp != $FileMy"
		echo "$diffRes"
	fi
	
done

#make clean
