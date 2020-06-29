//ORIGINAL MACRO BY DENNIS PERIPELITSA
//UPDATE BY CHRIS MCGINN (2019.08.01)
#ifndef __TREE_MAKER_H__
#define __TREE_MAKER_H__

//cpp
#include <string>
#include <vector>

//ROOT
#include "TEnv.h"
#include "TFile.h"
#include "TTree.h"

//CoreSoftwaree
#include <fun4all/SubsysReco.h>
#include <calobase/RawTowerGeomContainer_Cylinderv1.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calobase/RawTowerContainer.h>     
#include <calobase/RawClusterContainer.h>

class PHCompositeNode;

class TreeMaker: public SubsysReco
{

 public:

  TreeMaker(const std::string &name="TreeMaker.root");

  int Init(PHCompositeNode*);
  int process_event(PHCompositeNode*);  
  int End(PHCompositeNode*);

  void SetVerbosity( int verb ) {
    _verbosity = verb;
  }

  TEnv m_config;  

 private:

  int _verbosity;

  TFile *_f;

  TTree *_tree;

  std::string _foutname;

  float _b_truth_vx;
  float _b_truth_vy;
  float _b_truth_vz;

  static const int nTowers = 100000;
  int _b_tower_sim_n;
  int _b_tower_sim_layer[nTowers];
  float _b_tower_sim_E[nTowers];
  float _b_tower_sim_eta[nTowers];
  float _b_tower_sim_phi[nTowers];

  int _b_tower_raw_n;
  int _b_tower_raw_layer[nTowers];
  float _b_tower_raw_E[nTowers];
  float _b_tower_raw_eta[nTowers];
  float _b_tower_raw_phi[nTowers];

  int _b_tower_calib_n;
  int _b_tower_calib_layer[nTowers];
  float _b_tower_calib_E[nTowers];
  float _b_tower_calib_eta[nTowers];
  float _b_tower_calib_phi[nTowers];

  static const int nParts = 100000;
  int _b_part_n;
  float _b_part_pt[nParts];
  float _b_part_eta[nParts];
  float _b_part_phi[nParts];
  float _b_part_m[nParts];
  int _b_part_pdgid[nParts];

  static const int nClusters = 10000;
  //Standard photon reco. clusters
  int _b_cluster_n;
  float _b_cluster_eta[nClusters];
  float _b_cluster_phi[nClusters];
  float _b_cluster_E[nClusters];

  //Towers w/in each cluster
  int _b_cluster_ntower[nClusters];
  std::vector< std::vector<float> > _b_cluster_tower_phi;
  std::vector< std::vector<float> > _b_cluster_tower_eta;
  std::vector< std::vector<int> > _b_cluster_tower_layer;

  //topoclusters, just emcal (effectively 2D version of topo algo)
  int _b_clusterT_n;
  float _b_clusterT_eta[nClusters];
  float _b_clusterT_phi[nClusters];
  float _b_clusterT_E[nClusters];

  //Towers w/in each cluster
  int _b_clusterT_ntower[nClusters];
  std::vector< std::vector<float> > _b_clusterT_tower_phi;
  std::vector< std::vector<float> > _b_clusterT_tower_eta;
  std::vector< std::vector<int> > _b_clusterT_tower_layer;

  //topoclusters, all three layers
  int _b_clusterT2_n;
  float _b_clusterT2_eta[nClusters];
  float _b_clusterT2_phi[nClusters];
  float _b_clusterT2_E[nClusters];

  //Towers w/in each cluster
  int _b_clusterT2_ntower[nClusters];
  std::vector< std::vector<float> > _b_clusterT2_tower_phi;
  std::vector< std::vector<float> > _b_clusterT2_tower_eta;
  std::vector< std::vector<int> > _b_clusterT2_tower_layer;


  void processTowersSim(RawTowerContainer* towers, RawTowerGeomContainer* geom, int layer);
  void processTowersRaw(RawTowerContainer* towers, RawTowerGeomContainer* geom, int layer);
  void processTowersCalib(RawTowerContainer* towers, RawTowerGeomContainer* geom, int layer);
  void processClusters(RawClusterContainer* clusters, RawTowerContainer* towersEM, RawTowerContainer* towersIH, RawTowerContainer* towersOH, RawTowerGeomContainer* geomEM, RawTowerGeomContainer* geomIH, RawTowerGeomContainer* geomOH, int* n, float* eta, float* phi, float* E, int* ntow, std::vector<std::vector<float> >* towphi, std::vector<std::vector<float> >* toweta, std::vector<std::vector<int> >* towlayer);
};

#endif 
