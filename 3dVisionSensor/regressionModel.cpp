#include "regressionModel.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
regressionModel::regressionModel() {
}

regressionModel::regressionModel(vector < Vec4i >& regressionSpace){
	// attempt to read from file
	char filename[] = "regression.csv";
	fstream appendFileToWorkWith("regression.csv");
	if (appendFileToWorkWith.good()) {
		appendFileToWorkWith.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
		// file is found, ask user if recalibration is required
		string calibration;
		cout << "recalibrate data? (Y/N):\n";
		getline(cin, calibration);
		if (calibration == "Y") {
			// time to recalibrate!
			fileSet = false;
		}
		else {
			// no calibration required, read regression data from file
			getCSVData(regressionSpace);
		}
	}
	else {
		cout << "this application requires calibration\n";
		// time to calibrate!
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
	//get stream to file
	char filename[] = "regression.csv";
	ofstream outFile(filename);
	for (double i = 0; i < dataCalibration.size(); i++) {
		for (double j = 0; j < dataCalibration[0].size(); j++) {
			vector<double>points(numberOfCalibrationTests * 3);
			for (double l = 0; l < numberOfCalibrationTests; l++) {
				//get points from accross different calibration steps
				points.push_back(dataCalibration[i][j][l].x);
				points.push_back(dataCalibration[i][j][l].y);
				points.push_back(dataCalibration[i][j][l].z);
			}
			// plane will contain double(A,B,C,D) such that Ax + By + Cz + D = 0
			vector<double>plane(numberOfCalibrationTests);
			
			// build regression
			bestFit afit = bestFit(numberOfCalibrationTests, &points, sizeof(double) * 3, 0, 0, plane);
			for (unsigned int i = 0; i < plane.size(); i++) {
				// write regression data to file
				outFile << plane.at(i);
			}
		}
	}
}

void regressionModel::build3dSpace(vector<Vec4i> regressionSpace, vector < vector<Point3d>>& threeDPointSpace, vector<vector<Point>> twoDPointSpace) {
	// initialize variables
	int x, y, C, z = 0;
	int xSize = threeDPointSpace.size();
	int ySize = threeDPointSpace[0].size();
	int xCounter=0, yCounter = 0;

	// for each vector of integers(A,B,C,D) such that Ax + By + Cz + D = 0 
	for each(Vec4i vector in regressionSpace) {
		if (xCounter == xSize) {
			xCounter = 0;
			yCounter++;
		}
		if (yCounter == ySize) {
			yCounter = 0;
		}
		// get z value for each two dimensional point
		x = (twoDPointSpace.at(xCounter).at(yCounter).x * vector[0]) / -(vector[2]);
		y = (twoDPointSpace.at(xCounter).at(yCounter).y * vector[1]) / -(vector[2]);
		C = vector[3] / -vector[2];
		z = x + y + C;
		
		// add to 3d point structure
		threeDPointSpace.at(xCounter).at(yCounter) = Point3d(x, y, z);
		xCounter++;
	}
}