#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <assert.h>
#include <math.h>
#include <vector>

class bestFit
{
public:
	bestFit();
	bestFit(unsigned int vcount, const std::vector<double> *points, unsigned int vstride, const double *weights, unsigned int wstride, std::vector<double> &plane);
	~bestFit();
};

struct vector3 {
	double p[3];
	vector3(void){
		p[0] = 0.0;
		p[1] = 0.0;
		p[2] = 0.0;
	}
};