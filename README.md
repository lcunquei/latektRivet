# latektRivet
Rivet is a framework to make truth-level MC predictions and compare them to published, unfolded data. This is mostly with the purpose of MC tuning, and to have a centralized archive of physics analyses independent of experiment-specific code. Rivet can be run in standalone mode, but for us it’s more convenient to run it in CMSSW.

CMS twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/Rivet 
CMS Rivet GitLab repo: https://gitlab.cern.ch/cms-gen/Rivet


How to install Rivet in CMSSW:

First, fork from the CMSSW Rivet repository on your CERN gitlab account: https://gitlab.cern.ch/cms-gen/Rivet

Then, go to lxplus (you could try at the LLR cluster, but I didn’t test it there, but should work there too):

cmsrel CMSSW_11_2_4
cd CMSSW_11_2_4/src
cmsenv

git cms-merge-topic mseidel42:RivetPaths_11_2

git clone ssh://git@gitlab.cern.ch:7999/${USER}/Rivet.git
cd Rivet
git remote add cms-gen ssh://git@gitlab.cern.ch:7999/cms-gen/Rivet.git
git pull cms-gen master

source rivetSetup.sh
scram b -j8

Rivet already provides us with a database of Rivet routines, you can check these with:

rivet --list-analyses

Many published analyses (typically cross section measurements) have their rivet subroutine in that database, you can see them online as well here: https://rivet.hepforge.org/analyses.html 



To run latekt rivet routine: 
First place CMS_2023_xxx.cc  in RIVET/SMP/src
Then place runRivetAnalyzer_DIJET_13TeV_DjetsLatekt_cfg.py in RIVET/SMP/test
compile the code again with scram build -j8
then do cmsRun runRivetAnalyzer_DIJET_13TeV_DjetsLatekt_cfg.py

This will generate pythia events for which the D0s, B0s and B+s are stable particles. The D0s are set stable because those are the particles that we reconstruct. 
The B0s and the B+s are made stable because in our analysis we correct for non-prompt decays. The output is of the .yoda type, you havre to convert it to root by doing yoda2root file.yoda file.root
If instead we run runRivetAnalyzer_DIJET_13TeV_inclusiveLatekt_cfg.py, then we'll run latkT in inclusively, and for that we need to change the flaghf to zero in CMS_2023_xxx.cc
To run on the grid you can use crab_myMC.py. You'll then have to transform the output from .yoda to .root and merge. 














