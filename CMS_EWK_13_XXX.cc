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
#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"
#include "Math/VectorUtil.h"


/// @todo Include more projections as required, e.g. ChargedFinalState, FastJets, ZFinder...

namespace Rivet
{

  class CMS_EWK_13_XXX:public Analysis
  {
  public:

    /// Constructor
    CMS_EWK_13_XXX():Analysis("CMS_EWK_13_XXX")
    {
      /// @todo Set whether your finalize method needs the generator cross section
      setNeedsCrossSection(true);
    }

  public:

    /// Book histograms and initialise projections before the run
    void init()
    {

      /// @todo Initialise and register projections here

      /// @todo Book histograms here, e.g.:

      const FinalState fs;

      addProjection(fs, "FS");

      //num_ZBjet = 0;
      //num_Zjet = 0;
      num_weights_tot = 0;
      sum_weights_tot = 0.;
      sum_weights2_tot = 0.;

      //h_gen_weights = bookHistogram1D(1, 1, 1, "MyDbn - HepData dataset 1, x-axis 1 and y-axis 2");
      //h_jetmultiplicity = bookHistogram1D(2, 1, 1);
      //h_jet_pt = bookHistogram1D(3, 1, 1);
      //h_ele_pt = bookHistogram1D(4, 1, 1);
      //h_muon_pt = bookHistogram1D(5, 1, 1);
      //w_pt_Z_ee = bookHistogram1D("d06-x01-y01",40, 0., 400.);
      w_pt_Z_ee = bookHistogram1D(6, 1, 1);
      w_pt_Z_mm = bookHistogram1D(7, 1, 1);
      w_pt_Z_ee_b = bookHistogram1D(8, 1, 1);
      w_pt_Z_mm_b = bookHistogram1D(9, 1, 1);
      w_jetmultiplicity = bookHistogram1D("d10-x01-y01",8, 0.5, 8.5);
      w_first_jet_pt = bookHistogram1D(11, 1, 1);
      w_first_jet_eta = bookHistogram1D(12, 1, 1);
      w_first_bjet_pt = bookHistogram1D(13, 1, 1);
      w_first_bjet_eta = bookHistogram1D(14, 1, 1);
      w_bjetmultiplicity = bookHistogram1D("d15-x01-y01",5, 0.5, 5.5);
      w_first_jet_pt_b = bookHistogram1D("d16-x01-y01",50, 30., 700.);
      w_first_jet_eta_b = bookHistogram1D("d17-x01-y01",16, -2.5, 2.5);
      w_first_ele_pt = bookHistogram1D("d18-x01-y01",50, 0., 450.);

      w_second_ele_pt = bookHistogram1D("d19-x01-y01",50, 0., 450.);
      //w_dressed_ele_pt = bookHistogram1D(20, 1, 1);
      w_first_muon_pt = bookHistogram1D("d21-x01-y01",50, 0., 450.);
      w_mass_ee = bookHistogram1D("d22-x01-y01",80, 71., 111.);

      w_mass_mm= bookHistogram1D("d23-x01-y01",80, 71., 111.);
      w_mass_ee_b = bookHistogram1D("d24-x01-y01",80, 71., 111.);
      w_mass_mm_b = bookHistogram1D("d25-x01-y01",80, 71., 111.);
      w_delta_ee_b = bookHistogram1D(26, 1, 1);

      w_delta_mm_b = bookHistogram1D(27, 1, 1);
      w_delta_ee = bookHistogram1D(28, 1, 1);
      w_delta_mm = bookHistogram1D(29, 1, 1);
      w_Ht = bookHistogram1D(30, 1, 1);
      w_Ht_b = bookHistogram1D(31, 1, 1);

    }

