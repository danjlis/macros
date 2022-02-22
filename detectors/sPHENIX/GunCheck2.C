#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h" 
#include "AtlasUtils.C"
int GunCheck2(){

  TFile *f = new TFile("./hcal_output/hcal_RANGE_100k_full.root");
  if (!f) std::cout<<"Could not find the file!!!"<<std::endl;

  TH2D *h_vtx_dist;
  h_vtx_dist = (TH2D*)f->Get("h_vertex_dist");
  //  h_vert_y = (TH1D*)f->Get("h_vertex_y_dist");

  gStyle->SetOptStat(0);
  TTree *T = (TTree*) f->Get("T");
  if (!T) std::cout<<"Could not find the tree!!!"<<std::endl;

  float truth_vx, truth_vy, truth_vz;
  float part_px, part_py, part_pz, part_E;

  T->SetBranchAddress("truth_vx",&truth_vx);
  T->SetBranchAddress("truth_vy",&truth_vy);
  T->SetBranchAddress("truth_vz",&truth_vz);
  T->SetBranchAddress("part_px",&part_px);
  T->SetBranchAddress("part_py",&part_py);
  T->SetBranchAddress("part_pz",&part_pz);
  T->SetBranchAddress("part_E", &part_E);

  double e_min = -1; //GeV in 10^x
  double e_max = 2; //GeV in 10^x
  double ey_min = -2; //GeV in 10^x
  double ey_max = 2; //GeV in 10^x

  const int en = 100;
  const double de = (e_max - e_min)/en;
  const double dey = (ey_max - ey_min)/en;

  double e_list[en];
  double e_bins[en+1];
  double ey_bins[en+1];

  for (int i = 0; i < en; i++){
    e_list[i] = TMath::Power(10, e_min + i*de);
    e_bins[i] = TMath::Power(10, e_min + i*de);
    ey_bins[i] = TMath::Power(10, ey_min + i*dey);
  }
  e_bins[en] = TMath::Power(10, e_max);
  ey_bins[en] = TMath::Power(10, ey_max);

  TH1F *h_energy = new TH1F("h_energy","",en,e_bins); 
  TH1F *h_energy_y = new TH1F("h_energy_y","",en, ey_bins);
  TH2F *h_vertex_xy = new TH2F("h_vertex_xy","",50, -300, 300,50 , -300, 300);
  TH1F *h_vertex_z = new TH1F("h_vertex_z","",50, -310, 310);
  TH2F *h_dir = new TH2F("h_dir","",24, 0, TMath::Pi()*2,24, 0, TMath::Pi()/2);
  TH2F *h_dummy = new TH2F("h_dummy","",1, -1*TMath::Pi()/2,TMath::Pi()/2,1, -1*TMath::Pi()/2,TMath::Pi()/2);

  int nen = T->GetEntries();
  float theta, phi;
  for (int i = 0; i < nen; i++){
    T->GetEntry(i);
    h_energy->Fill(part_E);
    h_energy_y->Fill(-1*part_py);
    h_vertex_xy->Fill(truth_vx,truth_vy);
    h_vertex_z->Fill(truth_vz);
    theta = atan2(sqrt(part_px*part_px+part_pz*part_pz), -1*part_py);
    phi = atan2(part_px, part_pz) + TMath::Pi();
    h_dir->Fill(phi, theta);
  }
  
  TCanvas *vc = new TCanvas("vc","vc", 800, 600);
  vc->cd();
  TPad *vPad = new TPad("vPad","",0.0, 0.0, .3, 1.0);
  vPad->SetRightMargin(0);
  vPad->Draw();
  TPad *pPad = new TPad("pPad","",0.3, 0.5, 0.7, 1.0);
  pPad->SetRightMargin(.1);
  pPad->SetLeftMargin(.1);
  pPad->SetTopMargin(.15);
  pPad->SetBottomMargin(0);
  pPad->Draw();
  TPad *dPad = new TPad("tPad","",0.3,0.0, 0.7, 0.5);
  dPad->SetTopMargin(0);
  //dPad->SetBottomMargin(.1);
  dPad->SetRightMargin(.1);
  dPad->SetLeftMargin(.1);
  dPad->SetBottomMargin(.25);
  dPad->Draw();
  TPad *yPad = new TPad("yPad","", 0.7, 0.0, 1.0, 1.0);
  yPad->SetLeftMargin(0);
  yPad->Draw();

  gStyle->SetTitleFont(43);
  gStyle->SetTitleFontSize(15);
  gStyle->SetPalette(kBlueGreenYellow);
  vPad->cd();
  vPad->SetLogx();
  h_energy->SetTitle("Energy of Gun; E (GeV/c); Counts");
  h_energy->GetXaxis()->SetLabelSize(10);
  h_energy->GetXaxis()->SetLabelFont(43);
  //h_energy->GetXaxis()->SetTitleOffset(2.2);
  h_energy->GetXaxis()->SetTitleSize(15);
  h_energy->GetXaxis()->SetTitleFont(43);
  h_energy->GetYaxis()->SetLabelSize(10);
  h_energy->GetYaxis()->SetLabelFont(43);
  //h_energy->GetXaxis()->SetTitleOffset(2.2);                       
  h_energy->GetYaxis()->SetTitleSize(15);
  h_energy->GetYaxis()->SetTitleFont(43);
  h_energy->Draw("hist");
  myText(.1, 0.98,kBlack, "Cosmic Spray Energy Distribution", 0.06);
  
  pPad->cd();
  h_vertex_xy->SetTitle("Vertex in Azimuthal Plane; v_{x}; v_{y}");
  h_vertex_xy->GetXaxis()->SetLabelSize(10);
  h_vertex_xy->GetXaxis()->SetLabelFont(43);
  h_vertex_xy->GetXaxis()->SetTitleOffset(1.5);          
  h_vertex_xy->GetXaxis()->SetTitleSize(15);
  h_vertex_xy->GetXaxis()->SetTitleFont(43);
  h_vertex_xy->GetYaxis()->SetLabelSize(10);
  h_vertex_xy->GetYaxis()->SetLabelFont(43);
  //h_vertex_xy->GetXaxis()->SetTitleOffset(2.2);                 
  h_vertex_xy->GetYaxis()->SetTitleSize(15);
  h_vertex_xy->GetYaxis()->SetTitleFont(43);
  h_vertex_xy->Draw("col x+");
  myText(.2, 0.97,kBlack, "Azimuthal Vertex Dist", 0.04);

  dPad->cd();
  h_dir->SetTitle("");
  h_dir->GetXaxis()->SetLabelSize(10);
  h_dir->GetXaxis()->SetLabelFont(43);
  h_dir->GetXaxis()->SetTitleOffset(1.5);       
  h_dir->GetXaxis()->SetTitleSize(15);
  h_dir->GetXaxis()->SetTitleFont(43);
  h_dir->GetYaxis()->SetLabelSize(10);
  h_dir->GetYaxis()->SetLabelFont(43);
  //h_dir->GetXaxis()->SetTitleOffset(2.2);  
  h_dir->GetYaxis()->SetTitleSize(15);
  h_dir->GetYaxis()->SetTitleFont(43);
  h_dir->SetLineColor(kGreen - 2);
  h_dummy->Draw();
  h_dir->Draw("pol col same");

  yPad->cd();
  yPad->SetLogx();
  h_energy_y->SetTitle("Momentum of CosmicSpray in y (vertical) Direction; p_{y} (GeV/c); Counts");
  h_energy_y->GetXaxis()->SetLabelSize(10);
  h_energy_y->GetXaxis()->SetLabelFont(43);
  //h_energy_y->GetXaxis()->SetTitleOffset(2.2);  
  h_energy_y->GetXaxis()->SetTitleSize(15);
  h_energy_y->GetXaxis()->SetTitleFont(43);
  h_energy_y->GetYaxis()->SetLabelSize(10);
  h_energy_y->GetYaxis()->SetLabelFont(43);
  h_energy_y->GetYaxis()->SetTitleOffset(.16);    
  h_energy_y->GetYaxis()->SetTitleSize(15);
  h_energy_y->GetYaxis()->SetTitleFont(43);
  h_energy_y->SetLineColor(kGreen-2);
  h_energy_y->SetFillColorAlpha(kGreen-2, .3);
  h_energy_y->Draw("hist Y+");
  myText(.02, 0.98,kBlack, "Cosmic Spray Vertical Momentum", 0.06);

  vc->SaveAs("./guncheck.pdf");

  return 0;

}
