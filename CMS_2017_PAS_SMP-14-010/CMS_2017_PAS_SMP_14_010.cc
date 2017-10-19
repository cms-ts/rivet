#include "Rivet/Analysis.hh"
#include "Rivet/Tools/Cuts.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "TLorentzVector.h"

//#define DebugLog 
//#define Check

namespace Rivet {

  typedef std::vector<DressedLepton> DLcollection;
  
  // adapt the ZFinder projection to this analysis selection criteria
  class ThisZFinder : public FinalState{
    
  public:
    ThisZFinder(const FinalState& inputfs,
                const Cut & fsCut,
                PdgId pid,
                double minmass, double maxmass,
                double dRmax)
    {
      setName("ThisZFinder");
      
      _minmass = minmass;
      _maxmass = maxmass;
      _pid = pid;
      
      IdentifiedFinalState bareleptons(inputfs);
      bareleptons.acceptIdPair(_pid);
      DressedLeptons leptons(inputfs, bareleptons, dRmax, fsCut, true, false);
      addProjection(leptons, "DressedLeptons");
      
      VetoedFinalState remainingFS;
      remainingFS.addVetoOnThisFinalState(*this);
      addProjection(remainingFS, "RFS");
      
    }
    
    DEFAULT_RIVET_PROJ_CLONE(ThisZFinder);
    
    const VetoedFinalState& remainingFinalState() const {
      return getProjection<VetoedFinalState>("RFS");
    }
    
    /// @note Currently either 0 or 1 boson can be found.
    const Particles& bosons() const { return _bosons; }
    
    /// Selected dressed leptons
    const DLcollection& selectedDLeptons() const { return _selectedDLeptons; }

    /// Clear the projection
    void clear() {
      _theParticles.clear();
      _selectedDLeptons.clear();
      _bosons.clear();
    }  
    
  protected:
    /// Apply the projection on the supplied event.
    //    void project(const Event& e);
    
    void project(const Event& e) {
      
      clear();
      
      const DressedLeptons& leptons = applyProjection<DressedLeptons>(e, "DressedLeptons");
      
      /// Select lepton-antilepton pair with highest pt
      
      Particle dummy;
      DressedLepton plus(dummy), minus(dummy);
      foreach (const DressedLepton& p, leptons.dressedLeptons()){
        if (p.pdgId()>0 && p.momentum().pt() > plus.momentum().pt()){
          plus = p;
        }
        if (p.pdgId()<0 && p.momentum().pt() > minus.momentum().pt()){
          minus = p;
        }
      }
      
      FourMomentum pZ;
      if (plus.pdgId()>0 && minus.pdgId()<0 ) { pZ = plus.momentum() + minus.momentum(); }
      else { return; }
      
      /// Mass window selection
      if ( pZ.mass() < _minmass || pZ.mass() > _maxmass ) return;
      
      /// define the selected dressed leptons and the corresponding reconstructed candidate boson
      
      _selectedDLeptons.push_back(plus);
      _selectedDLeptons.push_back(minus);
      
      _bosons.push_back(Particle(PID::ZBOSON, pZ));
      
      /// define the collection of selected particles in the final state
      
      _theParticles.push_back(plus.constituentLepton());
      _theParticles.insert(_theParticles.end(), plus.constituentPhotons().begin(), plus.constituentPhotons().end());
      
      _theParticles.push_back(minus.constituentLepton());
      _theParticles.insert(_theParticles.end(), minus.constituentPhotons().begin(), minus.constituentPhotons().end());
      
    }
    
  private:
    
    /// Mass cuts to apply to clustered leptons
    double _minmass, _maxmass;
    /// Lepton flavour
    PdgId _pid;    
    
    /// list of found bosons (currently either 0 or 1)
    Particles _bosons;

    /// Selected dressed leptons
    std::vector<DressedLepton> _selectedDLeptons;

  };
  

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

      FinalState fs; ///< @todo No cuts?
      VisibleFinalState visfs(fs);

      // ThisZFinder zeeFinder(fs, Cuts::abseta < 2.4 && Cuts::pT > 20*GeV, PID::ELECTRON, 71.0*GeV, 111.0*GeV, 0.1 );
      // addProjection(zeeFinder, "ZeeFinder");

      // ThisZFinder zmumuFinder(fs, Cuts::abseta < 2.4 && Cuts::pT > 20*GeV, PID::MUON, 71.0*GeV, 111.0*GeV, 0.1 );
      // addProjection(zmumuFinder, "ZmumuFinder");

      ZFinder zeeFinder(fs, Cuts::abseta < 2.4 && Cuts::pT > 20*GeV, PID::ELECTRON, 71.0*GeV, 111.0*GeV, 0.1 );
      addProjection(zeeFinder, "ZeeFinder");

