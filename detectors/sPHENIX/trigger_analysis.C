// Look at event in phi and eta
//Simulated events on entir OHCAL using Cosmic Spray
double get_eff_emcal(int &pass, std::vector<float> &pphi, std::vector<float> &peta, int &n, float E[1000], float phi[1000], float eta[1000], TH2F *hetaphi, float &e_thresh){
  int n_phi = 0;

  for (int i = 0; i < n; i++){
    float iphi = phi[i];
    float ieta = eta[i];
    float iE = E[i] * 1000; // GeV to MeV
    //if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;

    if (iE > e_thresh){
      pphi.push_back(iphi);
      peta.push_back(ieta);
      n_phi++;
    }
  }

  int pass_n = 0;
  int pass_p = 0;

  double phi_n, phi_p;
  int i_n, j_n, i_p, j_p;

  pass = 0;
  float a1, a2, a3, a4, s1, s2, s3, s4;
  float b1, b2, b3, b4, x1, x2, x3, x4;
  for (int i = 1; i < 24; i++){
    for (int j = 1; j < 32; j++){
      a1 = hetaphi->GetBinContent(i,j);
      a2 = hetaphi->GetBinContent(i + 1,j);
      a3 = hetaphi->GetBinContent(i,j + 1);
      a4 = hetaphi->GetBinContent(i + 1,j + 1);
      s1 = a1 + a2;
      s2 = a1 + a3;
      s3 = a2 + a4;
      s4 = a3 + a4;
      if ((pass_p == 0)&&((s1 > e_thresh)||(s2 > e_thresh)||(s3 > e_thresh)||(s4 > e_thresh))) {
        pass_p++;
        phi_p = (hetaphi->GetYaxis()->GetBinCenter(j) + hetaphi->GetYaxis()->GetBinCenter(j))/2;
        j_p = j;
        i_p = i;
      }
      if (((j > (j_p + 2))||(i > (i_p + 2))||(j < (j_p - 2))||(i < (i_p - 2)))&&(pass_p > 0)&&((s1 > e_thresh)||(s2 > e_thresh)||(s3 > e_thresh)||(s4 > e_thresh))) {
        pass_p++;
      }
      b1 = hetaphi->GetBinContent(i,65-j);
      b2 = hetaphi->GetBinContent(i + 1,65-j);
      b3 = hetaphi->GetBinContent(i,65 - (j + 1));
      b4 = hetaphi->GetBinContent(i + 1,65 - (j + 1));
      x1 = b1 + b2;
      x2 = b1 + b3;
      x3 = b2 + b4;
      x4 = b3 + b4;
      if ((pass_n == 0)&&((x1 > e_thresh)||(x2 > e_thresh)||(x3 > e_thresh)||(x4 > e_thresh))) {
        pass_n++;
        phi_n = (hetaphi->GetYaxis()->GetBinCenter(j) + hetaphi->GetYaxis()->GetBinCenter(j))/2;
        j_n = j;
        i_n = i;
      }
      else if (((j > (j_n + 2))||(i > (i_n + 2))||(j < (j_n - 2))||(i < (i_n - 2)))&&(pass_n > 0)&&((x1 > e_thresh)||(x2 > e_thresh)||(x3 > e_thresh)||(x4 > e_thresh))) {
        pass_n++;
      }
    }
  }
  if ((pass_n == 1)&&(pass_p == 1)) {
    pass = 1;
  }

  if (pass == 0) {
    pass = -1;
    return -99.;
  }

  double phi_t = 2*TMath::Pi() - (phi_p - phi_n);
  double DCA = 224.005*cos(phi_t/2);
  if (DCA < b) {
      pass = 1;
      return phi_p;
  }
  else {
    pass = 0;
    return phi_p;
  }
  return -99.;

}
void get_effs(int &pass, std::vector<float> &pphi, std::vector<float> &peta, int &n, float E[1000], float phi[1000], float eta[1000], float &et){
  int n_phi = 0;
  pass = 1;
  if (n == 0) {
    pass = -1;
    return;
  }

  for (int i = 0; i < n; i++){
    float iphi = phi[i];
    float ieta = eta[i];
    float iE = E[i] * 1000; // GeV to MeV
    //if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;

    if (iE > et){
      pphi.push_back(iphi);
      peta.push_back(ieta);
      n_phi++;
    }
  }
  //std::cout<<n_phi<<std::endl;
  //std:cout<<"Gong through the little bit"<<std::endl;
  if (n_phi < 2) {
    pass = 0;
    return;
  }

  return;
}
void get_effs_a(int &pass, std::vector<float> &pphi, std::vector<float> &peta, int &n, float E[1000], float phi[1000], float eta[1000], float &et){
  int n_phi = 0;
  pass = 1;
  if (n == 0) {
    pass = -1;
    return;
  }

  for (int i = 0; i < n; i++){
    float iphi = phi[i];
    float ieta = eta[i];
    float iE = E[i] * 1000; // GeV to MeV
    //if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;

    if (iE > et){
      pphi.push_back(iphi);
      peta.push_back(ieta);
      n_phi++;
    }
  }
  //std::cout<<n_phi<<std::endl;
  //std:cout<<"Gong through the little bit"<<std::endl;
  if (n_phi < 2) {
    pass = 0;
    return;
  }
  int d = 0;
  for (int i = 0; i < n_phi; i++){
    for (int k = 0; k < n_phi; k++){
      //std::cout<<i<<" "<<k<<std::endl;
      if (pphi.at(i) == pphi.at(k)) continue;
      if (peta.at(i) != peta.at(k)) continue;

      d++;

    }
  }
  if (d < 2) {
    pass = 0;
    return;
  }
  return;
}
void get_effs_b(int &pass, std::vector<float> &pphi, std::vector<float> &peta, int &n, float E[1000], float phi[1000], float eta[1000], float &et){
  int n_phi = 0;
  pass = 1;
  if (n == 0) {
    pass = -1;
    return;
  }

  for (int i = 0; i < n; i++){
    float iphi = phi[i];
    float ieta = eta[i];
    float iE = E[i] * 1000; // GeV to MeV
    //if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;

    if (iE > et){
      pphi.push_back(iphi);
      peta.push_back(ieta);
      n_phi++;
    }
  }
  //std::cout<<n_phi<<std::endl;
  //std:cout<<"Gong through the little bit"<<std::endl;
  if (n_phi < 2) {
    pass = 0;
    return;
  }

  int d = 0;
  for (int i = 0; i < n_phi; i++){
    for (int k = 0; k < n_phi; k++){
      //std::cout<<i<<" "<<k<<std::endl;
      if (pphi.at(i) != (-1*pphi.at(k))) continue;
      //if ((pphi.at(i) > 0)&&(!(( pphi.at(i) < (pphi.at(k) + TMath::Pi() + .1))&&(pphi.at(i) > (pphi.at(k) + TMath::Pi() - .1))))) continue;
      //else if ((pphi.at(i) < 0)&&(!(( pphi.at(i) < (pphi.at(k) - TMath::Pi() + .1))&&(pphi.at(i) > (pphi.at(k) - TMath::Pi() - .1))))) continue;

      if (peta.at(i) != peta.at(k)) continue;

      d++;

    }
  }
  if (d < 2) {
    pass = 0;
    return;
  }
  return;
}

