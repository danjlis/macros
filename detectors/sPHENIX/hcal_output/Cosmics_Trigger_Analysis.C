// Cosmics Trigger Analysis
// Author:: Daniel List

// Get response
int Three_in_a_Row(double cal_map[][64]){
  for (int i = 0; i < 24; i++){
    int n = 0;
    for (int j = 0; j < 64; j++){
      double adj_l = 0, adj_r = 0;
      if ( i != 0 ) adj_l = cal_map[i-1][j];
      if ( i != 23 ) adj_r = cal_map[i+1][j];
      if (cal_map[i][j] > 0.002 && 0.002 > adj_l && 0.002 > adj_r){
        n++;

        //std::cout<<n << " : "<<cal_map[i][j]<<std::endl;
      }
      else n = 0;
      if (n == 3) return 1;
    }
  }
  return 0;
}
void GetResponse(int &tower_n, float *tower_E, float *tower_phi, float *tower_eta, double cal_map[][64]){
  double deta = 1.1/12.;
  double dphi = TMath::Pi()/32.;
  int eta_bin;
  int phi_bin;

  for (int i = 0; i < tower_n; i++){
    eta_bin = 12 + floor(tower_eta[i]/deta);
    phi_bin = floor((TMath::Pi() + tower_phi[i])/dphi);
    cal_map[eta_bin][phi_bin] = tower_E[i];
  }
}

// get sums

void Get2x2Sums(double cal_map[][64], double cal_2x2_map[][32]){
  for (int i = 0; i < 12; i ++){
    for (int j = 0; j < 32; j++){
      int i1 = 2*i;
      int i2 = 2*i + 1;
      int j1 = 2*j;
      int j2 = 2*j + 1;
      cal_2x2_map[i][j] = cal_map[i1][j1] + cal_map[i1][j2] + cal_map[i2][j1] + cal_map[i2][j2];
    }
  }
}
// PeakFinding

void PeakFinding(double cal_2x2_map[][32], int &num_max, double *max_e, double &maxdynrange, double &max_et){
  num_max = 0;
  maxdynrange = 0;
  max_et = 0;
  double ul, um, ur, ml, mm,mr, dl, dm, dr;
  double global_min_de;
  for (int i = 0; i < 12; i++){
    for (int j = 0; j < 32; j ++){

      mm = cal_2x2_map[i][j];

      double sec_max = 0;
      if (i == 0 || j == 31) ul = 0;
      else ul = cal_2x2_map[i-1][j + 1];
      if (ul > sec_max) sec_max = ul;

      if (j == 31) um = 0;
      else um = cal_2x2_map[i][j + 1];
      if (um > sec_max) sec_max = um;

      if (i == 11 || j == 31) ur = 0;
      else ur = cal_2x2_map[i+1][j + 1];
      if (ur > sec_max) sec_max = ur;

      if ( i == 0) ml = 0;
      else ml = cal_2x2_map[i-1][j];
      if (ml > sec_max) sec_max = ml;

      if ( i == 11) mr = 0;
      else mr = cal_2x2_map[i+1][j];
      if (mr > sec_max) sec_max = mr;

      if (i == 0 || j == 0) dl = 0;
      else dl = cal_2x2_map[i-1][j - 1];
      if (dl > sec_max) sec_max = dl;

      if ( j == 0) dm = 0;
      else dm = cal_2x2_map[i][j - 1];
      if (dm > sec_max) sec_max = dm;


      if (i == 11 || j == 0) dr = 0;
      else dr = cal_2x2_map[i+1][j - 1];
      if (dr > sec_max) sec_max = dr;

      if (mm >= ul && mm > um && mm > ur && mm >= ml && mm > mr && mm >= dl && mm >= dm && mm > dr){
        max_e[num_max] = mm;
        if (mm > max_et) max_et = mm;
        if (mm - sec_max > global_min_de) global_min_de = mm - sec_max;
        num_max++;
      }
    }
  }
  maxdynrange = global_min_de * 256.;
  //std::cout<<maxdynrange<<std::endl;
}

