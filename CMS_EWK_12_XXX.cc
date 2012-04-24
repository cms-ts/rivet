// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/RivetAIDA.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ZFinder.hh"
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


      FastJets akt(FastJets::ANTIKT, 0.5);
      addProjection(akt, "Jets"); 
     
      //h_Z_pT_normalised = bookHistogram1D(1, 1, 1);
      //h_Z_eta = bookHistogram1D(2, 1, 1);
      h_number_jets = bookHistogram1D(3, 1, 1);
      }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      const double weight = event.weight();
      unsigned int ele=0;
      unsigned int pos=0;
      std::vector<fastjet::PseudoJet> vecs;
      bool is_ele = false;
      bool is_pos = false;

      const FinalState& fs = applyProjection<FinalState>(event, "FS");
      fs.particlesByPt();

      // Find the highest pt electron and positron 
      for (unsigned int i=0; i<fs.particles().size(); i++){
	// remove neutrinos
	if(fabs(fs.particles().at(i).pdgId())==12 || 
	   fabs(fs.particles().at(i).pdgId())==14 ||
	   fabs(fs.particles().at(i).pdgId())==16 ) continue;
		
	double eta = fs.particles().at(i).momentum().eta();
	FourMomentum p(fs.particles().at(i).momentum());
	double pt = p.pT();
	int Id = fs.particles().at(i).pdgId();

	// electron selection
	if(!is_ele &&
	   Id == ELECTRON &&
	   pt > 20 && 
           fabs(eta) < 2.4 &&
	   (fabs(eta) < 1.4 || fabs(eta) > 1.56)){
		ele = i;
		is_ele = true;
		continue;
	}

	// positron selection
	if(!is_pos && 
           Id == POSITRON && 
           pt > 20 &&
           fabs(eta) < 2.4 &&
           (fabs(eta) < 1.4 || fabs(eta) > 1.56)){
                pos = i;
                is_pos = true;
		continue;
        }
	
	// fill the list of particles to be clustered
	fastjet::PseudoJet pseudoJet(p.px(),
                                     p.py(),
                                     p.pz(),
                                     p.E());
        vecs.push_back(pseudoJet);
      }
      
      if(!is_ele || !is_pos)
	vetoEvent;     
	
      // construct the Z candidates momentum
      FourMomentum mom_ele(fs.particles().at(ele).momentum());
      FourMomentum mom_pos(fs.particles().at(pos).momentum());
      FourMomentum Z_momentum(add(mom_ele,mom_pos));
      
      // Z mass window
      if(Z_momentum.mass()<71. || Z_momentum.mass()>111.)
      	vetoEvent;


      // jet selection
      vector<fastjet::PseudoJet> jet_list; 
      fastjet::ClusterSequence cseq(vecs, fastjet::JetDefinition(fastjet::antikt_algorithm, 0.5));
      vector<fastjet::PseudoJet> jets = sorted_by_pt(cseq.inclusive_jets(30.0));
      for (unsigned int i = 0;  i < jets.size(); i++){
        double etaj = jets[i].eta();
        if (fabs(etaj) < 2.4){
		jet_list.push_back(jets[i]);
                }
      }


      	h_number_jets->fill(jet_list.size(), weight);
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

    AIDA::IHistogram1D * h_number_jets;

    //@}


  };



  // This global object acts as a hook for the plugin system
  AnalysisBuilder<CMS_EWK_12_XXX> plugin_CMS_EWK_12_XXX;


}
