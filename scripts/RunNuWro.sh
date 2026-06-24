#!/bin/bash

# Author: C Thorpe
# Purpose: Generate some NuWro events and run them through hepmc conversion
# Useage: source RunNuWro.sh

############################################################################

# Setup:
# You must have a build of NuWro somewhere on your machine
# and the nuwro executable somewhere in your PATH

# The following must also be set:
# $NUWRO_FLUX_DIR=/uboone/data/users/cthorpe/NuWro_Flux/config/

############################################################################

# Unique name for the sample
sample_name=uboone_BNB_test_run4a_2023

# Which beam are you using?
#beam=bnb
beam=bnb_run4a
#beam=numi_fhc
#beam=numi_rhc
#beam=numi_fhc_intrinsic_nue
#beam=numi_rhc_intrinsic_nue
#beam=numi_fhc_dirt

# Path to nuwro params file you want to use
nuwro_params=/uboone/app/users/cthorpe/NuWro/NuWroTools/scripts/NuMI_Official_params.txt

# Path to putput directory (should be on pnfs)
out_dir=/pnfs/uboone/scratch/users/cthorpe/samples/

# Dir to store files before moving to final location
tmp_dir=/uboone/data/users/cthorpe/NuWro/temp/

# Dir to run NuWro inside
work_dir=/uboone/data/users/cthorpe/NuWro/work/

# Path to directory contining nuwro_to_hepmc*.C
macro_dir=/uboone/app/users/cthorpe/NuWro/NuWroTools/convert_hepmc/

######### Don't change anything below here ###############

###########################################################
# Setup temporary storage directory and check environment #
###########################################################

start_dir=$PWD

#text colors
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check NUWRO_FLUX_DIR has been set
if [ ! -v NUWRO_FLUX_DIR ]; then 
echo -e "${RED}NUWRO_FLUX_DIR has not been set! Exiting${NC}"
return
fi

# Check users output dir is empty

# Check it exists
if [ -d "${out_dir}/${sample_name}/" ]; then 
echo -e "${RED}Output directory ${out_dir}/${sample_name} already exists, checking contents ${NC}"

# If it does exist, check it is empty
if [ ! -z "$(ls -A ${out_dir}/${sample_name}/)" ]; then 
echo -e "${RED}Outpt directory ${out_dir}/${sample_name} is not empty! Empty it first${NC}"
return
fi

fi

echo
echo -e "${BLUE}Preparing Temporary Output Directory${NC}"

# Make sure tmp_dir and work_dir exist
mkdir -p ${tmp_dir}
mkdir -p ${work_dir}

# If temporary output directory exists, empty it


cd ${tmp_dir}