    /// Perform the per-event analysis
    void analyze(const Event & event)
    {

      const double weight = event.weight();
      std::vector<fastjet::PseudoJet> vecs;
      bool ee_event = false;
      bool mm_event = false;

      double diele_mass = 0;
      double diele_phi = 0;
      double diele_pt = 0;

      double dimuon_mass = 0;
      double dimuon_phi = 0;
      double dimuon_pt = 0;

      num_weights_tot++;
      sum_weights_tot += weight;
      sum_weights2_tot += weight * weight;

      // vettore indici dell'elettrone e i suoi fotoni
      vector<unsigned int> ele_photons;
      // vettore indici dei fotoni
      vector<unsigned int> photons;
      // vettore indici del positrone e i suoi fotoni
      vector<unsigned int> pos_photons;
      // vettore indici dei neutrini
      vector<unsigned int> neutrinos;
      // vettore indici delle cariche a basso pt (< 250 MeV)
      vector<unsigned int> low_pt_charged;
      // vettore indici del muone e i suoi fotoni
      vector<unsigned int> muon_photons;
      // vettore indici del muone e i suoi fotoni
      vector<unsigned int> antimuon_photons;

      struct pt_and_particles
      {
        FourMomentum p_part;
        vector<unsigned int>lepton_photon;
      };
	


      // struttura che contiene il vettore lepton_photon che contiene l'indice del leptone candidato figlio della Z
      // e i suoi fotoni e contiene il quadrimomento p_part dell'elettrone rivestito. 
      struct pt_and_particles ele_dres;
      struct pt_and_particles pos_dres;
      struct pt_and_particles muon_dres;
      struct pt_and_particles antimuon_dres;

      vector<Particle> part_jets;

      const FinalState & fs = applyProjection< FinalState>(event, "FS");
      //fs.particlesByPt();

      // copy of the list of final state partcles
      part_jets = fs.particles();

      // riempio un vettore(photons) con gli indici dei fotoni in fs e un vettore(neutrinos) con gli indici dei neutrini in fs
      for (unsigned int i = 0; i < fs.particles().size(); i++)
        {
          if (fs.particles().at(i).pdgId() == PHOTON)
            photons.push_back(i);
          if (fabs(fs.particles().at(i).pdgId()) == 12 ||
              fabs(fs.particles().at(i).pdgId()) == 14 ||
              fabs(fs.particles().at(i).pdgId()) == 16)
            neutrinos.push_back(i);
/*
          if (PID::threeCharge(fs.particles().at(i).pdgId()) != 0)
            if (fs.particles().at(i).momentum().pT() < 0.25)
              low_pt_charged.push_back(i);
*/
        }


      //cout << " ------------- RIVET ----------" << endl;

      for (unsigned int i = 0; i < fs.particles().size(); i++)
        {

          double eta = fs.particles().at(i).momentum().eta();
          double phi = fs.particles().at(i).momentum().phi();
          int Id = fs.particles().at(i).pdgId();
	
          // electron selection
          if (Id == ELECTRON && fabs(eta) < 2.4)
            { 
	      ele_photons.clear();
	      //cout << "=========== RIVET =============" << endl;
              FourMomentum ele_p(fs.particles().at(i).momentum());
              //ho un elettrone e lo inserisco come primo elemento del vettore ele_photons che conterra' l'elettrone e a seguire i suoi fotoni
              ele_photons.push_back(i);
              //cerco i fotoni con cui rivestire l'elettrone
              for (unsigned int j = 0; j < photons.size(); j++)
                {
                  double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
                  double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();
		
		  double delta_phi = fabs(phi - phi_photon);

                  if (delta_phi > acos(-1))
                        delta_phi = 2*acos(-1) - delta_phi;

                  double deltaR = sqrt((eta - eta_photon) * (eta - eta_photon) + delta_phi * delta_phi);
		  //cout << "delatR = " << deltaR << endl;
                  if (deltaR < 0.1)
                    {
		      //cout << "deltaR RIVET sopra = " << deltaR << endl;
                      ele_photons.push_back(photons.at(j));
                      //aggiungo il momento del fotone a quello dell'elettrone
                      ele_p += fs.particles().at(photons.at(j)).momentum();
                    }
                }
              // ho rivestito l'elettrone, il suo nuovo momento ele_p e l'elettrone con i suoi fotoni sono contenuti in ele_photons

              //se il primo elettrone che rivesto e ha un pT>20 lo inserisco nella struttura in cui conservo il momento di e rivestito e il vettore di e con i suo gamma
              if (ele_dres.lepton_photon.empty() && ele_p.pT() > 20)
                {
                  ele_dres.p_part = ele_p;
                  ele_dres.lepton_photon = ele_photons;
                }
              else
                {
		  //cout << "nuovo positrone rivet = " << ele_p.pT() << "   positrone gia preso rivet = " << ele_dres.p_part.pT() << endl;
                  if (ele_p.pT() > ele_dres.p_part.pT() && ele_p.pT() > 20)
                    {
                      ele_dres.p_part = ele_p;
                      ele_dres.lepton_photon = ele_photons;
                    }
                }

            }

          // positron selection
          if (Id == POSITRON && fabs(eta) < 2.4) {
	      pos_photons.clear();
              FourMomentum pos_p(fs.particles().at(i).momentum());
              pos_photons.push_back(i);
              for (unsigned int j = 0; j < photons.size(); j++) {
                  double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
                  double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();

		  double delta_phi = fabs(phi - phi_photon);

                  if (delta_phi > acos(-1))
                        delta_phi = 2*acos(-1) - delta_phi;

                  double deltaR = sqrt((eta - eta_photon) * (eta - eta_photon) + delta_phi * delta_phi);
                  if (deltaR < 0.1) {
                      pos_photons.push_back(photons.at(j));
                      pos_p += fs.particles().at(photons.at(j)).momentum();		      
                  }
              }

              if (pos_dres.lepton_photon.empty() && pos_p.pT() > 20) {
                  pos_dres.p_part = pos_p;
                  pos_dres.lepton_photon = pos_photons;
              } else {

                  if (pos_p.pT() > pos_dres.p_part.pT() && pos_p.pT() > 20) {
                      pos_dres.p_part = pos_p;
                      pos_dres.lepton_photon = pos_photons;
                  }

                }

          }

          // muon selection
          if (Id == MUON && fabs(eta) < 2.4) {
	      muon_photons.clear();
              FourMomentum muon_p(fs.particles().at(i).momentum());
              muon_photons.push_back(i);
              for (unsigned int j = 0; j < photons.size(); j++) {
                  double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
                  double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();

		  double delta_phi = fabs(phi - phi_photon);

                  if (delta_phi > acos(-1))
                        delta_phi = 2*acos(-1) - delta_phi;

                  double deltaR = sqrt((eta - eta_photon) * (eta - eta_photon) + delta_phi * delta_phi);
                  if (deltaR < 0.1) {
                      muon_photons.push_back(photons.at(j));
                      muon_p += fs.particles().at(photons.at(j)).momentum();
                  }
              }


              if (muon_dres.lepton_photon.empty() && muon_p.pT() > 20) {
                  muon_dres.p_part = muon_p;
                  muon_dres.lepton_photon = muon_photons;
              } else {
                  if (muon_p.pT() > muon_dres.p_part.pT() && muon_p.pT() > 20) {
                      muon_dres.p_part = muon_p;
                      muon_dres.lepton_photon = muon_photons;
                  }
                }

          }

          // antimuon selection
          if (Id == ANTIMUON && fabs(eta) < 2.4) {
	      antimuon_photons.clear();
              FourMomentum muon_p(fs.particles().at(i).momentum());
              antimuon_photons.push_back(i);
              for (unsigned int j = 0; j < photons.size(); j++) {
                  double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
                  double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();

                  double delta_phi = fabs(phi - phi_photon);

                  if (delta_phi > acos(-1))
                        delta_phi = 2*acos(-1) - delta_phi;

                  double deltaR = sqrt((eta - eta_photon) * (eta - eta_photon) + delta_phi * delta_phi);
                  if (deltaR < 0.1) {
                      antimuon_photons.push_back(photons.at(j));
                      muon_p += fs.particles().at(photons.at(j)).momentum();
                  }
              }

              if (antimuon_dres.lepton_photon.empty() && muon_p.pT() > 20) {
                  antimuon_dres.p_part = muon_p;
                  antimuon_dres.lepton_photon = antimuon_photons;
              } else {
                  if (muon_p.pT() > antimuon_dres.p_part.pT() && muon_p.pT() > 20) {
                      antimuon_dres.p_part = muon_p;
                      antimuon_dres.lepton_photon = antimuon_photons;
                  }
                }
          }

      }

       // construct the Z candidates momentum
      FourMomentum Z_momentum_ele(add(ele_dres.p_part, pos_dres.p_part));
      FourMomentum Z_momentum_muon(add(muon_dres.p_part, antimuon_dres.p_part));

      if(Z_momentum_ele.mass2()<0) {
        //cout << "Z_mass_ele minore di 0" << endl;
        vetoEvent;
      }

      if(Z_momentum_muon.mass2()<0) {
        //cout << "Z_mass_muon minore di 0" << endl;
        vetoEvent;
      }



      // Z mass window
      if ( (!ele_dres.lepton_photon.empty() && !pos_dres.lepton_photon.empty())) {
	  diele_mass = Z_momentum_ele.mass();
	  diele_pt = Z_momentum_ele.pT();
	  diele_phi = Z_momentum_ele.phi();	  
	  if(diele_mass > 71 && diele_mass < 111)
          	ee_event = true;
      }

      if ( (!muon_dres.lepton_photon.empty() && !antimuon_dres.lepton_photon.empty())) {

	  dimuon_mass = Z_momentum_muon.mass();
          dimuon_pt = Z_momentum_muon.pT();
          dimuon_phi = Z_momentum_muon.phi(); 
          if(dimuon_mass > 71 && dimuon_mass < 111)
          	mm_event = true;
      }

     

      // costruisco un vettore che contiene gli indici delle particelle da cancellare nella lista part_jets
      vector<unsigned int> canc_part;

      if (mm_event) {
          canc_part = muon_dres.lepton_photon;
          for (unsigned int l = 0; l < antimuon_dres.lepton_photon.size(); l++)
            canc_part.push_back(antimuon_dres.lepton_photon.at(l));
      }
      if (ee_event) {
          canc_part = ele_dres.lepton_photon;
          for (unsigned int y = 0; y < pos_dres.lepton_photon.size(); y++)
            canc_part.push_back(pos_dres.lepton_photon.at(y));
      }

      //aggiungo gli indici delle particelle cariche con pT minore di 250 MeV
      if (!low_pt_charged.empty()) {
          for (unsigned int j = 0; j < low_pt_charged.size(); j++)
            canc_part.push_back(low_pt_charged.at(j));
      }
      // aggiungo gli indici dei neutrini
      if (!neutrinos.empty()) {
          for (unsigned int k = 0; k < neutrinos.size(); k++)
            canc_part.push_back(neutrinos.at(k));
      }


      //ordino il vettore in modo crescente     
      stable_sort(canc_part.begin(), canc_part.end());
      //scorro il vettore dal basso e per ogni elemento cancello il corrispondente elemento da part_jets
      for (int i = (canc_part.size() - 1); i >= 0; i--)
        part_jets.erase(part_jets.begin() + canc_part.at(i));

      // part_jets contiene ora tutte le particelle su cui fare la riclusterizzazione 
      for (unsigned int l = 0; l < part_jets.size(); l++)
        {
          FourMomentum p(part_jets.at(l).momentum());
          fastjet::PseudoJet pseudoJet(p.px(), p.py(), p.pz(), p.E());

          pseudoJet.set_user_index(l);
          vecs.push_back(pseudoJet);
        }


      int Nj = 0;

      // jet selection
      double ht = 0;
      vector<fastjet::PseudoJet> jet_list;
      fastjet::ClusterSequence cseq(vecs, fastjet::JetDefinition(fastjet:: antikt_algorithm, 0.5));
      vector<fastjet::PseudoJet> jets = sorted_by_pt(cseq.inclusive_jets(30.0));
      for (unsigned int i = 0; i < jets.size(); i++)
        {
          double etaj = jets[i].eta();
          if (fabs(etaj) < 2.5 && jets[i].perp() > 30)
            {

	      Nj++;
              jet_list.push_back(jets[i]);
              ht = ht + jets[i].perp();
            }
        }

	// b-jets
        vector<fastjet::PseudoJet> vect_bjets;

	bool bjet_found = false;
        unsigned int Nb=0;

	for(unsigned int k = 0; k<jet_list.size() ; k++) {

		bjet_found = false;

		vector<fastjet::PseudoJet> constituents = cseq.constituents(jet_list[k]);
		
		for (unsigned int c = 0; c < constituents.size() && !bjet_found; c++) {
			int index = constituents.at(c).user_index();
			const Particle & part = part_jets.at(index);
			const PdgId pid = part.pdgId();
			HepMC::GenVertex* gv = part.genParticle().production_vertex();
			if (gv) {
				foreach (const GenParticle* pi, Rivet::particles(gv, HepMC::ancestors)){
					const PdgId pid = pi->pdg_id();
					if ((abs(pid)/100)%10 == 5 || (abs(pid)/1000)%10 == 5) bjet_found = true;
		        	}

			}

		}

		if(bjet_found) {
			Nb++;
			vect_bjets.push_back(jet_list[k]);		
		}

	}
	      

if (ee_event && jet_list.size()!=0) {
    w_first_ele_pt->fill (ele_dres.p_part.pT(), weight);
    w_second_ele_pt->fill (pos_dres.p_part.pT(), weight);
    w_mass_ee->fill(diele_mass, weight);
    w_pt_Z_ee->fill(diele_pt, weight);
    double delta_phi_ee = fabs(diele_phi - jet_list[0].phi());
    if (delta_phi_ee > acos (-1)) delta_phi_ee = 2 * acos (-1) - delta_phi_ee;
    w_delta_ee->fill(delta_phi_ee, weight);
  }

if (mm_event && jet_list.size()!=0) { 
    w_first_muon_pt->fill (muon_dres.p_part.pT(), weight);
    //w_second_muon_pt->fill (antimu_dres.p_part, weight);
    w_mass_mm->fill(dimuon_mass, weight);
    w_pt_Z_mm->fill(dimuon_pt, weight);
    double delta_phi_mm = fabs(dimuon_phi - jet_list[0].phi());
    if (delta_phi_mm > acos (-1)) delta_phi_mm = 2 * acos (-1) - delta_phi_mm;
    w_delta_mm->fill(delta_phi_mm, weight);
  }

if ((ee_event || mm_event) && jet_list.size()!=0) {
    w_first_jet_pt->fill (jet_list[0].pt(), weight);
    w_first_jet_eta->fill (jet_list[0].eta(), weight);
    w_jetmultiplicity->fill (jet_list.size(), weight);
    w_Ht->fill (ht, weight);
  }

if (mm_event && vect_bjets.size()!=0 && jet_list.size()!=0) {
    w_first_bjet_pt->fill (vect_bjets[0].pt(), weight);
    w_first_bjet_eta->fill (vect_bjets[0].eta(), weight);
    w_first_jet_pt_b->fill (jet_list[0].pt(), weight);
    w_first_jet_eta_b->fill (jet_list[0].eta(), weight);
    w_pt_Z_mm_b->fill (dimuon_pt, weight);
    w_mass_mm_b->fill (dimuon_mass, weight);
  }

if (ee_event && vect_bjets.size()!=0 && jet_list.size()!=0) {
    w_first_bjet_pt->fill (vect_bjets[0].pt(), weight);
    w_first_bjet_eta->fill (vect_bjets[0].eta(), weight);
    w_first_jet_pt_b->fill (jet_list[0].pt(), weight);
    w_first_jet_eta_b->fill (jet_list[0].eta(), weight);
    w_pt_Z_ee_b->fill (diele_pt, weight);
    w_mass_ee_b->fill (diele_mass, weight);
  }

if ((ee_event || mm_event) && vect_bjets.size()!=0 &&  jet_list.size()!=0) {
    w_bjetmultiplicity->fill (Nb, weight);
    w_Ht_b->fill (ht, weight);
  }

if (ee_event && jet_list.size()!=0 && vect_bjets.size()!=0) {
    double delta_phi_ee_b = fabs(diele_phi - vect_bjets[0].phi());
    if (delta_phi_ee_b > acos (-1)) delta_phi_ee_b = 2 * acos (-1) - delta_phi_ee_b;
    w_delta_ee_b->fill(delta_phi_ee_b, weight);
  }

if (mm_event && jet_list.size()!=0 && vect_bjets.size()!=0) {
    double delta_phi_mm_b = fabs(dimuon_phi - vect_bjets[0].phi());
    if (delta_phi_mm_b > acos (-1)) delta_phi_mm_b = 2 * acos (-1) - delta_phi_mm_b;
    w_delta_mm_b->fill(delta_phi_mm_b, weight);
  }
}

