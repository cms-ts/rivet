#include "TClass.h"
#include "TMath.h"

#include <sstream>

TString IdToRivet( Int_t id_ ) {
      
  TString idN;
  ostringstream convert;
  convert << id_;
  idN = convert.str();
  
  TString hName = "d"+idN+"-x01-y01";
  if ( id_ < 10 ) { hName = "d0"+idN+"-x01-y01"; }
      
  return hName;

}

Int_t findFirstSD( Double_t input ) {

  Int_t Iposi = 0;
  if ( input == 0. ) { return Iposi; }

  Int_t i = 0;

  if ( fabs(input) > 1. ) { 
    do{
      i++;
      Iposi = i;
      
    } while (fabs(input) / pow(10, i ) > 1. );  
  } else {
    do{
      i--;
      Iposi = i;
      
    } while (fabs(input) / pow(10, i ) < 1. );  
  }

  return Iposi;

}

Int_t definePrecision( Double_t unc , Int_t nSD ) {

  Int_t iprec = 0;
  Int_t iposi = findFirstSD(unc);
  if ( nSD-iposi > 0 ) {
    if ( iposi < 0 ) { iprec = -iposi+(nSD-1); }
    if ( iposi > 0 ) { iprec = nSD-iposi; }
  } 

  return iprec;

}


Double_t closest( Double_t original ) {

  Double_t New;
  
  Double_t Inte = (int)original;
  Double_t Deci = 10.*(original-Inte);

  //  std::cout << "input = " << original << std::endl;

  if ( 100.*(Deci-(int)Deci) != 99. ) { return original; }

  New = (Deci < 0 ? ceil(Deci - 0.5) : floor(Deci + 0.5)) ;

  //  std::cout << "input = " << original << " output = " << Inte+New/10. << std::endl;

  return Inte+New/10.;

}

TString requali( Int_t DistributionId ) {

  TString requa;

  switch ( DistributionId ) {
  case 1:
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 1 B JET"; break;
  case 2:  
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 1 B JET"; break;
  case 3:
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 1 B JET"; break;
  case 4:
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 1 B JET"; break;
  case 5:
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 1 B JET"; break;
  case 6:
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 1 B JET"; break;
  case 7:  
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 1 B JET"; break;    
  case 8:
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 1 B JET"; break;
  case 9:
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 1 B JET"; break;
  case 10:
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 1 B JET"; break;
  case 11:
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 2 B JETS"; break;
  case 12:  
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 2 B JETS"; break;
  case 13:
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 2 B JETS"; break;
  case 14:  
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 2 B JETS"; break;
  case 15:  
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 2 B JETS"; break;
  case 16:  
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 2 B JETS"; break;
  case 17:  
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 2 B JETS"; break;
  case 18:  
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 2 B JETS"; break;
  case 19:  
    requa = "P P --> Z0 < LEPTON+ LEPTON- > + >= 2 B JETS"; break;
  default:
    requa = "Dummy";
  }

  return requa;

}

