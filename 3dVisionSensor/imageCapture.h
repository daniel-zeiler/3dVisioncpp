#pragma once
#include "colorObject.h"
#include "colorContainer.h"

class imageCapture
{
public:
	imageCapture();
	~imageCapture();
	imageCapture(Mat image, int& counter, int amountOfColors, int amountOfProjectedPointersPerColor, int totalProjectedColumns, vector<vector<Point>>& twoDPointSpace);
	int getCounter();

private:
	Mat HSV;
	Mat threshold;
	Mat upperRedImage;
	Mat lowerRedImage;
	int counter;
	const int FRAME_WIDTH = 640;
	const int FRAME_HEIGHT = 480;
	vector<Point> left;
	vector<Point> right;
	void trackFilteredObject(colorObject aColorObject, Mat threshold, Mat &image, int counter, vector<vector<Point>>& twoDPointSpace);
	void morphOps(Mat &threshold);
	void filterPointData(vector<colorObject> colorVector, colorObject aColorObject, int counter, vector<vector<Point>>& twoDPointSpace);
};