#!/bin/nash

echo "HT01";
(time ./main -i inputs/HT01.csv) &>> results/HT01
echo "HT02";
(time ./main -i inputs/HT02.csv) &>> results/HT02
echo "HT03";
(time ./main -i inputs/HT03.csv) &>> results/HT03
echo "HT04";
(time ./main -i inputs/HT04.csv) &>> results/HT04
echo "HT05";
(time ./main -i inputs/HT05.csv) &>> results/HT05
echo "HT06";
(time ./main -i inputs/HT06.csv) &>> results/HT06
echo "HT07";
(time ./main -i inputs/HT07.csv) &>> results/HT07
echo "HT08";
(time ./main -i inputs/HT08.csv) &>> results/HT08
echo "HT09";
(time ./main -i inputs/HT09.csv) &>> results/HT09
echo "c4-p1";
(time ./main -i inputs/c4-p1\(Hopper\).csv) &> results/c4-p1
echo "c4-p2";
(time ./main -i inputs/c4-p2\(Hopper\).csv) &> results/c4-p2
echo "c4-p3";
(time ./main -i inputs/c4-p3\(Hopper\).csv) &> results/c4-p3
echo "c5-p1";
(time ./main -i inputs/c5-p1\(Hopper\).csv) &> results/c5-p1
echo "c5-p2";
(time ./main -i inputs/c5-p2\(Hopper\).csv) &> results/c5-p2
echo "c5-p3";
(time ./main -i inputs/c5-p3\(Hopper\).csv) &> results/c5-p3
echo "c6-p1";
(time ./main -i inputs/c6-p1\(Hopper\).csv) &> results/c6-p1
echo "c6-p2";
(time ./main -i inputs/c6-p2\(Hopper\).csv) &> results/c6-p2
echo "c6-p3";
(time ./main -i inputs/c6-p3\(Hopper\).csv) &> results/c6-p3
echo "c7-p1";
(time ./main -i inputs/c7-p1\(Hopper\).csv) &> results/c7-p1
echo "c7-p2";
(time ./main -i inputs/c7-p2\(Hopper\).csv) &> results/c7-p2
echo "c7-p3";
(time ./main -i inputs/c7-p3\(Hopper\).csv) &> results/c7-p3
