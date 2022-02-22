#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h" 
#include "AtlasUtils.C"
int GunCheck(){

  TFile *f = new TFile("./hcal_output/hcal_test.root");
  TH2D *h_vtx_dist;
  TH2D *h_mom_dist;
  TH1D *h_mom_y;
  h_vtx_dist = (TH2D*)f->Get("h_vertex_dist");
  h_mom_dist = (TH2D*)f->Get("h_momentum_dist");
  h_mom_y = (TH1D*)f->Get("h_momentum_y");
  gStyle->SetOptStat(0);
  double zero = 0.0;
  int centbin_x = h_mom_dist->GetXaxis()->FindBin(zero);
  int centbin_z = h_mom_dist->GetYaxis()->FindBin(zero);
  TH1D *h_cent_x = h_mom_dist->ProjectionX("h_cent_x", centbin_x, centbin_x+1);
  TH1D *h_cent_z = h_mom_dist->ProjectionY("h_cent_z", centbin_z, centbin_z+1);
  int int_x = h_cent_x->Integral();
  int int_z = h_cent_z->Integral();

  TF1 *fit = new TF1("fit","[0]*cos(asin(x/4))*cos(asin(x/4))",-4,4);
  fit->SetParName(0, "amplitude");
  
  double amp_guess, arg_guess;
  amp_guess = 200;//int_z*(2/3.14);
  fit->SetParameter(0, amp_guess);
  h_cent_z->Fit("fit", "RI");
  double chi_z = fit->GetChisquare()/fit->GetNDF();
  fit->SetParameter(0, amp_guess);
  h_cent_x->Fit("fit","RI");
  double chi_x = fit->GetChisquare()/fit->GetNDF();
  
  TCanvas *vc = new TCanvas("vc","vc", 800, 600);
  vc->cd();
  TPad *vPad = new TPad("vPad","",0.0, 0.0, .3, 1.0);
  vPad->SetRightMargin(0);
  vPad->Draw();
  TPad *pPad = new TPad("pPad","",0.3, 0.4, 0.7, 1.0);
  pPad->SetRightMargin(.1);
  pPad->SetLeftMargin(.1);
  pPad->SetTopMargin(.15);
  pPad->SetBottomMargin(0);
  pPad->Draw();
  TPad *dPad = new TPad("tPad","",0.3,0.0, 0.7, 0.4);
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
  h_vtx_dist->SetTitle("Vertex Position of Gun; x (cm); z (cm)");
  h_vtx_dist->GetXaxis()->SetLabelSize(10);
  h_vtx_dist->GetXaxis()->SetLabelFont(43);
  //h_vtx_dist->GetXaxis()->SetTitleOffset(2.2);
  h_vtx_dist->GetXaxis()->SetTitleSize(15);
  h_vtx_dist->GetXaxis()->SetTitleFont(43);
  h_vtx_dist->GetYaxis()->SetLabelSize(10);
  h_vtx_dist->GetYaxis()->SetLabelFont(43);
  //h_vtx_dist->GetXaxis()->SetTitleOffset(2.2);                       
  h_vtx_dist->GetYaxis()->SetTitleSize(15);
  h_vtx_dist->GetYaxis()->SetTitleFont(43);
  h_vtx_dist->Draw("col");
  myText(.1, 0.98,kBlack, "Cosmic Spray Vertex Distribution", 0.06);
  
  pPad->cd();
  h_mom_dist->SetTitle("Momentum of Cosmic Spray in the Sector Plane; p_{x} (GeV/c); p_{z} (GeV/c)");
  h_mom_dist->GetXaxis()->SetLabelSize(10);
  h_mom_dist->GetXaxis()->SetLabelFont(43);
  h_mom_dist->GetXaxis()->SetTitleOffset(1.5);          
  h_mom_dist->GetXaxis()->SetTitleSize(15);
  h_mom_dist->GetXaxis()->SetTitleFont(43);
  h_mom_dist->GetYaxis()->SetLabelSize(10);
  h_mom_dist->GetYaxis()->SetLabelFont(43);
  //h_mom_dist->GetXaxis()->SetTitleOffset(2.2);                 
  h_mom_dist->GetYaxis()->SetTitleSize(15);
  h_mom_dist->GetYaxis()->SetTitleFont(43);
  h_mom_dist->Draw("col X+");
  myText(.2, 0.97,kBlack, "Cosmic Spray In-Plane Momentum", 0.04);

  dPad->cd();
  h_cent_x->SetTitle(";p_{i} (GeV/c); Counts");
  h_cent_x->GetXaxis()->SetLabelSize(10);
  h_cent_x->GetXaxis()->SetLabelFont(43);
  h_cent_x->GetXaxis()->SetTitleOffset(1.5);       
  h_cent_x->GetXaxis()->SetTitleSize(15);
  h_cent_x->GetXaxis()->SetTitleFont(43);
  h_cent_x->GetYaxis()->SetLabelSize(10);
  h_cent_x->GetYaxis()->SetLabelFont(43);
  //h_cent_x->GetXaxis()->SetTitleOffset(2.2);  
  h_cent_x->GetYaxis()->SetTitleSize(15);
  h_cent_x->GetYaxis()->SetTitleFont(43);
  h_cent_z->SetMinimum(0);
  h_cent_x->SetLineColor(kGreen - 2);
  h_cent_z->SetLineColor(kBlue - 2);
  fit->SetLineColor(kGreen - 2);
  TF1 *fitz = h_cent_z->GetFunction("fit");
  fitz->SetLineColor(kBlue - 2);
  h_cent_x->Draw("hist");
  h_cent_z->Draw("hist same");
  fit->Draw("same");
  fitz->Draw("same");
  myText(0.1, 0.16,kBlack,"Fit for cos^{2}(#theta) held at +/- 4 GeV/c",0.05);
  myText(0.1, 0.10,kBlack, "#theta = arcsin(p_{i}/4 GeV)",0.05);
  TLegend *tl = new TLegend(0.33,0.25, 0.67,0.48);
  tl->AddEntry("h_cent_x",Form("p_{x,z=0} #chi^{2}: %.2f", chi_x));
  tl->AddEntry("h_cent_z",Form("p_{z,x=0} #chi^{2}: %.2f", chi_z));
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(0);
  gStyle->SetLegendFont(43);
  gStyle->SetLegendTextSize(0.05);
  tl->Draw();
  yPad->cd();
  h_mom_y->SetTitle("Momentum of CosmicSpray in y (vertical) Direction; p_{y} (GeV/c); Counts");
  h_mom_y->GetXaxis()->SetLabelSize(10);
  h_mom_y->GetXaxis()->SetLabelFont(43);
  //h_mom_y->GetXaxis()->SetTitleOffset(2.2);  
  h_mom_y->GetXaxis()->SetTitleSize(15);
  h_mom_y->GetXaxis()->SetTitleFont(43);
  h_mom_y->GetYaxis()->SetLabelSize(10);
  h_mom_y->GetYaxis()->SetLabelFont(43);
  h_mom_y->GetYaxis()->SetTitleOffset(.16);    
  h_mom_y->GetYaxis()->SetTitleSize(15);
  h_mom_y->GetYaxis()->SetTitleFont(43);
  h_mom_y->SetLineColor(kGreen-2);
  h_mom_y->SetFillColorAlpha(kGreen-2, .3);
  h_mom_y->Draw("hist Y+");
  myText(.02, 0.98,kBlack, "Cosmic Spray Vertical Momentum", 0.06);

  vc->SaveAs("./guncheck.pdf");

  return 0;

}
