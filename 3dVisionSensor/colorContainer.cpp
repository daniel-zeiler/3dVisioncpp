#include "colorContainer.h"
#include <algorithm>

colorContainer::colorContainer()
{
}

bool colorContainer::xOperator(Point p1, Point p2) {
	return (p1.x < p2.x);
}

bool colorContainer::yOperator(Point p1, Point p2) {
	return (p1.y < p2.y);
}

colorContainer::colorContainer(vector<colorObject> theColors, colorObject color, int counter) {
	
	int size = theColors.size();
	pointArray.resize(size);
	for (int i = 0; i < size; i++) {
		pointArray.push_back(Point(theColors.at(i).getXPos(), theColors.at(i).getYPos()));
	}
	setType(color.getType());
	setCounter(counter);
	//std::sort(pointArray.begin(), pointArray.end(), xOperator);
	divideArray();
}

colorContainer::~colorContainer()
{
}

void colorContainer::divideArray() {
	int size = pointArray.size() / 2;
	leftHalf.resize(size);
	rightHalf.resize(size);
	for (int i = 0; i < size/2; i++) {
		leftHalf.push_back(pointArray.at(i));
	}
	//std::sort(leftHalf.begin(), leftHalf.end(), yOperator);
	for (int i = (size / 2); i < size; i++) {
		rightHalf.push_back(pointArray.at(i));
	}
	//std::sort(rightHalf.begin(), rightHalf.end(), yOperator);
}

void colorContainer::setType(string typeName) {
	colorContainer::type = typeName;
}

string colorContainer::getType() {
	return colorContainer::type;
}

void colorContainer::setCounter(int counter) {
	colorContainer::counter = counter;
}

int colorContainer::getCounter() {
	return colorContainer::counter;
}

vector<Point> colorContainer::getLeftHalf() {
	return colorContainer::leftHalf;
}

vector<Point> colorContainer::getRightHalf() {
	return colorContainer::rightHalf;
}
