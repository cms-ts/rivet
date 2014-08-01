#include "Rivet/Analysis.hh"
#include "Rivet/RivetAIDA.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/InvMassFinalState.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Math/Vector4.hh"

//#define DebugLog 

namespace Rivet
{

  class CMS_SMP_12_017:public Analysis
  {
  public:

    /// Constructor
    CMS_SMP_12_017():Analysis("CMS_SMP_12_017")
    {
      setNeedsCrossSection(true);
    }

  public:

    /// Book histograms and initialise projections before the run
    void init()
    {

#ifdef DebugLog
      // set optionally the verbosity for the internal Rivet message system
      getLog().setLevel(0);
#endif

      const FinalState fs;

      addProjection(fs, "FS");

      SumW2 = 0.;

      _h_excmult_jets_ele = bookHistogram1D(1, 1, 1);
      _h_incmult_jets_ele = bookHistogram1D(2, 1, 1);
      _h_leading_jet_pt_ele = bookHistogram1D(3, 1, 1);
      _h_second_jet_pt_ele = bookHistogram1D(4, 1, 1);
      _h_third_jet_pt_ele = bookHistogram1D(5, 1, 1);
      _h_fourth_jet_pt_ele = bookHistogram1D(6, 1, 1);
      _h_leading_jet_eta_ele = bookHistogram1D(7, 1, 1);
      _h_second_jet_eta_ele = bookHistogram1D(8, 1, 1);
      _h_third_jet_eta_ele = bookHistogram1D(9, 1, 1);
      _h_fourth_jet_eta_ele = bookHistogram1D(10, 1, 1);
      _h_ht1_ele = bookHistogram1D(11, 1, 1);
      _h_ht2_ele = bookHistogram1D(12, 1, 1);
      _h_ht3_ele = bookHistogram1D(13, 1, 1);
      _h_ht4_ele = bookHistogram1D(14, 1, 1);

      _h_excmult_jets_muo = bookHistogram1D(15, 1, 1);
      _h_incmult_jets_muo = bookHistogram1D(16, 1, 1);
      _h_leading_jet_pt_muo = bookHistogram1D(17, 1, 1);
      _h_second_jet_pt_muo = bookHistogram1D(18, 1, 1);
      _h_third_jet_pt_muo = bookHistogram1D(19, 1, 1);
      _h_fourth_jet_pt_muo = bookHistogram1D(20, 1, 1);
      _h_leading_jet_eta_muo = bookHistogram1D(21, 1, 1);
      _h_second_jet_eta_muo = bookHistogram1D(22, 1, 1);
      _h_third_jet_eta_muo = bookHistogram1D(23, 1, 1);
      _h_fourth_jet_eta_muo = bookHistogram1D(24, 1, 1);
      _h_ht1_muo = bookHistogram1D(25, 1, 1);
      _h_ht2_muo = bookHistogram1D(26, 1, 1);
      _h_ht3_muo = bookHistogram1D(27, 1, 1);
      _h_ht4_muo = bookHistogram1D(28, 1, 1);

      _h_excmult_jets_tot = bookHistogram1D(29, 1, 1);
      _h_incmult_jets_tot = bookHistogram1D(30, 1, 1);
      _h_leading_jet_pt_tot = bookHistogram1D(31, 1, 1);
      _h_second_jet_pt_tot = bookHistogram1D(32, 1, 1);
      _h_third_jet_pt_tot = bookHistogram1D(33, 1, 1);
      _h_fourth_jet_pt_tot = bookHistogram1D(34, 1, 1);
      _h_leading_jet_eta_tot = bookHistogram1D(35, 1, 1);
      _h_second_jet_eta_tot = bookHistogram1D(36, 1, 1);
      _h_third_jet_eta_tot = bookHistogram1D(37, 1, 1);
      _h_fourth_jet_eta_tot = bookHistogram1D(38, 1, 1);
      _h_ht1_tot = bookHistogram1D(39, 1, 1);
      _h_ht2_tot = bookHistogram1D(40, 1, 1);
      _h_ht3_tot = bookHistogram1D(41, 1, 1);
      _h_ht4_tot = bookHistogram1D(42, 1, 1);

    }

