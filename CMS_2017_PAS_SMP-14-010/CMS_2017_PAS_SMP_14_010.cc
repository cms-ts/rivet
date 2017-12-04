#include "Rivet/Analysis.hh"
#include "Rivet/Tools/Cuts.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "TLorentzVector.h"

//#define DebugLog 

namespace Rivet {
  
  
  class CMS_2017_PAS_SMP_14_010 : public Analysis {
  public:
    
    /// Constructor
    DEFAULT_RIVET_ANALYSIS_CTOR(CMS_2017_PAS_SMP_14_010);
    
    /// Book histograms and initialise projections before the run
    void init() {

#ifdef DebugLog
      // set optionally the verbosity for the internal Rivet message system
      getLog().setLevel(0);
#endif      

      const FinalState fs;
      addProjection(fs, "FS");
      
      // projection to find the electrons and muons
      IdentifiedFinalState part(fs);
      part.acceptIdPair(PID::ELECTRON);
      part.acceptIdPair(PID::MUON);
      addProjection(part, "part");
      
      // Get photons to dress leptons
      IdentifiedFinalState photons(fs);
      photons.acceptIdPair(PID::PHOTON);
      addProjection(photons, "photons");
      
      //Histograms booking
      
      _h_first_bjet_pt_b = bookHisto1D(1,1,1);
      _h_first_bjet_abseta_b = bookHisto1D(3,1,1);
      _h_Z_pt_b = bookHisto1D(5,1,1);
      _h_HT_b = bookHisto1D(7,1,1);
      _h_Dphi_Zb_b = bookHisto1D(9,1,1);
      
      _h_first_jet_pt_ratio = bookScatter2D(2,1,1);
      _h_first_jet_abseta_ratio = bookScatter2D(4,1,1);
      _h_Z_pt_ratio = bookScatter2D(6,1,1);
      _h_HT_ratio = bookScatter2D(8,1,1);
      _h_Dphi_Zj_ratio = bookScatter2D(10,1,1);
      
      _h_first_jet_pt = std::make_shared<YODA::Histo1D>(*_h_first_bjet_pt_b);
      _h_first_jet_abseta = std::make_shared<YODA::Histo1D>(*_h_first_bjet_abseta_b);
      _h_Z_pt = std::make_shared<YODA::Histo1D>(*_h_Z_pt_b);
      _h_HT = std::make_shared<YODA::Histo1D>(*_h_HT_b);
      _h_Dphi_Zj = std::make_shared<YODA::Histo1D>(*_h_Dphi_Zb_b);

      _h_first_bjet_pt_bb = bookHisto1D(11,1,1);
      _h_second_bjet_pt_bb = bookHisto1D(12,1,1);
      _h_Z_pt_bb = bookHisto1D(13,1,1);
      _h_bb_mass_bb = bookHisto1D(14,1,1);
      _h_Zbb_mass_bb = bookHisto1D(15,1,1);
      _h_Dphi_bb = bookHisto1D(16,1,1);
      _h_DR_bb = bookHisto1D(17,1,1);
      _h_DR_Zbmin_bb = bookHisto1D(18,1,1);
      _h_A_DR_Zb_bb = bookHisto1D(19,1,1);

      _h_bjet_multiplicity = bookHisto1D(20,1,1);

    }
                
        
    /// Perform the per-event analysis
    void analyze(const Event& event) {
            
      //Event weight
      const double w = 0.5*event.weight();

      double Ht = 0;
            
      //Structure to contain original particle, dressed lepton and photons used for dressing
      struct pt_and_particles {
        FourMomentum p_part;
        vector < FourMomentum >lepton_photon;
      };
            
      // struttura che contiene il vettore lepton_photon che contiene l'indice del leptone candidato figlio della Z
      // e i suoi fotoni e contiene il quadrimomento p_part dell'elettrone rivestito.
      struct pt_and_particles ele_dres;
      struct pt_and_particles pos_dres;
      struct pt_and_particles muon_dres;
      struct pt_and_particles antimuon_dres;
            
      //all leptons
      vector<Particle> part;
      part = applyProjection<IdentifiedFinalState>(event, "part").particles();
      //all photons
      vector<Particle> photons;
      photons = applyProjection<IdentifiedFinalState>(event, "photons").particles();
            
      //dressed lepton and photons fourmomentum
      FourMomentum partm;
      vector <FourMomentum> photonsm;
            
      //if no leptons veto
      if (part.size()==0) vetoEvent;
            
      //dressing of each lepton
      foreach (const Particle& e, part){
        partm=e.momentum();
        photonsm.push_back(e.momentum());
        foreach (const Particle& p, photons){
          if  (deltaR(p.momentum(),e.momentum())<0.1) {
            partm+=p.momentum();
            photonsm.push_back(p.momentum());
          }
        }
                
        //saving of dressed lepton into the right ID structure
        if(e.pdgId()==PID::ELECTRON && partm.pt()>20*GeV && partm.pt()>ele_dres.p_part.pt() && partm.abseta()<2.4){
          ele_dres.p_part=partm;
          ele_dres.lepton_photon=photonsm;
        }
        if(e.pdgId()==-PID::ELECTRON && partm.pt()>20*GeV && partm.pt()>pos_dres.p_part.pt() && partm.abseta()<2.4){
          pos_dres.p_part=partm;
          pos_dres.lepton_photon=photonsm;
        }
        if(e.pdgId()==PID::MUON && partm.pt()>20*GeV && partm.pt()>muon_dres.p_part.pt() && partm.abseta()<2.4){
          muon_dres.p_part=partm;
          muon_dres.lepton_photon=photonsm;
        }
        if(e.pdgId()==-PID::MUON && partm.pt()>20*GeV && partm.pt()>antimuon_dres.p_part.pt() && partm.abseta()<2.4){
          antimuon_dres.p_part=partm;
          antimuon_dres.lepton_photon=photonsm;
        }
                
        photonsm.clear();
      }
            
      //if no good dressed lepton veto
      if ((ele_dres.lepton_photon.empty() || pos_dres.lepton_photon.empty()) && (muon_dres.lepton_photon.empty() || antimuon_dres.lepton_photon.empty())) vetoEvent;
            
      //Z boson fourmomentum
      FourMomentum ze(add(ele_dres.p_part, pos_dres.p_part));
      FourMomentum zm(add(muon_dres.p_part, antimuon_dres.p_part));
            
      if (ze.mass2()<0 || zm.mass2()<0) vetoEvent;
            
      //event identification depending on mass window
      bool ee_event=false;
      bool mm_event=false;
            
      if (!ele_dres.lepton_photon.empty() && !pos_dres.lepton_photon.empty() && ze.mass()>71*GeV && ze.mass()<111*GeV) ee_event = true;
      if (!muon_dres.lepton_photon.empty() && !antimuon_dres.lepton_photon.empty() && zm.mass()>71*GeV && zm.mass()<111*GeV) mm_event = true;

      //if not Z boson veto
      if (!ee_event && !mm_event) vetoEvent;

      // pseudojet building procedure
      const FinalState& fs=applyProjection < FinalState > (event, "FS");

      std::vector < fastjet::PseudoJet > vecs;
      vector < Particle > part_jets;
      int l=0;
            
      foreach (const Particle& p, fs.particles()) {
        if (fabs(p.pdgId())!=PID::NU_E && fabs(p.pdgId())!=PID::NU_MU && fabs(p.pdgId())!=PID::NU_TAU  ) {
          bool overcount=false;
          if (mm_event){
            int psize1=muon_dres.lepton_photon.size();
            int psize2=antimuon_dres.lepton_photon.size();
            for (int j=0;j<psize1;j++){
              if (p.momentum()==muon_dres.lepton_photon[j]) overcount=true;
            }
            for (int j=0;j<psize2;j++){
              if (p.momentum()==antimuon_dres.lepton_photon[j]) overcount=true;
            }
          }
          if (ee_event) {
            int psize3=ele_dres.lepton_photon.size();
            int psize4=pos_dres.lepton_photon.size();
            for (int j=0;j<psize3;j++){
              if (p.momentum()==ele_dres.lepton_photon[j]) overcount=true;
            }
            for (int j=0;j<psize4;j++){
              if (p.momentum()==pos_dres.lepton_photon[j]) overcount=true;
            }
          }
          if (overcount==false && (ee_event || mm_event)){
            fastjet::PseudoJet pseudoJet(p.momentum().px(), p.momentum().py(), p.momentum().pz(), p.momentum().E());
            pseudoJet.set_user_index(l);
            vecs.push_back(pseudoJet);
            part_jets.push_back(p);
            l++;
          }
        }
      }
            
      //jet building
      fastjet::ClusterSequence cseq(vecs, fastjet::JetDefinition(fastjet::antikt_algorithm, 0.5));
      vector < fastjet::PseudoJet > jets = sorted_by_pt(cseq.inclusive_jets(30.0));
      vector < fastjet::PseudoJet > jet_final;
      vector < fastjet::PseudoJet > jb_final;
            
      //identification of "good" jets and bjets
            
      foreach (const Jet& j, jets) {
        bool bjet_found = false;
        bool bjet_found1 = false;
        //trovo i jet buoni
        if ((j.abseta() < 2.4 && j.perp() > 30) && ((ee_event && deltaR(j.momentum(),ele_dres.p_part)>0.5 && deltaR(j.momentum(),pos_dres.p_part)>0.5) || (mm_event && deltaR(j.momentum(),muon_dres.p_part)>0.5 && deltaR(j.momentum(),antimuon_dres.p_part)>0.5))) {
          jet_final.push_back(j);
                    
          Ht = Ht + j.perp();
          FourMomentum pim=j.momentum();
                    
          foreach (const fastjet::PseudoJet&  c, cseq.constituents(j)) {
            const Particle & part = part_jets.at(c.user_index());
            const PdgId pid = part.pdgId();
            if (((abs(pid)/100)%10 == 5 ||(abs(pid)/1000)%10 == 5) && deltaR(j,part.momentum().eta(),part.momentum().phi())<0.5) bjet_found = true;
            HepMC::GenVertex * gv = part.genParticle()->production_vertex();
            if (gv) {
              foreach (const GenParticle * pi, Rivet::particles(gv, HepMC::ancestors)) {
                const PdgId pid2 = pi->pdg_id();
                                
                if (((abs(pid2)/100)%10 == 5 || (abs(pid2)/1000)%10 == 5) && (pi->status()>0 && pi->status()<4)) {
                  bjet_found = true;
                  pim=pi->momentum();
                }
              }
            }
            if (bjet_found && deltaR(j,pim.eta(),pim.phi())<0.5) bjet_found1=true;
          }
                    
          if (bjet_found1) {
            jb_final.push_back(j);
          }
                    
        }
      }

      //histogram filling

      if ((ee_event || mm_event) && jet_final.size() > 0) {

#ifdef DebugLog      
        std::string flavour;
        double mass(0.), ptz(0.);
        if ( ee_event ) { flavour = "EE" ; mass = ze.mass() ; ptz = ze.pt() ; };
        if ( mm_event ) { flavour = "MM" ; mass = zm.mass() ; ptz = zm.pt() ; };
        MSG_DEBUG("OLD " << flavour << " " << event.genEvent()->event_number() << std::setfill(' ') 
                  << std::setw(14) << std::fixed << std::setprecision(3) << mass 
                  << std::setw(14) << std::fixed << std::setprecision(3) << ptz
                  << std::setw(14) << std::fixed << std::setprecision(3) << jet_final.size()
                  << std::setw(14) << std::fixed << std::setprecision(3) << jb_final.size());
#endif
        
        FourMomentum j1(jet_final[0].e(),jet_final[0].px(),jet_final[0].py(),jet_final[0].pz());

        _h_first_jet_pt->fill(j1.pt(),w);
        _h_first_jet_abseta->fill(fabs(j1.eta()),w);
        if ( ee_event ) _h_Z_pt->fill(ze.pt(),w);
        if ( mm_event ) _h_Z_pt->fill(zm.pt(),w);
        _h_HT->fill(Ht,w);
        if ( ee_event ) _h_Dphi_Zj->fill(deltaPhi(ze, j1),w);
        if ( mm_event ) _h_Dphi_Zj->fill(deltaPhi(zm, j1),w);
        
        if ( jb_final.size() > 0 ) { 

          FourMomentum b1(jb_final[0].e(),jb_final[0].px(),jb_final[0].py(),jb_final[0].pz());

          _h_bjet_multiplicity->fill(1.,w);

          _h_first_bjet_pt_b->fill(b1.pt(),w);
          _h_first_bjet_abseta_b->fill(fabs(b1.eta()),w);
          if ( ee_event ) _h_Z_pt_b->fill(ze.pt(),w);
          if ( mm_event ) _h_Z_pt_b->fill(zm.pt(),w);
          _h_HT_b->fill(Ht,w);
          if ( ee_event ) _h_Dphi_Zb_b->fill(deltaPhi(ze, b1.phi()),w);
          if ( mm_event ) _h_Dphi_Zb_b->fill(deltaPhi(zm, b1.phi()),w);

          if ( jb_final.size() > 1 ) {

            FourMomentum b2(jb_final[1].e(),jb_final[1].px(),jb_final[1].py(),jb_final[1].pz());

            _h_bjet_multiplicity->fill(2.,w);

            _h_first_bjet_pt_bb->fill(b1.pt(),w);
            _h_second_bjet_pt_bb->fill(b2.pt(),w);
            if ( ee_event ) _h_Z_pt_bb->fill(ze.pt(),w);
            if ( mm_event ) _h_Z_pt_bb->fill(zm.pt(),w);

            FourMomentum bb = add(b1,b2);
            FourMomentum Zbb;
            if (ee_event) Zbb = add(ze,bb);
            if (mm_event) Zbb = add(zm,bb);

            _h_bb_mass_bb->fill(bb.mass(),w);
            _h_Zbb_mass_bb->fill(Zbb.mass(),w);

            _h_Dphi_bb->fill(deltaPhi(b1,b2),w);
            _h_DR_bb->fill(deltaR(b1,b2),w);

            double DR_Z_b1(0.), DR_Z_b2(0.);
            if ( ee_event ) {
              DR_Z_b1 = deltaR(ze,b1);
              DR_Z_b2 = deltaR(ze,b2);
            }
            if ( mm_event ) {
              DR_Z_b1 = deltaR(zm,b1);
              DR_Z_b2 = deltaR(zm,b2);
            }

            double DR_Zb_min = DR_Z_b1;
            double DR_Zb_max = DR_Z_b2;
            if ( DR_Zb_min > DR_Zb_max ) {
              DR_Zb_min = DR_Z_b2;
              DR_Zb_max = DR_Z_b1;
            }
            double A_Zbb = (DR_Zb_max - DR_Zb_min)/(DR_Zb_max + DR_Zb_min);

            _h_DR_Zbmin_bb->fill(DR_Zb_min,w);
            _h_A_DR_Zb_bb->fill(A_Zbb,w);

          }
          
        }
                                           
      }

    }
        
