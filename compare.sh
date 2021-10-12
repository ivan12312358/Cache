#!/bin/bash

for (( i=1; i <= 10; i++ ))
do
arc_hits=$(./arc < ./Tests/test$i/test)
beladi_hits=$(./beladi < ./Tests/test$i/test)
echo "Test $i: ARC $arc_hits  <  Beladi $beladi_hits"
done