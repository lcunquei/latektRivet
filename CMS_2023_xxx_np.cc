// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FinalPartons.hh"
#include "Rivet/Tools/RivetHepMC.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/PromptFinalState.hh"

#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/contrib/LundGenerator.hh"


namespace Rivet {


  /// @brief Add a short analysis description here
  class CMS_2023_xxx_np : public Analysis {
  public:

    /// Constructor
    DEFAULT_RIVET_ANALYSIS_CTOR(CMS_2023_xxx_np);


    void init() {

      //Projections
      FinalState fs(Cuts::abseta < 5.2);

      FinalPartons fp(Cuts::abseta < 5.2);  

      FastJets jet2(fs, FastJets::ANTIKT, 0.2); 
      declare(jet2, "jet2");

      // Particles for the jets                                                                                                          
      VetoedFinalState jet_input(fs);
      jet_input.vetoNeutrinos();  //in CMS we only veto neutrinos
      declare(jet_input, "JET_INPUT");     


     //Particles for the parton jets
     VetoedFinalState jet_inputp(fp);   
      jet_inputp.vetoNeutrinos();  //in CMS we only veto neutrinos
      declare(jet_inputp, "JET_INPUTP");

      book(_njets, "_njets");
      book(_h1,"_h1",{1.000,1.600,2.000,2.300,2.550,2.800,3.000,5.000});
      book(_h1p,"_h1p",{1.000,1.600,2.000,2.300,2.550,2.800,3.000,5.000});
      book(_h1_matched,"_h1_matched",{1.000,1.600,2.000,2.300,2.550,2.800,3.000,5.000});



    }

