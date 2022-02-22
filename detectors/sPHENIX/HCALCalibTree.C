// HCAL Calibration TreeMaker
#include <fun4all/Fun4AllBase.h>
#include <iostream>
#include "HCALCalibTree.h"

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>

#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4VtxPoint.h>

#include <calobase/RawTowerv2.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>

#include "TLorentzVector.h"
#include "TMath.h"
#include "TROOT.h"
#include "TH2.h"

HCALCalibTree::HCALCalibTree(const std::string &name, const int &startingSector, const int &numSectors, const double &o, const double &e_max, const double &x_min, const double &x_max, const double &z_max, const double &y_max, const int &nevents) : SubsysReco("HCAL_TREE")
{
  _foutname = name;
  _hcal_sim_name = "TOWER_SIM_HCALOUT";
  _hcal_raw_name = "TOWER_RAW_HCALOUT";
  _hcal_calib_name = "TOWER_CALIB_HCALOUT";
  _truthinfo_name = "G4TruthInfo";
  _startingSector = startingSector;
  _numSectors = numSectors;
  int nbin = floor(sqrt(nevents));
  _h_vertex_y_dist = new TH1F("h_vertex_y_dist","Vertex in y-direction",nbin, 0, y_max);
  _h_vertex_dist = new TH2F("h_vertex_dist","Vertex Distribution in x-z plane; x; z", 50, x_min - o, x_max + o, 50, -1*z_max - o, z_max + o);
  _h_momentum_dist = new TH2F("h_momentum_dist","Momentum Distribution in x-z plane; x; z", 50, -1*e_max, e_max, 50, -1*e_max, e_max);
  _h_momentum_y = new TH1F("h_momentum_y","; mom y; counts", 40, -1*e_max, 0);
  _h_edep_21 = new TH1F("h_edep_21","edep distribution tile 21; edep (GeV); counts", 100, 0, 1);
  _h_edep_22 = new TH1F("h_edep_22","edep distribution tile 22; edep (GeV); counts", 100, 0, 1);
  _h_edep_23 = new TH1F("h_edep_23","edep distribution tile 23; edep (GeV); counts", 100, 0, 1);
  _h_edep_24 = new TH1F("h_edep_24","edep distribution tile 24; edep (GeV); counts", 100, 0, 1);
  _h_edep_25 = new TH1F("h_edep_25","edep distribution tile 25; edep (GeV); counts", 100, 0, 1);
  _h_edep_26 = new TH1F("h_edep_26","edep distribution tile 26; edep (GeV); counts", 100, 0, 1);
  _h_edep_27 = new TH1F("h_edep_27","edep distribution tile 27; edep (GeV); counts", 100, 0, 1);
  _h_edep_28 = new TH1F("h_edep_28","edep distribution tile 28; edep (GeV); counts", 100, 0, 1);
  _h_edep_29 = new TH1F("h_edep_29","edep distribution tile 29; edep (GeV); counts", 100, 0, 1);
  _h_edep_30 = new TH1F("h_edep_30","edep distribution tile 30; edep (GeV); counts", 100, 0, 1);
  _h_edep_31 = new TH1F("h_edep_31","edep distribution tile 31; edep (GeV); counts", 100, 0, 1);
  _h_edep_32 = new TH1F("h_edep_32","edep distribution tile 32; edep (GeV); counts", 100, 0, 1);
  _h_edep_list[0] = _h_edep_21;
  _h_edep_list[1] = _h_edep_22;
  _h_edep_list[2] = _h_edep_23;
  _h_edep_list[3] = _h_edep_24;
  _h_edep_list[4] = _h_edep_25;
  _h_edep_list[5] = _h_edep_26;
  _h_edep_list[6] = _h_edep_27;
  _h_edep_list[7] = _h_edep_28;
  _h_edep_list[8] = _h_edep_29;
  _h_edep_list[9] = _h_edep_30;
  _h_edep_list[10] = _h_edep_31;
  _h_edep_list[11] = _h_edep_32;
}

