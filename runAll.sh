#!/bin/bash

source setupGallery.sh
make

if [ "$2" != "rerun" ]; then
  python getFilePathList.py ${1} | grep root | sort -t '_' -k 3 >> filelist_run${1}.txt
  
  i=0
  while read line
  do
    array[ $i ]="$line"        
    (( i++ ))
  done < <(cat filelist_run${1}.txt)
  
  mkdir -p files_run${1}
  
  num=0
  for j in "${array[@]}"
  do
    num=$((num+1))
    lar -c standard_eventbuilder.fcl $j
  
    mv standard_eventbuilder.root files_run${1}/file_${num}.root
  done
  
  rm filelist_run${1}.txt
fi

find $PWD/files_run${1} | grep root | sort -t '_' -nk2 >> new_filelist_run${1}.txt

./PixLArPurityStudy new_filelist_run${1}.txt

rm new_filelist_run${1}.txt
mv output.root output_run${1}.root