    /// Normalise histograms etc., after the run
    void finalize()
    {

      /// @todo Normalise, scale and otherwise manipulate histograms here
      // scale(_h_YYYY, crossSection()/sumOfWeights()); # norm to cross section
      // normalize(_h_YYYY); # normalize to unity

      cout << endl;
      //cout << "numero di eventi Z+jet = " << num_Zjet  << endl;
      //cout << "numero di eventi Z+bjet = " << num_ZBjet  << endl;
      cout << "num_weights_tot=" << num_weights_tot << endl;
      cout << "sum_weights_tot=" << sum_weights_tot << endl;
      cout << "sum_weights2_tot=" << sum_weights2_tot << endl;
      double mean_weight_tot = 0.;
      if (num_weights_tot != 0)
        mean_weight_tot = sum_weights_tot/(double) num_weights_tot;
      cout << "mean_weight_tot=" << mean_weight_tot << endl;
      cout << endl;

    }

  private:

    // Data members like post-cuts event weight counters go here


  private:


    //int num_ZBjet;
    //int num_Zjet;
    unsigned int num_weights_tot;
    double sum_weights_tot;
    double sum_weights2_tot;

    /// @name Histograms

    //AIDA::IHistogram1D* h_gen_weights;
    //AIDA::IHistogram1D* h_jetmultiplicity;
    //AIDA::IHistogram1D* h_jet_pt;
    //AIDA::IHistogram1D* h_ele_pt;
    //AIDA::IHistogram1D* h_muon_pt;
    AIDA::IHistogram1D* w_pt_Z_ee;
    AIDA::IHistogram1D* w_pt_Z_mm;
    AIDA::IHistogram1D* w_pt_Z_ee_b;
    AIDA::IHistogram1D* w_pt_Z_mm_b;
    AIDA::IHistogram1D* w_jetmultiplicity;
    AIDA::IHistogram1D* w_first_jet_pt;
    AIDA::IHistogram1D* w_first_jet_eta;
    AIDA::IHistogram1D* w_first_bjet_pt;
    AIDA::IHistogram1D* w_first_bjet_eta;
    AIDA::IHistogram1D* w_bjetmultiplicity;
    AIDA::IHistogram1D* w_first_jet_pt_b;
    AIDA::IHistogram1D* w_first_jet_eta_b;
    AIDA::IHistogram1D* w_first_ele_pt;
    AIDA::IHistogram1D* w_second_ele_pt;
    //AIDA::IHistogram1D* w_dressed_ele_pt;
    AIDA::IHistogram1D* w_first_muon_pt;
    AIDA::IHistogram1D* w_mass_ee;
    AIDA::IHistogram1D* w_mass_mm;
    AIDA::IHistogram1D* w_mass_ee_b;
    AIDA::IHistogram1D* w_mass_mm_b;
    AIDA::IHistogram1D* w_delta_ee_b;
    AIDA::IHistogram1D* w_delta_mm_b;
    AIDA::IHistogram1D* w_delta_ee;
    AIDA::IHistogram1D* w_delta_mm;
    AIDA::IHistogram1D* w_Ht;
    AIDA::IHistogram1D* w_Ht_b;

  };

  // This global object acts as a hook for the plugin system
  AnalysisBuilder<CMS_EWK_13_XXX> plugin_CMS_EWK_13_XXX;

}
