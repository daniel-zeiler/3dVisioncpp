#include "bestFit.h"

bestFit::bestFit()
{
}

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
namespace DoublePrecision
{
	class Vec3
	{
	public:
		Vec3(void) :x(0), y(0), z(0) { };
		Vec3(double _x, double _y, double _z) :x(0), y(0), z(0) { x = _x; y = _y; z = _z; };

		double dot(const Vec3 &v)
		{
			return x*v.x + y*v.y + z*v.z; // the dot product
		}

		double x;
		double y;
		double z;
	};

	class Eigen
	{
	public:


		void DecrSortEigenStuff(void)
		{
			Tridiagonal(); //diagonalize the matrix.
			QLAlgorithm(); //
			DecreasingSort();
			GuaranteeRotation();
		}

		void Tridiagonal(void)
		{
			double fM00 = mElement[0][0];
			double fM01 = mElement[0][1];
			double fM02 = mElement[0][2];
			double fM11 = mElement[1][1];
			double fM12 = mElement[1][2];
			double fM22 = mElement[2][2];

			m_afDiag[0] = fM00;
			m_afSubd[2] = 0;
			if (fM02 != (double)0.0)
			{
				double fLength = sqrt(fM01*fM01 + fM02*fM02);
				double fInvLength = ((double)1.0) / fLength;
				fM01 *= fInvLength;
				fM02 *= fInvLength;
				double fQ = ((double)2.0)*fM01*fM12 + fM02*(fM22 - fM11);
				m_afDiag[1] = fM11 + fM02*fQ;
				m_afDiag[2] = fM22 - fM02*fQ;
				m_afSubd[0] = fLength;
				m_afSubd[1] = fM12 - fM01*fQ;
				mElement[0][0] = (double)1.0;
				mElement[0][1] = (double)0.0;
				mElement[0][2] = (double)0.0;
				mElement[1][0] = (double)0.0;
				mElement[1][1] = fM01;
				mElement[1][2] = fM02;
				mElement[2][0] = (double)0.0;
				mElement[2][1] = fM02;
				mElement[2][2] = -fM01;
				m_bIsRotation = false;
			}
			else
			{
				m_afDiag[1] = fM11;
				m_afDiag[2] = fM22;
				m_afSubd[0] = fM01;
				m_afSubd[1] = fM12;
				mElement[0][0] = (double)1.0;
				mElement[0][1] = (double)0.0;
				mElement[0][2] = (double)0.0;
				mElement[1][0] = (double)0.0;
				mElement[1][1] = (double)1.0;
				mElement[1][2] = (double)0.0;
				mElement[2][0] = (double)0.0;
				mElement[2][1] = (double)0.0;
				mElement[2][2] = (double)1.0;
				m_bIsRotation = true;
			}
		}

		bool QLAlgorithm(void)
		{
			const int iMaxIter = 32;

			for (int i0 = 0; i0 < 3; i0++)
			{
				int i1;
				for (i1 = 0; i1 < iMaxIter; i1++)
				{
					int i2;
					for (i2 = i0; i2 <= (3 - 2); i2++)
					{
						double fTmp = fabs(m_afDiag[i2]) + fabs(m_afDiag[i2 + 1]);
						if (fabs(m_afSubd[i2]) + fTmp == fTmp)
							break;
					}
					if (i2 == i0)
					{
						break;
					}

					double fG = (m_afDiag[i0 + 1] - m_afDiag[i0]) / (((double)2.0) * m_afSubd[i0]);
					double fR = sqrt(fG*fG + (double)1.0);
					if (fG < (double)0.0)
					{
						fG = m_afDiag[i2] - m_afDiag[i0] + m_afSubd[i0] / (fG - fR);
					}
					else
					{
						fG = m_afDiag[i2] - m_afDiag[i0] + m_afSubd[i0] / (fG + fR);
					}
					double fSin = (double)1.0, fCos = (double)1.0, fP = (double)0.0;
					for (int i3 = i2 - 1; i3 >= i0; i3--)
					{
						double fF = fSin*m_afSubd[i3];
						double fB = fCos*m_afSubd[i3];
						if (fabs(fF) >= fabs(fG))
						{
							fCos = fG / fF;
							fR = sqrt(fCos*fCos + (double)1.0);
							m_afSubd[i3 + 1] = fF*fR;
							fSin = ((double)1.0) / fR;
							fCos *= fSin;
						}
						else
						{
							fSin = fF / fG;
							fR = sqrt(fSin*fSin + (double)1.0);
							m_afSubd[i3 + 1] = fG*fR;
							fCos = ((double)1.0) / fR;
							fSin *= fCos;
						}
						fG = m_afDiag[i3 + 1] - fP;
						fR = (m_afDiag[i3] - fG)*fSin + ((double)2.0)*fB*fCos;
						fP = fSin*fR;
						m_afDiag[i3 + 1] = fG + fP;
						fG = fCos*fR - fB;
						for (int i4 = 0; i4 < 3; i4++)
						{
							fF = mElement[i4][i3 + 1];
							mElement[i4][i3 + 1] = fSin*mElement[i4][i3] + fCos*fF;
							mElement[i4][i3] = fCos*mElement[i4][i3] - fSin*fF;
						}
					}
					m_afDiag[i0] -= fP;
					m_afSubd[i0] = fG;
					m_afSubd[i2] = (double)0.0;
				}
				if (i1 == iMaxIter)
				{
					return false;
				}
			}
			return true;
		}

		void DecreasingSort(void)
		{
			//sort eigenvalues in decreasing order, e[0] >= ... >= e[iSize-1]
			for (int i0 = 0, i1; i0 <= 3 - 2; i0++)
			{
				// locate maximum eigenvalue
				i1 = i0;
				double fMax = m_afDiag[i1];
				int i2;
				for (i2 = i0 + 1; i2 < 3; i2++)
				{
					if (m_afDiag[i2] > fMax)
					{
						i1 = i2;
						fMax = m_afDiag[i1];
					}
				}

				if (i1 != i0)
				{
					// swap eigenvalues
					m_afDiag[i1] = m_afDiag[i0];
					m_afDiag[i0] = fMax;
					// swap eigenvectors
					for (i2 = 0; i2 < 3; i2++)
					{
						double fTmp = mElement[i2][i0];
						mElement[i2][i0] = mElement[i2][i1];
						mElement[i2][i1] = fTmp;
						m_bIsRotation = !m_bIsRotation;
					}
				}
			}
		}

		void GuaranteeRotation(void)
		{
			if (!m_bIsRotation)
			{
				// change sign on the first column
				for (int iRow = 0; iRow < 3; iRow++)
				{
					mElement[iRow][0] = -mElement[iRow][0];
				}
			}
		}

		double mElement[3][3];
		double m_afDiag[3];
		double m_afSubd[3];
		bool m_bIsRotation;
	};
}