TString hTitle( Int_t DistributionId ) {

  TString htit_;

  switch ( DistributionId ) {
  case 1:
    htit_ = "Title=CMS, Leading b jet transverse momentum, at least one b jet" ; break;
  case 2:
    htit_ = "Title=CMS, Leading b/inclusive jet transverse momentum ratio, at least one b jet over inclusive" ; break;
  case 3:  
    htit_ = "Title=CMS, Leading b jet absolute pseudorapidity, at least one b jet" ; break;
  case 4:  
    htit_ = "Title=CMS, Leading b/inclusive jet absolute pseudorapidity ratio, at least one b jet over inclusive" ; break;    
  case 5:
    htit_ = "Title=CMS, Z boson pt, at least one b jet" ; break;
  case 6:
    htit_ = "Title=CMS, Z boson pt ratio, at least one b jet over inclusive" ; break;
  case 7:
    htit_ = "Title=CMS, HT, at least one b jet" ; break;
  case 8:
    htit_ = "Title=CMS, HT ratio, at least one b jet over inclusive" ; break;
  case 9:
    htit_ = "Title=CMS, DeltaPhi_Zb, at least one b jet" ; break;
  case 10:
    htit_ = "Title=CMS, DeltaPhi_Z(b/j) ratio, at least one b jet over inclusive" ; break;
  case 11:
    htit_ = "Title=CMS, Leading b jet transverse momentum, at least two b jets" ; break;
  case 12:  
    htit_ = "Title=CMS, Subleading b jet transverse momentum, at least two b jets" ; break;
  case 13:
    htit_ = "Title=CMS, Z boson pt, at least two b jets" ; break;
  case 14:  
    htit_ = "Title=CMS, bb system mass, at least two b jets" ; break;
  case 15:  
    htit_ = "Title=CMS, Zbb system mass, at least two b jets" ; break;
  case 16:  
    htit_ = "Title=CMS, DeltaPhi bb, at least two b jets" ; break;
  case 17:  
    htit_ = "Title=CMS, DeltaR bb, at least two b jets" ; break;
  case 18:  
    htit_ = "Title=CMS, DeltaR Zb minimum, at least two b jets" ; break;
  case 19:  
    htit_ = "Title=CMS, Asymmetry DR Zb max DR Zb min , at least two b jets" ; break;
  default:
    htit_ = "Dummy";
  }

  return htit_;

}

TString xlabel( Int_t DistributionId ) {

  TString xname;

  switch ( DistributionId ) {
  case 1:
    xname = "PT(b) "; break;
  case 2:
    xname = "PT(b/j)"; break;
  case 3:  
    xname = "ETA(b)"; break;
  case 4:  
    xname = "ETA(b/j)"; break;
  case 5:
    xname = "PT(Z)"; break;
  case 6:
    xname = "PT(Z)"; break;
  case 7:
    xname = "HT"; break;
  case 8:
    xname = "HT"; break;
  case 9:
    xname = "DPHI(Zb)"; break;
  case 10:
    xname = "DPHI(Zb/j)"; break;
  case 11:
    xname = "PT(b)"; break;
  case 12:  
    xname = "PT(2nd b)"; break;
  case 13:
    xname = "PT(Z)"; break;
  case 14:  
    xname = "M(bb)"; break;
  case 15:  
    xname = "M(Zbb)"; break;
  case 16:  
    xname = "DPHI(bb)"; break;
  case 17:  
    xname = "DR(bb)"; break;
  case 18:  
    xname = "MIN DR(Zb)"; break;
  case 19:  
    xname = "AZB(MAX DR(Zb) MIN DR(Zb))"; break;
  default:
    xname = "Dummy";
  }

  return xname;

}

TString xunits( Int_t DistributionId ) {

  TString xunit;

  switch ( DistributionId ) {
  case 1:
    xunit = "GEV"; break;
  case 2:
    xunit = "GEV"; break;
  case 3:  
    xunit = ""; break;
  case 4:  
    xunit = ""; break;
  case 5:
    xunit = "GEV"; break;
  case 6:
    xunit = "GEV"; break;
  case 7:
    xunit = "GEV"; break;
  case 8:
    xunit = "GEV"; break;
  case 9:
    xunit = "RAD"; break;
  case 10:
    xunit = ""; break;
  case 11:
    xunit = "GEV"; break;
  case 12:  
    xunit = "GEV"; break;
  case 13:
    xunit = "GEV"; break;
  case 14:  
    xunit = "GEV"; break;
  case 15:  
    xunit = "GEV"; break;
  case 16:  
    xunit = "RAD"; break;
  case 17:  
    xunit = ""; break;
  case 18:  
    xunit = ""; break;
  case 19:  
    xunit = ""; break;
  default:
    xunit = "Dummy";
  }

  return xunit;

}

