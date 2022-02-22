#include "TROOT.h"
#include "TMath.h"
#include "TF1.h"
#include "TH1.h"

int hcal_sim_data(){

  int maptochannel[48] = {2,0,6,4,10,8,14,12,18,16,22,26,24,30,28,34,38,36,42,40,46,44,3,1,7,5,11,9,15,13,19,17,23,21,27,25,31,29,35,33,39,37,43,41,47,45};
  int binsize = 70;
  TFile *f = new TFile("./hcal_output/hcal_2to10GeV_500k_single.root");
  TTree *T = (TTree *)f->Get("T");
  int sim_n;
  float sim_E[48];
  float sim_eta[48];
  float sim_phi[48];
  
  T->SetBranchAddress("tower_sim_n", &sim_n);
  T->SetBranchAddress("tower_sim_E", sim_E);
  T->SetBranchAddress("tower_sim_eta", sim_eta);
  T->SetBranchAddress("tower_sim_phi",sim_phi);

  TH2F *h_up_down_corr = new TH2F("h_up_down_corr",";Top Tower; Bottom Tower",200,0., 0.04, 200, 0.0, 0.04);
  TH2F *h_up_down_corr_zoom_bot = new TH2F("h_up_down_corr_zoom_bot",";Top Tower; Bottom Tower",200,0.0, 0.04, 50, 0.002, 0.005);
  TH2F *h_up_down_corr_zoom_top = new TH2F("h_up_down_corr_zoom_top",";Bottom Tower; Top Tower",200,0.0, 0.04, 50, 0.002, 0.005);

  TH1D *h_edep_tile[24];
  TH1D* h_edep_tower[48];
  for (int i = 0; i < 48; i++){
    string n_or_s = "";
    string u_or_l = "";
    char int_to_tile[10];
    if ((i<12)||((i>23)&&(i<36))) n_or_s = "S ";
    else if ((i>35)||((i>11)&&(i<24))) n_or_s = "N ";
    if (i<12) {
      sprintf(int_to_tile,"%d ", 32 - i);
    }
    else if ((i>23)&&(i<36)) {
      sprintf(int_to_tile,"%d ",32 - (i-24));
    }
    else if (i>35) {
      sprintf(int_to_tile,"%d ",21 + (i-36));
    }
    else if ((i>11)&&(i<24)) {
      sprintf(int_to_tile,"%d ",21 + (i-12));
    }
    if (i<24) u_or_l = "U";
    else u_or_l = "L";
    string title = n_or_s + int_to_tile + u_or_l;
    string title_tile = int_to_tile + u_or_l;
    std::cout<<title<<std::endl;
    TString name = Form("h_edep_tower_%d",i);
    TString titles = Form("Energy Dep. in Channel %d (Tile %s); edep (GeV); counts", maptochannel[i], title.c_str());
    h_edep_tower[i] = new TH1D(name,titles, binsize, 0, 0.04);
    if (i < 24){
      name = Form("h_edep_tile_%d",i);
      TString tiles = Form("Energy Dep. in Tile %s; edep (GeV); counts", title_tile.c_str());
      h_edep_tile[i] = new TH1D(name, tiles, binsize, 0, 0.04);
    }
  }
  
  int debug = 0;
  float e_cut = 0.002;
  float veto_cut = 0.001;
  int pass_count = 0;
  int entries = T->GetEntries();
  std::vector<float> top_e, bot_e;
  for (int i = 0; i <entries; i++){
    if (i%10000==0){
      std::cout<<"------------------------------------------"<<std::endl;
      std::cout<<"Event "<<i<<" with "<<sim_n<< " entries..."<<std::endl;
    }    
    T->GetEntry(i);
    float keep_eta[2];
    int pass = 0;
    int single = 0;
    float keep_energy[2] = {0};
    float energy_tb[2];
    int ei = 0;
    for (int j = 0; j < sim_n; j++){

      if (pass > 1) break;
      //if (keep_eta == sim_eta[j]) continue;
      if (sim_E[j] > e_cut){
	if (ei == 2) pass = 2;
	else {
	  keep_energy[ei] = sim_E[j];
	  keep_eta[ei] = sim_eta[j];
	  ei++;
	}
      }
      else if (sim_E[j]>veto_cut) pass = 2;
      else continue;
      /*
      if (sim_E[j] < e_cut) continue;
      single = 1;
      if ((debug)&&(i%1000==0)) std::cout<<"Found an energy: "<< j << " / " << sim_E[j] << " / "<<sim_eta[j]<<" / "<<sim_phi[j]<<std::endl;
      for (int k = j+1; k < sim_n; k++){
	if ((sim_eta[j] == sim_eta[k])&&(sim_E[k] > e_cut)){
	  if (pass == 0) {
	    if ((debug)&&(i%1000==0)) std::cout<<"Found a match"<< k<<" / "<<sim_E[k] << " / "<<sim_eta[k]<<" / "<<sim_phi[k]<<std::endl;
	    pass = 1;
	    single = 0;
	    keep_eta = sim_eta[j];
	  } // pass = 0
	  else if (pass == 1) {
	    pass = 2; 
	    if ((debug)&&(i%1000==0)) std::cout<<"Found a second match"<< k << " / " << sim_E[k] << " / "<<sim_eta[k]<<" / "<<sim_phi[k]<<std::endl;
	  } //pass = 1
	} // if eta matches and energy is good
      } // for k
      if (single != 0) {
	pass = 3;
	if ((debug)&&(i%1000==0)) std::cout<<"Found Single!!"<<std::endl;
      }
      */
    } // for j
    if (pass == 2) continue;
    if (keep_eta[0] != keep_eta[1]) continue;
    if (keep_energy[1] == 0) continue;
    pass_count++;
    if ((debug)&&(i%1000==0)) std::cout<<"Found a Good Event: "<< i<<" ! "<<std::endl;
    
    for (int j = 0; j <sim_n; j++){
      if (sim_E[j] < e_cut) continue;
      int bin = 12 + (int)floor(sim_eta[j]*12/1.1);
      int tbin;
      int ul;
      if (sim_phi[j] < .1) {
	bin += 24;
	ul = 0;
      }
      else ul = 1;
      h_edep_tower[bin]->Fill(sim_E[j]);
      if ((bin < 24)&&(bin > 11)) tbin = 23 - bin;
      else if ((bin < 36)&&(bin > 23)) tbin = bin - 12;
      else if (bin >35) tbin = 59 - bin;
      else tbin = bin;
      h_edep_tile[tbin]->Fill(sim_E[j]);
      energy_tb[ul] = sim_E[j];
    }
    h_up_down_corr->Fill(energy_tb[1], energy_tb[0]);
    h_up_down_corr_zoom_bot->Fill(energy_tb[1], energy_tb[0]);
    h_up_down_corr_zoom_top->Fill(energy_tb[0], energy_tb[1]);

  }
  
  TFile *outf = new TFile("./hcal_output/sim_hists_2MeVCut_rGeV.root","recreate");
  for (int i = 0; i < 48; i++){
    std::vector<double> x,y;
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
    h_edep_tower[i]->Write();
  }
  for (int i = 0; i < 24; i++){
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
    h_edep_tile[i]->Write();
  }

  h_up_down_corr->Write();  
  h_up_down_corr_zoom_bot->Write();
  h_up_down_corr_zoom_top->Write();

  outf->Close();
  std::cout<<pass_count<<std::endl;
  return 1;
}
