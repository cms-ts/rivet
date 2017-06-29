#!/bin/bash

rm -f CMS_2017_PAS_SMP-14-010.yoda

exec > >(tee -a CMS_2017_PAS_SMP-14-010.yoda )

root -l -b -q 'HistoToHepdata.C(1,2,"./Zb/combined/v14/xsecs_unfolding/w_first_bjet_pt_xsecs_unfolding.root","","w_first_bjet_pt_data_tot;1","w_first_bjet_pt_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(2,2,"./Zb/combined/v14/ratios_unfolding/w_first_bjet_pt_ratio_unfolding.root","","w_first_bjet_pt_data_tot;1","w_first_bjet_pt_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(3,2,"./Zb/combined/v14/xsecs_unfolding/w_first_bjet_eta_abs_xsecs_unfolding.root","","w_first_bjet_eta_abs_data_tot;1","w_first_bjet_eta_abs_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(4,2,"./Zb/combined/v14/ratios_unfolding/w_first_bjet_eta_abs_ratio_unfolding.root","","w_first_bjet_eta_abs_data_tot;1","w_first_bjet_eta_abs_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(5,2,"./Zb/combined/v14/xsecs_unfolding/w_pt_Z_b_xsecs_unfolding.root","","w_pt_Z_b_data_tot;1","w_pt_Z_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(6,2,"./Zb/combined/v14/ratios_unfolding/w_pt_Z_b_ratio_unfolding.root","","w_pt_Z_b_data_tot;1","w_pt_Z_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(7,2,"./Zb/combined/v14/xsecs_unfolding/w_Ht_b_xsecs_unfolding.root","","w_Ht_b_data_tot;1","w_Ht_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(8,2,"./Zb/combined/v14/ratios_unfolding/w_Ht_b_ratio_unfolding.root","","w_Ht_b_data_tot;1","w_Ht_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(9,2,"./Zb/combined/v14/xsecs_unfolding/w_delta_phi_b_xsecs_unfolding.root","","w_delta_phi_b_data_tot;1","w_delta_phi_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(10,2,"./Zb/combined/v14/ratios_unfolding/w_delta_phi_b_ratio_unfolding.root","","w_delta_phi_b_data_tot;1","w_delta_phi_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(11,2,"./Zb/combined/v14/xsecs_unfolding_2b/w_first_bjet_pt_xsecs_unfolding.root","","w_first_bjet_pt_data_tot;1","w_first_bjet_pt_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(12,2,"./Zb/combined/v14/xsecs_unfolding_2b/w_second_bjet_pt_xsecs_unfolding.root","","w_second_bjet_pt_data_tot;1","w_second_bjet_pt_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(13,2,"./Zb/combined/v14/xsecs_unfolding_2b/w_pt_Z_b_xsecs_unfolding.root","","w_pt_Z_b_data_tot;1","w_pt_Z_b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(14,2,"./Zb/combined/v14/xsecs_unfolding_2b/w_bb_mass_xsecs_unfolding.root","","w_bb_mass_data_tot;1","w_bb_mass_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(15,2,"./Zb/combined/v14/xsecs_unfolding_2b/w_Zbb_mass_xsecs_unfolding.root","","w_Zbb_mass_data_tot;1","w_Zbb_mass_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(16,2,"./Zb/combined/v14/xsecs_unfolding_2b/w_delta_phi_2b_xsecs_unfolding.root","","w_delta_phi_2b_data_tot;1","w_delta_phi_2b_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(17,2,"./Zb/combined/v14/xsecs_unfolding_2b/w_DR_bb_xsecs_unfolding.root","","w_DR_bb_data_tot;1","w_DR_bb_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(18,2,"./Zb/combined/v14/xsecs_unfolding_2b/w_DR_Zb_min_xsecs_unfolding.root","","w_DR_Zb_min_data_tot;1","w_DR_Zb_min_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

root -l -b -q 'HistoToHepdata.C(19,2,"./Zb/combined/v14/xsecs_unfolding_2b/w_A_Zb_xsecs_unfolding.root","","w_A_Zb_data_tot;1","w_A_Zb_data_stat;1")' | awk 'f;/^Processing/{f=1}' 

