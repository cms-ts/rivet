// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/RivetAIDA.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/InvMassFinalState.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Math/Vector4.hh"

/// @todo Include more projections as required, e.g. ChargedFinalState, FastJets, ZFinder...



namespace Rivet {

  class CMS_EWK_12_XXX : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    CMS_EWK_12_XXX()
      : Analysis("CMS_EWK_12_XXX")
    {
      /// @todo Set whether your finalize method needs the generator cross section
      setNeedsCrossSection(true);
    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      /// @todo Initialise and register projections here

      /// @todo Book histograms here, e.g.:

      const FinalState fs;
      addProjection(fs, "FS");

    
      h_leading_jet_pt_ele = bookHistogram1D(1, 1, 1, "MyDbn - HepData dataset 1, x-axis 1 and y-axis 2");
      h_second_jet_pt_ele = bookHistogram1D(2, 1, 1);
      h_number_jets_ele = bookHistogram1D(3, 1, 1);
      h_third_jet_pt_ele = bookHistogram1D(4, 1, 1); 
      h_fourth_jet_pt_ele = bookHistogram1D(5, 1, 1);
      h_leading_jet_pt_muon = bookHistogram1D(6, 1, 1);
      h_second_jet_pt_muon = bookHistogram1D(7, 1, 1);
      h_number_jets_muon = bookHistogram1D(8, 1, 1);
      h_third_jet_pt_muon = bookHistogram1D(9, 1, 1);
      h_fourth_jet_pt_muon = bookHistogram1D(10, 1, 1);
      h_eta_leading_muon = bookHistogram1D(11, 1, 1);
      h_eta_second_muon = bookHistogram1D(12, 1, 1);
      h_eta_third_muon = bookHistogram1D(13, 1, 1);
      h_eta_fourth_muon = bookHistogram1D(14, 1, 1);
      h_eta_leading_ele = bookHistogram1D(15, 1, 1);
      h_eta_second_ele = bookHistogram1D(16, 1, 1);
      h_eta_third_ele = bookHistogram1D(17, 1, 1);
      h_eta_fourth_ele = bookHistogram1D(18, 1, 1);

      h_ht1_ele = bookHistogram1D(19, 1, 1);           
      h_ht2_ele = bookHistogram1D(20, 1, 1);           
      h_ht3_ele = bookHistogram1D(21, 1, 1);           
      h_ht4_ele = bookHistogram1D(22, 1, 1);           

      h_ht1_muo = bookHistogram1D(23, 1, 1);           
      h_ht2_muo = bookHistogram1D(24, 1, 1);           
      h_ht3_muo = bookHistogram1D(25, 1, 1);           
      h_ht4_muo = bookHistogram1D(26, 1, 1);           
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {


      const double weight = event.weight();
      std::vector<fastjet::PseudoJet> vecs;
      bool Z_ele = false;
      bool Z_muon = false; 

      // vettore indici dell'elettrone e i suoi fotoni
      vector <unsigned int> ele_photons;
      // vettore indici dei fotoni
      vector <unsigned int> photons;
      // vettore indici del positrone e i suoi fotoni
      vector <unsigned int> pos_photons;
      // vettore indici dei neutrini
      vector <unsigned int> neutrinos;
      // vettore indici delle cariche a basso pt (< 250 MeV)
      vector <unsigned int> low_pt_charged;
      // vettore indici del muone e i suoi fotoni
      vector <unsigned int> muon_photons;
      // vettore indici del muone e i suoi fotoni
      vector <unsigned int> antimuon_photons;

      struct pt_and_particles {
        FourMomentum p_part;
        vector <unsigned int> lepton_photon;
      };
      
      // struttura che contiene il vettore lepton_photon che contiene l'indice del leptone candidato figlio della Z e i suoi fotoni e contiene il quadrimomento p_part dell'         // elettrone rivestito. 
      struct pt_and_particles ele_dres;
      struct pt_and_particles pos_dres;
      struct pt_and_particles muon_dres;
      struct pt_and_particles antimuon_dres;


      vector <Particle> part_jets; 
       
      
      const FinalState& fs = applyProjection<FinalState>(event, "FS");
      fs.particlesByPt();
      
      // copy of the list of final state partcles
      part_jets = fs.particles();

      double pTcut = 0.00;
      //double pTcut = 0.25;

      // riempio un vettore(photons) con gli indici dei fotoni in fs e un vettore(neutrinos) con gli indici dei neutrini in fs
      for (unsigned int i=0; i<fs.particles().size(); i++){
        if(fs.particles().at(i).pdgId()==PHOTON)
          photons.push_back(i);
        if(fabs(fs.particles().at(i).pdgId())==12 || 
           fabs(fs.particles().at(i).pdgId())==14 ||
           fabs(fs.particles().at(i).pdgId())==16 )
          neutrinos.push_back(i);
        if(PID::threeCharge(fs.particles().at(i).pdgId()) != 0)
          if(fs.particles().at(i).momentum().pT() < pTcut)
			low_pt_charged.push_back(i); 
      }


      for (unsigned int i=0; i<fs.particles().size(); i++){
		
        double eta = fs.particles().at(i).momentum().eta();
        double phi = fs.particles().at(i).momentum().phi();
        int Id = fs.particles().at(i).pdgId();
        double R;

        // electron selection
        if(Id == ELECTRON && 
           fabs(eta) < 2.4 &&
           (fabs(eta) < 1.4442 || fabs(eta) > 1.5660)){
          FourMomentum ele_p(fs.particles().at(i).momentum());
          //ho un elettrone e lo inserisco come primo elemento del vettore ele_photons che conterra' l'elettrone e a seguire i suoi fotoni
          ele_photons.push_back(i);
          //cerco i fotoni con cui rivestire l'elettrone
          for(unsigned int j=0; j<photons.size(); j++){
			double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
			double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();
			double deltaR = sqrt((eta-eta_photon)*(eta-eta_photon) + (phi-phi_photon)*(phi-phi_photon));
			if(fabs(eta) < 1.4442)
              R = 0.05;
			else 
              R = 0.07;
            if(deltaR < R){
              ele_photons.push_back(photons.at(j));
              //aggiungo il momento del fotone a quello dell'elettrone
              ele_p += fs.particles().at(photons.at(j)).momentum();	
            }
          }// ho rivestito l'elettrone, il suo nuovo momento Ã ¨ele_p e l'elettrone con i suoi fotoni sono contenuti in ele_photons

          //se Ã¨ il primo elettrone che rivesto e ha un pT>20 lo inserisco nella struttura in cui conservo il momento di e rivestito e il vettore di e con i suo gamma
          if(ele_dres.lepton_photon.empty() && ele_p.pT()>20){
			ele_dres.p_part = ele_p;
			ele_dres.lepton_photon = ele_photons;
          }
          else{ 
			if(ele_p.pT() > ele_dres.p_part.pT() && ele_p.pT()>20){
              ele_dres.p_part = ele_p;
              ele_dres.lepton_photon = ele_photons; 
			}
          }
	
        }	



        // positron selection
        if(Id == POSITRON &&
           fabs(eta) < 2.4 &&
           (fabs(eta) < 1.4442 || fabs(eta) > 1.5660)){
          FourMomentum pos_p(fs.particles().at(i).momentum());
          pos_photons.push_back(i);
          for(unsigned int j=0; j<photons.size(); j++){
            double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
            double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();
            double deltaR = sqrt((eta-eta_photon)*(eta-eta_photon) + (phi-phi_photon)*(phi-phi_photon));
                                
			if(fabs(eta) < 1.4442)
              R = 0.05;
            else
              R = 0.07;

            if(deltaR < R){
              pos_photons.push_back(photons.at(j));
              pos_p += fs.particles().at(photons.at(j)).momentum();
            }
          }

          if(pos_dres.lepton_photon.empty() && pos_p.pT()>20){
            pos_dres.p_part = pos_p;
            pos_dres.lepton_photon = pos_photons;
          }
          else{
            if(pos_p.pT() > pos_dres.p_part.pT() && pos_p.pT()>20){
              pos_dres.p_part = pos_p;
              pos_dres.lepton_photon = pos_photons;
            }

          }

        }


        // muon selection
        if(Id == MUON &&
           fabs(eta) < 2.4){
          FourMomentum muon_p(fs.particles().at(i).momentum());
          muon_photons.push_back(i);
          for(unsigned int j=0; j<photons.size(); j++){
            double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
            double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();
            double deltaR = sqrt((eta-eta_photon)*(eta-eta_photon) + (phi-phi_photon)*(phi-phi_photon));
            if(deltaR < 0.1){
              muon_photons.push_back(photons.at(j));
              muon_p += fs.particles().at(photons.at(j)).momentum();
            }
          }


          if(muon_dres.lepton_photon.empty() && muon_p.pT()>20){
            muon_dres.p_part = muon_p;
            muon_dres.lepton_photon = muon_photons;
          }
          else{
            if(muon_p.pT() > muon_dres.p_part.pT() && muon_p.pT()>20){
              muon_dres.p_part = muon_p;
              muon_dres.lepton_photon = muon_photons;
            }
          }

        }

        // antimuon selection
        if(Id == ANTIMUON &&
           fabs(eta) < 2.4){
          FourMomentum muon_p(fs.particles().at(i).momentum());
          antimuon_photons.push_back(i);
          for(unsigned int j=0; j<photons.size(); j++){
            double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
            double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();
            double deltaR = sqrt((eta-eta_photon)*(eta-eta_photon) + (phi-phi_photon)*(phi-phi_photon));
            if(deltaR < 0.1){
              antimuon_photons.push_back(photons.at(j));
              muon_p += fs.particles().at(photons.at(j)).momentum();
            }
          }

          if(antimuon_dres.lepton_photon.empty() && muon_p.pT()>20){
            antimuon_dres.p_part = muon_p;
            antimuon_dres.lepton_photon = antimuon_photons;
          }
          else{
            if(muon_p.pT() > antimuon_dres.p_part.pT() && muon_p.pT()>20){
              antimuon_dres.p_part = muon_p;
              antimuon_dres.lepton_photon = antimuon_photons;
            }
          }
        }

	
      }

	
      // construct the Z candidates momentum
      FourMomentum Z_momentum_ele(add(ele_dres.p_part,pos_dres.p_part));
      FourMomentum Z_momentum_muon(add(muon_dres.p_part,antimuon_dres.p_part));


      // Z mass window
      if((Z_momentum_ele.mass()>71. && Z_momentum_ele.mass()<111.) && (!ele_dres.lepton_photon.empty() && !pos_dres.lepton_photon.empty())){
      	Z_ele = true;
      }
      if((Z_momentum_muon.mass()>71. && Z_momentum_muon.mass()<111.) && (!muon_dres.lepton_photon.empty() && !antimuon_dres.lepton_photon.empty())){
      	Z_muon = true;
      }
      if (!Z_ele && !Z_muon)
        vetoEvent;

      if (Z_ele && Z_muon)
        vetoEvent;

      // costruisco un vettore che contiene gli indici delle particelle da cancellare nella lista part_jets
      vector <unsigned int> canc_part;
   
      if(Z_muon){
        canc_part=muon_dres.lepton_photon;
        for(unsigned int l=0; l<antimuon_dres.lepton_photon.size(); l++)
          canc_part.push_back(antimuon_dres.lepton_photon.at(l));
      }
      else{
        canc_part = ele_dres.lepton_photon;
        for(unsigned int y=0; y<pos_dres.lepton_photon.size(); y++)
          canc_part.push_back(pos_dres.lepton_photon.at(y));
      }

      //aggiungo gli indici delle particelle cariche con pT minore di 250 MeV
      if(!low_pt_charged.empty()){
        for(unsigned int j = 0; j<low_pt_charged.size(); j++)
          canc_part.push_back(low_pt_charged.at(j));
      }
      // aggiungo gli indici dei neutrini
      if(!neutrinos.empty()){
      	for(unsigned int k = 0; k<neutrinos.size(); k++)
          canc_part.push_back(neutrinos.at(k));
      }


      //ordino il vettore in modo crescente	
      stable_sort(canc_part.begin(),canc_part.end());
      //scorro il vettore dal basso e per ogni elemento cancello il corrispondente elemento da part_jets
      for(int i = (canc_part.size()-1); i>=0; i--)
      	part_jets.erase(part_jets.begin()+canc_part.at(i));

      // part_jets contiene ora tutte le particelle su cui fare la riclusterizzazione 
      for(unsigned int l=0; l<part_jets.size(); l++){
        FourMomentum p(part_jets.at(l).momentum());
        fastjet::PseudoJet pseudoJet(p.px(),
                                     p.py(),
                                     p.pz(),
                                     p.E());

        pseudoJet.set_user_index(l);
        vecs.push_back(pseudoJet);   
      }
 
     
      // jet selection
      double ht = 0;
      vector<fastjet::PseudoJet> jet_list; 
      fastjet::ClusterSequence cseq(vecs, fastjet::JetDefinition(fastjet::antikt_algorithm, 0.5));
      vector<fastjet::PseudoJet> jets = sorted_by_pt(cseq.inclusive_jets(30.0));
      for (unsigned int i = 0;  i < jets.size(); i++){
        double etaj = jets[i].eta();
        bool found = false;
        vector<fastjet::PseudoJet> constituents = cseq.constituents(jets[i]);
		for(unsigned int s = 0; s < constituents.size() && !found; s++){
          int index = constituents.at(s).user_index();
          if(PID::threeCharge(part_jets.at(index).pdgId())!=0);
          found = true;
		}
        if (fabs(etaj) < 2.4 && found && jets[i].perp()>30){
          jet_list.push_back(jets[i]);
          ht = ht + jets[i].perp();
        }
      }
      /*   
           if(jet_list.size()>0){
           if(Z_ele)
           h_ht_ele->fill(ht, weight);
           }
      */

      if(jet_list.size()>0){
      	double leadingjet_pt = jets[0].perp();
        double leadingEta=jets[0].eta();
        if(Z_ele){
          h_leading_jet_pt_ele->fill(leadingjet_pt, weight);
          h_eta_leading_ele->fill(leadingEta, weight);
          h_number_jets_ele->fill(jet_list.size(), weight);
          h_ht1_ele->fill(ht,weight);
        }
        if(Z_muon){
          h_leading_jet_pt_muon->fill(leadingjet_pt, weight);	
          h_eta_leading_muon->fill(leadingEta, weight);           
          h_number_jets_muon->fill(jet_list.size(), weight);
          h_ht1_muo->fill(ht,weight);
        }
      }

      if(jet_list.size()>1){
        double second_jet_pt = jets[1].perp();
        double second_jet_eta=jets[1].eta();
        if(Z_ele) {
          h_second_jet_pt_ele->fill(second_jet_pt, weight);
          h_eta_second_ele->fill(second_jet_eta, weight);
          h_ht2_ele->fill(ht,weight);
        }
        if(Z_muon) {
          h_second_jet_pt_muon->fill(second_jet_pt, weight);
          h_eta_second_muon->fill(second_jet_eta, weight);
          h_ht2_muo->fill(ht,weight);
        }
      }
      if(jet_list.size()>2){
        double third_jet_pt = jets[2].perp();
        double third_jet_eta=jets[2].eta();
        if(Z_ele) {
          h_third_jet_pt_ele->fill(third_jet_pt, weight);
          h_eta_third_ele->fill(third_jet_eta, weight);
          h_ht3_ele->fill(ht,weight);
        }
        if(Z_muon) {
          h_third_jet_pt_muon->fill(third_jet_pt, weight);
          h_eta_third_muon->fill(third_jet_eta, weight);
          h_ht3_muo->fill(ht,weight);
        }
      }
      if(jet_list.size()>3){
        double fourth_jet_pt = jets[3].perp();
        double fourth_jet_eta=jets[3].eta();
        if(Z_ele) {
          h_fourth_jet_pt_ele->fill(fourth_jet_pt, weight);
          h_eta_fourth_ele->fill(fourth_jet_eta, weight);
          h_ht4_ele->fill(ht,weight);
        }
        if(Z_muon) {
          h_fourth_jet_pt_muon->fill(fourth_jet_pt, weight);
          h_eta_fourth_muon->fill(fourth_jet_eta, weight);
          h_ht4_muo->fill(ht,weight);
        }
      }

    }


    /// Normalise histograms etc., after the run
    void finalize() {

      /// @todo Normalise, scale and otherwise manipulate histograms here
      // scale(_h_YYYY, crossSection()/sumOfWeights()); # norm to cross section
      // normalize(_h_YYYY); # normalize to unity
      
    }

    //@}


  private:

    // Data members like post-cuts event weight counters go here


  private:

    /// @name Histograms
    //@{

    AIDA::IHistogram1D * h_number_jets_ele;
    AIDA::IHistogram1D * h_leading_jet_pt_ele;
    AIDA::IHistogram1D * h_second_jet_pt_ele;
    AIDA::IHistogram1D * h_third_jet_pt_ele;
    AIDA::IHistogram1D * h_fourth_jet_pt_ele;
    AIDA::IHistogram1D * h_number_jets_muon;
    AIDA::IHistogram1D * h_leading_jet_pt_muon;
    AIDA::IHistogram1D * h_second_jet_pt_muon;
    AIDA::IHistogram1D * h_third_jet_pt_muon;
    AIDA::IHistogram1D * h_fourth_jet_pt_muon;
    AIDA::IHistogram1D * h_eta_leading_muon;
    AIDA::IHistogram1D * h_eta_second_muon;
    AIDA::IHistogram1D * h_eta_third_muon;
    AIDA::IHistogram1D * h_eta_fourth_muon;
    AIDA::IHistogram1D * h_eta_leading_ele;
    AIDA::IHistogram1D * h_eta_second_ele;
    AIDA::IHistogram1D * h_eta_third_ele;
    AIDA::IHistogram1D * h_eta_fourth_ele;
    AIDA::IHistogram1D * h_ht1_ele;
    AIDA::IHistogram1D * h_ht2_ele;
    AIDA::IHistogram1D * h_ht3_ele;
    AIDA::IHistogram1D * h_ht4_ele;
    AIDA::IHistogram1D * h_ht1_muo;
    AIDA::IHistogram1D * h_ht2_muo;
    AIDA::IHistogram1D * h_ht3_muo;
    AIDA::IHistogram1D * h_ht4_muo;
    //@}


  };



  // This global object acts as a hook for the plugin system
  AnalysisBuilder<CMS_EWK_12_XXX> plugin_CMS_EWK_12_XXX;


}
