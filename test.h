#pragma once

#ifndef TEST_H
#define TEST_H

#define _USE_MATH_DEFINES

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <QtWidgets/QMainWindow>
#include <QtGui\qpainter.h>
#include "GeneratedFiles\ui_test.h"
#include "DepthViewer.h"
#include "gbxmlManager.h"
#include <iostream>
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <type_traits>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <math.h>


struct sonarPoints{
	CvPoint3D32f p1;
	CvPoint3D32f p2;
};


class test : public QMainWindow // public DataCsv
{
	Q_OBJECT

public:
	test(QWidget *parent = 0);
	~test();
	void createMenus();
	void createActions();
	double degreesTorads(double degrees);
	void drawSenorReturns(double timestamp_odometry);
	void initHash();
	int find_index_in_sonar_matrix(double timestamp_from_odom);
	void initOdomHash();
	void dispDepth(double timestamp_depth);
	void moveForward();

	void addToBinaryImage(sonarPoints points);

	sonarPoints getSenorReturn(double timestamp_odometry);
	gbxmlManager* gbxml_manager;

	//under devel//
	int find_index_in_odom(double timestamp_from_sonar);
	void drawLines(std::vector<cv::Vec4i> linesToDraw);
	
	
	
	//BELOW TRACKING//
	cv::Point3d test::convert2Dto3DOffline(unsigned x_dpt, unsigned y_dpt, unsigned depth);
	void test::parseSeqIndex();
	std::vector<std::pair<double,double>> test::calculateContourCentroids(std::vector<std::vector<cv::Point>> contours,cv::Mat DepthFrame);
	cv::Point test::calculateCentroidClusterCenter(std::vector<std::pair<double,double>> centroids);

	int test::extractDepthFromCentroidClusterCenter(cv::Point IRcenter,cv::Mat DepthFrame);
	void test::addToTxt(cv::Point3d WorldPoint,double theta);
	void start_tracking();
	double CalculateAngleBetweenFrames(cv::Point3d pt1,cv::Point3d pt2);
	int extractDepth(int x,int y,cv::Mat DepthFrame);
	cv::Point3d test::transformFromCameraToWorld(cv::Point3d pt3d);
	double test::CalcEuclDistance(cv::Point3d pt1,cv::Point3d pt2);
	double test::CalculateAngle(cv::Point3d pt1,cv::Point3d pt2);
	void test::addToTxtComplete(cv::Point3d WorldPoint,double theta,double timestamp,int index);
	void test::addToTxtComplete();
	//void test::BatchProcessing();
	//end tracking//







	public slots:

		void load_scene(void);

		void load_csv(); //load csv files  [button]
		void run();	     //visualize the data from the csv files [button] || step is 1 time frame
		void runFast();  // same as above, step is 100 time frames
		void GoToFrame(); //offers goto frame functionality
		void LoadKinectData();
		void drawall(void); //draws all the sonar sensor returns in the active window
		void drawFurniture(); // Draws the appartment's furniture ::  under devel
		void clearPoints();
		void load_odom_csv();
		void load_sonar_csv();

		//under devel://
		void generate_2d_point_map();





		//BELOW TRACKING//

		//void start_tracking();
		void BatchProcessing();

		//end tracking//


private:
	Ui::testClass ui;


	int index_csv;
	double startingx,startingy,startingz,startorientz;
	double robotx,roboty,robotz,robotorientz;

	bool is_first; //To controll the first time frame
	bool kinect_loaded;
	bool furniture_loaded;
	bool is_valid_return;

	std::string planfilename; //loaded plan filename

	std::ofstream SonarReturnsCSV;

	std::vector<std::vector<double> > front_sonar_matrix;
	std::vector<std::vector<double> > front_sweep_matrix;
	std::vector<std::vector<double> > odom_matrix;

	std::unordered_map<int,int> hashtable;

	std::unordered_map<int,int>::const_iterator gotstart;
	std::unordered_map<int,int>::const_iterator gotend;

	QAction* skipAction;
	QAction* _exitAction;
	QMenu* barMenu;

	QGraphicsEllipseItem* item;

	double syncOffset; // Robot's timestamp = Kinect's timestamp - syncOffset
	double orientOffset; // Used to calibrate robot's orientation
	depthViewer dpt; // kinect data stream viewer class


	cv::Mat BinaryImage;


	//Below : under Devel
	int offsetx;
	int offsety;

	std::vector<cv::Vec2f> lines2; //used for HoughLines	
	std::vector<cv::Vec4i> lines; // Used for HoughLinesP (probabilistic houghlines)



	std::unordered_map<int,int> hashtableOdom;
	std::unordered_map<int,int>::const_iterator gotstartOdom;
	std::unordered_map<int,int>::const_iterator gotendOdom;

	double robotxOdom,robotyOdom,robotzOdom;

	double houghBuffer;
	double threshold_update;

	std::vector<cv::Vec4i> TotalLinesFound;


	//BELOW TRACKING//

	std::vector<std::vector<double>> SeqIndexData;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<std::pair<double,double>> centroids;
	std::ofstream GroundTruth2D;
	int index_tracking;
	cv::Point3d prevFramePoint; //initialized in constructor? Yeah, why not.
	cv::Point2d offsetWindow; //check test::LoadKinectdata() for initialization of the window
	cv::Point2d midpoint;
	cv::Point3d midpointWorld;//mid point of the max eucl distance connector line from 3D coordinates

	cv::Point2d midpointIR;//mid point of the max eucl distance in IR coordinates
	
	//end tracking//





};

#endif // TEST_H
