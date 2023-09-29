from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')


config.General.requestName = 'Pythia_Latekt_Djets'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_('JobType')
config.JobType.pluginName = 'PRIVATEMC'
config.JobType.psetName = 'runRivetAnalyzer_DIJET_13TeV_DjetsLatekt_cfg.py'
config.JobType.outputFiles = ['QCD_Pythia8_CP5_Mar16.yoda']
config.section_("Data")
config.Data.outputPrimaryDataset = 'Pythia_Djets_latekt'
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 10000
NJOBS = 100  # This is not a configuration parameter, but an auxiliary variable that we use in the next line.                                                    
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB3_tutorial_Aug2021_MC_generation'









config.Data.outLFNDirBase = '/store/group/phys_heavyions/ec/lcunquei/'


config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'



