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
#include <fun4all/Fun4AllServer.h>
#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>

#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4VtxPoint.h>

TreeMaker::TreeMaker(const std::string &name) : SubsysReco("OUTPUT_TREE")
{
  _foutname = name;
  _verbosity = 0;
}

int TreeMaker::Init(PHCompositeNode *topNode)
{
  _f = new TFile( _foutname.c_str(), "RECREATE");

  _f->cd();
  m_config.Write("config", TObject::kOverwrite);

  _tree = new TTree("aftburnTree","");

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

  _tree->Branch("cluster_n", &_b_cluster_n, "cluster_n/I");
  _tree->Branch("cluster_E", _b_cluster_E, "cluster_E[cluster_n]/F");
  _tree->Branch("cluster_eta", _b_cluster_eta, "cluster_eta[cluster_n]/F");
  _tree->Branch("cluster_phi", _b_cluster_phi, "cluster_phi[cluster_n]/F");

  _tree->Branch("cluster_ntower", _b_cluster_ntower, "cluster_ntower[cluster_n]/I");
  _tree->Branch("cluster_tower_phi", &_b_cluster_tower_phi);
  _tree->Branch("cluster_tower_eta", &_b_cluster_tower_eta);
  _tree->Branch("cluster_tower_layer", &_b_cluster_tower_layer);

  _tree->Branch("clusterT_n", &_b_clusterT_n, "clusterT_n/I");
  _tree->Branch("clusterT_E", _b_clusterT_E, "clusterT_E[clusterT_n]/F");
  _tree->Branch("clusterT_eta", _b_clusterT_eta, "clusterT_eta[clusterT_n]/F");
  _tree->Branch("clusterT_phi", _b_clusterT_phi, "clusterT_phi[clusterT_n]/F");

  _tree->Branch("clusterT_ntower", _b_clusterT_ntower, "clusterT_ntower[clusterT_n]/I");
  _tree->Branch("clusterT_tower_phi", &_b_clusterT_tower_phi);
  _tree->Branch("clusterT_tower_eta", &_b_clusterT_tower_eta);
  _tree->Branch("clusterT_tower_layer", &_b_clusterT_tower_layer);

  _tree->Branch("clusterT2_n", &_b_clusterT2_n, "clusterT2_n/I");
  _tree->Branch("clusterT2_E", _b_clusterT2_E, "clusterT2_E[clusterT2_n]/F");
  _tree->Branch("clusterT2_eta", _b_clusterT2_eta, "clusterT2_eta[clusterT2_n]/F");
  _tree->Branch("clusterT2_phi", _b_clusterT2_phi, "clusterT2_phi[clusterT2_n]/F");

  _tree->Branch("clusterT2_ntower", _b_clusterT2_ntower, "clusterT2_ntower[clusterT2_n]/I");
  _tree->Branch("clusterT2_tower_phi", &_b_clusterT2_tower_phi);
  _tree->Branch("clusterT2_tower_eta", &_b_clusterT2_tower_eta);
  _tree->Branch("clusterT2_tower_layer", &_b_clusterT2_tower_layer);


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
  //Clear all vectors

  _b_cluster_tower_phi.clear();
  _b_cluster_tower_eta.clear();
  _b_cluster_tower_layer.clear();

  _b_clusterT_tower_phi.clear();
  _b_clusterT_tower_eta.clear();
  _b_clusterT_tower_layer.clear();

  _b_clusterT2_tower_phi.clear();
  _b_clusterT2_tower_eta.clear();
  _b_clusterT2_tower_layer.clear();

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

  RawClusterContainer *clustersEM = findNode::getClass<RawClusterContainer>(topNode, "CLUSTER_CEMC");
  RawClusterContainer *clustersT = findNode::getClass<RawClusterContainer>(topNode, "TOPOCLUSTER_EMCAL");
  RawClusterContainer *clustersT2 = findNode::getClass<RawClusterContainer>(topNode, "TOPOCLUSTER_ALLCALO");


  //std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  _b_tower_sim_n = 0;
  _b_tower_raw_n = 0;
  _b_tower_calib_n = 0;

  //Zero the clusters
  _b_cluster_n = 0;
  _b_clusterT_n = 0;
  _b_clusterT2_n = 0;

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

  
  processClusters(clustersEM, towersCalibEM4, towersCalibIH4, towersCalibOH4, geomCEMC, geomIH, geomOH, &_b_cluster_n, _b_cluster_eta, _b_cluster_phi, _b_cluster_E, _b_cluster_ntower, &_b_clusterT_tower_phi, &_b_clusterT_tower_eta, &_b_clusterT_tower_layer);
  processClusters(clustersT, towersCalibEM4, towersCalibIH4, towersCalibOH4, geomCEMC, geomIH, geomOH, &_b_clusterT_n, _b_clusterT_eta, _b_clusterT_phi, _b_clusterT_E, _b_clusterT_ntower, &_b_clusterT_tower_phi, &_b_clusterT_tower_eta, &_b_clusterT_tower_layer);
  processClusters(clustersT2, towersCalibEM4, towersCalibIH4, towersCalibOH4, geomCEMC, geomIH, geomOH, &_b_clusterT2_n, _b_clusterT2_eta, _b_clusterT2_phi, _b_clusterT2_E, _b_clusterT2_ntower, &_b_clusterT2_tower_phi, &_b_clusterT2_tower_eta, &_b_clusterT2_tower_layer);  

  _b_part_n = 0;

  //Lets grab truth info too (Based on macro coresoftware/offline/packages/jetbackground/DetermineTowerBackground as of 2019.08.02
  PHG4TruthInfoContainer *truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");

  PHG4VtxPoint *point = truthinfo->GetPrimaryVtx(truthinfo->GetPrimaryVertexIndex());
  _b_truth_vx = point->get_x();
  _b_truth_vy = point->get_y();
  _b_truth_vz = point->get_z();

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

//Really this should just be a class but I leave that to others
void TreeMaker::processClusters(RawClusterContainer* clusters, RawTowerContainer* towersEM, RawTowerContainer* towersIH, RawTowerContainer* towersOH, RawTowerGeomContainer* geomEM, RawTowerGeomContainer* geomIH, RawTowerGeomContainer* geomOH, int* n, float* eta, float* phi, float* E, int* ntow, std::vector<std::vector<float> >* towphi, std::vector<std::vector<float> >* toweta, std::vector<std::vector<int> >* towlayer)
{
  (*n) = 0;

  RawClusterContainer::ConstIterator hiter;
  RawClusterContainer::ConstRange begin_end = clusters->getClusters();
  for(hiter = begin_end.first; hiter != begin_end.second; ++hiter){
    float theta = 3.14159/2.0 - TMath::ATan2(hiter->second->get_z(), hiter->second->get_r());
    float tempEta = -1.0*log(tan(theta/2.0));

    if(hiter->second->get_energy() < 0.1) continue; //make a safe minumum cut on cluster energy

    eta[*n] = tempEta;
    phi[*n] = hiter->second->get_phi();
    E[*n] = hiter->second->get_energy();
    ntow[*n] =  hiter->second->getNTowers();

    towphi->push_back({});
    toweta->push_back({});
    towlayer->push_back({});

    RawCluster::TowerConstRange begin_end = hiter->second->get_towers();
    for(RawCluster::TowerConstIterator iter = begin_end.first; iter != begin_end.second; ++iter){
      RawTower* tower;
      RawTowerGeom *tower_geom;

      if(RawTowerDefs::decode_caloid(iter->first) == RawTowerDefs::CalorimeterId::CEMC){
	tower = towersEM->getTower(iter->first);
	tower_geom = geomEM->get_tower_geometry(tower->get_key());
	towlayer->at(towlayer->size()-1).push_back(3);
	towphi->at(towphi->size()-1).push_back(tower_geom->get_phi());
	toweta->at(toweta->size()-1).push_back(tower_geom->get_eta());
      }
      else if(RawTowerDefs::decode_caloid(iter->first) == RawTowerDefs::CalorimeterId::HCALIN){
	tower = towersIH->getTower(iter->first);
	tower_geom = geomIH->get_tower_geometry(tower->get_key());
	towlayer->at(towlayer->size()-1).push_back(4);
	towphi->at(towphi->size()-1).push_back(tower_geom->get_phi());
	toweta->at(toweta->size()-1).push_back(tower_geom->get_eta());
      }
      else if(RawTowerDefs::decode_caloid(iter->first) == RawTowerDefs::CalorimeterId::HCALOUT){
	tower = towersOH->getTower(iter->first);
	tower_geom = geomOH->get_tower_geometry(tower->get_key());
	towlayer->at(towlayer->size()-1).push_back(5);
	towphi->at(towphi->size()-1).push_back(tower_geom->get_phi());
	toweta->at(toweta->size()-1).push_back(tower_geom->get_eta());
      }      
    }
    
    ++(*n);
  }


  return;
}