void get_effs_c(int &pass, std::vector<float> &pphi, std::vector<float> &peta, int &n, float E[1000], float phi[1000], float eta[1000], TH2F *hetaphi, float &e_thresh){
  pass = 0;
  int n_phi = 0;

  float a1, a2, a3, a4, s1, s2, s3, s4;

  for (int i = 0; i < n; i++){
    float iphi = phi[i];
    float ieta = eta[i];
    float iE = E[i] * 1000; // GeV to MeV
    //if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;

    if (iE > e_thresh){
      pphi.push_back(iphi);
      peta.push_back(ieta);
      n_phi++;
    }
  }

  for (int i = 1; i < 24; i++){
    for (int j = 1; j < 64; j++){
      a1 = hetaphi->GetBinContent(i,j);
      a2 = hetaphi->GetBinContent(i + 1,j);
      a3 = hetaphi->GetBinContent(i,j + 1);
      a4 = hetaphi->GetBinContent(i + 1,j + 1);
      s1 = a1 + a2;
      s2 = a1 + a3;
      s3 = a2 + a4;
      s4 = a3 + a4;

      if ((s1 > e_thresh)||(s2 > e_thresh)||(s3 > e_thresh)||(s4 > e_thresh)) pass++;
    }
  }
  return pass;
}

