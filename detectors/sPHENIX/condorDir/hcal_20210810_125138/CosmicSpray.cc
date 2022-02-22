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


double CosmicSpray::_x_max;
double CosmicSpray::_z_max;
double CosmicSpray::_gun_e;
double CosmicSpray::_R_earth;
double CosmicSpray::_d_earth;


class PHCompositeNode;
class PHG4Particle;
class PHG4ParticleGeneratorBase;

CosmicSpray::CosmicSpray(const std::string &name = "COSMICS", const double &gun_e = 4., const double &z_max = 304.91, const double &x_max = 264.71, const int &debug = 1)
  : PHG4ParticleGeneratorBase(name)
{
  _gun_e = gun_e;
  _z_max = z_max;
  _x_max = x_max;
  _debug = debug;
  _R_earth = 637100000;
  _d_earth = 1000000;
  _f_vertex = new TF1("_f_vertex",CosmicSpray::choose_vertex, -1*TMath::Pi()/2, TMath::Pi()/2);
  _f_momentum = new TF2("_f_momentum",CosmicSpray::choose_momentum,-1*TMath::Pi()/2, TMath::Pi()/2, -1*TMath::Pi()/2, TMath::Pi()/2);

  return;
}

void CosmicSpray::set_energy(const double &e){
  _gun_e = e;
}
void CosmicSpray::add_particle(const std::string &name, const unsigned int num)
{
  _particle_names.clear();
  _particle_names.push_back(std::make_pair(name, num));
  return;
}

int CosmicSpray::process_event(PHCompositeNode *topNode)
{
  // set_vertex
  if(_debug) std::cout<<"Processing Event"<<std::endl;
  std::string pdgname;
  for (unsigned int i = 0; i < _particle_names.size(); ++i){
    pdgname = _particle_names[i].first;
    if(_debug) std::cout<<"Particle added: "<<pdgname << std::endl;
  }
  gRandom->SetSeed(0);
  int pdgcode = get_pdgcode(pdgname);
  int trackid = 0;
  double gun_t = 0.;
  double gun_x, gun_y, gun_z;
  double gun_px, gun_py, gun_pz;
  TRandom *randomGen = new TRandom();
  randomGen->SetSeed();
  gun_z = randomGen->Uniform(-1*_z_max, _z_max);
  double theta_ran;
  theta_ran = _f_vertex->GetRandom();
  gun_x = _x_max * sin(theta_ran);
  gun_y = _x_max * cos(theta_ran);
  if(_debug) std::cout<<"Vertex: "<<gun_x<<" / " <<gun_y<<" / "<<gun_z<<std::endl;
  double tr, pr;
  TVector3 axis;
  if (theta_ran > 0) {
    axis = TVector3(1, 0, 0);
    _f_momentum->SetRange(-1*TMath::Pi()/2, TMath::Pi()/2 - theta_ran);
  }
  else {
    axis = TVector3(-1, 0, 0);
    _f_momentum->SetRange(-1*(TMath::Pi()/2 + theta_ran), TMath::Pi()/2);
  }
  _f_momentum->GetRandom2(tr, pr);
  TVector3 momo = TVector3(0, -1*_gun_e, 0);
  momo.RotateZ(tr);
  axis.RotateZ(tr);
  momo.Rotate(pr, axis);
  gun_px = momo.X();
  gun_py = momo.Y();
  gun_pz = momo.Z();

  TVector3 gun_p(gun_px, gun_py, gun_pz);
  TVector3 dir = gun_p.Unit();
  //if(_debug) std::cout<<"Dir: "<<dx<<" / " <<dy<< " / " <<dz<<std::endl;
  if(_debug) std::cout<<"Norm Dir: "<<dir.X()<<" / " <<dir.Y()<<" / "<<dir.Z()<<std::endl;

  if(_debug) std::cout<<"Momentum: "<<gun_px<<" / "<<gun_py<<" / "<<gun_pz<<std::endl;
  if(_debug)std::cout<<"total mom: "<<_gun_e<<std::endl;
  if(_debug)std::cout<<"Before adding vertex"<<std::endl;
  _InEvent = findNode::getClass<PHG4InEvent>(topNode, "PHG4INEVENT");

  int vtxindex = _InEvent->AddVtx(gun_x, gun_y ,gun_z,gun_t);
  if(_debug)std::cout<<"After adding vertex"<<std::endl;

  PHG4Particle *particle = new PHG4Particlev2();
  particle->set_track_id(trackid);
  if(_debug)std::cout<<"track_id: "<<trackid<<std::endl;
  particle->set_vtx_id(vtxindex);
  if(_debug)std::cout<<"vtxindex: "<<vtxindex<<std::endl;
  particle->set_parent_id(0);
  if(_debug)std::cout<<"parent_id "<<std::endl;
  particle->set_name(pdgname);
  if(_debug)std::cout<<"pdgname: "<<pdgname<<std::endl;
  particle->set_pid(pdgcode);
  if(_debug)std::cout<<"pdgcode: "<<pdgcode<<std::endl;
  particle->set_px(gun_px);
  if(_debug)std::cout<<"px"<<std::endl;
  particle->set_py(gun_py);
  if(_debug)std::cout<<"py"<<std::endl;
  particle->set_pz(gun_pz);
  if(_debug)std::cout<<"pz"<<std::endl;
  particle->set_e(_gun_e);
  if(_debug)std::cout<<"ene"<<std::endl;

  _InEvent->AddParticle(vtxindex, particle);
  if(_debug)std::cout<<"left COSMICS"<<std::endl;

  return 0;
}

  double CosmicSpray::choose_vertex(double *var, double *par){
    double p = 0.0007*cos(63*var[0]/48) + 0.0007;
    return p;
  }
  double CosmicSpray::choose_momentum(double *var, double *par){
    double p = 1 - TMath::Power(_R_earth/(_R_earth+_d_earth),2)*(1 - cos(var[0])*cos(var[1])*cos(var[0])*cos(var[1]));
    return p;
  }
