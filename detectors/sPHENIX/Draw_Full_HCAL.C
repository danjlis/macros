#include "TROOT.h"
#include "TMath.h"
#include "TF1.h"
#include "TH1.h"

int Draw_Full_HCAL(){

  int maptochannel[48] = {2,0,6,4,10,8,14,12,18,16,22,26,24,30,28,34,38,36,42,40,46,44,3,1,7,5,11,9,15,13,19,17,23,21,27,25,31,29,35,33,39,37,43,41,47,45};
  int binsize = 100;
  int binsize_low = 30;
  TFile *f = new TFile("./hcal_output/hcal_trigger_4GeV_Full_All.root");
  TTree *T = (TTree *)f->Get("T_1");
  int sim_n;
  float sim_E[48];
  float sim_eta[48];
  float sim_phi[48];
  int e_thresh;
  int event_num;

  T->SetBranchAddress("event_num", &event_num);
  T->SetBranchAddress("E_thresh", &e_thresh);
  T->SetBranchAddress("tower_sim_n", &sim_n);
  T->SetBranchAddress("tower_sim_E", sim_E);
  T->SetBranchAddress("tower_sim_eta", sim_eta);
  T->SetBranchAddress("tower_sim_phi",sim_phi);

  TH1D *h_edep_tile[24][4];
  TH1D *h_edep_phi[64][4];
  TH1D *h_edep_tower[24][64];
  
  for (int i = 0; i < 24; i++){
    for (int j = 0; j < 64; j++){
      for (int k = 0; k < 4; k++){
	string eta_name = "eta";
	string phi_name = "phi";
	char eta_num[10];
	char phi_num[10];
	sprintf(eta_num,"%d", i);
	sprintf(phi_num,"%d", j);
	string title_tower =  eta_name + eta_num + phi_name + phi_num;
	string title_tile = eta_name + eta_num;
	string title_phi = phi_name + phi_num;
	//      std::cout<<title<<std::endl;
	TString name_tower = Form("h_edep_tower_eta%d_phi%d",i, j);
	int bins;
	if (k ==0) bins = binsize;
	else bins = binsize_low;
	if (k ==0) h_edep_tower[i][j] = new TH1D(name_tower, title_tower.c_str(), bins, 0, 0.04);
	
	if (j == 0) {
	  TString name_tile = Form("h_edep_tile_eta%d_%d",i,k);
	  h_edep_tile[i][k] = new TH1D(name_tile, title_tile.c_str(), bins, 0, 0.04);
	}    
	if (i == 0){
	  TString name_phi = Form("h_edep_phi_phi%d_%d",j,k);
	  h_edep_phi[j][k] = new TH1D(name_phi, title_phi.c_str(), bins, 0, 0.04);
	}
      }
    }
  }
  
  int debug = 0;
  float e_cut = 0.002;
  float veto_cut = 0.002;
  int pass_count = 0;
  int entries = T->GetEntries();

  float deta = 2.1/24.;
  float dphi = TMath::Pi()/32;
  int last_event = -1;
  for (int i = 0; i <entries; i++){
    if (i%10000==0){
      std::cout<<"------------------------------------------"<<std::endl;
      std::cout<<"Event "<<i<<" with "<<sim_n<< " entries..."<<std::endl;
    }    
    T->GetEntry(i);
    
    for (int j = 0; j < sim_n; j++){
      
      if (sim_E[j] > e_cut){
	
	int ebin = 12 + (int)floor(sim_eta[j]*12/1.1);
	int pbin = 64 - (int) floor((sim_phi[j] + TMath::Pi())/dphi);
	
	if (last_event == event_num) h_edep_tower[ebin][pbin]->Fill(sim_E[j]);
	h_edep_tile[ebin][e_thresh]->Fill(sim_E[j]);
	h_edep_phi[pbin][e_thresh]->Fill(sim_E[j]);
      }
    }
    last_event = event_num;
  }  
  TFile *outf = new TFile("./hcal_output/sim_hists_4GeV_Full.root","recreate");
  for (int i = 0; i < 24; i++){
    /*    std::vector<double> x,y;
	  int n = 0;
	  int nb = h_edep_tower[i]->GetNbinsX();
	  for (int j = 0; j < nb;j++){
	  double xv = h_edep_tower[i]->GetXaxis()->GetBinCenter(j);
	  double yv = h_edep_tower[i]->GetBinContent(j);
	  if (yv > 0){
	  x.push_back(xv);
	  y.push_back(yv);
	  n++;
	  }
	  }
	  int size = x.size();
	  double xa[size], ya[size];
	  for (int j = 0; j < size; j ++){
	  xa[j] = x.at(j);
	  ya[j] = y.at(j);
	  }
	  TGraph *tg = new TGraph(size,xa,ya);
	  tg->Write();
	  char g_name[50];
	  sprintf(g_name,"g_edep_tower_%d",i);
	  tg->SetName(g_name);
	  tg->Write();
    */
    h_edep_tile[i][0]->Write();
    h_edep_tile[i][1]->Write();
    h_edep_tile[i][2]->Write();
    h_edep_tile[i][3]->Write();

    //for (int j = 0; j < 64; j++){
      //h_edep_tower[i][j]->Write();
      //    }
  }
  for (int i = 0; i < 64; i++){
    /*
      std::vector<double> x,y;
      int n = 0;
      int nb = h_edep_tile[i]->GetNbinsX();
      for (int j = 0; j < nb;j++){
      double xv = h_edep_tile[i]->GetXaxis()->GetBinCenter(j);
      double yv = h_edep_tile[i]->GetBinContent(j);
      if (yv > 0){
      x.push_back(xv);
      y.push_back(yv);
      n++;
      }
      }
      int size = x.size();
      double xa[size], ya[size];
      for (int j = 0; j < size; j ++){
      xa[j] = x.at(j);
      ya[j] = y.at(j);
      }
      TGraph *tg = new TGraph(size,xa,ya);
      tg->Write();
      char g_name[50];
      sprintf(g_name,"g_edep_tile_%d",i);
      tg->SetName(g_name);
      tg->Write();
    */
    h_edep_phi[i][0]->Write();
    h_edep_phi[i][1]->Write();
    h_edep_phi[i][2]->Write();
    h_edep_phi[i][3]->Write();

  }
  
  
  outf->Close();
  
  return 1;
}
