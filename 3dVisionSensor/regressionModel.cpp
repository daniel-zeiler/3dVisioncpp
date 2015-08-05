#include "regressionModel.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
regressionModel::regressionModel() {
}

regressionModel::regressionModel(vector < Vec4i >& regressionSpace)
{
	char filename[] = "regression.csv";
	fstream appendFileToWorkWith;
	appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);	
	if (appendFileToWorkWith) {
		string calibration;
		cout << "recalibrate data? (Y/N):";
		getline(cin, calibration);
		if (calibration == "Y") {
			fileSet = false;
		}
		else {
			getCSVData(regressionSpace);
		}
	}
	else {
		fileSet = false;
	}
}

bool regressionModel::isFileSet() {
	return fileSet;
}


regressionModel::~regressionModel()
{
}


void regressionModel::getCSVData(vector < Vec4i >& regressionSpace) {
	ifstream data(filenamespace);
	string line;
	int row = 0;
	int column = 0;
	//while data from csv
	while (getline(data, line)) {
		stringstream linestream(line);
		string cell;
		while (getline(linestream, cell, ',')) {
			//fill regressionSpace with data
			istringstream(cell) >> regressionSpace[row][column];
			if (column == 3) {
				column = 0;
				row++;
			}
			else { 
				column++; 
			}

		}
	}
}


void regressionModel::buildRegressionSpace(vector<vector<vector<Point3d>>> dataCalibration, double numberOfCalibrationTests) {
	char filename[] = "regression.csv";
	ofstream outFile(filename);
	for (double i = 0; i < dataCalibration.size(); i++) {
		for (double j = 0; j < dataCalibration[0].size(); j++) {
			vector<double>points(numberOfCalibrationTests * 3);
			for (double l = 0; l < numberOfCalibrationTests; l++) {
				points.push_back(dataCalibration[i][j][l].x);
				points.push_back(dataCalibration[i][j][l].y);
				points.push_back(dataCalibration[i][j][l].z);
			}
			vector<double>plane(numberOfCalibrationTests);
			bestFit afit = bestFit(numberOfCalibrationTests, &points, sizeof(double) * 3, 0, 0, plane);
			for (unsigned int i = 0; i < plane.size(); i++) {
				outFile << plane.at(i);
			}
		}
	}
}

void build3dSpace(vector<Vec4i> regressionSpace, vector < vector<Point3d>>& threeDPointSpace, vector<vector<Point>> twoDPointSpace) {
	int x, y, C, z = 0;
	int xSize = threeDPointSpace.size();
	int ySize = threeDPointSpace[0].size();
	int xCounter, yCounter = 0;
	for each(Vec4i vector in regressionSpace) {
		if (xCounter == xSize) {
			xCounter = 0;
			yCounter++;
		}
		if (yCounter == ySize) {
			yCounter = 0;
		}
		x = (twoDPointSpace.at(xCounter).at(yCounter).x * vector[0]) / -(vector[2]);
		y = (twoDPointSpace.at(xCounter).at(yCounter).y * vector[1]) / -(vector[2]);
		C = vector[3] / -vector[2];
		z = x + y + C;
		Point3d aThreeDPoint = Point3d(x, y, z);
		threeDPointSpace.at(xCounter).at(yCounter) = aThreeDPoint;
		xCounter++;
	}
}