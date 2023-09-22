#!/usr/bin/env python
#

import os,sys,string, time
import ROOT 
from ROOT import TChain
ROOT.gSystem.Load("GSimpleTree_C.so")
from ROOT import GSimpleTree
from glob import glob

#pattern = "/pnfs/uboone/persistent/uboonebeam/bnb_gsimple/bnb_gsimple_fluxes_02.05.2018_463/converted_beammc_000*.root"
#pattern = "/pnfs/uboone/persistent/uboonebeam/bnb_gsimple/bnb_gsimple_fluxes_02.05.2018_463/converted_beammc_000*.root"
#pattern = "/pnfs/uboone/persistent/uboonebeam/bnb_gsimple/bnb_gsimple_fluxes_01.09.2019_463/converted_beammc_wincorr_100*.root"

# BNB Run 4a
pattern = "/cvmfs/uboone.osgstorage.org/stash/uboonebeam/bnb_gsimple/bnb_gsimple_fluxes_run4a_08.02.2023_463/gsimple_beammc_run4a_wincorr_000*.root"

# NuMI FHC (10 files, 1e8 POT per file)
#pattern = "/pnfs/uboone/persistent/uboonebeam/numi_dk2nu_zero_threshold/gsimple/FHC/gsimple_microboone-numi-v2_g4numiv6_minervame_me000z200i_31*_0000_00001.root"

# NuMI RHC (10 files, 1e8 POT per file)
#pattern = "/pnfs/uboone/persistent/uboonebeam/numi_dk2nu_zero_threshold/gsimple/RHC/gsimple_microboone-numi-v2_g4numiv6_minervame_me000z-200i_98*_0000_00001.root"

# Dirt fluxes
#pattern = "/pnfs/uboone/persistent/uboonebeam/bnb_gsimple/bnb_gsimple_fluxes_01.09.2019_463_dirt/converted_beammc_dirt_wincorr_000*.root"
#pattern = "/pnfs/uboone/persistent/uboonebeam/numi_dk2nu_zero_threshold/dirt/FHC/gsimple_microboone-numi-v2-dirt_g4numiv6_minervame_me000z200i_10*_0000_00001.root"
#pattern = "/pnfs/uboone/persistent/uboonebeam/numi_dk2nu_zero_threshold/dirt/RHC/gsimple_microboone-numi-v2-dirt_g4numiv6_minervame_me000z-200i_50*_0000_00001.root"


cflux = TChain('flux')
cflux.Add(pattern)
Nfiles = cflux.GetNtrees()
print ("Number of files: ", Nfiles)
gsimple = GSimpleTree(cflux)
gsimple.Loop()

raw_input("Please press enter to exit.")
