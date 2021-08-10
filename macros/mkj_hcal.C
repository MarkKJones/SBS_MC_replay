#include <TSystem.h>
#include <TString.h>
#include "TFile.h"
#include "TTree.h"
#include <TNtuple.h>
#include "TCanvas.h"
#include <iostream>
#include <fstream>
#include "TMath.h"
#include "TH1F.h"
#include <TH2.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TROOT.h>
#include <TMath.h>
#include <TLegend.h>
#include <TPaveLabel.h>
#include <TProfile.h>
#include <TPolyLine.h>
#include <TObjArray.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include<math.h>
#include <TRandom3.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompSVD.h>
using namespace std;

void mkj_hcal(TString basename="") {
   TString inputroot;
   inputroot="../../ReplayedFiles/"+basename+".root";
TFile *fsimc = new TFile(inputroot); 
TTree *tsimc = (TTree*) fsimc->Get("T");
   TString outputhist;
   outputhist= "hist/"+basename+"_cal_hist.root";
 TObjArray HList(0);
 Double_t hcal_Elem[288];
 Double_t hcal_tdc[288];
 Double_t hcal_aInt[288];
 Double_t hcal_aAmp[288];
 Double_t hcal_aTime[288];
 Double_t hcal_aElemID[288];
 Double_t hcal_tElemID[288];
 Int_t NumHcal;
 Int_t NumHcalTdc;
 tsimc->SetBranchAddress("Ndata.sbs.hcal.tdc",&NumHcalTdc) ;
 tsimc->SetBranchAddress("sbs.hcal.tdc",&hcal_tdc) ;
 tsimc->SetBranchAddress("Ndata.sbs.hcal.a_time",&NumHcal) ;
 tsimc->SetBranchAddress("sbs.hcal.adcelemID",&hcal_aElemID) ;
 tsimc->SetBranchAddress("sbs.hcal.tdcelemID",&hcal_tElemID) ;
 tsimc->SetBranchAddress("sbs.hcal.a_time",&hcal_aTime) ;
 tsimc->SetBranchAddress("sbs.hcal.a_p",&hcal_aInt) ;
 tsimc->SetBranchAddress("sbs.hcal.a_amp_p",&hcal_aAmp) ;
 TH1F *h_HcalNum = new TH1F("h_HcalNum"," ; Hcal  # of Blocks Hit ; Counts",300,0,300);
 HList.Add(h_HcalNum);
 TH1F *h_HcalTDCNum = new TH1F("h_HcalTDCNum"," ; Hcal TDC # of Blocks Hit ; Counts",300,0,300);
 HList.Add(h_HcalTDCNum);
 TH1F *h_HcalNum_cut = new TH1F("h_HcalNum_cut"," ; Hcal # of Blocks Hit (amp > 20) ; Counts",40,0,40);
 HList.Add(h_HcalNum_cut);
 TH1F *h_HcalNum_all = new TH1F("h_HcalNum_all"," ; Hcal # of Blocks Hit (amp > 5) ; Counts",40,0,40);
 HList.Add(h_HcalNum_all);
 TH1F *h_HcalTime = new TH1F("h_HcalTime"," ; Hcal TDC Time; Counts",400,0,200);
 HList.Add(h_HcalTime);
 TH2F *h_HcalTime_elemID = new TH2F("h_HcalTime_elemID"," ; HCAl elem ID ; Hcal TDC Time; ",300,0,300,400,0,200);
 HList.Add(h_HcalTime_elemID);
 TH1F *h_HcalATime_cut = new TH1F("h_HcalATime_cut"," ; Hcal Time (amp > 20); Counts",400,0,200);
 HList.Add(h_HcalATime_cut);
 TH1F *h_HcalATime_all = new TH1F("h_HcalATime_all"," ; Hcal Time (amp > 5); Counts",400,0,200);
 HList.Add(h_HcalATime_all);
 TH2F *h_HcalATime_tdc = new TH2F("h_HcalATime_tdc"," ; Hcal ADC Time ; TDC time",400,0,200,400,0,50);
 HList.Add(h_HcalATime_tdc);
 TH2F *h_HcalATime_AAmp = new TH2F("h_HcalATime_AAmp"," ; Hcal ADC Time ; ADc AMp",400,0,200,400,0,2000);
 HList.Add(h_HcalATime_AAmp);
 Long64_t nentries = tsimc->GetEntries();
	for (int i = 0; i < nentries; i++) {
      		tsimc->GetEntry(i);		
                if (i%1000==0) cout << " Entry = " << i << endl;
		//h_ps_samps->Clear();
		Int_t numHcal_cut = 0;
		Int_t numHcal_all = 0;
		Int_t MaxAmp=0;
		Int_t MaxAmpIndex=-1;
		for (Int_t n=0;n<NumHcal;n++) {
		  if (hcal_aTime[n] > 0) numHcal_all++;
		  if (hcal_aAmp[n] >= 20 && hcal_aTime[n] > 0) numHcal_cut++;
		  if (hcal_aAmp[n] >= 20) h_HcalATime_cut->Fill(hcal_aTime[n]);
		  if (hcal_aTime[n] > 0) h_HcalATime_all->Fill(hcal_aTime[n]);
		  if (hcal_aAmp[n] >= MaxAmp) {
		    MaxAmpIndex=n;
		    MaxAmp=hcal_aAmp[n];
		  }
		}
		for (Int_t n=0;n<NumHcalTdc;n++) {
		  if (hcal_tdc[n] < 100000 ) {
		    h_HcalTime->Fill(hcal_tdc[n]);
		    h_HcalTime_elemID->Fill(hcal_tElemID[n],hcal_tdc[n]);
		  }
		}
		h_HcalTDCNum->Fill(float(NumHcalTdc));
		h_HcalNum_all->Fill(float(numHcal_all));
		h_HcalNum_cut->Fill(float(numHcal_cut));
	}  //
	//
 TFile hsimc(outputhist,"recreate");
	HList.Write();
}