    /// Perform the per-event analysis
    void analyze(const Event & event)
    {

      const double weight = event.weight();
      std::vector<fastjet::PseudoJet> vecs;
      bool Z_ele = false;
      bool Z_muon = false;

      SumW2 += weight * weight;

      vector<unsigned int> ele_photons;
      vector<unsigned int> pos_photons;
      vector<unsigned int> muon_photons;
      vector<unsigned int> antimuon_photons;
      vector<unsigned int> photons;
      vector<unsigned int> neutrinos;

      struct pt_and_particles
      {
        FourMomentum p_part;
        vector<unsigned int>lepton_photon;
      };

      // structure containing the lepton_photon vector index of the lepton candidate daughter of Z and dressed lepton kinematics

      struct pt_and_particles ele_dres;
      struct pt_and_particles pos_dres;
      struct pt_and_particles muon_dres;
      struct pt_and_particles antimuon_dres;

      vector<Particle> part_jets;

      const FinalState & fs = applyProjection< FinalState>(event, "FS");
      fs.particlesByPt();

      // copy of the list of final state partcles
      part_jets = fs.particles();

      for (unsigned int i = 0; i < fs.particles().size(); i++)
        {
          if (fs.particles().at(i).pdgId() == PHOTON)
            photons.push_back(i);
          if (fabs(fs.particles().at(i).pdgId()) == 12 ||
              fabs(fs.particles().at(i).pdgId()) == 14 ||
              fabs(fs.particles().at(i).pdgId()) == 16)
            neutrinos.push_back(i);
        }

      for (unsigned int i = 0; i < fs.particles().size(); i++)
        {

          double eta = fs.particles().at(i).momentum().eta();
          double phi = fs.particles().at(i).momentum().phi();
          int Id = fs.particles().at(i).pdgId();

          // electron selection
          if (Id == ELECTRON && fabs(eta) < 2.4)
            {
              FourMomentum ele_p(fs.particles().at(i).momentum());

              ele_photons.clear();
              ele_photons.push_back(i);

              //lepton dressing

              for (unsigned int j = 0; j < photons.size(); j++)
                {
                  double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
                  double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();
		
                  double deltaR = Rivet::deltaR(eta_photon,phi_photon,eta,phi);

                  if (deltaR < 0.1)
                    {
                      ele_photons.push_back(photons.at(j));
                      ele_p += fs.particles().at(photons.at(j)).momentum();
                    }
                }

              // keep dressed electron if above threshold

              if (ele_dres.lepton_photon.empty() && ele_p.pT() > 20)
                {
                  ele_dres.p_part = ele_p;
                  ele_dres.lepton_photon = ele_photons;
                }
              else
                {
                  if (ele_p.pT() > ele_dres.p_part.pT() && ele_p.pT() > 20)
                    {
                      ele_dres.p_part = ele_p;
                      ele_dres.lepton_photon = ele_photons;
                    }
                }

            }

          // positron selection
          if (Id == POSITRON && fabs(eta) < 2.4)
            {
              FourMomentum pos_p(fs.particles().at(i).momentum());
              pos_photons.clear();
              pos_photons.push_back(i);
              for (unsigned int j = 0; j < photons.size(); j++)
                {
                  double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
                  double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();
		
                  double deltaR = Rivet::deltaR(eta_photon,phi_photon,eta,phi);

                  if (deltaR < 0.1)
                    {
                      pos_photons.push_back(photons.at(j));
                      pos_p += fs.particles().at(photons.at(j)).momentum();
                    }
                }

              if (pos_dres.lepton_photon.empty() && pos_p.pT() > 20)
                {
                  pos_dres.p_part = pos_p;
                  pos_dres.lepton_photon = pos_photons;
                }
              else
                {
                  if (pos_p.pT() > pos_dres.p_part.pT() && pos_p.pT() > 20)
                    {
                      pos_dres.p_part = pos_p;
                      pos_dres.lepton_photon = pos_photons;
                    }

                }

            }

          // muon selection
          if (Id == MUON && fabs(eta) < 2.4)
            {
              FourMomentum muon_p(fs.particles().at(i).momentum());
              muon_photons.clear();
              muon_photons.push_back(i);
              for (unsigned int j = 0; j < photons.size(); j++)
                {
                  double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
                  double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();
		
                  double deltaR = Rivet::deltaR(eta_photon,phi_photon,eta,phi);

                  if (deltaR < 0.1)
                    {
                      muon_photons.push_back(photons.at(j));
                      muon_p += fs.particles().at(photons.at(j)).momentum();
                    }
                }
              

              if (muon_dres.lepton_photon.empty() && muon_p.pT() > 20)
                {
                  muon_dres.p_part = muon_p;
                  muon_dres.lepton_photon = muon_photons;
                }
              else
                {
                  if (muon_p.pT() > muon_dres.p_part.pT()
                      && muon_p.pT() > 20)
                    {
                      muon_dres.p_part = muon_p;
                      muon_dres.lepton_photon = muon_photons;
                    }
                }

            }

          // antimuon selection
          if (Id == ANTIMUON && fabs(eta) < 2.4)
            {
              FourMomentum muon_p(fs.particles().at(i).momentum());
              antimuon_photons.clear();
              antimuon_photons.push_back(i);
              for (unsigned int j = 0; j < photons.size(); j++)
                {
                  double eta_photon = fs.particles().at(photons.at(j)).momentum().eta();
                  double phi_photon = fs.particles().at(photons.at(j)).momentum().phi();

                  double deltaR = Rivet::deltaR(eta_photon,phi_photon,eta,phi);

                  if (deltaR < 0.1)
                    {
                      antimuon_photons.push_back(photons.at(j));
                      muon_p += fs.particles().at(photons.at(j)).momentum();
                    }
                }

              if (antimuon_dres.lepton_photon.empty() && muon_p.pT() > 20)
                {
                  antimuon_dres.p_part = muon_p;
                  antimuon_dres.lepton_photon = antimuon_photons;
                }
              else
                {
                  if (muon_p.pT() > antimuon_dres.p_part.pT()
                      && muon_p.pT() > 20)
                    {
                      antimuon_dres.p_part = muon_p;
                      antimuon_dres.lepton_photon = antimuon_photons;
                    }
                }
            }

        }

      // construct the Z candidates momentum
      FourMomentum Z_momentum_ele(add(ele_dres.p_part, pos_dres.p_part));
      FourMomentum Z_momentum_muon(add(muon_dres.p_part, antimuon_dres.p_part));

      if(Z_momentum_ele.mass2()<0){
        MSG_WARNING("WARNING: negative e+e- invariant mass squared" << Z_momentum_ele.mass2() );
        vetoEvent;
      }

      if(Z_momentum_muon.mass2()<0){
        MSG_WARNING("WARNING: negative mu+mu- invariant mass squared" << Z_momentum_muon.mass2());
        vetoEvent;
      }


      double l1_eta(-999.),l1_phi(-999.),l2_eta(-999.),l2_phi(-999.);

      // Z mass window
      if ((Z_momentum_ele.mass() > 71. && Z_momentum_ele.mass() < 111.) && (!ele_dres.lepton_photon.empty() && !pos_dres.lepton_photon.empty()))
        {
          Z_ele = true;
          l1_eta = ele_dres.p_part.eta();
          l1_phi = ele_dres.p_part.phi();
          l2_eta = pos_dres.p_part.eta();
          l2_phi = pos_dres.p_part.phi();
        }
      if ((Z_momentum_muon.mass() > 71. && Z_momentum_muon.mass() < 111.) && (!muon_dres.lepton_photon.empty() && !antimuon_dres.lepton_photon.empty()))
        {
          Z_muon = true;
          l1_eta = muon_dres.p_part.eta();
          l1_phi = muon_dres.p_part.phi();
          l2_eta = antimuon_dres.p_part.eta();
          l2_phi = antimuon_dres.p_part.phi();
        }

      if (!Z_ele && !Z_muon)
        vetoEvent;

      if (Z_ele && Z_muon)
        vetoEvent;

      // particles to be removed from jets
      vector<unsigned int> canc_part;

      if (Z_muon)
        {
          canc_part = muon_dres.lepton_photon;
          for (unsigned int l = 0; l < antimuon_dres.lepton_photon.size(); l++)
            canc_part.push_back(antimuon_dres.lepton_photon.at(l));
        }
      else
        {
          canc_part = ele_dres.lepton_photon;
          for (unsigned int y = 0; y < pos_dres.lepton_photon.size(); y++)
            canc_part.push_back(pos_dres.lepton_photon.at(y));
        }

      if (!neutrinos.empty())
        {
          for (unsigned int k = 0; k < neutrinos.size(); k++)
            canc_part.push_back(neutrinos.at(k));
        }


      stable_sort(canc_part.begin(), canc_part.end());
      for (int i = (canc_part.size() - 1); i >= 0; i--)
        part_jets.erase(part_jets.begin() + canc_part.at(i));

      // jet clustering

      for (unsigned int l = 0; l < part_jets.size(); l++)
        {
          FourMomentum p(part_jets.at(l).momentum());
          fastjet::PseudoJet pseudoJet(p.px(), p.py(), p.pz(), p.E());

          pseudoJet.set_user_index(l);
          vecs.push_back(pseudoJet);
        }


      // jet selection

      double ht = 0;
      vector<fastjet::PseudoJet> jet_list;
      fastjet::ClusterSequence cseq(vecs, fastjet::JetDefinition(fastjet:: antikt_algorithm, 0.5));
      vector<fastjet::PseudoJet> jets = sorted_by_pt(cseq.inclusive_jets(30.0));
      for (unsigned int i = 0; i < jets.size(); i++)
        {
          double etaj = jets[i].eta();
          double phij = jets[i].phi();
          bool found = false;
          vector<fastjet::PseudoJet> constituents = cseq.constituents(jets[i]);
          for (unsigned int s = 0; s < constituents.size() && !found; s++)
            {
              int index = constituents.at(s).user_index();
              if (PID::threeCharge(part_jets.at(index).pdgId()) != 0);
              found = true;
            }
          if (fabs(etaj) < 2.4 && found && jets[i].perp() > 30)
            {

              double DR1 = Rivet::deltaR(etaj,phij,l1_eta,l1_phi);
              double DR2 = Rivet::deltaR(etaj,phij,l2_eta,l2_phi);
              if ( std::min(DR1,DR2) < 0.5 ) { 
                // drop jet closer than 0.5 from the selected collection
                continue;
                // drop event if at least one jet close than 0.5 in the selected collection
                //                vetoEvent;
              }
              
              jet_list.push_back(jets[i]);
              ht = ht + jets[i].perp();
            }
        }
              
      if (jet_list.size() > 0)
        {
          double leadingjet_pt = jets[0].perp();
          double leadingEta = jets[0].eta();

          _h_leading_jet_pt_tot->fill(leadingjet_pt, weight*0.5);
          _h_leading_jet_eta_tot->fill(std::fabs(leadingEta), weight*0.5);
          _h_ht1_tot->fill(ht, weight*0.5);
          _h_excmult_jets_tot->fill(jet_list.size(), weight*0.5);
          for ( unsigned int iter = 1; iter <= jet_list.size(); iter++ ) {
            _h_incmult_jets_tot->fill(iter, weight*0.5);
          }
          if (Z_ele)
            {
              _h_leading_jet_pt_ele->fill(leadingjet_pt, weight);
              _h_leading_jet_eta_ele->fill(std::fabs(leadingEta), weight);
              _h_excmult_jets_ele->fill(jet_list.size(), weight);
              _h_ht1_ele->fill(ht, weight);
              for ( unsigned int iter = 1; iter <= jet_list.size(); iter++ ) {
                _h_incmult_jets_ele->fill(iter, weight);
              }
            }
          if (Z_muon)
            {
              _h_leading_jet_pt_muo->fill(leadingjet_pt, weight);
              _h_leading_jet_eta_muo->fill(std::fabs(leadingEta), weight);
              _h_excmult_jets_muo->fill(jet_list.size(), weight);
              _h_ht1_muo->fill(ht, weight);
              for ( unsigned int iter = 1; iter <= jet_list.size(); iter++ ) {
                _h_incmult_jets_muo->fill(iter, weight);
              }
            }
        }

      if (jet_list.size() > 1)
        {
          double second_jet_pt = jets[1].perp();
          double second_jet_eta = jets[1].eta();
          _h_second_jet_pt_tot->fill(second_jet_pt, weight*0.5);
          _h_second_jet_eta_tot->fill(std::fabs(second_jet_eta), weight*0.5);
          _h_ht2_tot->fill(ht, weight*0.5);
          if (Z_ele)
            {
              _h_second_jet_pt_ele->fill(second_jet_pt, weight);
              _h_second_jet_eta_ele->fill(std::fabs(second_jet_eta), weight);
              _h_ht2_ele->fill(ht, weight);
            }
          if (Z_muon)
            {
              _h_second_jet_pt_muo->fill(second_jet_pt, weight);
              _h_second_jet_eta_muo->fill(std::fabs(second_jet_eta), weight);
              _h_ht2_muo->fill(ht, weight);
            }
        }
      if (jet_list.size() > 2)
        {
          double third_jet_pt = jets[2].perp();
          double third_jet_eta = jets[2].eta();
          _h_third_jet_pt_tot->fill(third_jet_pt, weight*0.5);
          _h_third_jet_eta_tot->fill(std::fabs(third_jet_eta), weight*0.5);
          _h_ht3_tot->fill(ht, weight*0.5);
          if (Z_ele)
            {
              _h_third_jet_pt_ele->fill(third_jet_pt, weight);
              _h_third_jet_eta_ele->fill(std::fabs(third_jet_eta), weight);
              _h_ht3_ele->fill(ht, weight);
            }
          if (Z_muon)
            {
              _h_third_jet_pt_muo->fill(third_jet_pt, weight);
              _h_third_jet_eta_muo->fill(std::fabs(third_jet_eta), weight);
              _h_ht3_muo->fill(ht, weight);
            }
        }
      if (jet_list.size() > 3)
        {
          double fourth_jet_pt = jets[3].perp();
          double fourth_jet_eta = jets[3].eta();
          _h_fourth_jet_pt_tot->fill(fourth_jet_pt, weight*0.5);
          _h_fourth_jet_eta_tot->fill(std::fabs(fourth_jet_eta), weight*0.5);
          _h_ht4_tot->fill(ht, weight*0.5);
          if (Z_ele)
            {
              _h_fourth_jet_pt_ele->fill(fourth_jet_pt, weight);
              _h_fourth_jet_eta_ele->fill(std::fabs(fourth_jet_eta), weight);
              _h_ht4_ele->fill(ht, weight);
            }
          if (Z_muon)
            {
              _h_fourth_jet_pt_muo->fill(fourth_jet_pt, weight);
              _h_fourth_jet_eta_muo->fill(std::fabs(fourth_jet_eta), weight);
              _h_ht4_muo->fill(ht, weight);
            }
        }

    }

