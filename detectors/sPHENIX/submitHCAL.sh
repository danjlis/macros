#!/bin/bash

retDir=$PWD
cFile=hcalCondor_dl.job

DATE=`date +%Y%m%d`
TIME=`date +%H%M%S`
cDir=condorDir/hcal_"$DATE"_"$TIME"

mkdir -p $cDir
mkdir -p $cDir/HCALCalibTree

cFile2=${cFile%.job}
cFile2="$cFile2"_"$DATE"_"$TIME".job
cp $cFile $cFile2
cp hcal_condor.config $cDir
#cp phpythia8.cfg $cDir
#cp sPHENIXActsGeom.gdml $cDir
#cp sPHENIXActsGeom.root $cDir
cp ../../../coresoftware/simulation/g4simulation/g4main/CosmicSpray.* $cDir

cp G4_Input.C $cDir

cp G4_HcalOut_ref.C $cDir
cp $MYINSTALL/lib/libg4testbench.so $cDir
cp G4Setup_sPHENIX.C $cDir 
cp ../../../coresoftware/simulation/g4simulation/g4detectors/PHG4OuterHcalDetector* $cDir


cp HCALCalibTree/* $cDir/HCALCalibTree
cp $MYINSTALL/lib/libHCALCalibTree.so $cDir
cp $MYINSTALL/lib/HCALCalibTree_Dict_rdict.pcm $cDir
sed -i -e "s@INITDIR@$PWD/$cDir@g" $cFile2

echo "Queue 1" >> $cFile2

cp $cFile2 $cDir
cp scriptForHCAL.sh $cDir

cd $cDir 
condor_submit $cFile2
