void CombineTrees(){
  int num = 2;
  TList *list = new TList;
  for (int i = 1; i < num+1; i++){
    TString fileName = Form("hcal_4GeV_100k_full%d.root",i);
    TFile *f = new TFile(fileName, "read");
    TTree *t = (TTree*)f->Get("T");
    t->SetName(Form("T_%d",i));
    list->Add(t);
  }

  TFile *fout = new TFile("hcal_trigger_4GeV_Full_200k.root","recreate");
  TTree *newtree = TTree::MergeTrees(list);
  newtree->SetName("T_out");
  newtree->Write();
  fout->Close();
}
