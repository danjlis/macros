#!/usr/bin/bash

id=0
if [[ $# -eq 1 ]]
then
    id=$1
fi

source /opt/sphenix/core/bin/sphenix_setup.sh -n
export SPHENIX=/sphenix/u/cmcginn/Packages/sPHENIX/
export MYINSTALL="$SPHENIX"install

source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL

tar -xzvf inTar.tar.gz

if [[ -f $MYINSTALL/lib/libTreeMaker.so ]]
then
    dumm=0
else
    make
fi

nEvt=100

etaBins=(-40300059 -37929469 -35558879 -33188283 -30817693 -28447104 -26076508 -23705918 -21335328 -18964732 -16594142 -14223552 -11852956 -094823599 -071117759 -047411799 -023705840 0000000 023705959 047411919 071117759 094823718 11852968 14223552 16594148 18964744 21335328 23705924 26076519 28447104 30817699 33188295 35558879 37929475 40300071)

phiBins=(-39270043 -36815667 -34361291 -31906915 -29452515 -26998138 -24543762 -22089386 -19635010 -17180634 -14726257 -12271881 -098175049 -073631287 -049087524 -024543762 0000000 024543762 049087524 073631287 098175049 12271881 14726257 17180634 19635010 22089386 24543762 26998138 29452515 31906915 34361291 36815667 39270043)

nEtaBins=-1
nPhiBins=-1

for i in ${etaBins[@]}
do
    nEtaBins=$((nEtaBins+1))
done
for i in ${phiBins[@]}
do
    nPhiBins=$((nPhiBins+1))
done

nTotal=$((nEtaBins * $nPhiBins))

if [[ $id -ge $nTotal ]]
then
    echo "Process # exceeds available bins. return"
    return
    exit 1
fi

etaPos=$((id / nPhiBins))
phiPos=$((id % nPhiBins))

etaVal=${etaBins[$etaPos]}
etaPos=$((etaPos + 1))
etaVal=$((etaVal + ${etaBins[$etaPos]}))
etaVal=$((etaVal / 2 ))

phiVal=${phiBins[$phiPos]}
phiPos=$((phiPos + 1))
phiVal=$((phiVal + ${phiBins[$phiPos]}))
phiVal=$((phiVal / 2 ))

if [[ $etaVal -lt 0 ]]
then
    etaVal=$(echo $etaVal | sed -e "s@-@@g")
    etaVal=-0.$etaVal
else
    etaVal=0.$etaVal
fi

if [[ $phiVal -lt 0 ]]
then
    phiVal=$(echo $phiVal | sed -e "s@-@@g")
    phiVal=-0.$phiVal
else
    phiVal=0.$phiVal
fi


root -l -b "Fun4All_G4_sPHENIX.C($nEvt, \"zsOff_Job$id.root\", -100, -100, -100, $etaVal, $phiVal)" >& zsOff_$DATE.log 

root -l -b "Fun4All_G4_sPHENIX.C($nEvt, \"zsOn_Job$id.root\", 16, 16, 16, $etaVal, $phiVal)" >& zsOn_$DATE.log                          
cat zsOff_$DATE.log

echo ""
zsOn_$DATE.log
echo ""

echo "JOB COMPLETE!"