//main calls the rest from here

void Cosmics_Trigger_Analysis(){

  gStyle->SetOptStat(0);
  int use_sim = 1;

  bool debug = false;
  if (debug) std::cout<<"Starting Cosmics Trigger Analysis..."<<std::endl;

  //read file
  TFile *f = new TFile("hcal_RANGE_100k_full.root", "read");
  // write file
  TString namef;
  namef = "hcal_trigger_sim_full.root";

  TFile *f_new = new TFile(namef, "recreate");

  if (!f) std::cout<<"Could not find the file!!!"<<std::endl;

  //grab TTree
  TTree *t = (TTree*) f->Get("T");

  if (!t) std::cout<<"Could not find the tree!!!"<<std::endl;
  //load in the tower info
  float truth_vx, truth_vy, truth_vz, part_px, part_py, part_pz;

  t->SetBranchAddress("truth_vx", &truth_vx);
  t->SetBranchAddress("truth_vy", &truth_vy);
  t->SetBranchAddress("truth_vz", &truth_vz);
  t->SetBranchAddress("part_px", &part_px);
  t->SetBranchAddress("part_py", &part_py);
  t->SetBranchAddress("part_pz", &part_pz);

  int nen = t->GetEntries();

  float tower_sim_E[1000];
  float tower_sim_phi[1000];
  float tower_sim_eta[1000];
  int tower_sim_n;
  t->SetBranchAddress("tower_sim_E", tower_sim_E);
  t->SetBranchAddress("tower_sim_eta", tower_sim_eta);
  t->SetBranchAddress("tower_sim_phi", tower_sim_phi);
  t->SetBranchAddress("tower_sim_n", &tower_sim_n);


  int entries = t->GetEntries();

  double phi_of_entry, angle_of_incidence, angle_from_norm;
  double maxdynrange, min_et;

  //Making histograms for the root file.

  if (debug) std::cout<<"Making histograms..."<<std::endl;

  TProfile *h_eff_et = new TProfile("h_eff_Et","",51, -0.5, 50.5);
  TProfile *h_eff_dynrange = new TProfile("h_eff_dynrange","", 300, 0, 15);
  TProfile *h_angle_v_eT = new TProfile("h_angle_v_eT","", 32, -1.7, 1.7, 0, 100, "S");
  TH1D *h_angle_range[7];
  TH1D *h_ar_copy[7];

  TProfile *h_eff_phi[7];
  TProfile *h_eff_eta[7];
  TProfile *h_eff_top_phi[7];
  TProfile *h_eff_dynrange_trig[7];
  TH1D *h_num_triggers[7];

  int et[7] = {2, 5, 10, 20, 25, 30, 40};
  double et_fl[7] = {.002, .005, .01, .02, .025, .030, .040};

  double bins_phi[65];
  for (int i = 0; i < 65; i++){
    bins_phi[i] = -1*TMath::Pi()/64 + i*TMath::Pi()/32;
  }
  double bins_top_phi[33];
  for (int i = 0; i < 33; i++){
    bins_top_phi[i] = TMath::Pi() - 1*TMath::Pi()/64 + i*TMath::Pi()/32;
  }
  double bins_eta[25];
  for (int i = 0; i < 25; i++){
    bins_eta[i] = -1.1*((25/24) - i/12.);
  }
  for (int i = 0; i < 7; i++){
    h_eff_phi[i] = new TProfile(Form("h_eff_phi_%dMeV", et[i]),"", 64, bins_phi);
    h_eff_eta[i] = new TProfile(Form("h_eff_eta_%dMeV", et[i]),"", 24, bins_eta);
    h_eff_top_phi[i] = new TProfile(Form("h_eff_top_phi_%dMeV", et[i]),"", 32, bins_top_phi);
    h_num_triggers[i] = new TH1D(Form("h_num_triggers_%dMeV", et[i]),"", 31, -0.5, 30.5);
    h_eff_dynrange_trig[i] = new TProfile(Form("h_eff_dynrange_%dMeV", et[i]),"", 200, 0, 10);
    h_angle_range[i] = new TH1D(Form("h_angle_range_%d",i),"",32, -1.7, 1.7);
  }

  TH1D *h_phi_edep[64];
  TH1D *h_eta_edep[24];
  TH1D *h_phi_edep_cut[64];
  TH1D *h_eta_edep_cut[24];
  TH1D *h_tower_edep[24][64];
  TH1D *h_tower_edep_cut[24][64];
  TH1D *h_phi_edep_trig[64][7];
  TH1D *h_eta_edep_trig[24][7];
  TH1D *h_phi_edep_cut_trig[64][7];
  TH1D *h_eta_edep_cut_trig[24][7];
  TH1D *h_tower_edep_trig[24][64][7];

  TH1D *h_phi_edep_range[64][7];
  TH1D *h_eta_edep_range[24][7];
  TH1D *h_phi_edep_range_copy[64][7];
  TH1D *h_eta_edep_range_copy[24][7];
  TH1D *h_phi_edep_range_cut[64][7];
  TH1D *h_eta_edep_range_cut[24][7];
  TH1D *h_tower_edep_range[24][64][7];

  int nbins_phi = 70;//static_cast<int>(floor(sqrt(entries/64.)));
  int nbins_eta = 70; //floorstatic_cast<int>(floor(sqrt(entries/24.)));
  int nbins_all = 70;//static_cast<int>(floor(sqrt(entries/(24.*64.))));

  int nbins_phi_range = 50;//static_cast<int>(floor(sqrt(entries/64.)));
  int nbins_eta_range = 50; //floorstatic_cast<int>(floor(sqrt(entries/24.)));

  for (int i = 0; i < 64; i++){
    h_phi_edep[i] = new TH1D(Form("h_phi_edep_%d",i),"", nbins_phi, 0, 0.04);
    if ( i < 24) h_eta_edep[i] = new TH1D(Form("h_eta_edep_%d",i),"", nbins_eta, 0, 0.04);
    h_phi_edep_cut[i] = new TH1D(Form("h_phi_edep_cut_%d",i),"", nbins_phi, 0, 0.04);
    if ( i < 24) h_eta_edep_cut[i] = new TH1D(Form("h_eta_edep_cut_%d",i),"", nbins_eta, 0, 0.04);
    for (int j = 0; j < 7; j++){
      h_phi_edep_trig[i][j] = new TH1D(Form("h_phi_edep_%d_%dMeV", i, et[j]),"", nbins_phi, 0, 0.04);
      if (i < 24) h_eta_edep_trig[i][j] = new TH1D(Form("h_eta_edep_%d_%dMeV", i, et[j]),"", nbins_eta, 0, 0.04);
      h_phi_edep_range[i][j] =  new TH1D(Form("h_phi_edep_range_%d_%d", i, j),"", nbins_phi_range, 0, 0.04);
      if (i < 24) h_eta_edep_range[i][j] = new TH1D(Form("h_eta_edep_range_%d_%d", i, j),"", nbins_eta_range, 0, 0.04);
      h_phi_edep_range_copy[i][j] =  new TH1D(Form("h_phi_edep_range_copy_%d_%d", i, j),"", nbins_phi_range, 0, 0.04);
      if (i < 24) h_eta_edep_range_copy[i][j] = new TH1D(Form("h_eta_edep_range_copy_%d_%d", i, j),"", nbins_eta_range, 0, 0.04);
      h_phi_edep_range_cut[i][j] =  new TH1D(Form("h_phi_edep_range_cut_%d_%d", i, j),"", nbins_phi_range, 0, 0.04);
      if (i < 24) h_eta_edep_range_cut[i][j] = new TH1D(Form("h_eta_edep_range_cut_%d_%d", i, j),"", nbins_eta_range, 0, 0.04);
      h_phi_edep_cut_trig[i][j] =  new TH1D(Form("h_phi_edep_cut_%d_%d", i, j),"", nbins_phi_range, 0, 0.04);
      if (i < 24) h_eta_edep_cut_trig[i][j] = new TH1D(Form("h_eta_edep_cut_%d_%d", i, j),"", nbins_eta_range, 0, 0.04);
    }
    for (int j = 0; j < 24; j++) {
      h_tower_edep[j][i] = new TH1D(Form("h_tower_edep_%d_%d",j, i),"",nbins_all, 0, 0.04);
      h_tower_edep_cut[j][i] = new TH1D(Form("h_tower_edep_cut_%d_%d",j, i),"",nbins_all, 0, 0.04);

      for (int k = 0; k < 7; k++){
        h_tower_edep_trig[j][i][k] =  new TH1D(Form("h_tower_edep_trig_%d_%d_%dMeV",j, i, et[k]),"", nbins_all, 0, 0.04);
        h_tower_edep_range[j][i][k] =  new TH1D(Form("h_tower_edep_range_%d_%d_%d",j, i, k),"", nbins_all, 0, 0.04);
      }
    }
  }
  if (debug) entries = 10;
  for (int i = 0; i < entries; i++){
    double cal_map[24][64] = {0};
    double cal_2x2_map[12][32] = {0};
    int num_max;
    double max_e[100];
    if (debug || i%1000 == 0) std::cout<<"Event "<< i <<std::endl;
    t->GetEntry(i);
    if (debug) std::cout<<"Computing phi of entry"<<std::endl;
    if (tower_sim_n == 0) continue;
    phi_of_entry = atan2(truth_vy, truth_vx);
    if (debug) std::cout<<phi_of_entry<<std::endl;
    TVector2 part_p = TVector2(part_px, part_py);
    TVector2 down = TVector2(0, -1);
    angle_of_incidence = part_p.DeltaPhi(down);
    angle_from_norm = std::fabs(angle_of_incidence) + phi_of_entry - TMath::Pi()/2.;
    if (debug) std::cout<<angle_of_incidence<<std::endl;

    if (debug) std::cout<<angle_from_norm<<std::endl;

    double max_et;
    if (debug) std::cout<<"Getting response"<<std::endl;

    double deta = 1.1/12.;
    double dphi = TMath::Pi()/32.;
    int eta_bin;
    int phi_bin;

    if (debug) std::cout<<"tower_sim_n: "<<tower_sim_n<<"..."<<std::endl;
    GetResponse(tower_sim_n, tower_sim_E, tower_sim_phi, tower_sim_eta, cal_map);
    /*
    for (int j = 0; j < tower_sim_n; j++){
      eta_bin = 12 + floor(tower_sim_eta[j]/deta);
      phi_bin = floor((TMath::Pi() + tower_sim_phi[j])/dphi);
      cal_map[eta_bin][phi_bin] = tower_sim_E[j];
      if (debug) std::cout<<" "<<tower_sim_E[j]<<" at "<<eta_bin<<" / "<<phi_bin<<std::endl;
    }
    */
    if (debug) std::cout<<"..."<<std::endl;

    if (debug) std::cout<<"Get2x2Sums"<<std::endl;

    Get2x2Sums(cal_map, cal_2x2_map);
    if (debug){
      for (int j = 0; j < 32; j++){
        std::cout<<"phi "<<j<<" : ";
        for (int k = 0; k< 12; k++){
          std::cout<<cal_2x2_map[k][j]<<" ";
        }
        std::cout<<"/n"<<std::endl;
      }
      std::cout<<"Fullmap: "<<std::endl;
      for (int j = 0; j < 64; j++){
        std::cout<<"phi "<<j<<" : ";
        for (int k = 0; k< 24; k++){
          std::cout<<cal_map[k][j]<<" ";
        }
        std::cout<<"/n"<<std::endl;
      }
    }
    if (debug) std::cout<<"peakfinding"<<std::endl;

    PeakFinding(cal_2x2_map, num_max, max_e, maxdynrange, max_et);
    if (debug) std::cout<<"Filling the histograms"<<std::endl;
    if (debug) std::cout<<"max e threshold: "<<max_et<<std::endl;
    if (debug) std::cout<<"max dynrange threshold: "<<maxdynrange<<std::endl;
    h_angle_v_eT->Fill(angle_from_norm, max_et*1000);

    int made_cut = Three_in_a_Row(cal_map);

    for (int j = 0; j < 51; j++){
      if (j*0.001 <= max_et) h_eff_et->Fill( j, 1);
      else h_eff_et->Fill(j, 0);
    }
    for (int j = 0; j < 300; j++){
      if (j*.05 <= maxdynrange) {
        //if (debug) std::cout<< "Filling in dynrange: "<<maxdynrange<<std::endl;
          h_eff_dynrange->Fill(j*.05, 1);
        }
        else {
          h_eff_dynrange->Fill(j*.05, 0);
          //if (debug) std::cout<< "Filling in dynrange: "<<0<<std::endl;
        }
        for (int k = 0; k < 7; k++){
          if (max_et >= et_fl[k]) {
          if (j*.05 <= maxdynrange) h_eff_dynrange_trig[k]->Fill(j*.05, 1);
          else h_eff_dynrange_trig[k]->Fill(j*.05, 0);
        }
      }
    }

  //  for (int j = 0; j < 7; j++){
  //    if (max_et > static_cast<double>(et[j])) h_eff_dynrange[j]->Fill(maxdynrange);
  //  }
    //Fill edeps
    if (debug) std::cout<<"Filling it up"<<std::endl;

    for (int j = 0; j < 8; j++){

      for ( int k = 0; k < 64; k ++){
        for ( int l = 0; l < 24; l++){
          if (j == 0){
            if (cal_map[l][k] > 0.002){
              if (debug) std::cout<<"Im in..."<<std::endl;
              h_phi_edep[k]->Fill(cal_map[l][k],1);
              h_eta_edep[l]->Fill(cal_map[l][k],1);
              if (made_cut == 1){
                h_phi_edep_cut[k]->Fill(cal_map[l][k],1);
                h_eta_edep_cut[l]->Fill(cal_map[l][k],1);
		h_tower_edep_cut[l][k]->Fill(cal_map[l][k], 1);
              }
              h_tower_edep[l][k]->Fill(cal_map[l][k],1);
              angle_from_norm = std::fabs(angle_of_incidence) + std::fabs((-63*TMath::Pi()/64.) + (k*TMath::Pi()/32))- TMath::Pi()/2.;
              if (k > 31){
                if (max_et < 0.01){
                  h_angle_range[0]->Fill(angle_from_norm);
                }
                else if (cal_map[l][k] < 0.015){
                  h_angle_range[1]->Fill(angle_from_norm);
                }
                else if (cal_map[l][k] < 0.02){
                  h_angle_range[2]->Fill(angle_from_norm);

                }
                else if (cal_map[l][k] < 0.025){
                  h_angle_range[3]->Fill(angle_from_norm);

                }
                else if (cal_map[l][k] < 0.03){
                  h_angle_range[4]->Fill(angle_from_norm);

                }
                else if (cal_map[l][k] < 0.035){
                  h_angle_range[5]->Fill(angle_from_norm);

                }
                else if (cal_map[l][k] < 0.04){
                  h_angle_range[6]->Fill(angle_from_norm);
                }
              }
              if (max_et < 0.01){
                h_phi_edep_range[k][0]->Fill(cal_map[l][k],1);
                h_eta_edep_range[l][0]->Fill(cal_map[l][k],1);
                h_tower_edep_range[l][k][0]->Fill(cal_map[l][k],1);
                h_angle_range[0]->Fill(angle_from_norm);

              }
              else if (max_et < 0.015){
                h_phi_edep_range[k][1]->Fill(cal_map[l][k],1);
                h_eta_edep_range[l][1]->Fill(cal_map[l][k],1);
                h_tower_edep_range[l][k][1]->Fill(cal_map[l][k],1);
                h_angle_range[1]->Fill(angle_from_norm);
              }
              else if (max_et < 0.02){
                h_phi_edep_range[k][2]->Fill(cal_map[l][k],1);
                h_eta_edep_range[l][2]->Fill(cal_map[l][k],1);
                h_tower_edep_range[l][k][2]->Fill(cal_map[l][k],1);
                h_angle_range[2]->Fill(angle_from_norm);

              }
              else if (max_et < 0.025){
                h_phi_edep_range[k][3]->Fill(cal_map[l][k],1);
                h_eta_edep_range[l][3]->Fill(cal_map[l][k],1);
                h_tower_edep_range[l][k][3]->Fill(cal_map[l][k],1);
                h_angle_range[3]->Fill(angle_from_norm);

              }
              else if (max_et < 0.03){
                h_phi_edep_range[k][4]->Fill(cal_map[l][k],1);
                h_eta_edep_range[l][4]->Fill(cal_map[l][k],1);
                h_tower_edep_range[l][k][4]->Fill(cal_map[l][k],1);
                h_angle_range[4]->Fill(angle_from_norm);

              }
              else if (max_et < 0.035){
                h_phi_edep_range[k][5]->Fill(cal_map[l][k],1);
                h_eta_edep_range[l][5]->Fill(cal_map[l][k],1);
                h_tower_edep_range[l][k][5]->Fill(cal_map[l][k],1);
                h_angle_range[5]->Fill(angle_from_norm);

              }
              else if (max_et < 0.04){
                h_phi_edep_range[k][6]->Fill(cal_map[l][k],1);
                h_eta_edep_range[l][6]->Fill(cal_map[l][k],1);
                h_tower_edep_range[l][k][6]->Fill(cal_map[l][k],1);
                h_angle_range[6]->Fill(angle_from_norm);
              }
            }
          }
          else {
            if (max_et > et_fl[j-1]) {
              if (cal_map[l][k] > 0.002){
                h_phi_edep_trig[k][j-1]->Fill(cal_map[l][k],1);
                h_eta_edep_trig[l][j-1]->Fill(cal_map[l][k],1);
                h_tower_edep_trig[l][k][j-1]->Fill(cal_map[l][k]);
                if (made_cut == 1){
                  h_phi_edep_cut_trig[k][j-1]->Fill(cal_map[l][k],1);
                  h_eta_edep_cut_trig[l][j-1]->Fill(cal_map[l][k],1);
                }
              }
            }
          }
        }
      }
    }
  }
  if (debug) std::cout<<"Making root file..."<<std::endl;
  TFile *fout = new TFile("hcal_sim_full_range_1m.root","recreate");
  h_eff_et->Write();
  h_eff_dynrange->Write();
  h_angle_v_eT->Write();
  for (int i = 0; i < 7; i++){
    h_angle_range[i]->Write();
  }
  for (int i = 0; i < 64; i++){
    h_phi_edep[i]->Write();
    h_phi_edep_cut[i]->Write();
    for (int j = 0; j < 7; j++){
      h_phi_edep_range[i][j]->Write();
    }
    for (int j = 0; j < 7; j++){
      h_phi_edep_trig[i][j]->Write();
      h_phi_edep_cut_trig[i][j]->Write();
    }
    
    for (int j = 0; j < 24; j++){
      h_tower_edep[j][i]->Write();
      h_tower_edep_cut[j][i]->Write();
      for (int k = 0; k < 7; k++){
        h_tower_edep_trig[j][i][k]->Write();
      }
      for (int k = 0; k < 4; k++){
        h_tower_edep_range[j][i][k]->Write();
      }
    }
    
  }

  for (int i = 0; i< 24; i++){
    h_eta_edep[i]->Write();
    h_eta_edep_cut[i]->Write();
    for (int j = 0; j < 7; j++){
      h_eta_edep_range[i][j]->Write();

    }
    for (int j = 0; j < 7; j++){
      h_eta_edep_trig[i][j]->Write();
      h_eta_edep_cut_trig[i][j]->Write();
    }
  }
  for (int i = 0; i < 7; i++){
    h_eff_dynrange_trig[i]->Write();
  }
  fout->Close();
  return;
}