if [ -d "${sample_name}/" ]; then
rm ${sample_name}/nuwro/*
rm ${sample_name}/hepmc/*
rm ${sample_name}/meta/*

# If it does not exist, create it
else

mkdir ${sample_name}
mkdir ${sample_name}/nuwro/
mkdir ${sample_name}/hepmc/
mkdir ${sample_name}/meta/
 
fi


#############
# run NuWro #
#############

nuwrofilename=${sample_name}.root

cd ${work_dir}

# Generate events
echo
echo -e "${BLUE}Generating NuWro Events${NC}"

# Run NuWro

# BNB
if [ "$beam" == "bnb" ]; then
nuwro -i ${nuwro_params} -p "@target/MicroBooNE.txt" -p "@${NUWRO_FLUX_DIR}/uBooNE_Booster_fhc.txt" -o ${nuwrofilename} > NuWro_Out.log

# BNB run 4a
elif [ "$beam" == "bnb_run4a" ]; then
nuwro -i ${nuwro_params} -p "@target/MicroBooNE.txt" -p "@${NUWRO_FLUX_DIR}/uBooNE_Booster_fhc_run4a.txt" -o ${nuwrofilename} > NuWro_Out.log

# BNB Dirt
elif [ "$beam" == "bnb_dirt" ]; then
nuwro -i ${nuwro_params} -p "@target/MicroBooNEFull.txt" -p "@${NUWRO_FLUX_DIR}/uBooNE_Booster_fhc_dirt.txt" -o ${nuwrofilename} > NuWro_Out.log

# NuMI FHC
elif [ "$beam" == "numi_fhc" ] || [ "$beam" == "numi_fhc_intrinsic_nue" ]; then
nuwro -i ${nuwro_params} -p "@target/MicroBooNE.txt" -p "@${NUWRO_FLUX_DIR}/uBooNE_Numi_fhc.txt" -o ${nuwrofilename} > NuWro_Out.log

# NuMI RHC
elif [ "$beam" == "numi_rhc" ] || [ "$beam" == "numi_rhc_intrinsic_nue" ]; then
nuwro -i ${nuwro_params} -p "@target/MicroBooNE.txt" -p "@${NUWRO_FLUX_DIR}/uBooNE_Numi_rhc.txt" -o ${nuwrofilename} > NuWro_Out.log

# NuMI FHC Dirt
elif [ "$beam" == "numi_fhc_dirt" ]; then
nuwro -i ${nuwro_params} -p "@target/MicroBooNEFull.txt" -p "@${NUWRO_FLUX_DIR}/uBooNE_Numi_fhc_dirt.txt" -o ${nuwrofilename} > NuWro_Out.log

# NuMI RHC Dirt
elif [ "$beam" == "numi_rhc_dirt" ]; then
nuwro -i ${nuwro_params} -p "@target/MicroBooNEFull.txt" -p "@${NUWRO_FLUX_DIR}/uBooNE_Numi_rhc_dirt.txt" -o ${nuwrofilename} > NuWro_Out.log

else 
echo -e "${RED} Unrecognized beam type set. Please use bnb for the BNB, numi_fhc for NuMI FHC, numi rhc for NuMI RHC${NC}"
return
fi

# Move sample to output file
cd ..
cd ${tmp_dir}

# Move nuwro event file to output
mv ${work_dir}/${nuwrofilename}* ${sample_name}/nuwro/
mv ${work_dir}/NuWro_Out.log ${sample_name}/nuwro/

# Move file with POT into output
mv ${work_dir}/POTinfo.txt ${sample_name}/meta/

nuwroeventfile=${tmp_dir}/${sample_name}/nuwro/${nuwrofilename}

#####################################
# Convert the NuWro events to hepmc #
#####################################

echo
echo -e "${BLUE}Converting them to HEPMC${NC}"

cd ${macro_dir}


if [ "$beam" == "numi_fhc" ] || [ "$beam" == "numi_rhc" ]; then
root -l -b <<-EOF
.L nuwro_to_hepmc_numi.C
nuwro_to_hepmc_numi( "${tmp_dir}/${sample_name}/nuwro/" , "${sample_name}" ,  "${tmp_dir}/${sample_name}/hepmc/" )
EOF

elif [ "$beam" == "numi_fhc_dirt" ] || [ "$beam" == "numi_rhc_dirt" ]; then
root -l -b <<-EOF
.L nuwro_to_hepmc_numi.C
nuwro_to_hepmc_numi( "${tmp_dir}/${sample_name}/nuwro/" , "${sample_name}" ,  "${tmp_dir}/${sample_name}/hepmc/" )
EOF

elif [ "$beam" == "numi_fhc_intrinsic_nue" ] || [ "$beam" == "numi_rhc_intrinsic_nue" ]; then
root -l -b <<-EOF
.L nuwro_to_hepmc_numi_pure_nue.C
nuwro_to_hepmc_numi_pure_nue( "${tmp_dir}/${sample_name}/nuwro/" , "${sample_name}" ,  "${tmp_dir}/${sample_name}/hepmc/" )
EOF

elif [ "$beam" == "bnb" ] || [ "$beam" == "bnb_run4a" ]; then
root -l -b <<-EOF
.L nuwro_to_hepmc_bnb.C
nuwro_to_hepmc_bnb( "${tmp_dir}/${sample_name}/nuwro/" , "${sample_name}" ,  "${tmp_dir}/${sample_name}/hepmc/" )
EOF

elif [ "$beam" == "bnb_dirt" ]; then
root -l -b <<-EOF
.L nuwro_to_hepmc_bnb.C
nuwro_to_hepmc_bnb( "${tmp_dir}/${sample_name}/nuwro/" , "${sample_name}" ,  "${tmp_dir}/${sample_name}/hepmc/" )
EOF

else 
echo -e "${RED} Unrecognized beam type set. Please use bnb for the BNB or numi for NuMI${NC}"
return 
fi 

# Move eventcount metadata to output
mv eventcount.meta ${tmp_dir}/${sample_name}/meta/

#########################
# Create metadata file  #
#########################

echo
echo -e "${BLUE}Creating POT Record${NC}"

cd ${macro_dir}

root -l -b <<-EOF
.L make_meta.C
make_meta( "${tmp_dir}/${sample_name}/meta/POTinfo.txt" , "${tmp_dir}/${sample_name}/meta/eventcount.meta" , "${tmp_dir}/${sample_name}/meta/" )
EOF

############################# 
# Move everything to output #
#############################

echo
echo -e "${BLUE}Moving Everything to final locations${NC}"

cd ${tmp_dir}

mv ${sample_name} ${out_dir}

cd ${out_dir}/${sample_name}/hepmc/

# Create list of files
ls -d $PWD/* > hepmc_files.list

#cd -

echo "POT file is ${out_dir}/${sample_name}/meta/hepmc_POT.meta"
echo "HEPMC file list is ${out_dir}/${sample_name}/hepmc/hepmc_files.list"

echo
echo -e "${BLUE}Done!${NC}"
echo

cd ${start_dir}
