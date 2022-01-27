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

	FileInput="$File".in
	FileCmm="$File".cmm
	FileRsk="$File".rsk
	FileMyE="$File"_my.e
	FileOrE="$File".e
	FileMyO="$File".out
	FileOrO="$File".out

	./rx-cc "$FileCmm" #This creates FileRsk
	./rx-linker "$FileRsk"
	mv out.e "$FileMyE"
	./rx-vm1.1 "$FileMyE" <"$FileInput" >"$FileMyO"
	./rx-vm1.1 "$FileOrE" <"$FileInput" >"$FileOrO"

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
