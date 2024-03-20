# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: sherpa_ZJets_MASTER_cff.py -s GEN -n 100 --no_exec --conditions auto:mc --eventcontent RAWSIM
import FWCore.ParameterSet.Config as cms



process = cms.Process('GEN')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500000)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('sherpa_ZJets_MASTER_cff.py nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(1),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(20971520),
    fileName = cms.untracked.string('sherpa_ZJets_MASTER_cff_py_GEN.root'),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

# Seed

process.RandomNumberGeneratorService.generator.initialSeed = cms.untracked.uint32(123456789)

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')

process.generator = cms.EDFilter("SherpaGeneratorFilter",
  maxEventsToPrint = cms.int32(0),
  filterEfficiency = cms.untracked.double(1.0),
  crossSection = cms.untracked.double(-1),
  SherpaProcess = cms.string('QCDLOM'),
  SherpackLocation = cms.string('./'),
  SherpackChecksum = cms.string('e3bd5eca220b6d9f849d101a11b37795'),
  FetchSherpack = cms.bool(True),
  SherpaPath = cms.string('./'),
  SherpaPathPiece = cms.string('./'),
  SherpaResultDir = cms.string('Result'),
  SherpaDefaultWeight = cms.double(1.0),
  SherpaParameters = cms.PSet(parameterSets = cms.vstring(
                             "MPI_Cross_Sections",
                             "Run"),
                              MPI_Cross_Sections = cms.vstring(
                        " MPIs in Sherpa, Model = Amisic:",
                        " semihard xsec = 39.647 mb,",
                        " non-diffractive xsec = 17.0318 mb with nd factor = 0.3142."
                                                  ),
                              Run = cms.vstring(
                        " (run){",
                        " EVENTS 10000; ERROR 0.99;",
                        " FSF:=1.; RSF:=1.; QSF:=1.;",
                        " Rjet:=0.8;",
                        " RJsq:=Rjet*Rjet;",
                        " SCALES METS{FSF*MU_F2}{RSF*MU_R2}{RJsq*QSF*MU_Q2};",
                        " ME_SIGNAL_GENERATOR Comix Amegic;",
                        " EVENT_GENERATION_MODE Weighted;",
                        " BEAM_1 2212; BEAM_ENERGY_1 = 2510.;",
                        " BEAM_2 2212; BEAM_ENERGY_2 = 2510.;",
                        " MAX_PROPER_LIFETIME = 10.0;",
                        " RANDOM_SEED1=A;",
                        #"FRAGMENTATION Lund; DECAYMODEL Lund;",
#                                " FRAGMENTATION = None",
                        #" FRAGMENTATION L",
                        "}(run)",
                        " (processes){",
                        " Process 93 93 -> 93 93",
                        " Enhance_Function VAR{pow(PPerp(p[3])-15.,4.5)} {2}",
                        " Order (*,0);",
                        " Integration_Error 0.02 {2};",
                        " End process;",
                        "}(processes)",
                        " (selector){",
                        " FastjetFinder antikt 1 100 0.0 Rjet;",
                        "}(selector)",
                        " (analysis){",
                        " BEGIN_RIVET {",
                        " -a PAPER_FIN_R8",
                        "} END_RIVET;",
                        "}(analysis)"
                                                  ),
                             )
)


process.ProductionFilterSequence = cms.Sequence(process.generator)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.endjob_step,process.RAWSIMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)
# filter all path with the production filter sequence
for path in process.paths:
    getattr(process,path).insert(0, process.ProductionFilterSequence)

# customisation of the process.
process.load('GeneratorInterface.RivetInterface.rivetAnalyzer_cfi')

# Automatic addition of the customisation function from Configuration.GenProduction.rivet_customize
#from Configuration.GenProduction.rivet_customize import customise 

def customise(process):
    process.load('GeneratorInterface.RivetInterface.rivetAnalyzer_cfi')
    process.rivetAnalyzer.AnalysisNames = cms.vstring('CMS_2022_PAS_SMP_22_999')
    process.rivetAnalyzer.CrossSection = cms.double(4.956e+09)
    process.rivetAnalyzer.OutputFile = cms.string('sherpa_lund_ak8.yoda')
    process.rivetAnalyzer.UseExternalWeight = cms.bool(True)
    process.rivetAnalyzer.useGENweights = cms.bool(True)
    process.generation_step+=process.rivetAnalyzer
    process.schedule.remove(process.RAWSIMoutput_step)
    return(process)      
#call to customisation function customise imported from Configuration.GenProduction.rivet_customize
process = customise(process)

# End of customisation functions
