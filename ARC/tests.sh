#!/bin/bash

for ((i=1;i<=10;i++))
do
arc_hits=$(../arc < ../Tests/test$i/test)
answ=$(cat ../Tests/test$i/arc.answ)
if [ $arc_hits = $answ ]
then
echo "Test $i: Done" 
else
echo "Test $i: Failed"
echo "Beladi $arc_hits Answer $answ"
fi
done