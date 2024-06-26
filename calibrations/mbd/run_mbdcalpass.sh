#!/usr/bin/env bash
#
# run the mbd calibration passes
# usage:
# runmbd_calpass.sh <prdf> <calpass> <nevts>
#

prdf=$1
calpass=$2

nevts=0
if [[ $# -gt 2 ]] 
then
  nevts=$3
elif [[ $2 -eq 1 ]]
then
  # for calpass 1, we set nevts to 20K by default
  nevts=20000
fi

# get run number from prdf file
run=${prdf##*/}
run=${run%-*}
run=${run#*-}
run=${run#000}

# make calibration directory and fill it
caldir=${PWD}/results/${run}
echo mkdir -p ${caldir}
mkdir -p ${caldir}
PASS0DIR=${PWD}/results/.PASS0
#ln -sf /sphenix/user/chiu/sphenix_bbc/run2023/tpulser/results/00029705-0000/mbd_timecorr.calib ${caldir}/
#ln -sf /sphenix/user/chiu/sphenix_bbc/run2023/goodruns/results/00020869-0000/bbc_shape.calib ${caldir}/mbd_shape.calib
#ln -sf /sphenix/user/chiu/sphenix_bbc/run2023/goodruns/results/00020869-0000/bbc_sherr.calib ${caldir}/mbd_sherr.calib
#ln -sf /sphenix/user/chiu/sphenix_bbc/run2023/results/20869/mbd_slewcorr.calib ${caldir}/
#ln -sf /sphenix/user/chiu/sphenix_bbc/run2023/results/20869/mbd_tt_t0.calib ${caldir}/
#ln -sf /sphenix/user/chiu/sphenix_bbc/run2023/results/20869/mbd_tq_t0.calib ${caldir}/
ln -sf ${PASS0DIR}/mbd_timecorr.calib ${caldir}/
ln -sf ${PASS0DIR}/bbc_shape.calib ${caldir}/mbd_shape.calib
ln -sf ${PASS0DIR}/bbc_sherr.calib ${caldir}/mbd_sherr.calib
ln -sf ${PASS0DIR}/mbd_slewcorr.calib ${caldir}/
#ln -sf ${PASS0DIR}/mbd_tt_t0.calib ${caldir}/
#ln -sf ${PASS0DIR}/mbd_tq_t0.calib ${caldir}/

# if not interactive, run ROOT in batch mode
if [ ! -z $PS1 ]
then
  BATCH=-b
fi

echo root.exe ${BATCH} -q Fun4All_MBD_CalPass.C\(\"${prdf}\",${calpass},${nevts}\)
root.exe ${BATCH} -q Fun4All_MBD_CalPass.C\(\"${prdf}\",${calpass},${nevts}\)


