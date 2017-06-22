#!/bin/bash

rm -f CMS_2016_SMP_14_010.txt 

exec > >(tee -a CMS_2016_SMP_14_010.txt )

echo "Title=CMS, Leading b jet transverse momentum, at least one b jet" 

root -l -b -q 'HistoToHepdata.C(1,"./Zb/combined/v14/xsecs_unfolding/w_first_bjet_pt_xsecs_unfolding.root","","w_first_bjet_pt_data_tot;1","w_first_bjet_pt_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, Leading b jet absolute pseudorapidity, at least one b jet" 

root -l -b -q 'HistoToHepdata.C(2,"./Zb/combined/v14/xsecs_unfolding/w_first_bjet_eta_abs_xsecs_unfolding.root","","w_first_bjet_eta_abs_data_tot;1","w_first_bjet_eta_abs_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, Z boson pt, at least one b jet" 

root -l -b -q 'HistoToHepdata.C(3,"./Zb/combined/v14/xsecs_unfolding/w_pt_Z_b_xsecs_unfolding.root","","w_pt_Z_b_data_tot;1","w_pt_Z_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, HT, at least one b jet" 

root -l -b -q 'HistoToHepdata.C(4,"./Zb/combined/v14/xsecs_unfolding/w_Ht_b_xsecs_unfolding.root","","w_Ht_b_data_tot;1","w_Ht_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, DeltaPhi_Zb, at least one b jet" 

root -l -b -q 'HistoToHepdata.C(5,"./Zb/combined/v14/xsecs_unfolding/w_delta_phi_b_xsecs_unfolding.root","","w_delta_phi_b_data_tot;1","w_delta_phi_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 


echo "Title=CMS, Leading b/inclusive jet transverse momentum ratio, at least one b jet over inclusive" 

root -l -b -q 'HistoToHepdata.C(6,"./Zb/combined/v14/ratios_unfolding/w_first_bjet_pt_ratio_unfolding.root","","w_first_bjet_pt_data_tot;1","w_first_bjet_pt_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, Leading b/inclusive jet absolute pseudorapidity ratio, at least one b jet over inclusive" 

root -l -b -q 'HistoToHepdata.C(7,"./Zb/combined/v14/ratios_unfolding/w_first_bjet_eta_abs_ratio_unfolding.root","","w_first_bjet_eta_abs_data_tot;1","w_first_bjet_eta_abs_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, Z boson pt ratio, at least one b jet over inclusive" 

root -l -b -q 'HistoToHepdata.C(8,"./Zb/combined/v14/ratios_unfolding/w_pt_Z_b_ratio_unfolding.root","","w_pt_Z_b_data_tot;1","w_pt_Z_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, HT ratio, at least one b jet over inclusive" 

root -l -b -q 'HistoToHepdata.C(9,"./Zb/combined/v14/ratios_unfolding/w_Ht_b_ratio_unfolding.root","","w_Ht_b_data_tot;1","w_Ht_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, DeltaPhi_Z(b/j) ratio, at least one b jet over inclusive" 

root -l -b -q 'HistoToHepdata.C(10,"./Zb/combined/v14/ratios_unfolding/w_delta_phi_b_ratio_unfolding.root","","w_delta_phi_b_data_tot;1","w_delta_phi_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, Leading b jet transverse momentum, at least two b jets" 

root -l -b -q 'HistoToHepdata.C(11,"./Zb/combined/v14/xsecs_unfolding_2b/w_first_bjet_pt_xsecs_unfolding.root","","w_first_bjet_pt_data_tot;1","w_first_bjet_pt_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, Subleading b jet transverse momentum, at least two b jets" 

root -l -b -q 'HistoToHepdata.C(12,"./Zb/combined/v14/xsecs_unfolding_2b/w_second_bjet_pt_xsecs_unfolding.root","","w_second_bjet_pt_data_tot;1","w_second_bjet_pt_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, Z boson pt, at least two b jets" 

root -l -b -q 'HistoToHepdata.C(13,"./Zb/combined/v14/xsecs_unfolding_2b/w_pt_Z_b_xsecs_unfolding.root","","w_pt_Z_b_data_tot;1","w_pt_Z_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, bb system mass, at least two b jets" 

root -l -b -q 'HistoToHepdata.C(14,"./Zb/combined/v14/xsecs_unfolding_2b/w_bb_mass_xsecs_unfolding.root","","w_bb_mass_data_tot;1","w_bb_mass_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, Zbb system mass, at least two b jets" 

root -l -b -q 'HistoToHepdata.C(15,"./Zb/combined/v14/xsecs_unfolding_2b/w_Zbb_mass_xsecs_unfolding.root","","w_Zbb_mass_data_tot;1","w_Zbb_mass_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, DeltaPhi bb, at least two b jets" 

root -l -b -q 'HistoToHepdata.C(16,"./Zb/combined/v14/xsecs_unfolding_2b/w_delta_phi_2b_xsecs_unfolding.root","","w_delta_phi_2b_data_tot;1","w_delta_phi_2b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, DeltaR bb, at least two b jets" 

root -l -b -q 'HistoToHepdata.C(17,"./Zb/combined/v14/xsecs_unfolding_2b/w_DR_bb_xsecs_unfolding.root","","w_DR_bb_data_tot;1","w_DR_bb_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, DeltaR Zb minimum, at least two b jets" 

root -l -b -q 'HistoToHepdata.C(18,"./Zb/combined/v14/xsecs_unfolding_2b/w_DR_Zb_min_xsecs_unfolding.root","","w_DR_Zb_min_data_tot;1","w_DR_Zb_min_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 

echo "Title=CMS, Asymmetry DR Zb max DR Zb min , at least two b jets" 

root -l -b -q 'HistoToHepdata.C(19,"./Zb/combined/v14/xsecs_unfolding_2b/w_A_Zb_xsecs_unfolding.root","","w_A_Zb_data_tot;1","w_A_Zb_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

echo " " 
