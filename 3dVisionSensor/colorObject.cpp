#include "colorObject.h"

colorObject::colorObject()
{
}

colorObject::colorObject(String type) {
	setType(type);
	// define red, green, blue, or white point characteristics
	if (type == "blue") {
		setHSVmin(Scalar(100, 150, 0));
		setHSVmax(Scalar(140, 255, 255));
		setColorLabel(Scalar(255,0,0));
	}
	if (type == "redLower") {
		setHSVmin(Scalar(0, 150, 150));
		setHSVmax(Scalar(10, 255, 255));
		setColorLabel(Scalar(0,0,255));
	}
	if (type == "redUpper") {
		setHSVmin(Scalar(160, 100, 100));
		setHSVmax(Scalar(179, 255, 255));
		setColorLabel(Scalar(0,0,255));
	}
	if(type == "green"){
		setHSVmin(Scalar(53, 74, 81));
		setHSVmax(Scalar(90, 255, 255));
		setColorLabel(Scalar(255, 0, 0));
	}
	if (type == "white") {
		setHSVmin(Scalar(0, 0, 250));
		setHSVmax(Scalar(180, 255, 255));
		setColorLabel(Scalar(255, 0, 0));
	}
}

colorObject::~colorObject()
{
}

double colorObject::getXPos() {
	return colorObject::xPos;
}

double colorObject::getYPos() {
	return colorObject::yPos;
}

void colorObject::setXPos(double x) {
	colorObject::xPos = x;
}

void colorObject::setYPos(double y) {
	colorObject::yPos = y;
}

Scalar colorObject::getHSVmin() {
	return colorObject::HSVmin;
}

Scalar colorObject::getHSVmax() {
	return colorObject::HSVmax;
}

void colorObject::setHSVmin(Scalar HSVmin) {
	colorObject::HSVmin = HSVmin;
}

void colorObject::setHSVmax(Scalar HSVmax) {
	colorObject::HSVmax = HSVmax;
}