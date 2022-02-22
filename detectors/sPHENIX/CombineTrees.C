void CombineTrees(int &num = 2){
  TList *list = new TList;
  for (int i = 1; i < num+1; i++){
    TString fileName = Form("hcal_trigger_sim_hist_4GeV_100k%d.root",i);
    TFile *f = new TFile(fileName, "read");
    TTree *t = f->Get("T_out");
    t->SetName(Form("T_%d",i));
    list->Add(t);
  }
  TTree *newtree = TTree::MergeTrees(list);
  TFile *fout = new TFile("hcal_trigger_4GeV_Full_All.root","recreate");

  newtree->Write();
  fout->Close();
}
