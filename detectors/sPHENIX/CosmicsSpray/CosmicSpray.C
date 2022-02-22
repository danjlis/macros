//CosmicSpray
#include "CosmicSpray.h"

#include <g4main/PHG4InEvent.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4Particlev2.h>
#include <g4main/PHG4Utils.h>
#include <g4main/PHG4ParticleGeneratorBase.h>
#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHDataNode.h>      // for PHDataNode
#include <phool/PHNode.h>          // for PHNode
#include <phool/PHNodeIterator.h>  // for PHNodeIterator
#include <phool/PHObject.h>        // for PHObject
#include <phool/getClass.h>
#include <phool/phool.h>  // for PHWHERE

#include <iostream>
#include <TSystem.h>
#include "TROOT.h"
#include "TF2.h"
#include "TMath.h"
#include "TRandom.h"
#include "TVector3.h"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>  // for operator<<, endl, basic_ostream
#include <memory>    // for allocator_traits<>::value_type

#include <vector>  // for vector, vector<>::const_iterator

double CosmicSpray::_y_fix;
double CosmicSpray::_y_top_fix;
double CosmicSpray::_y_bot_fix;
double CosmicSpray::_x_max;
double CosmicSpray::_x_min;
double CosmicSpray::_z_max;

class PHCompositeNode;
class PHG4Particle;
class PHG4ParticleGeneratorBase;

CosmicSpray::CosmicSpray(const std::string &name = "COSMICS", const double &y_top_fix = 53., const double &y_bot_fix = 43., const double &z_max = 304.91, const double &x_max = 264.71, const double &x_min = 183.3, const int &debug = 1)
  : PHG4ParticleGeneratorBase(name)
{
  _y_top_fix = y_top_fix;
  _y_bot_fix = y_bot_fix;
  _y_fix = y_top_fix - y_bot_fix;
  _z_max = z_max;
  _x_max = x_max;
  _x_min = x_min;
  _debug = debug;
  _ftop = new TF2("ftop",CosmicSpray::TopDistributionFunction,x_min,x_max, -1*z_max, z_max);
  _fbottom = new TF2("fbottom",CosmicSpray::BottomDistributionFunction, x_min,x_max, -1*z_max, z_max, 2);
  return;
}

void CosmicSpray::add_particle(const std::string &name, const unsigned int num)
{
  _particle_names.clear();
  _particle_names.push_back(std::make_pair(name, num));
  return;
}

int CosmicSpray::InitRun(PHCompositeNode *topNode) {
  _InEvent = findNode::getClass<PHG4InEvent>(topNode, "PHG4INEVENT");
  return Fun4AllReturnCodes::EVENT_OK;
}

int CosmicSpray::process_event(PHCompositeNode *topNode)
{
  // set_vertex
  std::string pdgname;
  for (unsigned int i = 0; i < _particle_names.size(); ++i){
    pdgname = _particle_names[i].first;
  }
  int pdgcode = get_pdgcode(pdgname);
  gRandom->SetSeed(0);
  double gun_x, gun_y, gun_z;
  double gun_px, gun_py, gun_pz;
  _ftop->GetRandom2(gun_x, gun_z);
  _fbottom->SetParameters(gun_x, gun_z);
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
  int trackid = 0;
  double gun_t = 0.0;
  double gun_e = 4;
  int vtxindex = _InEvent->AddVtx(gun_x, gun_y ,gun_z,gun_t);

  PHG4Particle *particle = new PHG4Particlev2();
  particle->set_track_id(trackid);
  particle->set_vtx_id(vtxindex);
  particle->set_parent_id(0);
  particle->set_name(pdgname);
  particle->set_pid(pdgcode);
  particle->set_px(gun_px);
  particle->set_py(gun_py);
  particle->set_pz(gun_pz);
  particle->set_e(gun_e);

  _InEvent->AddParticle(vtxindex, particle);
  return 0;
}

double CosmicSpray::TopDistributionFunction(double *val, double *par) {
  double a1 = atan2(abs(_x_min - val[0]), _y_fix);
  double a2 = atan2(_z_max - val[1], _y_fix);
  double b1 = atan2(abs(_x_max - val[0]), _y_fix);
  double b2 = atan2(_z_max + val[1], _y_fix);
  double r1 = a1+b1+(1/2)*(sin(2*a1)+sin(2*b1));
  double r2 = a2+b2+(1/2)*(sin(2*a2)+sin(2*b2));
  return r1*r2/4;
}
double CosmicSpray::BottomDistributionFunction(double *val, double *par) {
  double a1 = atan2(val[0]-par[0],_y_fix);
  double a2 = atan2(val[1]-par[1],_y_fix);
  double p = cos(a1)*cos(a1)*cos(a2)*cos(a2);
  return p;
}