void get_effs_d(int &pass, std::vector<float> &pphi, std::vector<float> &peta, int &n, float E[1000], float phi[1000], float eta[1000], TH2F *hetaphi, float &e_thresh){
  int n_phi = 0;

  for (int i = 0; i < n; i++){
    float iphi = phi[i];
    float ieta = eta[i];
    float iE = E[i] * 1000; // GeV to MeV
    //if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;

    if (iE > e_thresh){
      pphi.push_back(iphi);
      peta.push_back(ieta);
      n_phi++;
    }
  }

  int pass_n = 0;
  int pass_p = 0;
  pass = 0;
  float a1, a2, a3, a4, s1, s2, s3, s4;
  float b1, b2, b3, b4, x1, x2, x3, x4;
  for (int i = 1; i < 24; i++){
    for (int j = 1; j < 32; j++){
      a1 = hetaphi->GetBinContent(i,j);
      a2 = hetaphi->GetBinContent(i + 1,j);
      a3 = hetaphi->GetBinContent(i,j + 1);
      a4 = hetaphi->GetBinContent(i + 1,j + 1);
      s1 = a1 + a2;
      s2 = a1 + a3;
      s3 = a2 + a4;
      s4 = a3 + a4;
      if ((pass_p == 0)&&((s1 > e_thresh)||(s2 > e_thresh)||(s3 > e_thresh)||(s4 > e_thresh))) pass_p++;
      b1 = hetaphi->GetBinContent(i,65-j);
      b2 = hetaphi->GetBinContent(i + 1,65-j);
      b3 = hetaphi->GetBinContent(i,65 - (j + 1));
      b4 = hetaphi->GetBinContent(i + 1,65 - (j + 1));
      x1 = b1 + b2;
      x2 = b1 + b3;
      x3 = b2 + b4;
      x4 = b3 + b4;
      if ((pass_n == 0)&&((x1 > e_thresh)||(x2 > e_thresh)||(x3 > e_thresh)||(x4 > e_thresh))) pass_n++;
      if ((pass_n == 1)&&(pass_p == 1)) {
        pass = 1;
        return;
      }
    }
  }
  return;
}
void get_effs_e(int &pass, std::vector<float> &pphi, std::vector<float> &peta, int &n, float E[1000], float phi[1000], float eta[1000], TH2F *hetaphi, float &e_thresh){
  int n_phi = 0;

  int pass_n = 0;
  int pass_p = 0;
  pass = 0;
  float a1, a2, a3, a4, s1, s2, s3, s4;
  float b1, b2, b3, b4, x1, x2, x3, x4;

  for (int i = 0; i < n; i++){
    float iphi = phi[i];
    float ieta = eta[i];
    float iE = E[i] * 1000; // GeV to MeV
    //if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;

    if (iE > e_thresh){
      pphi.push_back(iphi);
      peta.push_back(ieta);
      n_phi++;
    }
  }

  for (int i = 1; i < 24; i++){
    for (int j = 1; j < 32; j++){
      pass_n = 0;
      pass_p = 0;
      a1 = hetaphi->GetBinContent(i,j);
      a2 = hetaphi->GetBinContent(i + 1,j);
      a3 = hetaphi->GetBinContent(i,j + 1);
      a4 = hetaphi->GetBinContent(i + 1,j + 1);
      s1 = a1 + a2;
      s2 = a1 + a3;
      s3 = a2 + a4;
      s4 = a3 + a4;
      if ((s1 > e_thresh)||(s2 > e_thresh)||(s3 > e_thresh)||(s4 > e_thresh)) pass_p++;
      b1 = hetaphi->GetBinContent(i,65 - j);
      b2 = hetaphi->GetBinContent(i + 1,65 - j);
      b3 = hetaphi->GetBinContent(i,65 -(j + 1));
      b4 = hetaphi->GetBinContent(i + 1,65 - (j + 1));
      x1 = b1 + b2;
      x2 = b1 + b3;
      x3 = b2 + b4;
      x4 = b3 + b4;
      if ((x1 > e_thresh)||(x2 > e_thresh)||(x3 > e_thresh)||(x4 > e_thresh)) pass_n++;
      if ((pass_n==1)&&(pass_p == 1)) {
        pass++;
        //std::cout<<"Double up and down"<<std::endl;
        //std::cout<<"i/j: "<<i<<" / "<<j<<std::endl;
        return;
      }
    }
  }
  return;
}

