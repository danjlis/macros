//ORIGINAL MACRO BY DENNIS PERIPELITSA
//UPDATE BY CHRIS MCGINN (2019.08.01)

//cpp
#include <iostream>

//ROOT
#include "TLorentzVector.h"
#include "TMath.h"

//Local
#include "TreeMaker.h"

//CoreSoftware
#include <coresoftware/offline/framework/fun4all/Fun4AllServer.h>
#include <coresoftware/offline/framework/phool/getClass.h>
#include <coresoftware/offline/framework/phool/PHCompositeNode.h>
#include <coresoftware/offline/packages/CaloBase/RawTower.h>
#include <coresoftware/offline/packages/CaloBase/RawTowerGeom.h>
#include <coresoftware/simulation/g4simulation/g4main/PHG4Particle.h>
#include <coresoftware/simulation/g4simulation/g4main/PHG4TruthInfoContainer.h>

TreeMaker::TreeMaker(const std::string &name) : SubsysReco("OUTPUT_TREE")
{
  _foutname = name;
  _verbosity = 0;
}

int TreeMaker::Init(PHCompositeNode *topNode)
{
  _f = new TFile( _foutname.c_str(), "RECREATE");

  _tree = new TTree("aftburnTree","");

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
  _tree->Branch("part_pt",_b_part_pt, "part_pt[part_n]/F");
  _tree->Branch("part_eta",_b_part_eta, "part_eta[part_n]/F");
  _tree->Branch("part_phi",_b_part_phi, "part_phi[part_n]/F");
  _tree->Branch("part_m",_b_part_m, "part_m[part_n]/F");
  _tree->Branch("part_pdgid",_b_part_pdgid, "part_pdgid[part_n]/I");

  return 0;
}

int TreeMaker::process_event(PHCompositeNode *topNode)
{
  std::cout << "DVP TreeMaker : at process_event, tree size is: " << _tree->GetEntries() << std::endl;

  //std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  RawTowerContainer *towersSimEM4 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_SIM_CEMC");
  RawTowerContainer *towersSimIH4 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_SIM_HCALIN");
  RawTowerContainer *towersSimOH4 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_SIM_HCALOUT");

  RawTowerContainer *towersRawEM4 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_RAW_CEMC");
  RawTowerContainer *towersRawIH4 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_RAW_HCALIN");
  RawTowerContainer *towersRawOH4 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_RAW_HCALOUT");

  RawTowerContainer *towersCalibEM4 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_CEMC");
  RawTowerContainer *towersCalibIH4 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_HCALIN");
  RawTowerContainer *towersCalibOH4 = findNode::getClass<RawTowerContainer>(topNode, "TOWER_CALIB_HCALOUT");

  RawTowerGeomContainer *geomCEMC = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_CEMC");
  RawTowerGeomContainer *geomIH = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_HCALIN");
  RawTowerGeomContainer *geomOH = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_HCALOUT");

  //std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  _b_tower_sim_n = 0;
  _b_tower_raw_n = 0;
  _b_tower_calib_n = 0;

  //std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  processTowersSim(towersSimEM4, geomCEMC, 3);
  processTowersSim(towersSimIH4, geomIH, 4);
  processTowersSim(towersSimOH4, geomOH, 5);

  //std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  processTowersRaw(towersRawEM4, geomCEMC, 3);
  processTowersRaw(towersRawIH4, geomIH, 4);
  processTowersRaw(towersRawOH4, geomOH, 5);

  //std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  processTowersCalib(towersCalibEM4, geomCEMC, 3);
  processTowersCalib(towersCalibIH4, geomIH, 4);
  processTowersCalib(towersCalibOH4, geomOH, 5);

  //std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  _b_part_n = 0;

  //Lets grab truth info too (Based on macro coresoftware/offline/packages/jetbackground/DetermineTowerBackground as of 2019.08.02
  PHG4TruthInfoContainer *truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
  PHG4TruthInfoContainer::Range range = truthinfo->GetPrimaryParticleRange();
  for(PHG4TruthInfoContainer::ConstIterator iter = range.first; iter != range.second; ++iter){
    PHG4Particle *g4particle = iter->second;

    TLorentzVector t;
    t.SetPxPyPzE(g4particle->get_px(), g4particle->get_py(), g4particle->get_pz(), g4particle->get_e());

    if(t.Pt() < 0.1) continue; //Not sure what the pT cutoff is for reaching calorimeter with this magnetic field, but this is a safe lower bound
    if(TMath::Abs(t.Eta()) > 1.1) continue; //Don't need stuff beyond calorimeter acceptance
    
    _b_part_pt[_b_part_n] = t.Pt();
    _b_part_phi[_b_part_n] = t.Phi();
    _b_part_eta[_b_part_n] = t.Eta();
    _b_part_m[_b_part_n] = t.M();
    _b_part_pdgid[_b_part_n] = g4particle->get_pid();

    ++_b_part_n;
  }  

  _tree->Fill();

  return 0;
}

