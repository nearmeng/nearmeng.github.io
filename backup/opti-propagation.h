
#ifndef __opti_propagation_h__
#define __opti_propagation_h__

#include <packet-stamp.h>
#include <wireless-phy.h>
#include <propagation.h>

class OptiPropagation : public Propagation {
public:
  OptiPropagation();
  virtual double Pr(PacketStamp *tx, PacketStamp *rx, WirelessPhy *ifp,int flag1,int flag2,int flag3,int flag4);
  virtual double getDist(double Pr, double Pt, double Gt, double Gr,
			 double hr, double ht, double L, double lambda);
	//add by huangxin find the best antenna
//  virtual int which_antenna(double rx,double ry,double rz);

protected:
  double Opti(double Pt, double Gt, double Gr, double ht, double hr, double L, double d);
//  double last_hr, last_ht;
//  double crossover_dist;
};


#endif /* __opti_propagation_h__ */
