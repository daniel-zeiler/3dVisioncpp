#pragma once
#include "colorObject.h"
#include "bestFit.h"

class regressionModel
{
public:
	regressionModel();
	regressionModel(vector < Vec4i >& regressionSpace);
	~regressionModel();

	bool isFileSet();

	void buildRegressionSpace(vector<vector<vector<Point3d>>> calibrationData, double numberOfCalibrationTests);

	void build3dSpace(vector<Vec4i> regressionSpace, vector < vector<Point3d>>& threeDPointSpace, vector<vector<Point>> twoDPointSpace);
private:
	string filenamespace = "regression.csv";

	int counterTotal, amountOfProjectedPointsPerColor, amountOfColors, totalProjectedColumns;

	bool fileSet = true;

	vector<vector<Point3d>> regressionData;

	void getCSVData(vector < Vec4i >& regressionSpace);
};