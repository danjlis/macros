// RandomGunStuff Header File
#ifndef __RANDOMGUNSTUFF_H__
#define __RANDOMGUNSTUFF_H__

#include <fun4all/SubsysReco.h>
#include <TMath.h>
#include "TROOT.h"
#include "TF2.h"
#include "TH2.h"

class RandomGunStuff {
public:
  RandomGunStuff(double &y_top_fix, double &y_bot_fix, double &z, double &x_max, double &x_min, int &debug);
  void GetRandomTrajectory(double &gun_e, double &gun_x, double &gun_y, double &gun_z, double &gun_px, double &gun_py, double &gun_pz);

private:

  static double TopDistributionFunction(double *var, double *par);
  static double BottomDistributionFunction(double *var, double *par);

  TF2 *_ftop;
  TF2 *_fbottom;
  int _debug;
  static double _y_top_fix;
  static double _y_bot_fix;
  static double _y_fix;
  static double _z_max;
  static double _x_max;
  static double _x_min;
};
#endif
