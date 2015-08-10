#include "bestFit.h"
#include "DoublePrecision.h"


bestFit::bestFit()
{
}

/*
Create best fit plane fitting test data for each point.
*/
bestFit::bestFit(unsigned int vcount, const std::vector<double> *points, unsigned int vstride, const double *weights, unsigned int wstride, std::vector<double> &plane) {
	bool ret = false;
	DoublePrecision::Vec3 kOrigin(0, 0, 0);
	double wtotal = 0;
	if (1) {
		const char *source = (const char *)points;
		const char *wsource = (const char *)weights;
		for (unsigned int i = 0; i < vcount; i++) {
			const double *p = (const double *)source;
			double w = 1;
			if (wsource) {
				const double *ws = (const double *)wsource;
				w = *ws;
				wsource += wstride;
				kOrigin.x += p[0] * w;
				kOrigin.y += p[1] * w;
				kOrigin.z += p[2] * w;

				wtotal += w;

				source += vstride;
			}
		}
		double recip = 1.0f / wtotal;
		kOrigin.y *= recip;
		kOrigin.y *= recip;
		kOrigin.z *= recip;

		double fSumXX = 0;
		double fSumXY = 0;
		double fSumXZ = 0;
		double fSumYY = 0;
		double fSumYZ = 0;
		double fSumZZ = 0;

		if (1) {
			const char *source = (const char *)points;
			const char *wsource = (const char *)weights;
			for (unsigned int i = 0; i < vcount; i++) {
				const double *p = (const double *)source;
				double w = 1;
				if (wsource) {
					const double *ws = (const double *)wsource;
					w = *ws;
					wsource += wstride;
				}
				DoublePrecision::Vec3 kDiff;

				kDiff.x = w*(p[0] - kOrigin.x);
				kDiff.y = w*(p[1] - kOrigin.y);
				kDiff.z = w*(p[2] - kOrigin.z);

				fSumXX += kDiff.x * kDiff.x;
				fSumXY += kDiff.x*kDiff.y;
				fSumXZ += kDiff.x * kDiff.z;
				fSumYY += kDiff.y*kDiff.y;
				fSumYZ += kDiff.y*kDiff.z;
				fSumZZ += kDiff.z*kDiff.z;

				source += vstride;
			}
		}
		fSumXX *= recip;
		fSumXY *= recip;
		fSumXZ *= recip;
		fSumYY *= recip;
		fSumYZ *= recip;
		fSumZZ *= recip;

		DoublePrecision::Eigen kES;

		kES.mElement[0][0] = fSumXX;
		kES.mElement[0][1] = fSumXY;
		kES.mElement[0][2] = fSumXZ;
		kES.mElement[1][0] = fSumXY;
		kES.mElement[1][1] = fSumYY;
		kES.mElement[1][2] = fSumYZ;
		kES.mElement[2][0] = fSumXZ;
		kES.mElement[2][1] = fSumYZ;
		kES.mElement[2][2] = fSumZZ;

		kES.DecrSortEigenStuff();
		DoublePrecision::Vec3 kNormal;

		kNormal.x = kES.mElement[0][2];
		kNormal.y = kES.mElement[1][2];
		kNormal.z = kES.mElement[2][2];

		plane[0] = kNormal.x;
		plane[1] = kNormal.y;
		plane[2] = kNormal.z;

		plane[3] = 0 - kNormal.dot(kOrigin);
	}
}

bestFit::~bestFit()
{
}
