#include "colorObject.h"
#include "colorContainer.h"
#include "regressionModel.h"
#include "imageCapture.h"
#include <vector>
#include "Source.h"

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
const int COUNTER_TOTAL = 10;
const int AMOUNT_OF_PROJECTED_POINTS_PER_COLOR = 50;
const int AMOUNT_OF_COLORS = 4;
const int TOTAL_PROJECTED_COLUMNS = COUNTER_TOTAL * AMOUNT_OF_COLORS * 2;

// current 3d space
vector < vector<Point3d>> threeDPointSpace(TOTAL_PROJECTED_COLUMNS, vector<Point3d>(AMOUNT_OF_PROJECTED_POINTS_PER_COLOR));

// current 2d space
vector<vector<Point>> twoDPointSpace(TOTAL_PROJECTED_COLUMNS,vector<Point>(AMOUNT_OF_PROJECTED_POINTS_PER_COLOR));

// regression space
vector < Vec4i > regressionSpace;

int main() {

	Mat image;
	//initialize capture
	VideoCapture cap;
	cap.open(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	regressionModel regression = regressionModel(regressionSpace);
	int counter = 0;
	

	// if regression space is built
	if (regression.isFileSet()) {
		while (1) {

			//capture image
			cap >> image;

			//process image
			imageCapture processedImage = imageCapture(image, counter, AMOUNT_OF_COLORS, AMOUNT_OF_PROJECTED_POINTS_PER_COLOR, TOTAL_PROJECTED_COLUMNS, twoDPointSpace);
			if (counter == COUNTER_TOTAL) {
				regression.build3dSpace(regressionSpace,threeDPointSpace,twoDPointSpace);
				counter = 0;
			}
			//wait for next frame
			waitKey(33);
		}
	}
	// regression calibration required
	else {

		// user enters number of calibration steps
		int numberOfCalibrationTests;
		cout << "Number of test runs:";
		cin >> numberOfCalibrationTests;

		//calibration space
		vector< vector< vector<Point3d> > > dataCalibration(TOTAL_PROJECTED_COLUMNS,vector<vector<Point3d>>(AMOUNT_OF_PROJECTED_POINTS_PER_COLOR,vector<Point3d>(numberOfCalibrationTests)));

		for (int i = 0; i < numberOfCalibrationTests; i++) {
			bool oneStep = false;
			
			// user enters current calibration distance
			double distance;
			cout << "distance from camera to calibration zone:";
			cin >> distance;

			while (!oneStep) {

				//process image
				imageCapture processedImage = imageCapture(image, counter, AMOUNT_OF_COLORS, AMOUNT_OF_PROJECTED_POINTS_PER_COLOR, TOTAL_PROJECTED_COLUMNS, twoDPointSpace);
				cap >> image;

				//if all images processed from current calibration phase
				if (counter == COUNTER_TOTAL) {
					for (int j = 0; j < TOTAL_PROJECTED_COLUMNS; j++) {
						for (int k = 0; k < AMOUNT_OF_PROJECTED_POINTS_PER_COLOR; k++) {
							// build 3d point calibration space
							dataCalibration[k][j][i] = Point3d(twoDPointSpace[k][j].x, twoDPointSpace[k][j].y, distance);

							// get more data till done
							oneStep = true;
						}
					}
				}
				//wait for next frame
				waitKey(33);
			}
		}
		//build regression data for non-calibration state
		regression.buildRegressionSpace(dataCalibration, numberOfCalibrationTests);
		cout << "congratulations on successful calibration!";
		main();
	}
}