    /// Normalise histograms etc., after the run
    void finalize() {

      const double norm = (sumOfWeights() != 0) ? crossSection()/picobarn/sumOfWeights() : 1.0;

      MSG_INFO("Cross section = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(3) << crossSection() << " pb");
      MSG_INFO("# Events      = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(3) << numEvents() );
      MSG_INFO("SumW          = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(3) << sumOfWeights());
      MSG_INFO("Norm factor   = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(6) << norm);

      scale( _h_first_bjet_pt_b, 100. );
      scale( _h_first_bjet_abseta_b, 100. );
      scale( _h_Z_pt_b, 100. );
      scale( _h_HT_b, 100. );
      scale( _h_Dphi_Zb_b, 100. );

      divide( _h_first_bjet_pt_b , _h_first_jet_pt , _h_first_jet_pt_ratio );
      divide( _h_first_bjet_abseta_b , _h_first_jet_abseta , _h_first_jet_abseta_ratio );
      divide( _h_Z_pt_b , _h_Z_pt , _h_Z_pt_ratio );
      divide( _h_HT_b , _h_HT , _h_HT_ratio );
      divide( _h_Dphi_Zb_b , _h_Dphi_Zj , _h_Dphi_Zj_ratio );

      scale( _h_first_bjet_pt_b, norm/100. );
      scale( _h_first_bjet_abseta_b, norm/100. );
      scale( _h_Z_pt_b, norm/100. );
      scale( _h_HT_b, norm/100. );
      scale( _h_Dphi_Zb_b, norm/100. );

      scale( _h_first_bjet_pt_bb, norm);
      scale( _h_second_bjet_pt_bb, norm);
      scale( _h_Z_pt_bb, norm);
      scale( _h_bb_mass_bb, norm);
      scale( _h_Zbb_mass_bb, norm);
      scale( _h_Dphi_bb, norm);
      scale( _h_DR_bb, norm);
      scale( _h_DR_Zbmin_bb, norm);
      scale( _h_A_DR_Zb_bb, norm);

      scale( _h_bjet_multiplicity, norm );

    }


  private:

    /// @name Histograms
    
    Histo1DPtr     _h_first_jet_pt, _h_first_bjet_pt_b;
    Histo1DPtr     _h_first_jet_abseta, _h_first_bjet_abseta_b;
    Histo1DPtr     _h_Z_pt, _h_Z_pt_b;
    Histo1DPtr     _h_HT, _h_HT_b;
    Histo1DPtr     _h_Dphi_Zj, _h_Dphi_Zb_b;

    Scatter2DPtr     _h_first_jet_pt_ratio;
    Scatter2DPtr     _h_first_jet_abseta_ratio;
    Scatter2DPtr     _h_Z_pt_ratio;
    Scatter2DPtr     _h_HT_ratio;
    Scatter2DPtr     _h_Dphi_Zj_ratio;
    
    Histo1DPtr     _h_first_bjet_pt_bb, _h_second_bjet_pt_bb;
    Histo1DPtr     _h_Z_pt_bb;
    Histo1DPtr     _h_bb_mass_bb, _h_Zbb_mass_bb;
    Histo1DPtr     _h_Dphi_bb, _h_DR_bb, _h_DR_Zbmin_bb, _h_A_DR_Zb_bb;
    
    Histo1DPtr     _h_bjet_multiplicity;

  };
  
  
  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2017_PAS_SMP_14_010);
  
}