int test_event_c(TH2F *hetaphi, float &e_thresh){
  int pass = 0;
  float a1, a2, a3, a4, s1, s2, s3, s4;
  for (int i = 1; i < 24; i++){
    for (int j = 1; j < 64; j++){
      a1 = hetaphi->GetBinContent(i,j);
      a2 = hetaphi->GetBinContent(i + 1,j);
      a3 = hetaphi->GetBinContent(i,j + 1);
      a4 = hetaphi->GetBinContent(i + 1,j + 1);
      s1 = a1 + a2;
      s2 = a1 + a3;
      s3 = a2 + a4;
      s4 = a3 + a4;

      if ((s1 > e_thresh)||(s2 > e_thresh)||(s3 > e_thresh)||(s4 > e_thresh)) pass++;
    }
  }
  return pass;
}
int test_event_d(TH2F *hetaphi, float &e_thresh){
  int pass_n = 0;
  int pass_p = 0;
  int pass = 0;
  float a1, a2, a3, a4, s1, s2, s3, s4;
  float b1, b2, b3, b4, x1, x2, x3, x4;
  for (int i = 1; i < 24; i++){
    for (int j = 1; j < 32; j++){
      a1 = hetaphi->GetBinContent(i,j);
      a2 = hetaphi->GetBinContent(i + 1,j);
      a3 = hetaphi->GetBinContent(i,j + 1);
      a4 = hetaphi->GetBinContent(i + 1,j + 1);
      s1 = a1 + a2;
      s2 = a1 + a3;
      s3 = a2 + a4;
      s4 = a3 + a4;
      if ((pass_p == 0)&&((s1 > e_thresh)||(s2 > e_thresh)||(s3 > e_thresh)||(s4 > e_thresh))) pass_p++;
      b1 = hetaphi->GetBinContent(i,65-j);
      b2 = hetaphi->GetBinContent(i + 1,65-j);
      b3 = hetaphi->GetBinContent(i,65 - (j + 1));
      b4 = hetaphi->GetBinContent(i + 1,65 - (j + 1));
      x1 = b1 + b2;
      x2 = b1 + b3;
      x3 = b2 + b4;
      x4 = b3 + b4;
      if ((pass_n == 0)&&((x1 > e_thresh)||(x2 > e_thresh)||(x3 > e_thresh)||(x4 > e_thresh))) pass_n++;
      if ((pass_n == 1)&&(pass_p == 1)) {
        pass = 1;
        return pass;
      }
    }
  }
  return pass;
}
int test_event_e(TH2F *hetaphi, float &e_thresh){
  int pass_n = 0;
  int pass_p = 0;
  int pass = 0;
  float a1, a2, a3, a4, s1, s2, s3, s4;
  float b1, b2, b3, b4, x1, x2, x3, x4;

  for (int i = 1; i < 24; i++){
    for (int j = 1; j < 32; j++){
      pass_n = 0;
      pass_p = 0;
      a1 = hetaphi->GetBinContent(i,j);
      a2 = hetaphi->GetBinContent(i + 1,j);
      a3 = hetaphi->GetBinContent(i,j + 1);
      a4 = hetaphi->GetBinContent(i + 1,j + 1);
      s1 = a1 + a2;
      s2 = a1 + a3;
      s3 = a2 + a4;
      s4 = a3 + a4;
      if ((s1 > e_thresh)||(s2 > e_thresh)||(s3 > e_thresh)||(s4 > e_thresh)) pass_p++;
      b1 = hetaphi->GetBinContent(i,65 - j);
      b2 = hetaphi->GetBinContent(i + 1,65 - j);
      b3 = hetaphi->GetBinContent(i,65 -(j + 1));
      b4 = hetaphi->GetBinContent(i + 1,65 - (j + 1));
      x1 = b1 + b2;
      x2 = b1 + b3;
      x3 = b2 + b4;
      x4 = b3 + b4;
      if ((x1 > e_thresh)||(x2 > e_thresh)||(x3 > e_thresh)||(x4 > e_thresh)) pass_n++;
      if ((pass_n==1)&&(pass_p == 1)) {
        pass++;
        //std::cout<<"Double up and down"<<std::endl;
        //std::cout<<"i/j: "<<i<<" / "<<j<<std::endl;
        return pass;
      }
    }
  }
  return pass;
}

