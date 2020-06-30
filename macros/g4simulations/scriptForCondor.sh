#!/usr/bin/bash

id=0
if [[ $# -eq 1 ]]
then
    id=$1
fi

source /opt/sphenix/core/bin/sphenix_setup.sh -n
export SPHENIX=/sphenix/user/cmcginn/Projects/sPHENIX4/
export MYINSTALL="$SPHENIX"install

source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL

tar -xzvf inTar.tar.gz

#if [[ -f $MYINSTALL/lib/libTreeMaker.so ]]
#then
#    dummy=0
#else
#    make
#fi

#make
source autogen.sh --prefix=$MYINSTALL
make 
make install


#Generate a few random numbers
RANDMAX=32767

ETAMAX=40000000 #0.4 * 100000000 since bash doesnt handle integers nicely
ETARANGE=$((ETAMAX * 2))

RANDVAL=$RANDOM
ETAVAL=$((ETARANGE/$RANDMAX))
ETAVAL=$((ETAVAL*$RANDVAL))
ETAVAL=$((ETAVAL - $ETAMAX))

#Same for phi
PHIMAX=31415926 #3.1415926 * 10000000 since bash doesnt handle integers nicely
PHIRANGE=$((PHIMAX * 2))

RANDVAL=$RANDOM
PHIVAL=$((PHIRANGE/$RANDMAX))
PHIVAL=$((PHIVAL*$RANDVAL))
PHIVAL=$((PHIVAL - $PHIMAX))

#Some ZS values
ZSEMCal=16
ZSIHCal=0
ZSOHCal=0

#Lets modify the TEMPLATE FILE:
cp treeMaker_CONDORTEMPLATE.config treeMaker_Job$id.config

sed -i -e "s@INETAVAL@$ETAVAL./100000000@g" treeMaker_Job$id.config
sed -i -e "s@INPHIVAL@$PHIVAL./10000000@g" treeMaker_Job$id.config
sed -i -e "s@INCEMCZS@$ZSEMCal@g" treeMaker_Job$id.config
sed -i -e "s@INIHCALZS@$ZSIHCal@g" treeMaker_Job$id.config
sed -i -e "s@INOHCALZS@$ZSOHCal@g" treeMaker_Job$id.config


echo root -l -b -q "Fun4All_G4_sPHENIX.C(\"treeMaker_Job$id.config\")" #>& zs_$id.log

echo "JOB COMPLETE!"
