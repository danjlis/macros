#!/usr/bin/bash                                                                 
id=$1

source /opt/sphenix/core/bin/sphenix_setup.sh -n
export HOME=/sphenix/user/dlis/Projects/
export SPHENIX=$MYINSTALL
if [[ $SPHENIX == "YOURPATHHERE" ]]
then
    echo "YOU NEED TO EDIT scriptForCondor.sh path SPHENIX - currently set xsto\
 YOURPATHHERE"

    return
    exit 1
fi

export MYINSTALL="$SPHENIX"/install

source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL

root -b -q "/sphenix/user/dlis/Projects/HCALSims/macros/detectors/sPHENIX/Fun4All_G4_sPHENIX.C(\"hcal_test.config\")" >& hcal.log

echo "JOB COMPLETE!"
