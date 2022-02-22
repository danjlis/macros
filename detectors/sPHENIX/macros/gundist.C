#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TVector3.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>

//#include "AtlasUtils.C"
#include "/phenix/u/yuhw/RootMacros/sPHENIXStyle/sPhenixStyle.C"

int gundist() {
  SetsPhenixStyle();
  TFile* f = new TFile("../hcal_test.root");
  TH2F* h_mom = (TH2F*) f->Get("h_momentum_dist");
  TH1D* h_mom_z = h_mom->ProjectionX();
  TH1D* h_mom_x = h_mom->ProjectionY();
  TCanvas* tc = new TCanvas("tc","tc");
  h_mom_x->Draw();
  
  TCanvas* bc = new TCanvas("bc","bc");
  h_mom_z->Draw();
  
  return 1;
}
