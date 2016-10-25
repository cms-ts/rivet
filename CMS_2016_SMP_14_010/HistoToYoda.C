#include "TClass.h"

void HistoToYoda( TString inputfile = "w_first_bjet_pt_xsecs_unfolding.root",
                  TString theDir = "", 
                  TString theHisto = "w_first_bjet_pt_data_tot;1")
{

 gROOT ->Reset();
 const char*  sfilename = inputfile;

 TFile * sfile = new TFile(sfilename);

 const char* baseDir=theDir;

 sfile->cd(baseDir);
 // gDirectory->ls();

 TString theName;
 if ( theDir != "" ) {
   theName = theDir+"/"+theHisto;
 } else { 
   theName = theHisto;
 }

 TH1 *myHisto;
 sfile->GetObject(theName,myHisto);

 if ( myHisto) { 

   TString title = myHisto->GetTitle();

   std::cout << "# xval   xerr-   xerr+   yval   yerr-   yerr+" << std::endl;
   
   Int_t nbins = myHisto->GetNbinsX();
   
   for ( Int_t iBin = 1; iBin <= nbins; iBin++ ) {
     
     Double_t xlow = myHisto->GetBinLowEdge(iBin);
     Double_t xhigh = myHisto->GetXaxis()->GetBinUpEdge(iBin);

     Double_t xval = (xhigh+xlow)*Double_t(0.5);
     Double_t xhalfwidth = (xhigh-xlow)*Double_t(0.5);

     Double_t yval = myHisto->GetBinContent(iBin);
     Double_t yerrm = myHisto->GetBinErrorLow(iBin);
     Double_t yerrp = myHisto->GetBinErrorUp(iBin);

     std::cout << std::setw(14) << std::fixed << xval 
               << std::setw(14) << std::fixed << xhalfwidth
               << std::setw(14) << std::fixed << xhalfwidth
               << std::setw(14) << std::fixed << yval 
               << std::setw(14) << std::fixed << yerrm 
               << std::setw(14) << std::fixed << yerrp << std::endl;

   }
 } else {
   std::cout << "Null pointer!" << std::endl;
 } 

 return;

}
