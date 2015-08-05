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
	~colorObject();

	colorObject(String name);

	int getXPos();
	int getYPos();

	void setXPos(int x);
	void setYPos(int y);

	Scalar getHSVmin();
	Scalar getHSVmax();

	void setHSVmin(Scalar HSVmin);
	void setHSVmax(Scalar HSVmax);

	string getType() { return type; }
	void setType(string typeName) { type = typeName; }

	Scalar getColorLabel() { return colorLabel; }
	void setColorLabel(Scalar colorLabelScalar) { colorLabel = colorLabelScalar; }

private:
	int xPos, yPos;
	string type;
	Scalar HSVmin, HSVmax;
	Scalar colorLabel;
};