int HCALCalibTree::reset_tree_vars() {

  _b_event = -99;
  _b_truth_vx = -99;
  _b_truth_vy = -99;
  _b_truth_vz = -99;

  
  _b_tower_sim_n = -99;
  _b_tower_raw_n = -99;
  _b_tower_calib_n = -99;
  _b_part_n = -99;

  for (int i = 0; i <nTowers; i++){
    _b_tower_sim_layer[i] = -99;
    _b_tower_sim_E[i] = -99;
    _b_tower_sim_eta[i] = -99;
    _b_tower_sim_phi[i] = -99;
    
    _b_tower_raw_layer[i] = -99;
    _b_tower_raw_E[i] = -99;
    _b_tower_raw_eta[i] = -99;
    _b_tower_raw_phi[i] = -99;
    
    _b_tower_calib_layer[i] = -99;
    _b_tower_calib_E[i] = -99;
    _b_tower_calib_eta[i] = -99;
    _b_tower_calib_phi[i] = -99;
   
    _b_part_E[i] = -99;
    _b_part_px[i] = -99;
    _b_part_py[i] = -99;
    _b_part_pz[i] = -99;
    _b_part_pdgid[i]= -99;
  }
  return 1;
}

int HCALCalibTree::GetNodes(PHCompositeNode *topNode) {

  if (_debug) std::cout<<"GettingNodes..."<<std::endl;
  _geomOH = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_HCALOUT");
  if(!_geomOH) std::cout<<"No TOWERGeOM_HCALOUT"<<std::endl;

  _towersSimOH = findNode::getClass<RawTowerContainer>(topNode, _hcal_sim_name);
  if (!_towersSimOH) std::cout<<"No TOWER_SIM_HCALOUT Node"<<std::endl;

  _towersRawOH = findNode::getClass<RawTowerContainer>(topNode, _hcal_raw_name);
  if (!_towersRawOH) std::cout<<"No TOWER_RAW_HCALOUT Node"<<std::endl;

  _towersCalibOH = findNode::getClass<RawTowerContainer>(topNode, _hcal_calib_name);
  if (!_towersCalibOH) std::cout<<"No TOWER_CALIB_HCALOUT Node"<<std::endl;

  _truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode, _truthinfo_name);
  if (!_truthinfo) std::cout<<"No G4TruthInfo Node"<<std::endl;

  return 1;
}
int HCALCalibTree::Init(PHCompositeNode *topNode) {
  _ievent = 0;
  _b_event = -1;

  if (_debug) std::cout<<"Initiating..."<<std::endl;

  reset_tree_vars();

  _file = new TFile(_foutname.c_str(), "RECREATE");
  
  _tree = new TTree("T", "keep on giving tree");

  _tree->Branch("truth_vx", &_b_truth_vx, "truth_vx/F");
  _tree->Branch("truth_vy", &_b_truth_vy, "truth_vy/F");
  _tree->Branch("truth_vz", &_b_truth_vz, "truth_vz/F");

  _tree->Branch("tower_sim_n",&_b_tower_sim_n, "tower_sim_n/I");
  _tree->Branch("tower_sim_layer",_b_tower_sim_layer, "tower_sim_layer[tower_sim_n]/I");
  _tree->Branch("tower_sim_E",_b_tower_sim_E, "tower_sim_E[tower_sim_n]/F");
  _tree->Branch("tower_sim_eta",_b_tower_sim_eta, "tower_sim_eta[tower_sim_n]/F");
  _tree->Branch("tower_sim_phi",_b_tower_sim_phi, "tower_sim_phi[tower_sim_n]/F");

  _tree->Branch("tower_raw_n",&_b_tower_raw_n, "tower_raw_n/I");
  _tree->Branch("tower_raw_layer",_b_tower_raw_layer, "tower_raw_layer[tower_raw_n]/I");
  _tree->Branch("tower_raw_E",_b_tower_raw_E, "tower_raw_E[tower_raw_n]/F");
  _tree->Branch("tower_raw_eta",_b_tower_raw_eta, "tower_raw_eta[tower_raw_n]/F");
  _tree->Branch("tower_raw_phi",_b_tower_raw_phi, "tower_raw_phi[tower_raw_n]/F");

  _tree->Branch("tower_calib_n",&_b_tower_calib_n, "tower_calib_n/I");
  _tree->Branch("tower_calib_layer",_b_tower_calib_layer, "tower_calib_layer[tower_calib_n]/I");
  _tree->Branch("tower_calib_E",_b_tower_calib_E, "tower_calib_E[tower_calib_n]/F");
  _tree->Branch("tower_calib_eta",_b_tower_calib_eta, "tower_calib_eta[tower_calib_n]/F");
  _tree->Branch("tower_calib_phi",_b_tower_calib_phi, "tower_calib_phi[tower_calib_n]/F");

  _tree->Branch("part_n",&_b_part_n, "part_n/I");
  _tree->Branch("part_E",_b_part_E, "part_E[part_n]/F");
  _tree->Branch("part_px",_b_part_px, "part_px[part_n]/F");
  _tree->Branch("part_py",_b_part_py, "part_py[part_n]/F");
  _tree->Branch("part_pz",_b_part_pz, "part_pz[part_n]/F");
  _tree->Branch("part_pdgid",_b_part_pdgid, "part_pdgid[part_n]/I");

  return 0;
}