double test_event_b(int &n, float E[1000], float phi[1000], float eta[1000], float &et){
  std::vector<float> vec_phi;
  std::vector<float> vec_eta;
  if (n < 2) return -1;

  int n_phi = 0;
  for (int i = 0; i < n; i++){
    float iphi = phi[i];
    float ieta = eta[i];
    float iE = E[i] * 1000; // GeV to MeV
    //if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;

    if (iE > et){
      vec_phi.push_back(iphi);
      vec_eta.push_back(ieta);
      n_phi++;
    }
  }
  //std::cout<<n_phi<<std::endl;
  double dphi = 0.;
  int d = 0;
  //std:cout<<"Gong through the little bit"<<std::endl;
  if (n_phi < 2) return dphi;
  for (int i = 0; i < n_phi; i++){
    for (int k = 0; k < n_phi; k++){
      //std::cout<<i<<" "<<k<<std::endl;
      if (vec_phi.at(i) != (-1*vec_phi.at(k))) continue;
      //if ((vec_phi.at(i) > 0)&&(!(( vec_phi.at(i) < (vec_phi.at(k) + TMath::Pi() + .1))&&(vec_phi.at(i) > (vec_phi.at(k) + TMath::Pi() - .1))))) continue;
      //else if ((vec_phi.at(i) < 0)&&(!(( vec_phi.at(i) < (vec_phi.at(k) - TMath::Pi() + .1))&&(vec_phi.at(i) > (vec_phi.at(k) - TMath::Pi() - .1))))) continue;

      if (vec_eta.at(i) != vec_eta.at(k)) continue;

      dphi += fabs(vec_phi.at(i) - vec_phi.at(k));
      d++;
    }
  }
  if (d < 2) return 0;
  return dphi/d;
}
double test_event_a(int &n, float E[1000], float phi[1000], float eta[1000], float &et){
  std::vector<float> vec_phi;
  std::vector<float> vec_eta;
  if (n < 2) return -1;
  int n_phi = 0;
  for (int i = 0; i < n; i++){
    float iphi = phi[i];
    float ieta = eta[i];
    float iE = E[i] * 1000; // GeV to MeV
    //if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;

    if (iE > et){
      vec_phi.push_back(iphi);
      vec_eta.push_back(ieta);
      n_phi++;
    }
  }
  //std::cout<<n_phi<<std::endl;
  double dphi = 0.;
  int d = 0;
  //std:cout<<"Gong through the little bit"<<std::endl;
  if (n_phi < 2) return dphi;
  for (int i = 0; i < n_phi; i++){
    for (int k = 0; k < n_phi; k++){
      //std::cout<<i<<" "<<k<<std::endl;
      if (vec_phi.at(i) == vec_phi.at(k)) continue;
      if (vec_eta.at(i) != vec_eta.at(k)) continue;

      dphi += fabs(vec_phi.at(i) - vec_phi.at(k));
      d++;
    }
  }
  if (d < 2) return 0;

  return dphi/d;
}
double test_event(int &n, float E[1000], float phi[1000], float eta[1000], float &et){
  std::vector<float> vec_phi;
  int n_phi = 0;
  if (n < 2) return -1;

  for (int i = 0; i < n; i++){
    float iphi = phi[i];
    float ieta = eta[i];
    float iE = E[i] * 1000; // GeV to MeV
    //if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;

    if (iE > et){
      vec_phi.push_back(iphi);
      n_phi++;
    }
  }
  //std::cout<<n_phi<<std::endl;
  double dphi = 0.;
  int d = 0;
  //std:cout<<"Gong through the little bit"<<std::endl;
  if (n_phi < 2) return dphi;
  for (int i = 0; i < n_phi; i++){
    for (int k = 0; k < n_phi; k++){
      //std::cout<<i<<" "<<k<<std::endl;
      if (vec_phi.at(i) == vec_phi.at(k)) continue;
      dphi += fabs(vec_phi.at(i) - vec_phi.at(k));
      d++;
    }
  }
  if (d < 2) return 0;

  return dphi/d;
}
void trigger_analysis(){
  gStyle->SetOptStat(0);
  int use_sim = 0;

  bool debug = true;
  //read file
  TFile *f = new TFile("hcal_4GeV_100k_full.root", "read");
  // write file
  TString namef;
  if (use_sim ==1) namef = "hcal_trigger_4Gev_100k.root";
  else namef = "hcal_trigger_calib_hist.root";

  TFile *f_new = new TFile(namef, "recreate");
  //grab TTree
  TTree *t = (TTree*) f->Get("T");

  //load in the tower info
  float truth_vx, truth_vy, truth_vz, part_px, part_py, part_pz;

  t->SetBranchAddress("truth_vx", &truth_vx);
  t->SetBranchAddress("truth_vy", &truth_vy);
  t->SetBranchAddress("truth_vz", &truth_vz);
  t->SetBranchAddress("part_px", &part_px);
  t->SetBranchAddress("part_py", &part_py);
  t->SetBranchAddress("part_pz", &part_pz);

  int nen = t->GetEntries();

  float tower_E[1000];
  float tower_phi[1000];
  float tower_eta[1000];
  int tower_n;
  if (use_sim == 1){
    t->SetBranchAddress("tower_sim_E", tower_E);
    t->SetBranchAddress("tower_sim_eta", tower_eta);
    t->SetBranchAddress("tower_sim_phi", tower_phi);
    t->SetBranchAddress("tower_sim_n", &tower_n);
  }
  else {
    t->SetBranchAddress("tower_calib_E", tower_E);
    t->SetBranchAddress("tower_calib_eta", tower_eta);
    t->SetBranchAddress("tower_calib_phi", tower_phi);
    t->SetBranchAddress("tower_calib_n", &tower_n);
  }

  //TH2F *h_phi = new TH2F("h_phi", "hs_phi", 10, -3.2, 3.2, 10, -3.2, 3.2);
  //TProfile *h_phi_dphi = new TProfile("h_phi_dphi", "h_phi_dphi", 20, -3.2, 3.2);
  TProfile *h_eff_et;
  TProfile *h_et_dphi;

  TProfile *h_eff_et_a;
  TProfile *h_et_dphi_a;
  TProfile *h_eff_et_b;
  TProfile *h_et_dphi_b;

  TProfile *h_eff_et_c;
  TProfile *h_eff_et_d;
  TProfile *h_eff_et_e;

  if (use_sim == 1){
    h_eff_et = new TProfile("h_eff_et","h_eff_et", 61, -0.25, 30.25);
    h_et_dphi = new TProfile("h_et_dphi","h_et_dphi", 61, -0.25, 30.25);

    h_eff_et_a = new TProfile("h_eff_et_a","h_eff_et", 61, -0.25, 30.25);
    h_et_dphi_a = new TProfile("h_et_dphi_a","h_et_dphi", 61, -0.25, 30.25);
    h_eff_et_b = new TProfile("h_eff_et_b","h_eff_et", 61, -0.25, 30.25);
    h_et_dphi_b = new TProfile("h_et_dphi_b","h_et_dphi", 61, -0.25, 30.25);

    h_eff_et_c = new TProfile("h_eff_et_c","h_eff_et", 121, -0.25, 60.25);
    h_eff_et_d = new TProfile("h_eff_et_d","h_eff_et", 121, -0.25, 60.25);
    h_eff_et_e = new TProfile("h_eff_et_e","h_eff_et", 121, -0.25, 60.25);
  }
  else {
    h_eff_et = new TProfile("h_eff_et","h_eff_et", 51, -0.25, 300.25);
    h_et_dphi = new TProfile("h_et_dphi","h_et_dphi", 51, -0.25, 300.25);

    h_eff_et_a = new TProfile("h_eff_et_a","h_eff_et", 51, -0.25, 300.25);
    h_et_dphi_a = new TProfile("h_et_dphi_a","h_et_dphi", 51, -0.25, 300.25);

    h_eff_et_b = new TProfile("h_eff_et_b","h_eff_et", 51, -0.25, 300.25);
    h_et_dphi_b = new TProfile("h_et_dphi_b","h_et_dphi", 51, -0.25, 300.25);

    h_eff_et_c = new TProfile("h_eff_et_c","h_eff_et", 101, -0.25, 1000.25);
    h_eff_et_d = new TProfile("h_eff_et_d","h_eff_et", 101, -0.25, 1000.25);
    h_eff_et_e = new TProfile("h_eff_et_e","h_eff_et", 101, -0.25, 1000.25);
}


  TProfile *h_eff_phi[3];
  TProfile *h_eff_phi_a[3];
  TProfile *h_eff_phi_b[3];

  TProfile *h_eff_eta[3];
  TProfile *h_eff_eta_a[3];
  TProfile *h_eff_eta_b[3];

  TProfile *h_eff_phi_c[3];
  TProfile *h_eff_phi_d[3];
  TProfile *h_eff_phi_e[3];

  TProfile *h_eff_eta_c[3];
  TProfile *h_eff_eta_d[3];
  TProfile *h_eff_eta_e[3];

  TProfile *h_eff_phi_emcal[3];

  TH2F *hetaphi;

  for (int i = 0; i < 3; i++){
    h_eff_phi[i] = new TProfile(Form("h_eff_phi%d",i),"h_eff_phi", 64, -1*TMath::Pi(), TMath::Pi());
    h_eff_phi_a[i] = new TProfile(Form("h_eff_phi_a%d",i),"h_eff_phi_a", 64, -1*TMath::Pi(), TMath::Pi());
    h_eff_phi_b[i] = new TProfile(Form("h_eff_phi_b%d",i),"h_eff_phi_b", 64, -1*TMath::Pi(), TMath::Pi());

    h_eff_eta[i] = new TProfile(Form("h_eff_eta%d",i),"h_eff_eta", 24, -1.1, 1.1);
    h_eff_eta_a[i] = new TProfile(Form("h_eff_eta_a%d",i),"h_eff_eta_a", 24, -1.1, 1.1);
    h_eff_eta_b[i] = new TProfile(Form("h_eff_eta_b%d",i),"h_eff_eta_b", 24, -1.1, 1.1);

    h_eff_phi_c[i] = new TProfile(Form("h_eff_phi_c%d",i),"h_eff_phi_c", 64, -1*TMath::Pi(), TMath::Pi());
    h_eff_phi_d[i] = new TProfile(Form("h_eff_phi_d%d",i),"h_eff_phi_d", 64, -1*TMath::Pi(), TMath::Pi());
    h_eff_phi_e[i] = new TProfile(Form("h_eff_phi_e%d",i),"h_eff_phi_e", 64, -1*TMath::Pi(), TMath::Pi());

    h_eff_eta_c[i] = new TProfile(Form("h_eff_eta_c%d",i),"h_eff_eta_c", 24, -1.1, 1.1);
    h_eff_eta_d[i] = new TProfile(Form("h_eff_eta_d%d",i),"h_eff_eta_d", 24, -1.1, 1.1);
    h_eff_eta_e[i] = new TProfile(Form("h_eff_eta_e%d",i),"h_eff_eta_e", 24, -1.1, 1.1);

    h_eff_phi_emcal[i] = new TProfile("h_eff_phi_emcal","h_eff_phi_emcal", 64, -1*TMath::Pi(), TMath::Pi());
  }
  // radius of plot.
  //int rad = 264;
  //int irad = 183;

  //float radf = 264.;
  //float iradf = 183.;
  //float twobins[3] = {0, iradf, radf};
  //float phibins[65];
  //for (int i = 0; i<65; i++){
    //phibins[i] = (float) TMath::Pi()*i/32;
  //}
  int ev = 0;
  int end = nen;

  float e_t[3] = {1., 5., 10.};
  float e_tt[3] = {1., 10., 20.};
  if (use_sim == 0) {
    e_t[0] = 100;
    e_t[1] = 300;
    e_t[2] = 700;
    e_tt[0] = 100;
    e_tt[1] = 500;
    e_tt[2] = 1000;
  }
  float thresh;
  int eend;
  if (use_sim) {
    thresh = 0.5;
    eend = 121;
  }
  else {
    eend = 101;
    thresh = 10;
  }

  for (int j = ev; j < end; j++){
    if ((j%100) == 0) std::cout<<"Event "<<j<<std::endl;
    t->GetEntry(j);
    hetaphi = new TH2F(Form("hetaphi_%d",j),"Muon Path; #eta; sin(#phi)",24, -1.1, 1.1, 64, -1*TMath::Pi(), TMath::Pi());
    for ( int i = 0; i < tower_n; i++){
      hetaphi->Fill(tower_eta[i], tower_phi[i], tower_E[i] * 1000);
    }
    for (int i = 0; i < 3; i++){
      std::vector<float> pphi, pphi_a, pphi_b, pphi_c, pphi_d, pphi_e,pphi_em, peta, peta_a, peta_b, peta_c, peta_d, peta_e, peta_em;
      int pass, pass_a, pass_b, pass_c, pass_d, pass_e pass_em;
      get_effs(pass, pphi, peta, tower_n, tower_E, tower_phi, tower_eta, e_t[i]);
      get_effs_a(pass_a, pphi_a, peta_a, tower_n, tower_E, tower_phi, tower_eta, e_t[i]);
      get_effs_b(pass_b, pphi_b, peta_b, tower_n, tower_E, tower_phi, tower_eta, e_t[i]);
      get_effs_c(pass_c, pphi_c, peta_c, tower_n, tower_E, tower_phi, tower_eta,hetaphi, e_tt[i]);
      get_effs_d(pass_d, pphi_d, peta_d, tower_n, tower_E, tower_phi, tower_eta,hetaphi, e_tt[i]);
      get_effs_e(pass_e, pphi_e, peta_e, tower_n, tower_E, tower_phi, tower_eta,hetaphi, e_tt[i]);
      double phi_em = get_effs_em(pass_em, pphi_em, peta_em, tower_n, tower_E, tower_phi, tower_eta,hetaphi, e_tt[i]);


      int ev_size = pphi.size();
      int ev_size_a = pphi_a.size();
      int ev_size_b = pphi_b.size();
      int ev_size_c = pphi_c.size();
      int ev_size_d = pphi_d.size();
      int ev_size_e = pphi_e.size();
      int ev_size_e = pphi_e.size();

      if (pass == 1){
        for (int k = 0; k < ev_size; k ++){
          h_eff_phi[i]->Fill(pphi.at(k), 1, 1);
          h_eff_eta[i]->Fill(peta.at(k), 1, 1);
        }
      }
      else if (pass == 0){
        for (int k = 0; k < ev_size; k ++){
          h_eff_phi[i]->Fill(pphi.at(k), 0, 1);
          h_eff_eta[i]->Fill(peta.at(k), 0, 1);
        }
      }
      if (pass_a == 1){
        for (int k = 0; k < ev_size_a; k ++){
          h_eff_phi_a[i]->Fill(pphi_a.at(k), 1, 1);
          h_eff_eta_a[i]->Fill(peta_a.at(k), 1, 1);
        }
      }
      else if (pass_a == 0){
        for (int k = 0; k < ev_size_a; k ++){
          h_eff_phi_a[i]->Fill(pphi_a.at(k), 0, 1);
          h_eff_eta_a[i]->Fill(peta_a.at(k), 0, 1);
        }
      }
      if (pass_b == 1){
        for (int k = 0; k < ev_size_b; k ++){
          h_eff_phi_b[i]->Fill(pphi_b.at(k), 1, 1);
          h_eff_eta_b[i]->Fill(peta_b.at(k), 1, 1);
        }
      }
      else if (pass_b == 0){
        for (int k = 0; k < ev_size_b; k ++){
          h_eff_phi_b[i]->Fill(pphi_b.at(k), 0, 1);
          h_eff_eta_b[i]->Fill(peta_b.at(k), 0, 1);
        }
      }

      if (pass_c > 0){
        for (int k = 0; k < ev_size_c; k ++){
          h_eff_phi_c[i]->Fill(pphi_c.at(k), 1, 1);
          h_eff_eta_c[i]->Fill(peta_c.at(k), 1, 1);
        }
      }
      else if (pass_c == 0){
        for (int k = 0; k < ev_size_c; k ++){
          h_eff_phi_c[i]->Fill(pphi_c.at(k), 0, 1);
          h_eff_eta_c[i]->Fill(peta_c.at(k), 0, 1);
        }
      }
      if (pass_d == 1){
        for (int k = 0; k < ev_size_d; k ++){
          h_eff_phi_d[i]->Fill(pphi_d.at(k), 1, 1);
          h_eff_eta_d[i]->Fill(peta_d.at(k), 1, 1);
        }
      }
      else if (pass_d == 0){
        for (int k = 0; k < ev_size_d; k ++){
          h_eff_phi_d[i]->Fill(pphi_d.at(k), 0, 1);
          h_eff_eta_d[i]->Fill(peta_d.at(k), 0, 1);
        }
      }
      if (pass_e == 1){
        for (int k = 0; k < ev_size_e; k ++){
          h_eff_phi_e[i]->Fill(pphi_e.at(k), 1, 1);
          h_eff_eta_e[i]->Fill(peta_e.at(k), 1, 1);
        }
      }
      else if (pass_e == 0){
        for (int k = 0; k < ev_size_e; k ++){
          h_eff_phi_e[i]->Fill(pphi_e.at(k), 0, 1);
          h_eff_eta_e[i]->Fill(peta_e.at(k), 0, 1);
        }
      }
      if (pass_em == 1){
        h_eff_phi_emcal[i]->Fill(phi_em, 1, 1);
      }
      else if (pass_em == 0){
        h_eff_phi_emcal[i]->Fill(phi_em, 0, 1);
      }
    }
    for (int i = 0; i < eend; i++) {
      float e_thresh = (float) thresh*i;
      double does_it_go = test_event(tower_n, tower_E, tower_phi, tower_eta, e_thresh);
      double does_it_go_a = test_event_a(tower_n, tower_E, tower_phi, tower_eta, e_thresh);
      double does_it_go_b = test_event_b(tower_n, tower_E, tower_phi, tower_eta, e_thresh);

      int does_it_go_c = test_event_c(hetaphi, e_thresh);
      int does_it_go_d = test_event_d(hetaphi, e_thresh);
      int does_it_go_e = test_event_e(hetaphi, e_thresh);

      if (does_it_go > 0) {
        h_eff_et->Fill(e_thresh, 1, 1);
        h_et_dphi->Fill(e_thresh, does_it_go, 1);
      }
      else if (does_it_go == -1);
      else h_eff_et->Fill(e_thresh, 0, 1);
      if (does_it_go_a > 0) {
        h_eff_et_a->Fill(e_thresh, 1, 1);
        h_et_dphi_a->Fill(e_thresh, does_it_go_a, 1);
      }
      else if (does_it_go_a == -1);
      else h_eff_et_a->Fill(e_thresh, 0, 1);
      if (does_it_go_b > 0) {
        h_eff_et_b->Fill(e_thresh, 1, 1);
        h_et_dphi_b->Fill(e_thresh, does_it_go_b, 1);
      }
      else if (does_it_go_b == -1);
      else h_eff_et_b->Fill(e_thresh, 0, 1);
      if (does_it_go_c > 0) {
        h_eff_et_c->Fill(e_thresh, 1, 1);
      }
      else if (does_it_go == -1);
      else h_eff_et_c->Fill(e_thresh, 0, 1);
      if (does_it_go_d > 0) {
        h_eff_et_d->Fill(e_thresh, 1, 1);
      }
      else if (does_it_go == -1);
      else h_eff_et_d->Fill(e_thresh, 0, 1);
      if (does_it_go_e > 0) {
        h_eff_et_e->Fill(e_thresh, 1, 1);
      }
      else if (does_it_go == -1);
      else h_eff_et_e->Fill(e_thresh, 0, 1);
      //std::cout<<"cde: "<<does_it_go_c<<" / "<<does_it_go_d<<" / "<<does_it_go_e<<std::endl;
    }
  }

  h_eff_et->Write();
  h_et_dphi->Write();
  h_eff_et_a->Write();
  h_et_dphi_a->Write();
  h_eff_et_b->Write();
  h_et_dphi_b->Write();

  h_eff_et_c->Write();
  h_eff_et_d->Write();
  h_eff_et_e->Write();


  for (int i = 0; i < 3; i++){
    h_eff_eta[i]->Write();
    h_eff_phi[i]->Write();
    h_eff_eta_a[i]->Write();
    h_eff_phi_a[i]->Write();
    h_eff_eta_b[i]->Write();
    h_eff_phi_b[i]->Write();
    h_eff_eta_c[i]->Write();
    h_eff_phi_c[i]->Write();
    h_eff_eta_d[i]->Write();
    h_eff_phi_d[i]->Write();
    h_eff_eta_e[i]->Write();
    h_eff_phi_e[i]->Write();
    h_eff_phi_emcal[i]->Write();
  }

  f_new->Close();
  return;
}