    /// Normalise histograms etc., after the run
    void finalize()
    {
      
      MSG_INFO("Cross section = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(3) << crossSection() << " pb");
      MSG_INFO("# Events      = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(3) << numEvents() );
      MSG_INFO("SumW          = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(3) << sumOfWeights());
      MSG_INFO("SumW2         = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(3) << SumW2);
      double mean_weight(0.);
      if (numEvents() != 0) mean_weight = sumOfWeights()/(double) numEvents();
      MSG_INFO("Mean weight   = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(6) << mean_weight);
      double norm(1.);
      if ( std::fabs(sumOfWeights()) > 0. ) norm = crossSection()/sumOfWeights();
      MSG_INFO("Norm factor   = " << std::setfill(' ') << std::setw(14) << std::fixed << std::setprecision(6) << norm);
      
      scale(_h_excmult_jets_ele, norm );
      scale(_h_incmult_jets_ele, norm );
      scale(_h_leading_jet_pt_ele, norm );
      scale(_h_second_jet_pt_ele, norm );
      scale(_h_third_jet_pt_ele, norm );
      scale(_h_fourth_jet_pt_ele, norm );
      scale(_h_leading_jet_eta_ele, norm );
      scale(_h_second_jet_eta_ele, norm );
      scale(_h_third_jet_eta_ele, norm );
      scale(_h_fourth_jet_eta_ele, norm );
      scale(_h_ht1_ele, norm );
      scale(_h_ht2_ele, norm );
      scale(_h_ht3_ele, norm );
      scale(_h_ht4_ele, norm );

      scale(_h_excmult_jets_muo, norm );
      scale(_h_incmult_jets_muo, norm );
      scale(_h_leading_jet_pt_muo, norm );
      scale(_h_second_jet_pt_muo, norm );
      scale(_h_third_jet_pt_muo, norm );
      scale(_h_fourth_jet_pt_muo, norm );
      scale(_h_leading_jet_eta_muo, norm );
      scale(_h_second_jet_eta_muo, norm );
      scale(_h_third_jet_eta_muo, norm );
      scale(_h_fourth_jet_eta_muo, norm );
      scale(_h_ht1_muo, norm );
      scale(_h_ht2_muo, norm );
      scale(_h_ht3_muo, norm );
      scale(_h_ht4_muo, norm );

      scale(_h_excmult_jets_tot, norm );
      scale(_h_incmult_jets_tot, norm );
      scale(_h_leading_jet_pt_tot, norm );
      scale(_h_second_jet_pt_tot, norm );
      scale(_h_third_jet_pt_tot, norm );
      scale(_h_fourth_jet_pt_tot, norm );
      scale(_h_leading_jet_eta_tot, norm );
      scale(_h_second_jet_eta_tot, norm );
      scale(_h_third_jet_eta_tot, norm );
      scale(_h_fourth_jet_eta_tot, norm );
      scale(_h_ht1_tot, norm );
      scale(_h_ht2_tot, norm );
      scale(_h_ht3_tot, norm );
      scale(_h_ht4_tot, norm );


    }

  private:

    // Data members like post-cuts event weight counters go here

  private:

    double SumW2;

    /// @name Histograms

    AIDA::IHistogram1D* _h_excmult_jets_ele;
    AIDA::IHistogram1D* _h_incmult_jets_ele;
    AIDA::IHistogram1D* _h_leading_jet_pt_ele;
    AIDA::IHistogram1D* _h_second_jet_pt_ele;
    AIDA::IHistogram1D* _h_third_jet_pt_ele;
    AIDA::IHistogram1D* _h_fourth_jet_pt_ele;
    AIDA::IHistogram1D* _h_leading_jet_eta_ele;
    AIDA::IHistogram1D* _h_second_jet_eta_ele;
    AIDA::IHistogram1D* _h_third_jet_eta_ele;
    AIDA::IHistogram1D* _h_fourth_jet_eta_ele;
    AIDA::IHistogram1D* _h_ht1_ele;
    AIDA::IHistogram1D* _h_ht2_ele;
    AIDA::IHistogram1D* _h_ht3_ele;
    AIDA::IHistogram1D* _h_ht4_ele;

    AIDA::IHistogram1D* _h_excmult_jets_muo;
    AIDA::IHistogram1D* _h_incmult_jets_muo;
    AIDA::IHistogram1D* _h_leading_jet_pt_muo;
    AIDA::IHistogram1D* _h_second_jet_pt_muo;
    AIDA::IHistogram1D* _h_third_jet_pt_muo;
    AIDA::IHistogram1D* _h_fourth_jet_pt_muo;
    AIDA::IHistogram1D* _h_leading_jet_eta_muo;
    AIDA::IHistogram1D* _h_second_jet_eta_muo;
    AIDA::IHistogram1D* _h_third_jet_eta_muo;
    AIDA::IHistogram1D* _h_fourth_jet_eta_muo;
    AIDA::IHistogram1D* _h_ht1_muo;
    AIDA::IHistogram1D* _h_ht2_muo;
    AIDA::IHistogram1D* _h_ht3_muo;
    AIDA::IHistogram1D* _h_ht4_muo;

    AIDA::IHistogram1D* _h_excmult_jets_tot;
    AIDA::IHistogram1D* _h_incmult_jets_tot;
    AIDA::IHistogram1D* _h_leading_jet_pt_tot;
    AIDA::IHistogram1D* _h_second_jet_pt_tot;
    AIDA::IHistogram1D* _h_third_jet_pt_tot;
    AIDA::IHistogram1D* _h_fourth_jet_pt_tot;
    AIDA::IHistogram1D* _h_leading_jet_eta_tot;
    AIDA::IHistogram1D* _h_second_jet_eta_tot;
    AIDA::IHistogram1D* _h_third_jet_eta_tot;
    AIDA::IHistogram1D* _h_fourth_jet_eta_tot;
    AIDA::IHistogram1D* _h_ht1_tot;
    AIDA::IHistogram1D* _h_ht2_tot;
    AIDA::IHistogram1D* _h_ht3_tot;
    AIDA::IHistogram1D* _h_ht4_tot;

  };

  // This global object acts as a hook for the plugin system
  AnalysisBuilder<CMS_SMP_12_017> plugin_CMS_SMP_12_017;

}