int HCALCalibTree::process_event(PHCompositeNode *topNode) {

  GetNodes(topNode);
  
  _b_event = _ievent;
  if (_ievent %5000==0) std::cout<<"Event: "<<_ievent<<std::endl;
  float phi_start = _startingSector*M_PI/16;
  float phi_end = (_startingSector+_numSectors)*M_PI/16;

  TString histname = TString::Format("h_event_%d", _ievent);
  //TH2F *hist = ((THF *)(_hlist.FindObject(histname)));
  //if(!hist){
  TH2F *hist = new TH2F(histname,"HCAL Sector Event; #eta; #phi; Energy Deposit (GeV)", 24, -1.1, 1.1, 2, phi_start, phi_end);
    //    _hlist.Add(hist);
  //}

  if (_debug) std::cout<<"Processing Event: "<< _b_event<<std::endl;

  PHG4VtxPoint* first_point = _truthinfo->GetPrimaryVtx(_truthinfo->GetPrimaryVertexIndex());
  _b_truth_vx = first_point->get_x();
  _b_truth_vy = first_point->get_y();
  _b_truth_vz = first_point->get_z();
  _h_vertex_dist->Fill(_b_truth_vx, _b_truth_vz);
  int layer = 5;
  if (_debug) std::cout<<"First point: "<<_b_truth_vx<<" / "<<_b_truth_vy<<" / "<<_b_truth_vz<<std::endl;
  _b_tower_raw_n = 0;
  RawTowerContainer::ConstRange begin_end_raw = _towersRawOH->getTowers();
  for (RawTowerContainer::ConstIterator rtiter = begin_end_raw.first; rtiter != begin_end_raw.second; ++rtiter) {
    RawTower *tower = rtiter->second;
    RawTowerGeom *tower_geom = _geomOH->get_tower_geometry(tower->get_key());
    //float phi = tower_geom->get_phi();
    //if((phi<phi_end)&&(phi>phi_start)) {
      _b_tower_raw_layer[ _b_tower_raw_n ] = layer;
      _b_tower_raw_E[ _b_tower_raw_n ] = tower->get_energy();
      _b_tower_raw_eta[ _b_tower_raw_n ] = tower_geom->get_eta();
      _b_tower_raw_phi[ _b_tower_raw_n ] = tower_geom->get_phi();
      _b_tower_raw_n++;
      //}
    if(_b_tower_raw_n >= nTowers){
      std::cout << __FILE__ << " ERROR: _b_tower_raw_n has hit cap of " << nTowers << "!!!" << std::endl;
    }

  }

  if (_debug) std::cout<<"Got raw n: "<< _b_tower_raw_n<<std::endl;
  _b_tower_sim_n=0;
  RawTowerContainer::ConstRange begin_end_sim = _towersSimOH->getTowers();
  if (_debug) std::cout<<"Got the iterator"<<std::endl;

  for (RawTowerContainer::ConstIterator rtiter = begin_end_sim.first; rtiter != begin_end_sim.second; ++rtiter) {
    if ((_b_tower_sim_n%10==0)&&(_debug)) std::cout<<"At sim tower: "<< _b_tower_sim_n<<std::endl;

    RawTower *tower = rtiter->second;
    RawTowerGeom *tower_geom = _geomOH->get_tower_geometry(tower->get_key());
    //float phi = tower_geom->get_phi();
    // if((phi<phi_end)&&(phi>phi_start)) {
      _b_tower_sim_layer[ _b_tower_sim_n ] = layer;
      _b_tower_sim_E[ _b_tower_sim_n ] = tower->get_energy();
      _b_tower_sim_eta[ _b_tower_sim_n ] = tower_geom->get_eta();
      _b_tower_sim_phi[ _b_tower_sim_n ] = tower_geom->get_phi();
      _b_tower_sim_n++;
      //}
    //if ((_b_tower_sim_n%10==0)&&(_debug)) std::cout<<"At sim tower: "<< _b_tower_sim_n<<std::endl;

    if(_b_tower_sim_n >= nTowers){
      std::cout << __FILE__ << " ERROR: _b_tower_sim_n has hit cap of " << nTowers << "!!!" << std::endl;
    }
    
  }
  
  if (_debug) std::cout<<"Got sims n: "<< _b_tower_sim_n<<std::endl;
  _b_tower_calib_n = 0;
  
  RawTowerContainer::ConstRange begin_end_calib = _towersCalibOH->getTowers();
  for (RawTowerContainer::ConstIterator rtiter = begin_end_calib.first; rtiter != begin_end_calib.second; ++rtiter) {
    RawTower *tower = rtiter->second;
    RawTowerGeom *tower_geom = _geomOH->get_tower_geometry(tower->get_key());
    //float phi = tower_geom->get_phi();
    //if((phi<phi_end)&&(phi>phi_start)) {
      _b_tower_calib_layer[ _b_tower_calib_n ] = layer;
      _b_tower_calib_E[ _b_tower_calib_n ] = tower->get_energy();
      _b_tower_calib_eta[ _b_tower_calib_n ] = tower_geom->get_eta();
      _b_tower_calib_phi[ _b_tower_calib_n ] = tower_geom->get_phi();
      _b_tower_calib_n++;
      hist->Fill(tower_geom->get_eta(), tower_geom->get_phi(),tower->get_energy());
      //}

    if(_b_tower_calib_n >= nTowers){
      std::cout << __FILE__ << " ERROR: _b_tower_calib_n has hit cap of " << nTowers << "!!!" << std::endl;
    }
    
  }
  double min_e = 0.07;
  for ( int i = 0; i < 12; i++){
    if (hist->GetBinContent(i+1,1) > min_e) {
      _h_edep_list[i]->Fill(hist->GetBinContent(i+1,1));
    }
    if (hist->GetBinContent(i+1,2) > min_e) {
      _h_edep_list[i]->Fill(hist->GetBinContent(i+1,2));
    }
    if (hist->GetBinContent(24-i,1) > min_e) {
      _h_edep_list[i]->Fill(hist->GetBinContent(24-i,1));
    
    }
    if (hist->GetBinContent(24-i,2) > min_e) {
      _h_edep_list[i]->Fill(hist->GetBinContent(24-i,2));
    }
    if (_debug) std::cout<<"edep: "<<hist->GetBinContent(i+1,1)<<" / "<<hist->GetBinContent(i+1,2)<<" / "<<hist->GetBinContent(24-i,1)<<" / "<<hist->GetBinContent(24-i,2)<<std::endl;
  }
  if (_debug) std::cout<<"Got calib n: "<< _b_tower_calib_n<<std::endl;
  

  _b_part_n = 0;
  PHG4TruthInfoContainer::Range range = _truthinfo->GetPrimaryParticleRange();
  for(PHG4TruthInfoContainer::ConstIterator iter = range.first; iter != range.second; ++iter){
    PHG4Particle *g4particle = iter->second;
    
    //if(t.Pt() < 0.1) continue; 
    //if(TMath::Abs(t.Eta()) > 1.1) continue; //Don't need stuff beyond calorimeter acceptance
    
    _b_part_E[_b_part_n] = g4particle->get_e();
    _b_part_px[_b_part_n] = g4particle->get_px();
    _b_part_py[_b_part_n] = g4particle->get_py();
    _b_part_pz[_b_part_n] = g4particle->get_pz();
    _b_part_pdgid[_b_part_n] = g4particle->get_pid();
    if (g4particle->get_pid() == 13) {
      _h_momentum_dist->Fill(g4particle->get_px(), g4particle->get_pz());
      _h_momentum_y->Fill(g4particle->get_py());
    }
      ++_b_part_n;
  }
  
  if (_debug) std::cout<<"Got particle n: "<< _b_part_n<<std::endl;
  _file->cd();
  //hist->Write();
  _tree->Fill();
  _ievent++;
  
  return 0;
}

int HCALCalibTree::End(PHCompositeNode *topNode)
{
  if (_debug) std::cout<<"Writing File"<<std::endl;
  _file->cd();

  _h_edep_21->Write();
  _h_edep_22->Write();
  _h_edep_23->Write();
  _h_edep_24->Write();
  _h_edep_25->Write();
  _h_edep_26->Write();
  _h_edep_27->Write();
  _h_edep_28->Write();
  _h_edep_29->Write();
  _h_edep_30->Write();
  _h_edep_31->Write();
  _h_edep_32->Write();
  _h_vertex_dist->Write();
  _h_momentum_dist->Write();
  _h_momentum_y->Write();
  _file->Write();
  _file->Close();

  //std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  return 0;
}
