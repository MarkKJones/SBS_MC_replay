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

void mkj_bbcal(TString basename="") {
   TString inputroot;
   inputroot="../../ReplayedFiles/"+basename+".root";
TFile *fsimc = new TFile(inputroot); 
TTree *tsimc = (TTree*) fsimc->Get("T");
 Double_t ps_Elem[52];
 Double_t ps_aInt[52];
 Double_t ps_aTime[52];
 Double_t sh_Elem[189];
 Double_t sh_aInt[189];
 Double_t sh_aTime[189];
 Int_t NumPS;
 Int_t NumSH;
 tsimc->SetBranchAddress("Ndata.bb.ps.a_time",&NumPS) ;
 tsimc->SetBranchAddress("Ndata.bb.sh.a_time",&NumSH) ;
 tsimc->SetBranchAddress("bb.ps.a_time",&ps_aTime) ;
 tsimc->SetBranchAddress("bb.ps.a_c",&ps_aInt) ;
 tsimc->SetBranchAddress("bb.sh.a_time",&sh_aTime) ;
 tsimc->SetBranchAddress("bb.sh.a_c",&sh_aInt) ;
 TH1F *h_psNum = new TH1F("h_psNum"," ; PS # of Blocks Hit ; Counts",10,0,10);
 TH1F *h_shNum = new TH1F("h_shNum"," ; SH # of Blocks Hit ; Counts",20,0,20);
 TH1F *h_shATime = new TH1F("h_shAtime"," ; SH Time ; Counts",400,0,200);
 TH1F *h_psATime = new TH1F("h_psAtime"," ; PS Time ; Counts",400,0,200);
 Long64_t nentries = tsimc->GetEntries();
	for (int i = 0; i < nentries; i++) {
      		tsimc->GetEntry(i);		
                if (i%1000==0) cout << " Entry = " << i << endl;
		//h_ps_samps->Clear();
		Int_t numps = 0;
		Int_t numsh = 0;
		for (Int_t n=0;n<NumPS;n++) {
		  if (ps_aTime[n] > 0) numps++;
		  h_psATime->Fill(ps_aTime[n]);
		}
		for (Int_t n=0;n<NumSH;n++) {
		  if (sh_aTime[n] > 0) numsh++;
		  h_shATime->Fill(sh_aTime[n]);
		}
		h_psNum->Fill(float(numps));
		h_shNum->Fill(float(numsh));
	}  //
 TCanvas *canhit = new TCanvas("canhit","Hits", 700,700);
 canhit->Divide(1,2);
		canhit->cd(1);
		h_psNum->Draw();
		canhit->cd(2);
		h_shNum->Draw();
}
