#!/bin/bash

DATE=`date +%Y%m%d`
mkdir -p logs/$DATE

#root -l -b "Fun4All_G4_sPHENIX.C(10, \"zsOffTest_NOM.root\", -100, -100, -100)" >& logs/$DATE/zsOff_$DATE.log &

#root -l -b "Fun4All_G4_sPHENIX.C(10, \"zsOff_NOM.root\", -100, -100, -100)" >& logs/$DATE/zsOff_$DATE.log &

root -l -b "Fun4All_G4_sPHENIX.C(10, \"zsOn_NOM.root\", 16, 16, 16)" >& logs/$DATE/zsOn_$DATE.log &
