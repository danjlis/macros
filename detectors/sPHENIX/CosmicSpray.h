// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef G4MAIN_COSMICSPRAY_H
#define G4MAIN_COSMICSPRAY_H

#include <g4main/PHG4ParticleGeneratorBase.h>

#include <cmath>
#include <map>
#include <string>   // for string
#include <utility>  // for pair
#include <vector>
#include "TF3.h"
#include <g4main/PHG4InEvent.h>

class PHG4InEvent;
class PHCompositeNode;

class CosmicSpray : public PHG4ParticleGeneratorBase
{
 public:
  CosmicSpray(const std::string &name, const std::string &detector, const int &debug);
  ~CosmicSpray() override {}
  int process_event(PHCompositeNode *topNode) override;
  void add_particle(const std::string &name, const unsigned int count);
private:
  static double EnergyAngularDistribution(double *val, double *par);
  static double GetBeta(double &gamma);
  static double GetGamma0(double &gamma);
  TF1 *_f_vertex;
  TF3 *_f_momentum;

  static double _gun_e;
  static double _x_min;
  static double _x_max;
  static double _z_max;
  static double _y_top_fix;
  static double _y_bot_fix;

  static double _R_earth;
  static double _d_earth;

  int _debug;

  PHG4InEvent *_InEvent = nullptr;
  std::vector<std::pair<std::string, unsigned int> > _particle_names;  // <names, count>
  std::vector<std::pair<int, unsigned int> > _particle_codes;          // <pdgcode, count>
};
#endif
