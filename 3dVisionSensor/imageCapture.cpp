#include "imageCapture.h"



imageCapture::imageCapture()
{
}


imageCapture::~imageCapture()
{
}

imageCapture::imageCapture(Mat image, int& counter, int amountOfColors, int AmountOfProjectedPointsPerColor, int totalProjectedColumns, vector<vector<Point>>& twoDPointSpace) {
	colorObject redLower("redLower"), redUpper("redUpper"), green("green"), blue("blue"), white("white");

	cvtColor(image, HSV, COLOR_BGR2HSV);
	inRange(HSV, blue.getHSVmin(), blue.getHSVmax(), threshold);
	morphOps(threshold);
	trackFilteredObject(blue, threshold, image, counter, twoDPointSpace);

	cvtColor(image, HSV, COLOR_BGR2HSV);
	inRange(HSV, redLower.getHSVmin(), redLower.getHSVmax(), lowerRedImage);
	inRange(HSV, redUpper.getHSVmin(), redUpper.getHSVmax(), upperRedImage);
	addWeighted(lowerRedImage, 1.0, upperRedImage, 1.0, 0.0, threshold);
	morphOps(threshold);
	trackFilteredObject(redUpper, threshold, image, counter, twoDPointSpace);

	cvtColor(image, HSV, COLOR_BGR2HSV);
	inRange(HSV, green.getHSVmin(), green.getHSVmax(), threshold);
	morphOps(threshold);
	trackFilteredObject(green,threshold, image, counter, twoDPointSpace);

	cvtColor(image, HSV, COLOR_BGR2HSV);
	inRange(HSV, white.getHSVmin(), white.getHSVmax(), threshold);
	morphOps(threshold);
	trackFilteredObject(white, threshold, image, counter, twoDPointSpace);

	counter++;
}

void imageCapture::trackFilteredObject(colorObject aColorObject, Mat threshold, Mat & image, int counter, vector<vector<Point>>& twoDPointSpace)
{
	vector<colorObject> colorVector;

	//temporary matrix for contour detection
	Mat temp;
	threshold.copyTo(temp);
	
	//contours of detected areas in binary image matrix
	vector<vector<Point>> contours;
	//hierarchies of contour regions
	vector <Vec4i> hierarchy;
	
	//find color heirarchies from binary image and store data within vectors
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	
	double refArea = 0;
	bool objectsFound = false;

	//if found valid number of hierarchies
	if (hierarchy.size() >= 50) {
		for (int i = 0; i >= 0; i = hierarchy[i][0]) {
			
			// get moment of contour for generalized data access
			Moments moment = moments((Mat)contours[i]);
			double area = moment.m00;

			// if valid moment access moment and store within color vector
			if (area > 10 && area < FRAME_HEIGHT * FRAME_WIDTH / 1.5 && area>refArea) {
				colorObject color;
				color.setType(aColorObject.getType());
				color.setColorLabel(aColorObject.getColorLabel());
				color.setXPos(moment.m10 / area);
				color.setYPos(moment.m01 / area);
				colorVector.push_back(color);
				objectsFound = true;
			}
		}
	}
	// if data is found continue processing
	if (objectsFound) {
		filterPointData(colorVector, aColorObject, counter, twoDPointSpace);
	}
}

void imageCapture::filterPointData(vector<colorObject> colorVector, colorObject aColorObject, int counter, vector<vector<Point>>& twoDPointSpace) {
	
	//put all data within container for processing
	colorContainer coloredContainer = colorContainer(colorVector, aColorObject, counter);
	

	// divide and sort data for easy regression acquisition
	left = coloredContainer.getLeftHalf();
	right = coloredContainer.getRightHalf();

	// if red
	if (aColorObject.getType == "red") {
		// fill vector with spatial data
		int arraySpaceLeft = counter * 4;
		int arraySpaceRight = counter * 4 + 40;
		for (int i = 0; i < left.size(); i++) {
			twoDPointSpace[arraySpaceLeft][i] = left.at(i);
			twoDPointSpace[arraySpaceRight][i] = right.at(i);
		}
	}
	// if green
	if (aColorObject.getType == "green") {
		// fill vector with spatial data
		int arraySpaceLeft = counter * 4 + 1;
		int arraySpaceRight = counter * 4 + 41;
		for (int i = 0; i < left.size(); i++) {
			twoDPointSpace[arraySpaceLeft][i] = left.at(i);
			twoDPointSpace[arraySpaceRight][i] = right.at(i);
		}
	}
	// if blue
	if (aColorObject.getType == "blue") {
		// fill vector with spatial data
		int arraySpaceLeft = counter * 4 + 2;
		int arraySpaceRight = counter * 4 + 42;
		for (int i = 0; i < left.size(); i++) {
			twoDPointSpace[arraySpaceLeft][i] = left.at(i);
			twoDPointSpace[arraySpaceRight][i] = right.at(i);
		}
	}
	// if white
	if (aColorObject.getType == "white") {
		// fill vector with spatial data
		int arraySpaceLeft = counter * 4 + 3;
		int arraySpaceRight = counter * 4 + 43;
		for (int i = 0; i < left.size(); i++) {
			twoDPointSpace[arraySpaceLeft][i] = left.at(i);
			twoDPointSpace[arraySpaceRight][i] = right.at(i);
		}
	}
}

void imageCapture::morphOps(Mat &threshold) {
	//remove noise and dilate points
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(4, 4));
	erode(threshold, threshold, erodeElement);
	dilate(threshold, threshold, dilateElement);
}

int imageCapture::getCounter() {
	return counter;
}