TString ylabel( Int_t DistributionId ) {

  TString yname;

  switch ( DistributionId ) {
  case 1:
    yname = "D(SIG(fiducial))/DPT(b)"; break;
  case 2:
    yname = "B JET/INCLUSIVE JETS D(SIG(fiducial))/DPT(b/j) RATIO"; break;
  case 3:  
    yname = "D(SIG(fiducial))/DETA(b)"; break;
  case 4:  
    yname = "B JET/INCLUSIVE JETS D(SIG(fiducial))/DETA(b/j) RATIO"; break;
  case 5:
    yname = "D(SIG(fiducial))/DPT(Z)"; break;
  case 6:
    yname = "B JET/INCLUSIVE JETS D(SIG(fiducial))/DPT(Z) RATIO"; break;
  case 7:
    yname = "D(SIG(fiducial))/DHT"; break;
  case 8:
    yname = "B JET/INCLUSIVE JETS D(SIG(fiducial))/DHT RATIO"; break;
  case 9:
    yname = "D(SIG(fiducial))/DPHI(Zb)"; break;
  case 10:
    yname = "B JET/INCLUSIVE JETS D(SIG(fiducial))/DPHI(Zb/j) RATIO"; break;
  case 11:
    yname = "D(SIG(fiducial))/DPT(b)"; break;
  case 12:  
    yname = "D(SIG(fiducial))/DPT(2nd b)"; break;
  case 13:
    yname = "D(SIG(fiducial))/DPT(Z)"; break;
  case 14:  
    yname = "D(SIG(fiducial))/DM(bb)"; break;
  case 15:  
    yname = "D(SIG(fiducial))/DM(Zbb)"; break;
  case 16:  
    yname = "D(SIG(fiducial))/DPHI(bb)"; break;
  case 17:  
    yname = "D(SIG(fiducial))/DR(bb)"; break;
  case 18:  
    yname = "D(SIG(fiducial))/D(MIN DR(Zb))"; break;
  case 19:  
    yname = "D(SIG(fiducial))/DAZB"; break;
  default:
    yname = "Dummy";
  }

  return yname;

}

TString yunits( Int_t DistributionId ) {

  TString yunit;

  switch ( DistributionId ) {
  case 1:
    yunit = "PB/GEV"; break;
  case 2:
    yunit = "%"; break;
  case 3:  
    yunit = "PB"; break;
  case 4:  
    yunit = "%"; break;
  case 5:
    yunit = "PB/GEV"; break;
  case 6:
    yunit = "%"; break;
  case 7:
    yunit = "PB/GEV"; break;
  case 8:
    yunit = "%"; break;
  case 9:
    yunit = "PB/RAD"; break;
  case 10:
    yunit = "%"; break;
  case 11:
    yunit = "PB/GEV"; break;
  case 12:  
    yunit = "PB/GEV"; break;
  case 13:
    yunit = "PB/GEV"; break;
  case 14:  
    yunit = "PB/GEV"; break;
  case 15:  
    yunit = "PB/GEV"; break;
  case 16:  
    yunit = "PB/RAD"; break;
  case 17:  
    yunit = "PB"; break;
  case 18:  
    yunit = "PB"; break;
  case 19:  
    yunit = "PB"; break;
  default:
    yunit = "Dummy";
  }

  return yunit;

}

