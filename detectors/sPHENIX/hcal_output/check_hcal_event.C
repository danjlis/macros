// Look at event in phi and eta
//Simulated events on entir OHCAL using Cosmic Spray
void check_hcal_event(int event = 0, int use_sim=0){
  gStyle->SetOptStat(0);

  //read file
  TFile *f = new TFile("hcal_full.root", "read");
  // write file
  TFile *f_new = new TFile("hcal_lookhere_hist.root", "recreate");
  //grab TTree
  TTree *t = (TTree*) f->Get("T");

  //load in the tower info
  float tower_sim_E[1000];
  float tower_sim_phi[1000];
  float tower_sim_eta[1000];
  int tower_sim_n;
  float tower_calib_E[1000];
  float tower_calib_phi[1000];
  float tower_calib_eta[1000];
  int tower_calib_n;
  float truth_vx, truth_vy, truth_vz, part_px, part_py, part_pz;

  t->SetBranchAddress("tower_sim_E", tower_sim_E);
  t->SetBranchAddress("tower_sim_eta", tower_sim_eta);
  t->SetBranchAddress("tower_sim_phi", tower_sim_phi);
  t->SetBranchAddress("tower_sim_n", &tower_sim_n);
  t->SetBranchAddress("tower_calib_E", tower_calib_E);
  t->SetBranchAddress("tower_calib_eta", tower_calib_eta);
  t->SetBranchAddress("tower_calib_phi", tower_calib_phi);
  t->SetBranchAddress("tower_calib_n", &tower_calib_n);
  t->SetBranchAddress("truth_vx", &truth_vx);
  t->SetBranchAddress("truth_vy", &truth_vy);
  t->SetBranchAddress("truth_vz", &truth_vz);
  t->SetBranchAddress("part_px", &part_px);
  t->SetBranchAddress("part_py", &part_py);
  t->SetBranchAddress("part_pz", &part_pz);

  int nen = t->GetEntries();
  TH2F *heta;
  TH2F *polarPhi;
  TH2F *dummy;
  TCanvas *c1;

  TH2F *h_phi = new TH2F("h_phi", "hs_phi", 10, -3.2, 3.2, 10, -3.2, 3.2);
  TProfile *h_phi_dphi = new TProfile("h_phi_dphi", "h_phi_dphi", 20, -3.2, 3.2);
  // radius of plot.
  int rad = 264;
  int irad = 183;

  float radf = 264.;
  float iradf = 183.;
  float twobins[3] = {0, iradf, radf};
  float phibins[65];
  for (int i = 0; i<65; i++){
    phibins[i] = (float) TMath::Pi()*i/32;
  }
  int ev = event;
  int end = nen;
  if (event != 0) {
    end = event +1;
  }
  for (int j = ev; j < end; j++){
    if ((j%1000) == 0) std::cout<<"Event "<<j<<std::endl;
    heta = new TH2F(Form("heta%d",j),"Muon Path; #eta; sin(#phi)",24, -1.1, 1.1, 64, -1*TMath::Pi(), TMath::Pi());
    polarPhi = new TH2F(Form("polarPhi%d",j), "Event Display", 64, phibins, 2, twobins);
    dummy = new TH2F(Form("dummy%d",j),"", 100, -1*radf, radf, 100, -1*radf, radf);

    t->GetEntry(j);
    for (int i = 0; i < 32; i++){
      int bin = polarPhi->GetBin(i+1,rad);
      polarPhi->SetBinContent(bin, 0);
    }
    if (use_sim == 1){
      int n_phi =0;
      std::vector<float> vec_phi;
      for (int i = 0; i < tower_sim_n; i++){
        float phi = tower_sim_phi[i];
        float nphi = phi;
        if (phi < 0) phi += 2*TMath::Pi();
        float eta = tower_sim_eta[i];
        float E = tower_sim_E[i] * 1000; // GeV to MeV
        if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;
        int phibin = floor(phi/(2*TMath::Pi()/64)) +1;
        int bin = polarPhi->GetBin(phibin,2);
        float E_e = polarPhi->GetBinContent(bin);
        polarPhi->SetBinContent(bin, E+E_e);
        heta->Fill(eta, nphi, E);
        if (E > 10){
          vec_phi.push_back(nphi);
          n_phi++;
        }
      }
      for (int i = 0; i < n_phi; i++){
        for (int k = 0; k < n_phi; k++){
          if (vec_phi.at(i) == vec_phi.at(k)) continue;
          h_phi->Fill(vec_phi.at(i), vec_phi.at(k));
          h_phi_dphi->Fill(vec_phi.at(i), vec_phi.at(i) - vec_phi.at(k));
        }
      }
    }

    else {
      for (int i = 0; i < tower_calib_n; i++){

        float phi = tower_calib_phi[i];
        float nphi = phi;
        if (phi < 0) phi += 2*TMath::Pi();
        float eta = tower_calib_eta[i];
        float E = tower_calib_E[i] * 1000; // GeV to MeV
        if (E < 20) continue;
        if (event != 0) std::cout<<"Energy: "<<E<<" and phi: "<<phi<<std::endl;
        int phibin = floor(phi/(2*TMath::Pi()/64)) +1;
        int bin = polarPhi->GetBin(phibin,2);
        float E_e = polarPhi->GetBinContent(bin);
        polarPhi->SetBinContent(bin, E+E_e);
        heta->Fill(eta, nphi, E);
      }
    }

    TF1 *f1 = new TF1(Form("f1_%d",j),"[1] + ([3]/[2])*(x - [0])",-400, 400);
    f1->FixParameter(0, truth_vx);
    f1->FixParameter(1, truth_vy);
    f1->FixParameter(2, part_px);
    f1->FixParameter(3, part_py);
    if ((j%100)==0){
      f1->Write();
      heta->Write();

      polarPhi->Write();

      dummy->Write();
    }
  }
  h_phi_dphi->Write();
  h_phi->Write();
  f_new->Close();
  return;
}
