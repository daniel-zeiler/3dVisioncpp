#pragma once
#include "colorObject.h"

class colorContainer
{
public:
	colorContainer();
	colorContainer(vector<colorObject> theColors, colorObject color, int counter);
	~colorContainer();

	vector<Point> getLeftHalf(), getRightHalf();

	string getType();

	int getCounter();

private:
	vector<Point> pointArray, leftHalf, rightHalf;

	string type;

	int counter;
	
	void divideArray();

	void setType(string type);

	void setCounter(int counter);

	bool xOperator(Point p1, Point p2), yOperator(Point p1, Point p2);
};