void HistoToHepdata( Int_t DistributionId = 1,
                     Int_t Mode = 1,
                     TString inputfile = "w_first_bjet_pt_xsecs_unfolding.root",
                     TString theDir = "", 
                     TString theHistoTot = "w_first_bjet_pt_data_tot;1",
                     TString theHistoStat = "w_first_bjet_pt_data_stat;1")
{


  // Mode = 1 : write HepData record
  // Mode = 2 : write Yoda record
  // Mode = 3 : write Plot record

  /* Double_t prova(43.4); */
  /* std::cout << "Good = " << closest(prova) << std::endl; */
  /* prova = 56.799999; */
  /* std::cout << "Bad  = " << closest(prova) << std::endl; */

  /* Double_t prova(1205.73); */
  /* std::cout << "prova = " << prova << " first SD position = " << findFirstSD(prova) << std::endl; */
  /* prova = 0.000567; */
  /* std::cout << "prova = " << prova << " first SD position = " << findFirstSD(prova) << std::endl; */


  gROOT ->Reset();
  const char*  sfilename = inputfile;
  
  TFile * sfile = new TFile(sfilename);
  
  const char* baseDir=theDir;
  
  sfile->cd(baseDir);
  // gDirectory->ls();
  
  TString theNameT,theNameS;
  if ( theDir != "" ) {
    theNameT = theDir+"/"+theHistoTot;
    theNameS = theDir+"/"+theHistoStat;
  } else { 
    theNameT = theHistoTot;
    theNameS = theHistoStat;
  }
  
  TH1 *myHistoT;
  sfile->GetObject(theNameT,myHistoT);
  
  TH1 *myHistoS;
  sfile->GetObject(theNameS,myHistoS);
  
  if ( myHistoT && myHistoS ) { 
    
    TString title = myHistoT->GetTitle();
    
    //    std::cout << title << "\n" << std::endl;
    
    Int_t nbins = myHistoT->GetNbinsX();
    
    std::vector< Double_t > xxlow_;
    std::vector< Double_t > xxhigh_;
    std::vector< Double_t > xxval_;
    std::vector< Double_t > xxhalfwidth_;
    std::vector< Double_t > yyval_;
    std::vector< Double_t > yyErrTot_;
    std::vector< Double_t > yyErrStat_;
    std::vector< Double_t > yyErrSyst_;
    
    for ( Int_t iBin = 1; iBin <= nbins; iBin++ ) {
      
      Double_t xlow = myHistoT->GetBinLowEdge(iBin);
      Double_t xhigh = myHistoT->GetXaxis()->GetBinUpEdge(iBin);
      
      xxlow_.push_back(xlow);
      xxhigh_.push_back(xhigh);
      
      Double_t xval = (xhigh+xlow)*Double_t(0.5);
      Double_t xhalfwidth = (xhigh-xlow)*Double_t(0.5);
      
      xxval_.push_back(xval);
      xxhalfwidth_.push_back(xhalfwidth);

      Double_t yval = myHistoT->GetBinContent(iBin);
      
      yyval_.push_back(yval);
      
      Double_t yerrTotM = myHistoT->GetBinErrorLow(iBin);
      Double_t yerrTotP = myHistoT->GetBinErrorUp(iBin);
      
      if ( yerrTotM != yerrTotP ) { std::cout << "Asymmetric uncertainties" << std::endl; return; }
      
      yyErrTot_.push_back(yerrTotM);
      
      Double_t yerrStatM = myHistoS->GetBinErrorLow(iBin);
      Double_t yerrStatP = myHistoS->GetBinErrorUp(iBin);
      
      if ( yerrStatM != yerrStatP ) { std::cout << "Asymmetric uncertainties" << std::endl; return; }
      
      yyErrStat_.push_back(yerrStatM);
      
      Double_t yerrSystM = TMath::Sqrt(TMath::Max(yerrTotM*yerrTotM - yerrStatM*yerrStatM, 0.));
      Double_t yerrSystP = TMath::Sqrt(TMath::Max(yerrTotP*yerrTotP - yerrStatP*yerrStatP, 0.));
      
      if ( yerrSystM != yerrSystP ) { std::cout << "Asymmetric uncertainties" << std::endl; return; }
      
      yyErrSyst_.push_back(yerrSystM);
      
    }
    
    if ( Mode == 1 ) {

      TString xname = xlabel(DistributionId);
      TString xunit = xunits(DistributionId);

      std::cout << "independent_variables:" << std::endl;
      std::cout << "- header: {name: " << xname << ", units: " << xunit << "}" << std::endl;
      std::cout << "values:" << std::endl;
    
      for ( Int_t iBin = 1; iBin <= nbins; iBin++ ) {
        std::cout << "  - {low: " << std::setw(14) << std::fixed << std::setprecision(2) << xxlow_[iBin-1] 
                  << ", high: "  << std::setw(14) << std::fixed << std::setprecision(2) << xxhigh_[iBin-1] 
                  << "}" << std::endl;
      } 
    
      TString yname = ylabel(DistributionId);
      TString yunit = yunits(DistributionId);
      TString requa = requali(DistributionId);
    
      std::cout << "dependent_variables:" << std::endl;
      std::cout << "- header: {name: " << yname << ", units: " << yunit << "}" << std::endl;
      std::cout << "qualifiers:" << std::endl;
      std::cout << "- {name: RE, value: " << requa << "}" << std::endl;
      std::cout << "- {name: SQRT(S), units: GEV, value: 8000}" << std::endl;
      std::cout << "values:" << std::endl;
    
      for ( Int_t iBin = 1; iBin <= nbins; iBin++ ) {
      
        Int_t precision = definePrecision(yyErrTot_[iBin-1],2);
        //      precision = 6;

        std::cout << " - value:" << std::setw(14) << std::fixed << std::setprecision(precision) << yyval_[iBin-1] << std::endl;
        std::cout << "   errors:" << std::endl;
        std::cout << "   - {symerror:" << std::setw(14) << std::fixed << std::setprecision(precision) << yyErrTot_[iBin-1] << ", label: tot}" << std::endl;
        std::cout << "   - {symerror:" << std::setw(14) << std::fixed << std::setprecision(precision) << yyErrStat_[iBin-1] << ", label: stat}" << std::endl;
        std::cout << "   - {symerror:" << std::setw(14) << std::fixed << std::setprecision(precision) << yyErrSyst_[iBin-1] << ", label: syst}" << std::endl;

      }
      
    } else if ( Mode == 2 ) {

      TString theName = IdToRivet( DistributionId );

      std::cout << "# BEGIN YODA_SCATTER2D /REF/CMS_2017_PAS_SMP_14_010/"+theName << std::endl; 
      std::cout << "Path=/REF/CMS_2017_PAS_SMP_14_010/"+theName << std::endl;
      std::cout << hTitle(DistributionId) << std::endl;
      std::cout << "Type=Scatter2D" << std::endl;

      std::cout << "# xval   xerr-   xerr+   yval   yerr-   yerr+" << std::endl;
    
      for ( Int_t iBin = 1; iBin <= nbins; iBin++ ) {
      
        Int_t precision = definePrecision(yyErrTot_[iBin-1],2);
        // precision = 6;

        std::cout << std::setw(14) << std::fixed << std::setprecision(6) << xxval_[iBin-1]
                  << std::setw(14) << std::fixed << std::setprecision(6) << xxhalfwidth_[iBin-1]
                  << std::setw(14) << std::fixed << std::setprecision(6) << xxhalfwidth_[iBin-1]
                  << std::setw(14) << std::fixed << std::setprecision(precision) << yyval_[iBin-1] 
                  << std::setw(14) << std::fixed << std::setprecision(precision) << yyErrTot_[iBin-1] 
                  << std::setw(14) << std::fixed << std::setprecision(precision) << yyErrTot_[iBin-1] 
                  << std::endl;

      }
      
      std::cout << "# END YODA_SCATTER2D" << std::endl;
      std::cout << "\n" <<std::endl;
      
    } else if ( Mode == 3 ) {

      TString theName = IdToRivet( DistributionId );

      std::cout << "# BEGIN PLOT /CMS_2017_PAS_SMP-14-010/"+theName << std::endl;

      std::cout << hTitle( DistributionId ) << std::endl;

      std::cout << "XLabel="+xlabel( DistributionId )+" "+xunits( DistributionId ) << std::endl;
      std::cout << "YLabel="+ylabel( DistributionId )+" "+yunits( DistributionId ) << std::endl;
      std::cout << "LogY = 1" << std::endl;

      std::cout << "# END PLOT" << std::endl;

    } else {
      std::cout << "Null pointer!" << std::endl;
    } 

  }
  
  return;
 
}
