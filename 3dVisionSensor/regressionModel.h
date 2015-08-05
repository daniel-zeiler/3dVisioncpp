#pragma once
#include "colorObject.h"
#include "bestFit.h"

class regressionModel
{
public:
	regressionModel();
	regressionModel(vector < Vec3i >& regressionSpace);
	~regressionModel();
	bool isFileSet();
	void buildRegressionSpace(vector<vector<vector<Point3d>>> calibrationData, double numberOfCalibrationTests);
private:
	void getCSVData(vector < Vec3i >& regressionSpace);
	string filenamespace = "regression.csv";
	int counterTotal;
	int amountOfProjectedPointsPerColor;
	int amountOfColors;
	int totalProjectedColumns;
	bool fileSet = true;
	vector<vector<Point3d>> regressionData;
};