#!/bin/bash

rm -f CMS_2016_SMP_14_010.yoda 

exec > >(tee -a CMS_2016_SMP_14_010.yoda )

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d01-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d01-x01-y01" 
echo "Title=CMS, Leading b jet transverse momentum, at least one b jet" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding/w_first_bjet_pt_xsecs_unfolding.root","","w_first_bjet_pt_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d02-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d02-x01-y01" 
echo "Title=CMS, Leading b jet absolute pseudorapidity, at least one b jet" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding/w_first_bjet_eta_abs_xsecs_unfolding.root","","w_first_bjet_eta_abs_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d03-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d03-x01-y01" 
echo "Title=CMS, Z boson pt, at least one b jet" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding/w_pt_Z_b_xsecs_unfolding.root","","w_pt_Z_b_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d04-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d04-x01-y01" 
echo "Title=CMS, HT, at least one b jet" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding/w_Ht_b_xsecs_unfolding.root","","w_Ht_b_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d05-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d05-x01-y01" 
echo "Title=CMS, DeltaPhi_Zb, at least one b jet" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding/w_delta_phi_b_xsecs_unfolding.root","","w_delta_phi_b_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 


echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d06-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d06-x01-y01" 
echo "Title=CMS, Leading b/inclusive jet transverse momentum ratio, at least one b jet over inclusive" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/ratios_unfolding/w_first_bjet_pt_ratio_unfolding.root","","w_first_bjet_pt_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d07-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d07-x01-y01" 
echo "Title=CMS, Leading b/inclusive jet absolute pseudorapidity ratio, at least one b jet over inclusive" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/ratios_unfolding/w_first_bjet_eta_abs_ratio_unfolding.root","","w_first_bjet_eta_abs_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d08-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d08-x01-y01" 
echo "Title=CMS, Z boson pt ratio, at least one b jet over inclusive" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/ratios_unfolding/w_pt_Z_b_ratio_unfolding.root","","w_pt_Z_b_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d09-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d09-x01-y01" 
echo "Title=CMS, HT ratio, at least one b jet over inclusive" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/ratios_unfolding/w_Ht_b_ratio_unfolding.root","","w_Ht_b_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d10-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d10-x01-y01" 
echo "Title=CMS, DeltaPhi_Z(b/j) ratio, at least one b jet over inclusive" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/ratios_unfolding/w_delta_phi_b_ratio_unfolding.root","","w_delta_phi_b_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d11-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d11-x01-y01" 
echo "Title=CMS, Leading b jet transverse momentum, at least two b jets" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding_2b/w_first_bjet_pt_xsecs_unfolding.root","","w_first_bjet_pt_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d12-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d12-x01-y01" 
echo "Title=CMS, Subleading b jet transverse momentum, at least two b jets" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding_2b/w_second_bjet_pt_xsecs_unfolding.root","","w_second_bjet_pt_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d13-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d13-x01-y01" 
echo "Title=CMS, Z boson pt, at least two b jets" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding_2b/w_pt_Z_b_xsecs_unfolding.root","","w_pt_Z_b_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d14-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d14-x01-y01" 
echo "Title=CMS, bb system mass, at least two b jets" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding_2b/w_bb_mass_xsecs_unfolding.root","","w_bb_mass_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d15-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d15-x01-y01" 
echo "Title=CMS, Zbb system mass, at least two b jets" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding_2b/w_Zbb_mass_xsecs_unfolding.root","","w_Zbb_mass_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d16-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d16-x01-y01" 
echo "Title=CMS, DeltaPhi bb, at least two b jets" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding_2b/w_delta_phi_2b_xsecs_unfolding.root","","w_delta_phi_2b_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d17-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d17-x01-y01" 
echo "Title=CMS, DeltaR bb, at least two b jets" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding_2b/w_DR_bb_xsecs_unfolding.root","","w_DR_bb_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d18-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d18-x01-y01" 
echo "Title=CMS, DeltaR Zb minimum, at least two b jets" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding_2b/w_DR_Zb_min_xsecs_unfolding.root","","w_DR_Zb_min_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 

echo "# BEGIN YODA_SCATTER2D /REF/CMS_2016_SMP_14_010/d19-x01-y01" 
echo "Path=/REF/CMS_2016_SMP_14_010/d19-x01-y01" 
echo "Title=CMS, Asymmetry DR Zb max DR Zb min , at least two b jets" 
echo "Type=Scatter2D" 

root -l -b -q 'HistoToYoda.C("./Zb/combined/v14/xsecs_unfolding_2b/w_A_Zb_xsecs_unfolding.root","","w_A_Zb_data_tot;1")' | awk 'f;/^Processing/{f=1}' 

echo "# END YODA_SCATTER2D"
echo " " 
