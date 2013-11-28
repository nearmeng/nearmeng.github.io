#ifndef ns_multiantenna_h
#define ns_multiantenna_h

#include <antenna.h>

class MultiAntenna: public Antenna {
	public:
		MultiAntenna();
		virtual double getTxGain(double , double , double , double ) ;
  		// return the gain for a signal to a node at vector dX, dY, dZ
  		// from the transmitter at wavelength lambda
		virtual double getRxGain(double , double , double , double ) ;
  		// return the gain for a signal from a node at vector dX, dY, dZ
  		// from the receiver at wavelength lambda
	 	virtual Antenna * copy() { return this;}
   		virtual void release(){}
		int getangle(){ return Angle_;}
 		int getwidth(){ return Width_;}
// 		int getLA(){ return lowerAngle;}
   		int getUA(){ return upperAngle;}
	 	double getSAR(){ return solidAngleRatio;}
		double getDista(){ return Dista_;}
	 	
	 	
	 	
	 	
	protected:
		double Gt_;		//gain of transmitter
		double Gr_;		//gain of receiver
		int Angle_;		//orientation of the antenna(form the positive Z-axis)(degrees)
		int Width_;		//this is the width of the opti-antenna,basically, the angular range to which it can transfer.
	private:
//		int lowerAngle;
		double Dista_;
		int upperAngle;
		double solidAngleRatio;
	
};


#endif
