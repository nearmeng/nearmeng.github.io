#include <math.h>
#include <delay.h>
#include <packet.h>
#include <stdio.h>

#include <packet-stamp.h>
#include <antenna.h>
#include <mobilenode.h>
#include <propagation.h>
#include <wireless-phy.h>
#include <opti-propagation.h>

static class OptiPropagationClass: public TclClass {
public:
        OptiPropagationClass() : TclClass("Propagation/OptiPropagation") {}
        TclObject* create(int, const char*const*) {
                return (new OptiPropagation);
        }
} class_optipropagation;

OptiPropagation::OptiPropagation()
{
//  last_hr = last_ht = 0.0;
//  crossover_dist = 0.0;
}
double OptiPropagation::Opti(double Pt, double Gt, double Gr, double ht, double hr, double L, double d)
{
        /*
         *  
         *
         *	       Pt * Gt * Gr 
         *  Pr = -----------------
         *           d^2 * L
         *
         * The original equation assumes L = 1.
         * 
         */
  return Pt * Gt * Gr / (d * d  * 25 * L);
}

//add by huangxin
/*
int OptiPropagation::which_antenna(double rx,double ry,double rz)
{
	
}
*/

double
OptiPropagation::Pr(PacketStamp *t, PacketStamp *r, WirelessPhy *ifp,int flag1,int flag2,int flag3,int flag4)
{
  FILE *fp1;
  int i;
  int AnNow;
  double rX, rY, rZ;		// location of receiver
  double tX, tY, tZ;		// location of transmitter
  double rXa[4],tXa[4],rYa[4],tYa[4];		//location of Antenna
  double Pr_temp;
  double Dista;          //shift of the location of antennas.
  double d,dt[4];				// distance between the receiver and transmitter
//  double hr, ht;		// height of recv and xmit antennas
  double Pr,Prt;			// received signal power
  double Gt,Gr,Gtt,Grt;         //Gain of antenna
  double L = ifp->getL();			// system loss
  double lambda = ifp->getLambda();	// wavelength

  r->getNode()->getLoc(&rX, &rY, &rZ);
  t->getNode()->getLoc(&tX, &tY, &tZ);
  
  rX += r->getAntenna()->getX();
  rY += r->getAntenna()->getY();
  rZ += r->getAntenna()->getZ();
  tX += t->getAntenna()->getX();
  tY += t->getAntenna()->getY();
  tZ += t->getAntenna()->getZ();
  

  if(t->getAntenna()->getF()==2)
  {
 
    Dista=t->getAntenna()->getDista();
	tXa[0]=tX+Dista; tXa[1]=tX+Dista; tXa[2]=tX-Dista; tXa[3]=tX-Dista;
	tYa[0]=tY+Dista; tYa[1]=tY-Dista; tYa[2]=tY+Dista; tYa[3]=tY-Dista;
	Pr=0;d=0;Gt=0;Gr=0;AnNow=0;
	
	if(flag1 == 1)
	{
  		dt[3] = sqrt((rX - tXa[3]) * (rX - tXa[3]) 
	  	 + (rY - tYa[3]) * (rY - tYa[3]) 
	  	 + (rZ - tZ) * (rZ - tZ));

 		Gtt = t->getAntenna()->getTxGain(rX - tXa[3], rY - tYa[3], rZ - tZ, 
					 t->getLambda());
		Grt = r->getAntenna()->getRxGain(tXa[3] - rX, tYa[3] - rY, tZ - rZ,
					 r->getLambda());

    	Pr = Pr + Opti(t->getTxPr(), Gtt, Grt, tZ, rZ, L, dt[3]);

//		printf("TxPr:%f Gtt:%f Grt:%f d:%f Pr:%f\n",t->getTxPr(),Gtt,Grt,dt[3],Pr);

	}
	if(flag2 == 1)
	{
		
  		dt[1] = sqrt((rX - tXa[1]) * (rX - tXa[1]) 
	  	 + (rY - tYa[1]) * (rY - tYa[1]) 
	  	 + (rZ - tZ) * (rZ - tZ));

 		Gtt = t->getAntenna()->getTxGain(rX - tXa[1], rY - tYa[1], rZ - tZ, 
					 t->getLambda());
		Grt = r->getAntenna()->getRxGain(tXa[1] - rX, tYa[1] - rY, tZ - rZ,
					 r->getLambda());

    	Pr = Pr + Opti(t->getTxPr(), Gtt, Grt, tZ, rZ, L, dt[1]);
	
//		{
//			printf("TxPr:%f Gtt:%f Grt:%f d:%f Pr:%f\n",t->getTxPr(),Gtt,Grt,dt[1],Pr);
//			printf("tX:%f tY:%f tZ:%f rX:%f rY:%f rZ:%f\n",tXa[1],tYa[1],tZ,rX,rY,rZ);
//		}
	}
	if(flag3 == 1)
	{
		
  		dt[0] = sqrt((rX - tXa[0]) * (rX - tXa[0]) 
	  	 + (rY - tYa[0]) * (rY - tYa[0]) 
	  	 + (rZ - tZ) * (rZ - tZ));

 		Gtt = t->getAntenna()->getTxGain(rX - tXa[0], rY - tYa[0], rZ - tZ, 
					 t->getLambda());
		Grt = r->getAntenna()->getRxGain(tXa[0] - rX, tYa[0] - rY, tZ - rZ,
					 r->getLambda());

    	Pr = Pr + Opti(t->getTxPr(), Gtt, Grt, tZ, rZ, L, dt[0]);
//		printf("TxPr:%f Gtt:%f Grt:%f d:%f Pr:%f\n",t->getTxPr(),Gtt,Grt,dt[0],Pr);
	}
	if(flag4 == 1)
	{
		
  		dt[2] = sqrt((rX - tXa[2]) * (rX - tXa[2]) 
	  	 + (rY - tYa[2]) * (rY - tYa[2]) 
	  	 + (rZ - tZ) * (rZ - tZ));

 		Gtt = t->getAntenna()->getTxGain(rX - tXa[2], rY - tYa[2], rZ - tZ, 
					 t->getLambda());
		Grt = r->getAntenna()->getRxGain(tXa[2] - rX, tYa[2] - rY, tZ - rZ,
					 r->getLambda());

    	Pr = Pr + Opti(t->getTxPr(), Gtt, Grt, tZ, rZ, L, dt[2]);
//		printf("TxPr:%f Gtt:%f Grt:%f d:%f Pr:%f\n",t->getTxPr(),Gtt,Grt,dt[2],Pr);
	}
	
//	printf("\nant:%d %d %d %d Pr:%e\n",flag1,flag2,flag3,flag4,Pr);
//	printf("Now is use Antenna %d\n",AnNow);
//	printf("t 4 antennas!");
//	printf("Dist is %lf !", Dista);
  }
  else if(r->getAntenna()->getF()==2)
  {
 	 
    Dista=r->getAntenna()->getDista();
	rXa[0]=rX+Dista; rXa[1]=rX+Dista; rXa[2]=rX-Dista; rXa[3]=rX-Dista;
	rYa[0]=rY+Dista; rYa[1]=rY-Dista; rYa[2]=rY+Dista; rYa[3]=rY-Dista;
	Pr=0;d=0;Gt=0;Gr=0;

	if(flag1 == 1)
	{
		
  		dt[3] = sqrt((rXa[3] - tX) * (rXa[3] - tX) 
	  	 + (rYa[3] - tY) * (rYa[3] - tY) 
	  	 + (rZ - tZ) * (rZ - tZ));
    

 		Gtt = t->getAntenna()->getTxGain(rXa[3] - tX, rYa[3] - tY, rZ - tZ, 
					 t->getLambda());
		Grt = r->getAntenna()->getRxGain(tX - rXa[3], tY - rYa[3], tZ - rZ,
					 r->getLambda());

    	Pr_temp = Opti(t->getTxPr(), Gtt, Grt, tZ, rZ, L, dt[3]);
		
		if(Pr_temp > Pr)
		{
			Pr = Pr_temp;
		}
//		printf("*AP*TxPr:%f Gtt:%f Grt:%f d:%f Pr:%f\n",t->getTxPr(),Gtt,Grt,dt[3],Pr);
	}

	if(flag2 == 1)
	{
		
  		dt[1] = sqrt((rXa[1] - tX) * (rXa[1] - tX) 
	  	 + (rYa[1] - tY) * (rYa[1] - tY) 
	  	 + (rZ - tZ) * (rZ - tZ));
    

 		Gtt = t->getAntenna()->getTxGain(rXa[1] - tX, rYa[1] - tY, rZ - tZ, 
					 t->getLambda());
		Grt = r->getAntenna()->getRxGain(tX - rXa[1], tY - rYa[1], tZ - rZ,
					 r->getLambda());

    	Pr_temp = Opti(t->getTxPr(), Gtt, Grt, tZ, rZ, L, dt[1]);
			
		if(Pr_temp > Pr)
		{
			Pr = Pr_temp;
		}
//		printf("*AP*TxPr:%f Gtt:%f Grt:%f d:%f Pr:%f\n",t->getTxPr(),Gtt,Grt,dt[1],Pr);
	}


	if(flag3 == 1)
	{
		
  		dt[0] = sqrt((rXa[0] - tX) * (rXa[0] - tX) 
	  	 + (rYa[0] - tY) * (rYa[0] - tY) 
	  	 + (rZ - tZ) * (rZ - tZ));
    

 		Gtt = t->getAntenna()->getTxGain(rXa[0] - tX, rYa[0] - tY, rZ - tZ, 
					 t->getLambda());
		Grt = r->getAntenna()->getRxGain(tX - rXa[0], tY - rYa[0], tZ - rZ,
					 r->getLambda());

    	Pr_temp = Opti(t->getTxPr(), Gtt, Grt, tZ, rZ, L, dt[0]);

		if(Pr_temp > Pr)
		{
			Pr = Pr_temp;
		}
//		printf("*AP*TxPr:%f Gtt:%f Grt:%f d:%f Pr:%f\n",t->getTxPr(),Gtt,Grt,dt[0],Pr);
	}


	if(flag4 == 1)
	{
		
  		dt[2] = sqrt((rXa[2] - tX) * (rXa[2] - tX) 
	  	 + (rYa[2] - tY) * (rYa[2] - tY) 
	  	 + (rZ - tZ) * (rZ - tZ));
    

 		Gtt = t->getAntenna()->getTxGain(rXa[2] - tX, rYa[2] - tY, rZ - tZ, 
					 t->getLambda());
		Grt = r->getAntenna()->getRxGain(tX - rXa[2], tY - rYa[2], tZ - rZ,
					 r->getLambda());

    	Pr_temp = Opti(t->getTxPr(), Gtt, Grt, tZ, rZ, L, dt[2]);

		if(Pr_temp > Pr)
		{
			Pr = Pr_temp;
		}
//		printf("*AP*TxPr:%f Gtt:%f Grt:%f d:%f Pr:%f\n",t->getTxPr(),Gtt,Grt,dt[2],Pr);
	}

// 	printf("r 4 antennas!");
  }
  else
  {
  
   	d = sqrt((rX - tX) * (rX - tX) 
	   + (rY - tY) * (rY - tY) 
	   + (rZ - tZ) * (rZ - tZ));
    

 	Gt = t->getAntenna()->getTxGain(rX - tX, rY - tY, rZ - tZ, 
					 t->getLambda());
 	Gr = r->getAntenna()->getRxGain(tX - rX, tY - rY, tZ - rZ,
					 r->getLambda());
#if DEBUG > 3
    printf("TRG %.9f %d(%d,%d)@%d(%d,%d) d=%f :",
	 Scheduler::instance().clock(), 
	 t->getNode()->index(), (int)tX, (int)tY,
	 r->getNode()->index(), (int)rX, (int)rY,
	 d);
#endif
   Pr = Opti(t->getTxPr(), Gt, Gr, tZ, rZ, L, d);
  }

//#if DEBUG > 3
//  if((fp1 = fopen("./b","a+"))==NULL)
//	{
//		fprintf(stderr,"Cannot open output file.\n");
//		return 1;
//	}
 //   fprintf(fp1,"OptiPr: %e , OptiGt: %e , OptiGr_: %e, d: %e ,dZ: %e\n",Pr,Gt,Gr,d,rZ-tZ);
 	
//	fprintf(fp1,"%e\n",Pr);
//	fclose(fp1);
//#endif    
    return Pr;

}

double OptiPropagation::getDist(double Pr, double Pt, double Gt, double Gr, double hr, double ht, double , double )
{
       /* Get quartic root */
       return sqrt(sqrt(Pt * Gt * Gr /Pr));
}
