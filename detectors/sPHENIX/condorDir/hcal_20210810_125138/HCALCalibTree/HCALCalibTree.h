// HCAL Calibration TreeMaker

#ifndef __HCALCALIBTREE_H__
#define __HCALCALIBTREE_H__

#include <fun4all/SubsysReco.h>
#include <string>
#include <vector>
#include <map>
#include "TTree.h"
#include "TFile.h"
#include "TH2.h"

#include <g4main/PHG4TruthInfoContainer.h>
#include <phool/PHCompositeNode.h>

#include <calobase/RawTowerGeomContainer_Cylinderv1.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calobase/RawTowerContainer.h>

class PHCompositeNode;
class RawTowerContainer;
class PHG4TruthInfoContainer;
class RawTowerGeomContainer;

class HCALCalibTree: public SubsysReco {
 public:
  
  HCALCalibTree(const std::string &name = "HCALCalibTree.root", const int &startingSector = 0, const int &numSectors = 32, const double &o = 50., const double &e_max = 10., const double &x_min = 183., const double &x_max = 264.23, const double &z_max = 304.91, const double &y_max = 200, const int &nevents = 10000);
  
  virtual ~HCALCalibTree() {}
  
  int Init(PHCompositeNode *);
  
  int reset_tree_vars();

  int GetNodes(PHCompositeNode *);
  
  int process_event(PHCompositeNode *);
  
  int End(PHCompositeNode *);
  
 private:
  std::string _foutname;
  std::string _truthinfo_name;
  std::string _hcal_raw_name;
  std::string _hcal_sim_name;
  std::string _hcal_calib_name;
  int _startingSector, _numSectors;
  RawTowerContainer* _towersSimOH;
  RawTowerContainer* _towersRawOH;
  RawTowerContainer* _towersCalibOH;
  PHG4TruthInfoContainer* _truthinfo;
  RawTowerGeomContainer* _geomOH;
  TTree* _tree;
  TFile* _file;
  
  int _ievent, _b_event;
  float _b_truth_vx, _b_truth_vy, _b_truth_vz;

  static const bool _debug = false;
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
  float _b_part_E[nParts];
  float _b_part_px[nParts];
  float _b_part_py[nParts];
  float _b_part_pz[nParts];
  int _b_part_pdgid[nParts];
  
  TH1F* _h_edep_list[12];
  TH1F * _h_vertex_y_dist;
  TH2F* _h_vertex_dist;
  TH2F* _h_momentum_dist;
  TH1F* _h_momentum_y;
  TH1F* _h_edep_21;
  TH1F* _h_edep_22;
  TH1F* _h_edep_23;
  TH1F* _h_edep_24;
  TH1F* _h_edep_25;
  TH1F* _h_edep_26;
  TH1F* _h_edep_27;
  TH1F* _h_edep_28;
  TH1F* _h_edep_29;
  TH1F* _h_edep_30;
  TH1F* _h_edep_31;
  TH1F* _h_edep_32;

};

#endif
