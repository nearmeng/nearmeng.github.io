
#include <cmath>
#include <cstdio>
#include <multi-antenna.h>

static class MultiAntennaClass : public TclClass {
	public:
		MultiAntennaClass() : TclClass("Antenna/MultiAntenna") {}
		TclObject* create(int, const char*const*) {
			return (new MultiAntenna);
		}
}class_MultiAntenna;

MultiAntenna::MultiAntenna()
{
	Gt_=1.0;
	Gr_=1.0;
	Angle_=0;
	Width_=70;
	Flag_=2;
	Dista_=4;
	bind("Gt_", &Gt_);
	bind("Gr_", &Gr_);
	bind("Angle_", &Angle_);
	bind("Width_", &Width_);
	bind("Dista_", &Dista_);

	if (Width_<0 || Width_>90)
	{
		printf("Invalid antenna Angle given..should be between 0 and 90;\n");
		exit(1);
	}
//	lowerAngle=0-Width_;
	upperAngle=Width_;
//	solidAngleRatio=1.0;

}

double MultiAntenna::getTxGain(double x, double y, double z, double lambda)
{
	FILE *fpp;
	double angle;
	double angle_indegree;
	double dist_=sqrt(x*x+y*y+z*z);
//	int lowerAngle1;
	int upperAngle1;
	
//	double power=2.5;
	double power;
	
//	lowerAngle1=lowerAngle;
	upperAngle1=upperAngle;
	
	
	if (dist_==0.0)
	{	
		return Gt_;
	}
	
	if (z>0.0)
	{
		return 0.0;
	}
	angle = acos(-z/dist_);
	
	angle_indegree= angle*180/M_PI;
	
	
	power = 8;
	/*according to Lambertian radiant intensity introduced in <wireless infrared communications> writed by JOSEPH M etc.  */
	
#if DEBUG > 3
	if((fpp=fopen("./power","a+"))==NULL)
	{
		fprintf(stderr,"Can not open output file!\n");
		return 1;

	}
	fprintf(fpp,"power=%e, angle=%e\n",power,angle_indegree);
	fclose(fpp);
#endif

	if (angle_indegree<=upperAngle1)	
	{
		return Gt_*(power+1)*(pow(cos(angle),power)/7);
	}
	return 0.0;
}

double MultiAntenna::getRxGain(double,double,double,double)
{
	return Gr_;
}
