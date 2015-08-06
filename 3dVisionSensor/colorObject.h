#pragma once
#include <string>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

class colorObject
{
public:
	colorObject();
	colorObject(String name);
	~colorObject();

	double getXPos(), getYPos();

	void setXPos(double x);
	void setYPos(double y);

	Scalar getHSVmin(), getHSVmax();

	void setHSVmin(Scalar HSVmin);
	void setHSVmax(Scalar HSVmax);

	string getType() { return type; }
	void setType(string typeName) { type = typeName; }

	Scalar getColorLabel() { return colorLabel; }
	void setColorLabel(Scalar colorLabelScalar) { colorLabel = colorLabelScalar; }

private:
	double xPos, yPos;

	string type;

	Scalar HSVmin, HSVmax, colorLabel;
};