int TreeMaker::End(PHCompositeNode *topNode)
{
  _f->Write();
  _f->Close();

  //std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  return 0;
}

void TreeMaker::processTowersSim(RawTowerContainer* towers, RawTowerGeomContainer* geom, int layer)
{  
  RawTowerContainer::ConstRange begin_end = towers->getTowers();
  for (RawTowerContainer::ConstIterator rtiter = begin_end.first; rtiter != begin_end.second; ++rtiter) {
    RawTower *tower = rtiter->second;
    RawTowerGeom *tower_geom = geom->get_tower_geometry(tower->get_key());
    _b_tower_sim_layer[ _b_tower_sim_n ] = layer;
    _b_tower_sim_E[ _b_tower_sim_n ] = tower->get_energy();
    _b_tower_sim_eta[ _b_tower_sim_n ] = tower_geom->get_eta();
    _b_tower_sim_phi[ _b_tower_sim_n ] = tower_geom->get_phi();
    _b_tower_sim_n++;

    if(_b_tower_sim_n >= nTowers){
      std::cout << __FILE__ << " ERROR: _b_tower_sim_n has hit cap of " << nTowers << "!!!" << std::endl;
    }
  }  

  return;
}

void TreeMaker::processTowersRaw(RawTowerContainer* towers, RawTowerGeomContainer* geom, int layer)
{  
  RawTowerContainer::ConstRange begin_end = towers->getTowers();
  for (RawTowerContainer::ConstIterator rtiter = begin_end.first; rtiter != begin_end.second; ++rtiter) {
    RawTower *tower = rtiter->second;
    RawTowerGeom *tower_geom = geom->get_tower_geometry(tower->get_key());
    _b_tower_raw_layer[ _b_tower_raw_n ] = layer;
    _b_tower_raw_E[ _b_tower_raw_n ] = tower->get_energy();
    _b_tower_raw_eta[ _b_tower_raw_n ] = tower_geom->get_eta();
    _b_tower_raw_phi[ _b_tower_raw_n ] = tower_geom->get_phi();
    _b_tower_raw_n++;

    if(_b_tower_raw_n >= nTowers){
      std::cout << __FILE__ << " ERROR: _b_tower_raw_n has hit cap of " << nTowers << "!!!" << std::endl;
    }
  }  

  return;
}

void TreeMaker::processTowersCalib(RawTowerContainer* towers, RawTowerGeomContainer* geom, int layer)
{  
  RawTowerContainer::ConstRange begin_end = towers->getTowers();
  for (RawTowerContainer::ConstIterator rtiter = begin_end.first; rtiter != begin_end.second; ++rtiter) {
    RawTower *tower = rtiter->second;
    RawTowerGeom *tower_geom = geom->get_tower_geometry(tower->get_key());
    _b_tower_calib_layer[ _b_tower_calib_n ] = layer;
    _b_tower_calib_E[ _b_tower_calib_n ] = tower->get_energy();
    _b_tower_calib_eta[ _b_tower_calib_n ] = tower_geom->get_eta();
    _b_tower_calib_phi[ _b_tower_calib_n ] = tower_geom->get_phi();
    _b_tower_calib_n++;

    if(_b_tower_calib_n >= nTowers){
      std::cout << __FILE__ << " ERROR: _b_tower_calib_n has hit cap of " << nTowers << "!!!" << std::endl;
    }
  }  

  return;
}
