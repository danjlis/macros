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
#include "TF2.h"
#include <g4main/PHG4InEvent.h>

class PHG4InEvent;
class PHCompositeNode;

class CosmicSpray : public PHG4ParticleGeneratorBase
{
 public:
  CosmicSpray(const std::string &name, const double &y_top_fix, const double &y_bot_fix, const double &z_max, const double &x_max, const double &x_min, const int &debug);
  ~CosmicSpray() override {}
  int InitRun(PHCompositeNode *topNode) override;
  int process_event(PHCompositeNode *topNode) override;
  void add_particle(const std::string &name, const unsigned int count);
private:

  static double TopDistributionFunction(double *val, double *par);
  static double BottomDistributionFunction(double *val, double *par);

  static double _x_min;
  static double _x_max;
  static double _z_max;
  static double _y_top_fix;
  static double _y_bot_fix;
  static double _y_fix;
  int _debug;

  TF2 *_ftop;
  TF2 *_fbottom;

  PHG4InEvent *_InEvent = nullptr;
  std::vector<std::pair<std::string, unsigned int> > _particle_names;  // <names, count>
  std::vector<std::pair<int, unsigned int> > _particle_codes;          // <pdgcode, count>
};
#endif
