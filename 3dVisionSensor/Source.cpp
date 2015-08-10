#include "colorObject.h"
#include "colorContainer.h"
#include "regressionModel.h"
#include "imageCapture.h"
#include "Init_GLUT.h"
#include "Scene_Manager.h"
#include "PointCloud.h"
#include <vector>

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

using namespace Core;
using namespace Init;

int main() {

	//initialize capture and structured light display
	Mat image;
	VideoCapture cap,videoDisplay;
	cap.open(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	//initialize OpenGl
	WindowInfo window(std::string("3dPointCloud"), 400, 200, 800, 600, true);
	ContextInfo context(4, 3, true);
	FramebufferInfo frameBufferInfo(true, true, true, true);
	Init_GLUT::Init(window, context, frameBufferInfo);
	Managers::Scene_Manager* scene = new Managers::Scene_Manager();
	Init_GLUT::SetListener(scene);


	//build regression model
	regressionModel regression = regressionModel(regressionSpace);
	int counter = 0;

	// if regression space is built
	if (regression.isFileSet()) {
		videoDisplay.open("structuredLight.avi");
		while (1) {
			//capture image
			cap >> image;

			//process image
			imageCapture processedImage = imageCapture(image, counter, AMOUNT_OF_COLORS, AMOUNT_OF_PROJECTED_POINTS_PER_COLOR, TOTAL_PROJECTED_COLUMNS, twoDPointSpace);
			if (videoDisplay.get(CV_CAP_PROP_POS_FRAMES) < videoDisplay.get(CV_CAP_PROP_FRAME_COUNT)) {
				// when each scan complete update 3d space model
				regression.build3dSpace(regressionSpace,threeDPointSpace,twoDPointSpace);
				counter = 0;
				Rendering::Models::PointCloud* pointcloud = new Rendering::Models::PointCloud();
				pointcloud->SetProgram(Managers::Shader_Manager::GetShader("colorShader"));
				pointcloud->Create(threeDPointSpace);
				scene->GetModels_Manager()->SetModel("pointcloud", pointcloud);
				Init_GLUT::Run();
				videoDisplay.open("structuredLight.avi");
			}
			//wait for next frame
			waitKey(33);
		}
	}
	// regression calibration required
	else {

		// user enters number of calibration steps
		int numberOfCalibrationTests;
		std::cout << "Number of test runs:\n";
		cin >> numberOfCalibrationTests;

		//calibration space
		vector< vector< vector<Point3d> > > dataCalibration(TOTAL_PROJECTED_COLUMNS,vector<vector<Point3d>>(AMOUNT_OF_PROJECTED_POINTS_PER_COLOR,vector<Point3d>(numberOfCalibrationTests)));

		for (int i = 0; i < numberOfCalibrationTests; i++) {
			
			// user enters current calibration distance
			double distance;
			std::cout << "distance from camera to calibration zone:\n";
			cin >> distance;
			videoDisplay.open("structuredLight.avi");

			// while on current loop
			while (videoDisplay.get(CV_CAP_PROP_POS_FRAMES) < videoDisplay.get(CV_CAP_PROP_FRAME_COUNT)) {

				//process image
				cap >> image;
				imageCapture processedImage = imageCapture(image, counter, AMOUNT_OF_COLORS, AMOUNT_OF_PROJECTED_POINTS_PER_COLOR, TOTAL_PROJECTED_COLUMNS, twoDPointSpace);

				//if all images processed from current calibration phase
				if (counter == COUNTER_TOTAL) {
					for (int j = 0; j < TOTAL_PROJECTED_COLUMNS; j++) {
						for (int k = 0; k < AMOUNT_OF_PROJECTED_POINTS_PER_COLOR; k++) {
							// build 3d point calibration space
							dataCalibration[k][j][i] = Point3d(twoDPointSpace[k][j].x, twoDPointSpace[k][j].y, distance);
						}
					}
				}
				//wait for next frame
				waitKey(33);
			}
		}
		//build regression data for non-calibration state
		regression.buildRegressionSpace(dataCalibration, numberOfCalibrationTests);
		std::cout << "congratulations on successful calibration!\n";
		main();
	}
}