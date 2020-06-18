//ORIGINAL MACRO BY DENNIS PERIPELITSA
//UPDATE BY CHRIS MCGINN (2019.08.01)
#ifndef __TREE_MAKER_H__
#define __TREE_MAKER_H__

//cpp
#include <string>
#include <vector>

//ROOT
#include "TFile.h"
#include "TTree.h"

//CoreSoftwaree
#include <coresoftware/offline/framework/fun4all/SubsysReco.h>
#include <coresoftware/offline/packages/CaloBase/RawTowerGeomContainer_Cylinderv1.h>
#include <coresoftware/offline/packages/CaloBase/RawTowerGeomContainer.h>
#include <coresoftware/offline/packages/CaloBase/RawTowerContainer.h>     

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

 private:

  int _verbosity;

  TFile *_f;

  TTree *_tree;

  std::string _foutname;

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

  void processTowersSim(RawTowerContainer* towers, RawTowerGeomContainer* geom, int layer);
  void processTowersRaw(RawTowerContainer* towers, RawTowerGeomContainer* geom, int layer);
  void processTowersCalib(RawTowerContainer* towers, RawTowerGeomContainer* geom, int layer);
};

#endif 
