//RandomGunStuff
#include <iostream>
#include "RandomGunStuff.h"

#include <fun4all/Fun4AllServer.h>
#include "TROOT.h"
#include "TF2.h"
#include "TVector3.h"
#include "TRandom.h"

double RandomGunStuff::_y_fix;
double RandomGunStuff::_y_top_fix;
double RandomGunStuff::_y_bot_fix;

double RandomGunStuff::_x_max;
double RandomGunStuff::_x_min;
double RandomGunStuff::_z_max;


RandomGunStuff::RandomGunStuff(double &y_top_fix, double &y_bot_fix, double &z_max, double &x_max, double &x_min, int &debug) {
  _y_top_fix = y_top_fix;
  _y_bot_fix = y_bot_fix;
  _y_fix = y_top_fix - y_bot_fix;
  _z_max = z_max;
  _x_max = x_max;
  _x_min = x_min;
  _debug = debug;
  _ftop = new TF2("ftop",RandomGunStuff::TopDistributionFunction,-1*z_max,z_max,x_min,x_max);
  _fbottom = new TF2("fbottom",RandomGunStuff::BottomDistributionFunction,-1*z_max,z_max,x_min,x_max, 2);
}

void RandomGunStuff::GetRandomTrajectory(double &gun_e, double &gun_x, double &gun_y, double &gun_z, double &gun_px, double &gun_py, double &gun_pz) {
  gRandom->SetSeed(0);
  if (_debug)std::cout<<"Set the seed"<<std::endl;

  _ftop->GetRandom2(gun_x, gun_z);
  if (_debug)std::cout<<"gun_x and gun_z"<<gun_x<<" / "<<gun_z<<std::endl;

  _fbottom->SetParameters(gun_x, gun_y);
  double botx, botz;
  _fbottom->GetRandom2(botx, botz);
  double dx = botx-gun_x;
  double dz = botz-gun_z;
  TVector3 r(dx,_y_fix, dz);
  TVector3 dir = r.Unit();
  dir.SetMag(4.);
  gun_y = _y_top_fix;
  gun_px = dir.X();
  gun_py = dir.Y();
  gun_pz = dir.Z();
  if (_debug)std::cout<<"gun_p"<<gun_px<<" / "<<gun_py<<" / "<<gun_pz<<std::endl;

}

double RandomGunStuff::TopDistributionFunction(double *val, double *par) {
  double a1 = atan2(abs(_x_min - val[0]), _y_fix);
  double a2 = atan2(_z_max - val[1], _y_fix);
  double b1 = atan2(abs(_x_max - val[0]), _y_fix);
  double b2 = atan2(_z_max + val[1], _y_fix);
  double r1 = a1+b1+(1/2)*(sin(2*a1)+sin(2*b1));
  double r2 = a2+b2+(1/2)*(sin(2*a2)+sin(2*b2));
  return r1*r2/4;
}
double RandomGunStuff::BottomDistributionFunction(double *val, double *par) {
  double a1 = atan2(val[0]-par[0],_y_fix);
  double a2 = atan2(val[1]-par[1],_y_fix);
  double p = cos(a1)*cos(a1)*cos(a2)*cos(a2);
  return p;
}
