#!/bin/bash

cFile=testCondor.job

DATE=`date +%Y%m%d`
TIME=`date +%H%M%S`
cDir=condorDir/condor_"$DATE"_"$TIME"
mkdir -p $cDir

cFile2=${cFile%.job}
cFile2="$cFile2"_"$DATE"_"$TIME".job
cp $cFile $cFile2

sed -i -e "s@INITDIR@$PWD/$cDir@g" $cFile2

files=(TreeMaker.h TreeMaker.C Makefile Linkdef.h Fun4All_G4_sPHENIX.C rootlogon.C G4Setup_sPHENIX.C G4_Bbc.C G4_FEMC.C G4_Global.C G4_CaloTrigger.C G4_Jets.C G4_HIJetReco.C G4_DSTReader.C DisplayOn.C G4_Pipe.C G4_Tracking.C G4_PSTOF.C G4_CEmc_Spacal.C G4_HcalIn_ref.C G4_Magnet.C G4_HcalOut_ref.C G4_PlugDoor.C GlobalVariables.C phpythia8.cfg)

inputStr=""
for i in ${files[@]}
do
    cp $i $cDir
    inputStr="$inputStr $i"
done

retDir=$PWD
cd $cDir
tar -czvf inTar.tar.gz $inputStr
rm -f *.C
rm -f *.h
rm -f *.txt
rm -f *.cfg
rm Makefile
cd $retDir

echo "transfer_input_files = inTar.tar.gz,scriptForCondor.sh" >> $cFile2
echo "Queue 1088" >> $cFile2

mv $cFile2 $cDir
cp scriptForCondor.sh $cDir

sed -i -e "s@SPECIES@gamma@g" $cDir/scriptForCondor.sh
sed -i -e "s@ADCTHRESH@16@g" $cDir/scriptForCondor.sh
sed -i -e "s@INNERHCALMAT@0@g" $cDir/scriptForCondor.sh
