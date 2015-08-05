#include "regressionModel.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
regressionModel::regressionModel() {
}

regressionModel::regressionModel(vector < Vec3i >& regressionSpace)
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


void regressionModel::getCSVData(vector < Vec3i >& regressionSpace) {
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
	for (int i = 0; i < dataCalibration.size(); i++) {
		for (int j = 0; j < dataCalibration[0].size(); j++) {
			vector<double>points(numberOfCalibrationTests * 3);
			for (int l = 0; l < numberOfCalibrationTests; l++) {
				points.push_back(dataCalibration[i][j][l].x);
				points.push_back(dataCalibration[i][j][l].y);
				points.push_back(dataCalibration[i][j][l].z);
			}
			vector<double>plane(numberOfCalibrationTests);
			bestFit afit = bestFit(numberOfCalibrationTests, &points, sizeof(double) * 3, 0, 0, plane);
		}
	}

}