    void analyze(const Event& event) {
      const int flaghf=1; //change to zero if you run inclusive 
      const double weight = event.weights()[0];
      // Convert Particles into PseudoJets for clustering                                                                                                                                                                            
      const VetoedFinalState & fs = apply<VetoedFinalState>(event, "JET_INPUT");
      const Particles & fsParticles = fs.particles();
      vector<PseudoJet> particles;
      particles.reserve(fsParticles.size());
      for (uint iFS=0; iFS<fsParticles.size(); iFS++)
	{
	  PseudoJet p = fsParticles[iFS].pseudojet();
          if(p.perp()<1) continue;
          p.set_user_index(1);
          if(abs(fsParticles[iFS].pid())==421){if(fsParticles[iFS].fromBottom()==false){
	      p.set_user_index(421);}} 
	 
	  particles.push_back(p);
	}


      const VetoedFinalState & fp = apply<VetoedFinalState>(event, "JET_INPUTP");
      const Particles & fpPartons = fp.particles();
      vector<PseudoJet> partons;
      partons.reserve(fpPartons.size());
      for (uint iFS=0; iFS<fpPartons.size(); iFS++)
        {
          PseudoJet p = fpPartons[iFS].pseudojet();
	  p.set_user_index(1);
	  if(abs(fpPartons[iFS].pid())==4) p.set_user_index(4);
	  partons.push_back(p);
	}








     
      JetDefinition jetDefAKT_Sig(fastjet::antikt_algorithm, 0.2); 
      ClusterSequence antikTjets(particles, jetDefAKT_Sig);
      vector<PseudoJet> jets = fastjet::sorted_by_pt(antikTjets.inclusive_jets(100));

      JetDefinition jetDefAKT_Sigp(fastjet::antikt_algorithm, 0.2);
      ClusterSequence antikTjetsp(partons, jetDefAKT_Sigp);
      vector<PseudoJet> jetsp = fastjet::sorted_by_pt(antikTjetsp.inclusive_jets(10));
     

    
      _njets->fill(jets.size()); // for normalization
      


      for (size_t i = 0; i < jets.size(); ++i ) //loop over all jets
	{
	  if ( fabs(jets[i].eta())  > 1.6 ) continue;
	  if (jets[i].pt() < 100*GeV) continue;
	  if (jets[i].pt() > 120*GeV) continue;
	  PseudoJet jet1 = jets[i];
          int flagtag=0;
	  vector<PseudoJet> constits_ch;
          if(flaghf==1){         
	  for(uint m=0;m<jet1.constituents().size();m++){
	   
	    if(jet1.constituents()[m].user_index() == 421 && jet1.constituents()[m].pt()>=4 && fabs(jet1.constituents()[m].rapidity())<=1.2){
              flagtag=1; 
	      break;}

	  
	  }
	  if(flagtag==0) continue; }

        
	  for(uint m=0;m<jet1.constituents().size();m++){
	    constits_ch.push_back(jet1.constituents()[m]);}
	    
	  JetDefinition tjet1_def(fastjet::cambridge_algorithm, 10); 
	  ClusterSequence tjet1_cs(constits_ch, tjet1_def);
	  vector<PseudoJet> tjets1 = fastjet::sorted_by_pt(tjet1_cs.inclusive_jets(0.0));

	  if (tjets1.size() < 1) continue;
          
	  PseudoJet jj = tjets1[0];
	  PseudoJet j1;  // subjet 1 (largest pt)
	  PseudoJet j2;  // subjet 2 (smaller pt)
          double rg=0;
          int flagsubjet=0;
	  // Unclustering jet
	  while(jj.has_parents(j1,j2)){

	    flagsubjet=0;
           
	    if(j1.perp() < j2.perp()) std::swap(j1,j2);
	    vector <PseudoJet> constitj1 = sorted_by_pt(j1.constituents());
	    
	    for(uint m=0;m<constitj1.size();m++){
            
	     	      if(constitj1[m].user_index()==421) flagsubjet=1;
            }

	    if(flaghf==0) flagsubjet=1;
	    double delta_R = j1.delta_R(j2);
	    double cut=1;
	    
	    double kt= j2.perp()*delta_R;
	    

	    if(kt>cut && flagsubjet==1) {
             	  
    
	     rg = delta_R;
	      

	    }
          
	    jj=j1;
	  }
          double xax=0;
	  if(rg==0) xax=1.25;
          else xax=log(1/rg);
           _h1->fill(xax);




	   //I've filled the info for the tagged jet, now let's find the matched parton-level jet and fill that info too. 
	   double distmin=100;
	   int index=-1;
	   int flagtagp=0;
	   for (size_t j = 0; j < jetsp.size(); ++j ){ //loop over all jets     
	     double dist= jets[i].delta_R(jetsp[j]);
	     if(dist<distmin){distmin=dist;
	       index=j;} }
           if(index==-1) continue;
           if(distmin>0.4) continue; 
	   if(flaghf==1){
	     for(uint m=0;m<jetsp[index].constituents().size();m++){
	          
	       if(jetsp[index].constituents()[m].user_index() == 4){
		 flagtagp=1;
		 break;}
	          
	     }
	        
	     if(flagtagp==0) continue;}
	   _h1_matched->fill(xax);   


	   //uncluster the parton jet
	   vector<PseudoJet> constits_p;
	   for(uint n=0;n<jetsp[index].constituents().size();n++){
	     constits_p.push_back(jetsp[index].constituents()[n]);}

	   JetDefinition tjet1_def_p(fastjet::cambridge_algorithm, 10);
	   ClusterSequence tjet1_cs_p(constits_p, tjet1_def_p);
	   vector<PseudoJet> tjets1p = fastjet::sorted_by_pt(tjet1_cs_p.inclusive_jets(0.0));
	   if(tjets1p.size()<1) continue;
	   PseudoJet jjp = tjets1p[0];
	   PseudoJet j1p;  // subjet 1 (largest pt)
	   PseudoJet j2p;  // subjet 2 (smaller pt)                                                                                                       
	   double rgp=0;
	   int flagsubjetp=0;

	   // Unclustering jet                                                                                                                           
	   while(jjp.has_parents(j1p,j2p)){

	     flagsubjetp=0;

	     if(j1p.perp() < j2p.perp()) std::swap(j1p,j2p);
	     vector <PseudoJet> constitj1p = sorted_by_pt(j1p.constituents());

	     for(uint k=0;k<constitj1p.size();k++){
	       if(constitj1p[k].user_index()==4) flagsubjetp=1;
	     }

	     if(flaghf==0) flagsubjetp=1;
	     double delta_Rp = j1p.delta_R(j2p);
	     double cutp=1;

	     double ktp= j2p.perp()*delta_Rp;


	     if(ktp>cutp && flagsubjetp==1) {
	       rgp = delta_Rp;
	     }

	     jjp=j1p;
	   }
	   double xaxp=0;
	   if(rgp==0) xaxp=1.25;
	   else xaxp=log(1/rgp);

           _h1p->fill(xaxp);



	 

	}
    }


    void finalize() {

      double area = _njets->sumW();
     
      //scale(_h, 1/a); //make it per-jet
      

    }

  private:

    Histo1DPtr _h1;
    Histo1DPtr _h1p;
    Histo1DPtr _h1_matched;
    CounterPtr _njets;
  };



  DECLARE_RIVET_PLUGIN(CMS_2023_xxx_np);

}
