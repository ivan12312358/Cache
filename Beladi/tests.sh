#!/bin/bash

for (( i=1; i <= 10; i++ ))
do
beladi_hits=$(../beladi < ../Tests/test$i/test)
answ=$(cat ../Tests/test$i/beladi.answ)
if [ $beladi_hits = $answ ]
then
echo "Test $i: Done" 
else
echo "Test $i: Failed"
echo "Beladi $beladi_hits Answer $answ"
fi
done