      ZFinder zmumuFinder(fs, Cuts::abseta < 2.4 && Cuts::pT > 20*GeV, PID::MUON, 71.0*GeV, 111.0*GeV, 0.1 );
      addProjection(zmumuFinder, "ZmumuFinder");

      VetoedFinalState jetConstits(visfs);
      jetConstits.addVetoOnThisFinalState(zeeFinder);
      jetConstits.addVetoOnThisFinalState(zmumuFinder);

      FastJets akt05Jets(jetConstits, FastJets::ANTIKT, 0.5);
      addProjection(akt05Jets, "AntiKt05Jets");
      
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

#ifdef Check
      count1 = 0;
      count2 = 0;
#endif
    }
                
        
    /// Perform the per-event analysis
    void analyze(const Event& event) {
            
      //      const ThisZFinder& zeeFS = applyProjection<ThisZFinder>(event, "ZeeFinder");
      //      const ThisZFinder& zmumuFS = applyProjection<ThisZFinder>(event, "ZmumuFinder");
      const ZFinder& zeeFS = applyProjection<ZFinder>(event, "ZeeFinder");
      const ZFinder& zmumuFS = applyProjection<ZFinder>(event, "ZmumuFinder");

      const Particles& zees = zeeFS.bosons();
      const Particles& zmumus = zmumuFS.bosons();

#ifdef DebugLog
      std::cout << "NEW # ele = " << zees.size() << " # muo = " << zmumus.size() << std::endl;
      if (zees.size() > 0 ) { std::cout << "NEW " << zees[0].momentum().pt() << std::endl; }
      if (zmumus.size() > 0 ) { std::cout << "NEW " << zmumus[0].momentum().pt() << std::endl; } 
#endif

      // We did not find exactly one Z. No good.
      if (zees.size() + zmumus.size() != 1) {
        MSG_DEBUG("Did not find exactly one good Z candidate");
        vetoEvent;
      }

      //event identification depending on mass window
      bool ee_event=false;
      bool mm_event=false;
            
      //      DLcollection theLeptons;
      //      if (zees.size() == 1) { ee_event = true; theLeptons = zeeFS.selectedDLeptons(); }
      //      if (zmumus.size() == 1) { mm_event = true; theLeptons = zmumuFS.selectedDLeptons(); }
      if (zees.size() == 1) { ee_event = true; }
      if (zmumus.size() == 1) { mm_event = true; }
      const Particles& theLeptons = zees.size() ? zeeFS.constituents() : zmumuFS.constituents();

#ifdef Check
      if ( ee_event || mm_event ) { count1 = count1 + 1 ; }
#endif
#ifdef DebugLog
      std::cout << "NEW Z selected" << std::endl;
#endif

      // Cluster jets
      // NB. Veto has already been applied on leptons and photons used for dressing
      const FastJets& fj = applyProjection<FastJets>(event, "AntiKt05Jets");
      const Jets& jets = fj.jetsByPt(Cuts::abseta < 2.4 && Cuts::pT > 30*GeV);

      // Perform lepton-jet overlap and HT calculation
      double Ht = 0;
      Jets goodjets;
      foreach (const Jet& j, jets) {
        // Decide if this jet is "good", i.e. isolated from the leptons
        /// @todo Nice use-case for any() and a C++11 lambda
        bool overlap = false;
        foreach (const Particle& l, theLeptons) {
          if (Rivet::deltaR(j, l) < 0.5) {
            overlap = true;
            break;
          }
        }

        // Fill HT and good-jets collection
        if (overlap) continue;
        goodjets.push_back(j);
        Ht += j.pT();
      }

      // We don't care about events with no isolated jets
      if (goodjets.empty()) {
        MSG_DEBUG("No jets in event");
        vetoEvent;
      }

#ifdef Check
      count2 = count2+1;
#endif
            
      Jets jb_final;
            
      //identification of bjets
            
      foreach (const Jet& j, goodjets) {
        bool bjet_found = false;
        bool bjet_found1 = false;
        
        FourMomentum pim=j.momentum();
        
        foreach (const Particle& part, j.constituents()) {
          if ( part.hasBottom() && deltaR(j,part.momentum().eta(),part.momentum().phi())<0.5) bjet_found = true;
          GenVertex* prodVtx = part.genParticle()->production_vertex();
          if (prodVtx == NULL) continue;
          foreach (const GenParticle* ancestor, particles(prodVtx, HepMC::ancestors)) {
            const PdgId pid = ancestor->pdg_id();
            if (ancestor->status() == 2 && (PID::isHadron(pid) && PID::hasBottom(pid))) {
              bjet_found = true;
              pim=ancestor->momentum();
              break;
            }
          } 
          if (bjet_found && deltaR(j,pim.eta(),pim.phi())<0.5) bjet_found1=true;
        }
        
        if (bjet_found1) {
          jb_final.push_back(j);
        }
        
      }
            
      //Event weight
      const double w = 0.5*event.weight();
            
      //histogram filling

      if ((ee_event || mm_event) && goodjets.size() > 0) {
        
#ifdef DebugLog      
        std::string flavour;
        double mass(0.), ptz(0.);
        if ( ee_event ) { flavour = "EE" ; mass = zees[0].momentum().mass() ; ptz = zees[0].momentum().pt() ; };
        if ( mm_event ) { flavour = "MM" ; mass = zmumus[0].momentum().mass() ; ptz = zmumus[0].momentum().pt() ; };
        std::cout << "NEW " << flavour << " " << event.genEvent()->event_number() << std::setfill(' ') 
                  << std::setw(14) << std::fixed << std::setprecision(3) << mass 
                  << std::setw(14) << std::fixed << std::setprecision(3) << ptz
                  << std::setw(14) << std::fixed << std::setprecision(3) << goodjets.size()
                  << std::setw(14) << std::fixed << std::setprecision(3) << jb_final.size()
                  << std::endl;
#endif

        FourMomentum j1(goodjets[0].momentum());

        _h_first_jet_pt->fill(j1.pt(),w);
        _h_first_jet_abseta->fill(fabs(j1.eta()),w);
        if ( ee_event ) _h_Z_pt->fill(zees[0].pt(),w);
        if ( mm_event ) _h_Z_pt->fill(zmumus[0].pt(),w);
        _h_HT->fill(Ht,w);
        if ( ee_event ) _h_Dphi_Zj->fill(deltaPhi(zees[0], j1),w);
        if ( mm_event ) _h_Dphi_Zj->fill(deltaPhi(zmumus[0], j1),w);
        
        if ( jb_final.size() > 0 ) { 

          FourMomentum b1(jb_final[0].momentum());

          _h_bjet_multiplicity->fill(1.,w);

          _h_first_bjet_pt_b->fill(b1.pt(),w);
          _h_first_bjet_abseta_b->fill(fabs(b1.eta()),w);
          if ( ee_event ) _h_Z_pt_b->fill(zees[0].pt(),w);
          if ( mm_event ) _h_Z_pt_b->fill(zmumus[0].pt(),w);
          _h_HT_b->fill(Ht,w);
          if ( ee_event ) _h_Dphi_Zb_b->fill(deltaPhi(zees[0], b1.phi()),w);
          if ( mm_event ) _h_Dphi_Zb_b->fill(deltaPhi(zmumus[0], b1.phi()),w);

          if ( jb_final.size() > 1 ) {

            FourMomentum b2(jb_final[1].momentum());

            _h_bjet_multiplicity->fill(2.,w);

            _h_first_bjet_pt_bb->fill(b1.pt(),w);
            _h_second_bjet_pt_bb->fill(b2.pt(),w);
            if ( ee_event ) _h_Z_pt_bb->fill(zees[0].pt(),w);
            if ( mm_event ) _h_Z_pt_bb->fill(zmumus[0].pt(),w);

            FourMomentum bb = add(b1,b2);
            FourMomentum Zbb;
            if (ee_event) Zbb = add(zees[0],bb);
            if (mm_event) Zbb = add(zmumus[0],bb);

            _h_bb_mass_bb->fill(bb.mass(),w);
            _h_Zbb_mass_bb->fill(Zbb.mass(),w);

            _h_Dphi_bb->fill(deltaPhi(b1,b2),w);
            _h_DR_bb->fill(deltaR(b1,b2),w);

            double DR_Z_b1(0.), DR_Z_b2(0.);
            if ( ee_event ) {
              DR_Z_b1 = deltaR(zees[0],b1);
              DR_Z_b2 = deltaR(zees[0],b2);
            }
            if ( mm_event ) {
              DR_Z_b1 = deltaR(zmumus[0],b1);
              DR_Z_b2 = deltaR(zmumus[0],b2);
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

      //      double xs = 1177.3*2.;
      double xs = 1141.3*2.;
      setCrossSection(xs);

      const double norm = (sumOfWeights() != 0) ? crossSection()/picobarn/sumOfWeights() : 1.0;

      MSG_INFO("Cross section = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(3) << crossSection() << " pb");
      MSG_INFO("# Events      = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(3) << numEvents() );
      MSG_INFO("SumW          = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(3) << sumOfWeights());
      MSG_INFO("Norm factor   = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(6) << norm);

#ifdef Check
      std::cout << count1 << std::endl;
      std::cout << count2 << std::endl;
#endif

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

#ifdef Check
    int count1,count2;
#endif

  };
  
  
  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2017_PAS_SMP_14_010);
  
}
