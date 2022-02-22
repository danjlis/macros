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

cp hcal_condor.config hcal_condor_job$id.config

sed -i -e "s@hcal_full_condor.root@hcal_full_Job$id.root@g" hcal_condor_job$id.config

root -b -q "/sphenix/user/dlis/Projects/HCALSims/macros/detectors/sPHENIX/Fun4All_G4_sPHENIX.C(\"hcal_condor_job$id.config\")" >& hcal.log

echo "JOB COMPLETE!"
