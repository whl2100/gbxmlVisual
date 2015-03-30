#pragma once

#include "test.h"

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "opencv2\core\mat.hpp"

#include <opencv2\core\core.hpp>
#include <QtWidgets\qshortcut.h>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGraphicsEllipseItem>
#include <stdio.h>
#include <fstream>
#include <math.h>

test::test(QWidget *parent)
	: QMainWindow(parent)
{
	index_csv=0;

	startingx=14.7;//0.820;//old value = 1.5
	startingy=8.0;//2.080;//old value = 1.5
	startingz=0.5;

	startorientz=0.0;

	syncOffset=57.500; // Theoretical difference = ~67.0 || 59.0 seems to sync better.

	//orientOffset=(M_PI/2); // 90 degrees offset for callibration. Under test currently.
	orientOffset=1.5707963268;// in rads

	is_first=1;

	is_valid_return=0;

	furniture_loaded=0;
	kinect_loaded=0;

	threshold_update = 75.0; // threshold to update HoughLinesP matrix

	BinaryImage = cv::Mat(1000,1000,CV_8U,cv::Scalar(0));

	//cv::Mat BinaryImage(800,600,CV_8U);

	houghBuffer=0.0;

	offsetx=400;
	offsety=300;

	ui.setupUi(this);

	gbxml_manager= new gbxmlManager();


	createMenus();
	createActions();

	SonarReturnsCSV.open("..\\sonarReturns\\sonarReturns.csv",std::ios::out);


	connect(ui.m_ButtonLoad_gbXML, SIGNAL(clicked()), this, SLOT(load_scene()));
	connect(ui.m_ButtonZoomIn, SIGNAL(clicked()), this, SLOT(load_csv()));
	connect(ui.m_ButtonZoomOut, SIGNAL(clicked()), this, SLOT(run()));
	connect(ui.GoToFrameButton, SIGNAL(clicked()), this, SLOT(GoToFrame()));
	connect(ui.loadKinectDataButton, SIGNAL(clicked()), this, SLOT(LoadKinectData()));
	connect(ui.DrawFurnitureButton, SIGNAL(clicked()), this, SLOT(drawFurniture()));
	connect(ui.draw_all_button, SIGNAL(clicked()), this, SLOT(drawall()));
	connect(ui.clear_button, SIGNAL(clicked()), this, SLOT(clearPoints()));

	connect(ui.generate_2d_button, SIGNAL(clicked()), this, SLOT(generate_2d_point_map()));


	connect(ui.load_odometry_button, SIGNAL(clicked()), this, SLOT(load_odom_csv()));
	connect(ui.load_sonar_data_button, SIGNAL(clicked()), this, SLOT(load_sonar_csv()));

	//BELOW TRACKING//

	connect(ui.track_button, SIGNAL(clicked()), this, SLOT(BatchProcessing()));
	prevFramePoint.x=0;
	prevFramePoint.y=0;
	prevFramePoint.z=0;

	//TRACKING//


	ui.graphicsView->setScene(gbxml_manager);
	ui.graphicsView->setSceneRect(0, 0, 1000, 1000 );
}

test::~test()
{
	//SonarReturnsCSV.close();
	cv::destroyAllWindows();


}

void test::LoadKinectData()
{

	//dpt.setUp("F:\\gbxmlVisual\\Depth_peirama_2\\Depth"); OLD WORKING FOR VIZ

	dpt.setUp("F:\\gbxmlVisual\\models\\20150309T143101\\20150309T143101"); // NEW for robot tracking
	parseSeqIndex();//NEW for parsing seq.index file

	GroundTruth2D.open("GroundTruth.txt",std::ios::out);

	//initialize the window
	//currently we have a 8x8 window
	offsetWindow.x=4;//itan 2
	offsetWindow.y=4;//itan 2


	kinect_loaded=1;
	index_tracking=0; //ITAN 0
	std::cout<<"Kinect images loaded."<<std::endl;
}


//updates index_csv variable to desired
//user-input frameNumber
void test::GoToFrame()
{
	int n;
	QString user_input= ui.GoToFrameLineEdit->text();
	n=user_input.toInt();

	if (n<odom_matrix.size())
	{
		index_csv=n; //change frame number to the specified user input
		run();       //and load the frame
	}
	else
	{
		return;
	}
} 

void test::createMenus()
{
	skipAction = new QAction(tr("&Skip 100"), this);
	skipAction->setShortcuts(QKeySequence::Open);
	skipAction->setStatusTip(tr("Skip 100 time frames"));
	connect(skipAction, SIGNAL(triggered()), this, SLOT(runFast()));
}

void test::createActions()
{
	barMenu = menuBar()->addMenu(tr("&Action"));
	barMenu->addAction(skipAction);
	//_fileMenu->addSeparator();
	//_fileMenu->addAction(_exitAction);
}

void test::drawFurniture()
{

	QPen pen;
	pen.setStyle(Qt::DotLine);
	pen.setWidth(2);
	pen.setColor(Qt::cyan);

	CvPoint3D32f point1 ;
	CvPoint3D32f point2 ;

	CvPoint ctr1;
	CvPoint ctr2;

	//draw table start

	point1=cvPoint3D32f(1.270,3.645,0.500);
	point2=cvPoint3D32f(1.270,2.995,0.500);

	ctr1 = gbxml_manager->getPixelForPoint(point1);
	ctr2 = gbxml_manager->getPixelForPoint(point2);

	gbxml_manager->addLine((double)ctr1.x+gbxml_manager->getOffset().width,(double)ctr1.y+gbxml_manager->getOffset().height,(double)ctr2.x+gbxml_manager->getOffset().width,(double)ctr2.y+gbxml_manager->getOffset().height,pen);

	//

	point1=cvPoint3D32f(2.370,2.995,0.500);
	ctr1 = gbxml_manager->getPixelForPoint(point1);

	gbxml_manager->addLine((double)ctr1.x+gbxml_manager->getOffset().width,(double)ctr1.y+gbxml_manager->getOffset().height,(double)ctr2.x+gbxml_manager->getOffset().width,(double)ctr2.y+gbxml_manager->getOffset().height,pen);

	//

	point2=cvPoint3D32f(2.370,3.645,0.500);
	ctr2=gbxml_manager->getPixelForPoint(point2);

	gbxml_manager->addLine((double)ctr1.x+gbxml_manager->getOffset().width,(double)ctr1.y+gbxml_manager->getOffset().height,(double)ctr2.x+gbxml_manager->getOffset().width,(double)ctr2.y+gbxml_manager->getOffset().height,pen);

	//draw table end


	//draw fridge start

	point1=cvPoint3D32f(3.770,2.045,0.500);
	point2=cvPoint3D32f(3.120,2.045,0.500);

	ctr1 = gbxml_manager->getPixelForPoint(point1);
	ctr2 = gbxml_manager->getPixelForPoint(point2);

	gbxml_manager->addLine((double)ctr1.x+gbxml_manager->getOffset().width,(double)ctr1.y+gbxml_manager->getOffset().height,(double)ctr2.x+gbxml_manager->getOffset().width,(double)ctr2.y+gbxml_manager->getOffset().height,pen);

	//

	point1=cvPoint3D32f(3.120,1.445,0.500);

	ctr1 = gbxml_manager->getPixelForPoint(point1);

	gbxml_manager->addLine((double)ctr1.x+gbxml_manager->getOffset().width,(double)ctr1.y+gbxml_manager->getOffset().height,(double)ctr2.x+gbxml_manager->getOffset().width,(double)ctr2.y+gbxml_manager->getOffset().height,pen);

	//

	point2=cvPoint3D32f(3.770,1.445,0.500);

	ctr2 = gbxml_manager->getPixelForPoint(point2);

	gbxml_manager->addLine((double)ctr1.x+gbxml_manager->getOffset().width,(double)ctr1.y+gbxml_manager->getOffset().height,(double)ctr2.x+gbxml_manager->getOffset().width,(double)ctr2.y+gbxml_manager->getOffset().height,pen);

	//draw fridge end

	//draw pagous start

	point1=cvPoint3D32f(3.120,1.760,0.500);
	point2=cvPoint3D32f(3.120,1.160,0.500);

	ctr1 = gbxml_manager->getPixelForPoint(point1);
	ctr2 = gbxml_manager->getPixelForPoint(point2);

	gbxml_manager->addLine((double)ctr1.x+gbxml_manager->getOffset().width,(double)ctr1.y+gbxml_manager->getOffset().height,(double)ctr2.x+gbxml_manager->getOffset().width,(double)ctr2.y+gbxml_manager->getOffset().height,pen);
	//

	point2=cvPoint3D32f(3.120,0.845,0.500);
	ctr2 = gbxml_manager->getPixelForPoint(point2);
	gbxml_manager->addLine((double)ctr1.x+gbxml_manager->getOffset().width,(double)ctr1.y+gbxml_manager->getOffset().height,(double)ctr2.x+gbxml_manager->getOffset().width,(double)ctr2.y+gbxml_manager->getOffset().height,pen);

	//draw pagous end

	point1=cvPoint3D32f(1.280,1.120,0.500);
	ctr1 = gbxml_manager->getPixelForPoint(point1);
	gbxml_manager->addLine((double)ctr1.x+gbxml_manager->getOffset().width,(double)ctr1.y+gbxml_manager->getOffset().height,(double)ctr2.x+gbxml_manager->getOffset().width,(double)ctr2.y+gbxml_manager->getOffset().height,pen);
	//

	point2=cvPoint3D32f(0.050,1.120,0.500);
	ctr2 = gbxml_manager->getPixelForPoint(point2);
	gbxml_manager->addLine((double)ctr1.x+gbxml_manager->getOffset().width,(double)ctr1.y+gbxml_manager->getOffset().height,(double)ctr2.x+gbxml_manager->getOffset().width,(double)ctr2.y+gbxml_manager->getOffset().height,pen);

	furniture_loaded=1;

}

void test::load_scene(void)
{
	gbxml_manager->drawBuildingLayout(0);

	//QString tmpPath =QDir::currentPath() ;
	QString tmpPath ="..//Data";
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),tmpPath,tr("gbxml (*.xml);; all files (*.*)"));


	//std::cout<<fileName.toStdString()<<std::endl;

	if (!gbxml_manager->parsegbXMLFile(fileName.toStdString())) return;
	gbxml_manager->drawBuildingLayout(0);
	gbxml_manager->setSceneSize(width(), height());

	ui.graphicsView->scale(1,-1);
	ui.graphicsView->setScene(gbxml_manager);



}  


void test::drawall(void)
{
	int s;//start
	int e;//end

	QString start_input = ui.draw_all_start->text(); //start field user input
	QString end_input = ui.draw_all_end->text();	 //end field user input

	s=start_input.toInt();
	e=end_input.toInt();

	CvPoint3D32f robotPoint=cvPoint3D32f(startingx,startingy,startingz);
	CvPoint3D32f sonarPoint;

	sonarPoints sonarPoints_st;

	QPen pen(Qt::NoPen);
	QBrush brush(Qt::yellow, Qt::SolidPattern);

	int radius=2;

	CvPoint ctr;
	CvPoint ctr2;


	std::vector<CvPoint> robotPoints;
	std::vector<CvPoint> sonarPoints_mat;

	std::vector<CvPoint>::iterator sonarPointsIter;

	for (int i=s;i<e;i++)
	{

		//index_csv=i;
		ctr = gbxml_manager->getPixelForPoint(robotPoint);
		robotPoints.push_back(ctr);

		robotPoint.x=robotx;
		robotPoint.y=roboty;

		////////////////////

		sonarPoints_st=getSenorReturn(odom_matrix[i][0]);

		//std::cout<<i<<std::endl;
		if (is_valid_return==1)
		{
			sonarPoint.x=sonarPoints_st.p1.x;
			sonarPoint.y=sonarPoints_st.p1.y;

			ctr2 = gbxml_manager->getPixelForPoint(sonarPoint);
			sonarPoints_mat.push_back(ctr2);


			sonarPoint.x=sonarPoints_st.p2.x;
			sonarPoint.y=sonarPoints_st.p2.y;

			ctr2 = gbxml_manager->getPixelForPoint(sonarPoint);
			sonarPoints_mat.push_back(ctr2);

		}

		test::moveForward();

	}

	for (int i=0;i<sonarPoints_mat.size()-1;i=i+2)
	{
		//std::cout<<i<<std::endl;
		QRectF dot1((double) sonarPoints_mat[i].x+gbxml_manager->getOffset().width-radius, (double) sonarPoints_mat[i].y+gbxml_manager->getOffset().height+radius, 2*radius, 2*radius);
		gbxml_manager->addEllipse(dot1, pen, brush);

		QRectF dot2((double) sonarPoints_mat[i+1].x+gbxml_manager->getOffset().width-radius, (double) sonarPoints_mat[i+1].y+gbxml_manager->getOffset().height+radius, 2*radius, 2*radius);
		gbxml_manager->addEllipse(dot2, pen, brush);

	}


}

double test::degreesTorads(double degrees)
{
	double rads;
	rads=degrees*(M_PI/180.0);
	return rads;
}


int test::find_index_in_sonar_matrix(double timestamp_from_odom)
{

	double dif=0;
	double mindif=1000.0;
	int minindex=0;

	gotstart=hashtable.find(timestamp_from_odom); //search the threshold betwwen [stamp,stamp+1(second)]
	gotend=hashtable.find(timestamp_from_odom+1);

	if ((gotend->second>0)&&(gotstart->second>0))
	{

		//std::cout<<"between "<<gotstart->second<<" and "<<gotend->second<<std::endl;

		for (int i=gotstart->second;i<gotend->second;i++)
		{
			dif=abs(odom_matrix[index_csv][0]-front_sonar_matrix[i][0]);
			if (dif<mindif)
			{
				mindif=dif;
				minindex=i;
			}

		}

	}
	return minindex;
}

//stable working

//takes as input the timestamp from the odometry csv
//and matches timestamp with the "nearest" sensor timestamp
void test::drawSenorReturns(double timestamp_odometry)
{

	////sync variables
	//double dif=0;
	//double mindif=1000.0;
	int minindex=0;
	sonarPoints pointBinary;
	//drawing variables
	QPen pen(Qt::NoPen);
	QBrush brush(Qt::red, Qt::SolidPattern);
	int radius=5;

	//myarc arc; //testing

	//gotstart=hashtable.find(timestamp_odometry); //search the threshold between [stamp,stamp+1(second)]
	//gotend=hashtable.find(timestamp_odometry+1);

	//if ((gotend->second>0)&&(gotstart->second>0)){

	//	//std::cout<<"between "<<gotstart->second<<" and "<<gotend->second<<std::endl;

	//	for (int i=gotstart->second;i<gotend->second;i++)
	//	{
	//		dif=abs(odom_matrix[index_csv][0]-front_sonar_matrix[i][0]);
	//		if (dif<mindif)
	//		{
	//			mindif=dif;
	//			minindex=i;
	//		}

	//	}

	minindex=find_index_in_sonar_matrix(odom_matrix[index_csv][0]);

	std::cout<<"Timestamp: "<<front_sonar_matrix[minindex][0]<<std::endl;
	std::cout<<"Sonar returns: "<<front_sonar_matrix[minindex][1]<<","<<front_sonar_matrix[minindex][2]<<std::endl;



	//first sensor
	//create the sonar return coordinates with respect to the SoA pdf
	double x=(0.01*front_sonar_matrix[minindex][1]*std::cosf(	robotorientz -degreesTorads(90)/*degreesTorads(front_sonar_matrix[minindex][2])	 /*-degreesTorads(orientOffset) -orientOffset*/ )   )+robotx;
	double y=(0.01*front_sonar_matrix[minindex][1]*std::sinf(	robotorientz -degreesTorads(90)/*degreesTorads(front_sonar_matrix[minindex][2])   /*-degreesTorads(orientOffset) -orientOffset*/ )    )+roboty +0.20 ;


	//by multiplying by 10 we get better resolution of the
	//accumulator, when we visualize the data in drawLines() we use /10 at each point 
	//to restore and center it correctly
	pointBinary.p1.x=x*10;
	pointBinary.p1.y=y*10;
	pointBinary.p1.z=0.5;

	//draw sensor return

	CvPoint3D32f point = cvPoint3D32f(x,y,startingz);
	CvPoint ctr = gbxml_manager->getPixelForPoint(point);


	//SonarReturnsCSV<<ctr.x<<","<<ctr.y<<std::endl;

	QRectF dot1((double)ctr.x+gbxml_manager->getOffset().width-radius, (double)ctr.y+gbxml_manager->getOffset().height+radius, 2*radius, 2*radius);
	gbxml_manager->addEllipse(dot1, pen, brush);


	///////////////////////////

	//second sensor
	//create the sonar return coordinates with respect to the SoA pdf

	x=(0.01*front_sonar_matrix[minindex][2]*std::cosf(	robotorientz +degreesTorads(90)/*degreesTorads(front_sonar_matrix[minindex][2])	 /*-degreesTorads(orientOffset) -orientOffset*/ )  )+robotx;
	y=(0.01*front_sonar_matrix[minindex][2]*std::sinf(	robotorientz +degreesTorads(90)/*degreesTorads(front_sonar_matrix[minindex][2])   /*-degreesTorads(orientOffset) -orientOffset*/ )  )+roboty - 0.20;


	//by multiplying by 10 we get better resolution of the
	//accumulator, when we visualize the data in drawLines() we use /10 at each point 
	//to restore and center it correctly
	pointBinary.p2.x=x*10;
	pointBinary.p2.y=y*10;
	pointBinary.p2.z=0.5;

	point = cvPoint3D32f(x,y,startingz);
	ctr = gbxml_manager->getPixelForPoint(point);


	//add the points to binary image //

	addToBinaryImage(pointBinary);

	//


	//draw sensor return



	//SonarReturnsCSV<<ctr.x<<","<<ctr.y<<std::endl;

	QRectF dot2((double)ctr.x+gbxml_manager->getOffset().width-radius, (double)ctr.y+gbxml_manager->getOffset().height+radius, 2*radius, 2*radius);
	gbxml_manager->addEllipse(dot2, pen, brush);

	//else 
	//{
	//	return; // no sensor returns exit, gtfo.
	//}
}



sonarPoints test::getSenorReturn(double timestamp_odometry)
{
	//sync variables
	double dif=0;
	double mindif=1000.0;
	int minindex=0;

	sonarPoints points;


	gotstart=hashtable.find(timestamp_odometry);
	gotend=hashtable.find(timestamp_odometry+1);

	if ((gotend->second>0)&&(gotstart->second>0)){

		//std::cout<<"between "<<gotstart->second<<" and "<<gotend->second<<std::endl;

		for (int i=gotstart->second;i<gotend->second;i++)
		{
			dif=abs(odom_matrix[index_csv][0]-front_sonar_matrix[i][0]);
			if (dif<mindif)
			{
				mindif=dif;
				minindex=i;
			}

		}

		//create the sonar return coordinates with respect to the SoA pdf
		double x=(0.01*front_sonar_matrix[minindex][1]*std::cosf(	robotorientz-degreesTorads(90) /*degreesTorads(front_sweep_matrix[minindex][2])	 /*-degreesTorads(orientOffset) -orientOffset*/ )  )+robotx;
		double y=(0.01*front_sonar_matrix[minindex][1]*std::sinf(	robotorientz-degreesTorads(90) /*degreesTorads(front_sweep_matrix[minindex][2])    /*-degreesTorads(orientOffset) -orientOffset*/ )  )+roboty +0.20;

		points.p1.x=x;
		points.p1.y=y;
		points.p1.z=startingz;

		//CvPoint3D32f point = cvPoint3D32f(x,y,startingz);

		x=(0.01*front_sonar_matrix[minindex][2]*std::cosf(	robotorientz+test::degreesTorads(90) /*degreesTorads(front_sweep_matrix[minindex][2])	 /*-degreesTorads(orientOffset) -orientOffset*/ )  )+robotx;
		y=(0.01*front_sonar_matrix[minindex][2]*std::sinf(	robotorientz+test::degreesTorads(90) /*degreesTorads(front_sweep_matrix[minindex][2])    /*-degreesTorads(orientOffset) -orientOffset*/ )  )+roboty -0.20;

		points.p2.x=x;
		points.p2.y=y;
		points.p2.z=startingz;

		//draw sensor return

		/*
		QRectF dot((double)ctr.x+gbxml_manager->getOffset().width-radius, (double)ctr.y+gbxml_manager->getOffset().height+radius, 10*radius, 10*radius);

		QGraphicsEllipseItem* item = new QGraphicsEllipseItem(dot);
		item->setStartAngle(degreesTorads(front_sweep_matrix[minindex][2]-15));
		item->setSpanAngle(degreesTorads(30));
		gbxml_manager->addItem(item);
		*/
		is_valid_return=1;
		return points; //return 2D point of sonar return;

	}
	else 
	{
		is_valid_return=0;//return; // no sensor returns exit, gtfo.
	}
}



//Initialize front_sonar_matrix hash table, 
//In order to sync with the rest of the data streams.
void test::initHash()
{
	int hashtemp=0;
	hashtable.emplace(std::make_pair(front_sonar_matrix[0][0],0));
	for(int i=1;i<front_sonar_matrix.size()-1;i++){
		if ((int)front_sonar_matrix[i][0]!=hashtemp){
			hashtable.emplace(std::make_pair(front_sonar_matrix[i][0],i+1));
		}
		hashtemp=(int) front_sonar_matrix[i][0];
	}
}

//Absolete
//Loads csv data from DataSensors folder
//It's also connected with second left to right load button
void test::load_csv()
{
	//DataCsv data("DataSensors\\front_sonar_log.csv","DataSensors\\front_sweep_log.csv","DataSensors\\house_odom.csv");

	const int MAX_CHARS_PER_LINE = 1024;

	const int MAX_TOKENS_PER_LINE = 40;

	const char* const DELIMITER = " ,";

	std::string inputfile1="DataSensors\\front_sonar_log.csv";
	std::string inputfile2="DataSensors\\front_sweep_log.csv";
	std::string inputfile3="DataSensors\\house_odom.csv";

	std::ifstream front_sonar_log;
	std::ifstream front_sweep_log;
	std::ifstream house_odom;

	char *token;
	char buf[MAX_CHARS_PER_LINE];

	front_sonar_log.open(inputfile1);
	if (!front_sonar_log.good()){exit;}
	front_sweep_log.open(inputfile2);
	if (!front_sweep_log.good()){exit;}
	house_odom.open(inputfile3);
	if (!house_odom.good()){exit;}

	while(!front_sonar_log.eof()){
		std::vector<double> numbers;
		front_sonar_log.getline(buf,MAX_CHARS_PER_LINE);
		for(token=strtok(buf,DELIMITER);token!=NULL;token=strtok(NULL,DELIMITER)){
			numbers.push_back(atof(token));
		}
		front_sonar_matrix.push_back(numbers);
		numbers.clear();
	}
	std::cout<<"Front sonar data loaded. Size: "<<"["<<front_sonar_matrix.size()<<"*"<<front_sonar_matrix[0].size()<<"]"<<std::endl;

	while(!front_sweep_log.eof()){
		std::vector<double> numbers;
		front_sweep_log.getline(buf,MAX_CHARS_PER_LINE);
		for(token=strtok(buf,DELIMITER);token!=NULL;token=strtok(NULL,DELIMITER)){
			numbers.push_back(atof(token));
		}
		front_sweep_matrix.push_back(numbers);
		numbers.clear();
	}
	std::cout<<"Front sweep data loaded. Size: "<<"["<<front_sweep_matrix.size()<<"*"<<front_sweep_matrix[0].size()<<"]"<<std::endl;

	while(!house_odom.eof()){

		std::vector<double> numbers;
		house_odom.getline(buf,MAX_CHARS_PER_LINE);
		for(token=strtok(buf,DELIMITER);token!=NULL;token=strtok(NULL,DELIMITER)){
			numbers.push_back(atof(token));
		}
		odom_matrix.push_back(numbers);
		numbers.clear();

	}
	std::cout<<"Odometry data loaded. Size: "<<"["<<odom_matrix.size()<<"*"<<odom_matrix[0].size()<<"]"<<std::endl;
	//setupTheHashTable
	initHash();

}



//Loads odometry data from csv
//stores into std::vector<std::vector<double> > odom_matrix;
void test::load_odom_csv()
{

	const int MAX_CHARS_PER_LINE = 1024;

	const int MAX_TOKENS_PER_LINE = 40;

	const char* const DELIMITER = " ,";

	char *token;

	char buf[MAX_CHARS_PER_LINE];



	std::ifstream OdomData;

	QString tmpPath ="..//DataSensorsGrafeio";
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),tmpPath,tr("gbxml (*.csv);; all files (*.*)"));

	OdomData.open(fileName.toStdString());
	if(!OdomData.good())
	{
		exit;
	}

	while(!OdomData.eof()){
		std::vector<double> numbers;
		OdomData.getline(buf,MAX_CHARS_PER_LINE);
		for(token=strtok(buf,DELIMITER);token!=NULL;token=strtok(NULL,DELIMITER)){
			numbers.push_back(atof(token));
		}
		odom_matrix.push_back(numbers);
		numbers.clear();
	}
	std::cout<<"Odometry data loaded. Size: "<<"["<<odom_matrix.size()<<"*"<<odom_matrix[0].size()<<"]"<<std::endl;

}




// Loads the sonar data csv file
// and stores the contents in std::vector<std::vector<double> > front_sonar_matrix;
void test::load_sonar_csv()
{

	const int MAX_CHARS_PER_LINE = 1024;

	const int MAX_TOKENS_PER_LINE = 40;

	const char* const DELIMITER = " ,";

	char *token;

	char buf[MAX_CHARS_PER_LINE];


	std::ifstream SonarData;

	QString tmpPath ="..//DataSensorsGrafeio";

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),tmpPath,tr("gbxml (*.csv);; all files (*.*)"));

	SonarData.open(fileName.toStdString());
	if(!SonarData.good())
	{
		exit;
	}

	while(!SonarData.eof()){
		std::vector<double> numbers;
		SonarData.getline(buf,MAX_CHARS_PER_LINE);
		for(token=strtok(buf,DELIMITER);token!=NULL;token=strtok(NULL,DELIMITER)){
			numbers.push_back(atof(token));
		}
		front_sonar_matrix.push_back(numbers);
		numbers.clear();
	}
	std::cout<<"Sonar data loaded. Size: "<<"["<<front_sonar_matrix.size()<<"*"<<front_sonar_matrix[0].size()<<"]"<<std::endl;

	initHash();
}



//Displays a image taken by the kinect stream,
//with respect to the current timeframe.
void test::dispDepth(double timestamp_depth)
{
	QString filenameToDisp;
	filenameToDisp = dpt.getFrame(timestamp_depth);
	cv::namedWindow("Depth Data",1);
	if (!filenameToDisp.isEmpty())
	{

		cv::Mat frame=cv::imread(filenameToDisp.QString::toStdString(),CV_LOAD_IMAGE_UNCHANGED);
		//cv::normalize( frame, frame, 0, 255, cv::NORM_MINMAX );
		//frame.convertTo( frame, CV_8UC1 );
		//cv::flip(frame,frame,1);
		cv::imshow(("Depth Data"), frame);
		cv::waitKey(10); 
	}
}

//Display the next time frame
void test::run()
{	

	//double ts_odom;

	std::cout<<"index is:" <<index_csv<<std::endl;

	int radius =5;

	//redraw?
	gbxml_manager->clear();
	gbxml_manager->drawBuildingLayout(0);
	if (furniture_loaded==1)
	{
		drawFurniture();
	}

	ui.graphicsView->setScene(gbxml_manager);

	QPen pen(Qt::NoPen);
	QBrush brush(Qt::green, Qt::SolidPattern);

	//print the robot's starting location if its the first itteration
	if (is_first==1){
		CvPoint3D32f point = cvPoint3D32f(startingx,startingy,startingz);
		CvPoint ctr = gbxml_manager->getPixelForPoint(point);
		QRectF dot((double)ctr.x+gbxml_manager->getOffset().width-radius, (double)ctr.y+gbxml_manager->getOffset().height+radius, 2*radius, 2*radius);
		gbxml_manager->addEllipse(dot, pen, brush);

		robotx=startingx;
		roboty=startingy;
		robotz=startingz;

		robotorientz=startorientz;

		is_first=0; // not the first itter any more.

	}
	else{

		CvPoint3D32f point = cvPoint3D32f(robotx,roboty,0.500);

		CvPoint ctr = gbxml_manager->getPixelForPoint(point);
		QRectF dot((double)ctr.x+gbxml_manager->getOffset().width-radius, (double)ctr.y+gbxml_manager->getOffset().height+radius, 2*radius, 2*radius);
		gbxml_manager->addEllipse(dot, pen, brush);

		if (kinect_loaded==1)
		{
			dispDepth(this->odom_matrix[index_csv][0]/*-syncOffset*/);
		}

		printf("Timestamp: [%f], Robot.Pos.X: [%f], Robot.Pos.Y: [%f], Robot.Orient.Z: [%f]\n",this->odom_matrix[index_csv][0],this->odom_matrix[index_csv][5],this->odom_matrix[index_csv][6],this->odom_matrix[index_csv][10]);//column 0 is timestamp || column 5 is pos.X || column 6 is pos.Y || column 7 is orientation.Z

		drawSenorReturns(odom_matrix[index_csv][0]);

		//NOTE:
		//There's a MINUS sign before "this" in the next 2 lines
		//update robot's coords and orientation for next time frame
		robotx=-this->odom_matrix[index_csv][5]+startingx; //this->odom_matrix[index_csv][5]+startingx;
		roboty=-this->odom_matrix[index_csv][6]+startingy; //this->odom_matrix[index_csv][5]+startingy;
		robotorientz=2*std::asin(this->odom_matrix[index_csv][10])+startorientz; //the correct orientation is 2 * asin( pose.orientation.z ) + [any_offset]

	}
	index_csv++;

}


//Like test::run but moves +100 time frames
void test::runFast()
{	

	double ts_odom;

	std::cout<<"index is:" <<index_csv<<std::endl;

	int radius =5;

	//redraw?
	gbxml_manager->clear();
	gbxml_manager->drawBuildingLayout(0);
	if (furniture_loaded==1)
	{
		drawFurniture();
	}

	ui.graphicsView->setScene(gbxml_manager);

	QPen pen(Qt::NoPen);
	QBrush brush(Qt::green, Qt::SolidPattern);

	//print the robot's starting location if its the first itteration
	if (is_first==1){
		CvPoint3D32f point = cvPoint3D32f(startingx,startingy,startingz);
		CvPoint ctr = gbxml_manager->getPixelForPoint(point);
		QRectF dot((double)ctr.x+gbxml_manager->getOffset().width-radius, (double)ctr.y+gbxml_manager->getOffset().height+radius, 2*radius, 2*radius);
		gbxml_manager->addEllipse(dot, pen, brush);



		ts_odom=this->odom_matrix[0][0];
		robotx=startingx;
		roboty=startingy;
		robotz=startingz;

		is_first=0; // not the first itter any more.

	}
	else{

		CvPoint3D32f point = cvPoint3D32f(robotx,roboty,0.500);

		CvPoint ctr = gbxml_manager->getPixelForPoint(point);
		QRectF dot((double)ctr.x+gbxml_manager->getOffset().width-radius, (double)ctr.y+gbxml_manager->getOffset().height+radius, 2*radius, 2*radius);
		gbxml_manager->addEllipse(dot, pen, brush);

		if (kinect_loaded==1)
		{
			dispDepth(this->odom_matrix[index_csv][0]/*-syncOffset*/);
		}

		printf("Timestamp: [%f], Robot.Pos.X: [%f], Robot.Pos.Y: [%f], Robot.Orient.Z: [%f]\n",odom_matrix[index_csv][0],odom_matrix[index_csv][5],odom_matrix[index_csv][6],odom_matrix[index_csv][10]);//column 0 is timestamp || column 5 is pos.X || column 6 is pos.Y || column 7 is orientation.Z

		drawSenorReturns(odom_matrix[index_csv][0]);

		//NOTE:
		//There's a MINUS sign before "this" in the next 2 lines
		robotx=-this->odom_matrix[index_csv][5]+startingx; //used to be this->odom_matrix[index_csv][5]+startingx;
		roboty=-this->odom_matrix[index_csv][6]+startingy; //used to be this->odom_matrix[index_csv][6]+startingy;
		robotorientz=2*std::asin(this->odom_matrix[index_csv][10])+startorientz; //the correct orientation is   2 * asin( pose.orientation.z ) + [any_offset]

	}

	index_csv=index_csv+10; // gotta go fast

}


//update robot(x,y,z)
//without drawing anything
void test::moveForward()
{
	index_csv++;
	//NOTE:
	//There is a MINUS sign in the next two lines 
	//before "odom_matrix"
	//The minus sign is used to cope with wrong orientation
	robotx=-odom_matrix[index_csv][5]+startingx;
	roboty=-odom_matrix[index_csv][6]+startingy;
	robotorientz=2*std::asin(odom_matrix[index_csv][10])+startorientz;
}


//self explanatory -- clears the canvas
void test::clearPoints()
{
	gbxml_manager->clear();
	gbxml_manager->drawBuildingLayout(0);
}


//Below : under Devel

void test::generate_2d_point_map()
{

	//double dif=0;
	//double mindif=1000.0;
	//int minindex=0;


	initOdomHash();

	for (int i=0;i<front_sonar_matrix.size()-1;i++)
	{


		int index_min=find_index_in_odom(front_sonar_matrix[i][0]);
		//std::cout<<"Index is: "<<index_min<<", for timestamp: "<<front_sonar_matrix[i][0]<<std::endl;
		robotx=-this->odom_matrix[index_min][5]+startingx;
		roboty=-this->odom_matrix[index_min][6]+startingy;
		robotorientz=2*std::asin(this->odom_matrix[index_csv][10])+startorientz;

		double x1=(0.01*front_sonar_matrix[i][1]*std::cosf(	robotorientz -degreesTorads(90)/*degreesTorads(front_sonar_matrix[minindex][2])	 /*-degreesTorads(orientOffset) -orientOffset*/ )     )+robotx;
		double y1=(0.01*front_sonar_matrix[i][1]*std::sinf(	robotorientz -degreesTorads(90)/*degreesTorads(front_sonar_matrix[minindex][2])   /*-degreesTorads(orientOffset) -orientOffset*/ )    )+roboty +0.20 ;

		double x2=(0.01*front_sonar_matrix[i][2]*std::cosf(	robotorientz +degreesTorads(90)/*degreesTorads(front_sonar_matrix[minindex][2])	 /*-degreesTorads(orientOffset) -orientOffset*/ )     )+robotx;
		double y2=(0.01*front_sonar_matrix[i][2]*std::sinf(	robotorientz +degreesTorads(90)/*degreesTorads(front_sonar_matrix[minindex][2])   /*-degreesTorads(orientOffset) -orientOffset*/ )    )+roboty -0.20 ;

		SonarReturnsCSV<<std::setprecision(25)<<front_sonar_matrix[i][0]<<","<<x1<<","<<y1<<","<<x2<<","<<y2<<std::endl;

	}

	SonarReturnsCSV.close();
}

void test::initOdomHash()
{

	int hashtemp=0;
	hashtableOdom.emplace(std::make_pair(odom_matrix[0][0],0));
	for(int i=1;i<odom_matrix.size()-1;i++){
		if ((int)odom_matrix[i][0]!=hashtemp){
			hashtableOdom.emplace(std::make_pair(odom_matrix[i][0],i+1));
		}
		hashtemp=(int) odom_matrix[i][0];
	}


}

int test::find_index_in_odom(double timestamp_from_sonar)
{
	double dif=0;
	double mindif=1000.0;
	int minindex=0;


	gotstartOdom=hashtableOdom.find(timestamp_from_sonar); //search the threshold betwwen [stamp,stamp+1(second)]
	gotendOdom=hashtableOdom.find(timestamp_from_sonar+1);

	if ((gotendOdom->second>0)&&(gotstartOdom->second>0))
	{
		for (int i=gotstartOdom->second;i<gotendOdom->second;i++)
		{
			dif=abs(odom_matrix[i][0]-timestamp_from_sonar);
			if (dif<mindif)
			{
				mindif=dif;
				minindex=i;
			}
		}
	}


	return minindex;

}

void test::addToBinaryImage(sonarPoints points)
{


	houghBuffer++;
	/*
	double roundedtest=cvRound(points.p1.x);
	double roundedtest2=cvRound(points.p1.y);
	*/


	//add sonar returns to the buffer cv::Mat
	BinaryImage.at<uchar>(cvRound(points.p1.y),cvRound(points.p1.x))=255;
	BinaryImage.at<uchar>(cvRound(points.p2.y),cvRound(points.p2.x))=255;




	if ( houghBuffer > threshold_update )

	{
		//cv::namedWindow("BinaryImg",1);
		//imshow("BinaryImg",BinaryImage);
		//cv::waitKey(0);

		cv::HoughLinesP(BinaryImage, lines, 1, CV_PI/180, 5, 1, 8 ); //threshold 10 only finds bottom lines //itan : cv::HoughLinesP(BinaryImage, lines, 1, CV_PI/180, 8, 1, 3 );
		drawLines(lines);

		//prev version : cv::HoughLinesP(BinaryImage, lines, 1, CV_PI/180, 5, 1, 8 );

		//add extracted lines to TotalLinesFound vector
		for (int i=0;i<lines.size();i++)
		{

			test::TotalLinesFound.push_back(lines[i]);

		}

		//reset the buffer
		BinaryImage=cv::Mat::zeros(1000,1000,CV_8U);
		houghBuffer=0;


	}

}

void test::drawLines(std::vector<cv::Vec4i> linesToDraw)
{


	//QPen pen(Qt::NoPen);
	QPen pen(QPen(Qt::yellow,3));
	QBrush brush(Qt::darkBlue, Qt::SolidPattern);


	for (int i=0;i<linesToDraw.size();i++)
	{

		//CvPoint3D32f point = cvPoint3D32f(startingx,startingy,startingz);
		//CvPoint ctr = gbxml_manager->getPixelForPoint(point);


		CvPoint3D32f point1=cvPoint3D32f((linesToDraw[i][0])/10/*-offsetx*/,(linesToDraw[i][1])/10/*-offsety*/,0.5);
		CvPoint ctr1=gbxml_manager->getPixelForPoint(point1);

		CvPoint3D32f point2=cvPoint3D32f((linesToDraw[i][2])/10/*-offsetx*/,(linesToDraw[i][3])/10/*-offsety*/,0.5);
		CvPoint ctr2=gbxml_manager->getPixelForPoint(point2);

		QLine myline;

		myline.setLine(ctr1.x + gbxml_manager->getOffset().width,ctr1.y+ gbxml_manager->getOffset().height,ctr2.x + gbxml_manager->getOffset().width,ctr2.y+ gbxml_manager->getOffset().height);
		gbxml_manager->addLine(myline,pen);

		//myline.setLine(linesToDraw[i],linesToDraw[i],linesToDraw[i],linesToDraw[i]);

	}


}





// BELOW TRACKING //



cv::Point3d test::convert2Dto3DOffline(unsigned x_dpt, unsigned y_dpt, unsigned depth) 
{
	cv::Point3d pt3d;
	float fx = 365.150;
	float fy = 365.150;
	float cx = 253.983;
	float cy = 211.093;

	float k1 = 0.0922871;
	float k2 = -0.2667973;
	float k3 = 0.0901915;
	float p1 = 0;
	float p2 = 0;


	//undistort point
	float ps = (x_dpt * x_dpt) + (y_dpt * y_dpt);
	float qs = ((ps * k3 + k2) * ps + k1) * ps + 1.0;
	for (int i = 0; i < 9; i++) 
	{
		float qd = ps / (qs * qs);
		qs = ((qd * k3 + k2) * qd + k1) * qd + 1.0;
	}

	unsigned x_dpt_undist = x_dpt/qs;
	unsigned y_dpt_undist = y_dpt/qs;

	//convert to 3d

	float z_3D = (float) depth / 1000;
	float x_3D = (float)((x_dpt_undist - cx) * z_3D) / fx;
	float y_3D = -(float)((y_dpt_undist  - cy) * z_3D) / fy;

	pt3d.x=x_3D;
	pt3d.y=y_3D;
	pt3d.z=z_3D;

	return pt3d;

}



void test::start_tracking()
{

	QString filenameToDisp,filename_IR,filename_Depth,filename_Color,calibration_info;
	cv::Mat frame,frame_thresholded,frame_depth,frame_color;
	cv::Mat tmpR, tmpT;
	cv::Point3d WorldPoint;//world
	std::vector<cv::Point2f> pointsHomography;


	filenameToDisp = dpt.getFrame(SeqIndexData[index_tracking][0]); //seqIndex's first column contains timestamps used for dpt.getFrame() to return the name of .png file to process


	filename_IR="F:\\gbxmlVisual\\models\\20150309T143101\\20150309T143101\\IR\\" + filenameToDisp;
	filename_Depth="F:\\gbxmlVisual\\models\\20150309T143101\\20150309T143101\\Depth\\" + filenameToDisp;
	calibration_info="F:\\gbxmlVisual\\models\\0000000000000000_calibration_info.txt";



	cv::namedWindow("IR Data",1);
	cv::namedWindow("Depth Data",1);



	if (!filenameToDisp.isEmpty())
	{
		frame=cv::imread(filename_IR.QString::toStdString(),CV_LOAD_IMAGE_UNCHANGED);
		frame_depth=cv::imread(filename_Depth.QString::toStdString(),CV_LOAD_IMAGE_UNCHANGED);
	}

	//normalize & threshold IR frame
	cv::normalize(frame,frame,0,255,cv::NORM_MINMAX,CV_8UC1);
	cv::threshold(frame,frame_thresholded,255-1,255,cv::THRESH_BINARY);


	//viz
	cv::imshow(("IR Data"), frame_thresholded);
	cv::imshow(("Depth Data"),frame_depth);

	cv::waitKey(10); 



	cv::Mat contourImage(frame.size(), CV_8UC3, cv::Scalar(0,0,0));
	cv::Scalar colors[3];
	colors[0] = cv::Scalar(255, 0, 0);
	colors[1] = cv::Scalar(0, 255, 0);
	colors[2] = cv::Scalar(0, 0, 255);


	cv::findContours(frame_thresholded,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

	//draw contours
	for (size_t idx = 0; idx < contours.size(); idx++) {
		cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
	}
	//end


	//calculate contour centroids
	centroids = calculateContourCentroids(contours,frame_depth);


	//testing homography
	if(centroids.size()==3)
	{
		for (size_t idx=0;idx<centroids.size();idx++)
		{
			cv::Point2f tempPoints;
			tempPoints.x=centroids[idx].first;
			tempPoints.y=centroids[idx].second;
			pointsHomography.push_back(tempPoints);
		
		}
	}



	if ((centroids[0].first!=0)&&(centroids[0].second!=0))
	{



		//draw centroids
		for (size_t idx=0; idx<centroids.size();idx++)
		{
			cv::Point pt1;
			pt1.x=centroids[idx].first;
			pt1.y=centroids[idx].second;
			cv::line(contourImage,pt1,pt1, cv::Scalar(255, 255, 255),3,8,0);
		}
		//end


		//calculate the center of the centroid clusters
		//ptC contains the coordinates (in IR and Depth) of the centroid cluster center
		cv::Point ptC=calculateCentroidClusterCenter(centroids);


		//draw the center
		//reversed for vizualization reasons
		cv::Point ptC_reversed;
		ptC_reversed.x=ptC.y;
		ptC_reversed.y=ptC.x;
		cv::line(contourImage,ptC_reversed,ptC_reversed, cv::Scalar(200, 200, 200),4,8,0);
		cv::line(contourImage,ptC_reversed,midpointIR,cv::Scalar(255,192,0),4,8,0);
		//cv::line(frame_thresholded,ptC,ptC,cv::Scalar(255, 0, 0),4,8,0); //old


		cv::imshow("Contours", contourImage);
		cv::waitKey(10); 


		double ptC_Depth=extractDepthFromCentroidClusterCenter(ptC,frame_depth);

		//cv::Point3d pt3d=convert2Dto3DOffline(ptC.x,ptC.y,ptC_Depth);//old
		cv::Point3d pt3d=convert2Dto3DOffline(ptC.y,ptC.x,ptC_Depth);


		//open calibration file and transform
		cv::FileStorage fs(calibration_info.toStdString(), cv::FileStorage::READ);
		fs["Rotation"] >> tmpR;
		fs["Translation"] >> tmpT;

		WorldPoint.x = (float)(pt3d.x * tmpR.at<float>(0,0) + pt3d.y * tmpR.at<float>(0,1) + pt3d.z * tmpR.at<float>(0,2)) + tmpT.at<float>(0)/1000;
		WorldPoint.y = (float)(pt3d.x * tmpR.at<float>(1,0) + pt3d.y * tmpR.at<float>(1,1) + pt3d.z * tmpR.at<float>(1,2)) + tmpT.at<float>(1)/1000;
		WorldPoint.z = (float)(pt3d.x * tmpR.at<float>(2,0) + pt3d.y * tmpR.at<float>(2,1) + pt3d.z * tmpR.at<float>(2,2)) + tmpT.at<float>(2)/1000;


		double theta_sequential=CalculateAngleBetweenFrames(prevFramePoint,WorldPoint);
		double theta=test::CalculateAngle(midpointWorld,WorldPoint);

		prevFramePoint=WorldPoint;


		//addToTxt(WorldPoint,theta);
		addToTxtComplete(WorldPoint,theta,SeqIndexData[index_tracking][0],index_tracking,theta_sequential);
		std::cout<<index_tracking<<"#############	"<<theta/**180/M_PI*/<<std::endl;
	}
	else
	{
		centroids.clear();
		contours.clear();
		addToTxtComplete();
	}
	//std::cout<<"WORLD_POINT_1:"<<midpointWorld.x<<","<<midpointWorld.y<<","<<midpointWorld.z<<","<<std::endl;
	//std::cout<<"WORLD_POINT_2:"<<WorldPoint.x<<","<<WorldPoint.y<<","<<WorldPoint.z<<","<<std::endl;
	//std::cout<<"#############"<<theta<<std::endl;

}



void test::parseSeqIndex()
{

	const int MAX_CHARS_PER_LINE = 1024;

	const int MAX_TOKENS_PER_LINE = 2;

	const char* const DELIMITER = " \t";

	char *token;

	char buf[MAX_CHARS_PER_LINE];



	std::ifstream SeqIndexFile;

	//QString tmpPath ="..//DataSensorsGrafeio";
	//QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),tmpPath,tr("gbxml (*.csv);; all files (*.*)"));

	SeqIndexFile.open("F:\\gbxmlVisual\\models\\20150309T143101\\20150309T143101\\seq.index");

	if(!SeqIndexFile.good())
	{
		exit;
	}

	while(!SeqIndexFile.eof()){
		std::vector<double> numbers;
		SeqIndexFile.getline(buf,MAX_CHARS_PER_LINE);
		for(token=strtok(buf,DELIMITER);token!=NULL;token=strtok(NULL,DELIMITER)){
			numbers.push_back(atof(token));
		}
		SeqIndexData.push_back(numbers);
		numbers.clear();
	}
	std::cout<<"SeqIndex data loaded. Size: "<<"["<<SeqIndexData.size()<<"*"<<SeqIndexData[0].size()<<"]"<<std::endl;

}

std::vector<std::pair<double,double>> test::calculateContourCentroids(std::vector<std::vector<cv::Point>> contours,cv::Mat DepthFrame)
{
	std::vector<std::vector<double>> distances;

	std::vector<double> distancesWorld;

	std::vector<std::vector<double>> depths;

	std::vector<std::vector<double>> tempdepth;

	std::vector<std::vector<cv::Point3d>> tempdepthCamera;
	std::vector<std::vector<cv::Point3d>> tempdepthWorld;

	std::vector<std::pair<double,double>> centroids;

	int counter=0;

	for (size_t idx=0;idx<contours.size();idx++)
	{

		int tempx=0;
		int tempy=0;
		std::pair<double,double> tempPair;

		for(size_t idy=0;idy<contours[idx].size();idy++)
		{

			tempx=contours[idx][idy].x + tempx;
			tempy=contours[idx][idy].y + tempy;


		}

		tempPair.first=tempx/contours[idx].size();
		tempPair.second=tempy/contours[idx].size();

		centroids.push_back(tempPair);



	}
	//
	int i=0;
	//for (int i=0;i<centroids.size()-2;i++)
	//{
	for(int j=i+1;j<=centroids.size()-1;j++)
	{
		std::vector<double> temp;
		temp.push_back(	std::sqrt(	(centroids[i].first-centroids[j].first)*(centroids[i].first-centroids[j].first)	+ (centroids[i].second-centroids[j].second)*(centroids[i].second-centroids[j].second)						)					);
		temp.push_back(i);
		temp.push_back(j);
		if(centroids[i].first == centroids[j].first)
		{
			//kane kati gia division me zero
			centroids[0].first=0;
			centroids[0].second=0;
			return centroids;
		}
		double slope_temp=(centroids[i].second-centroids[j].second)/	(centroids[i].first-centroids[j].first)	;		//calculating slope	
		temp.push_back(	slope_temp			);	
		temp.push_back( (centroids[j].second- (slope_temp * centroids[j].first)		)); //calculating intercept


		//traverse the line between the indexes from the distances vector.
		if ( centroids[i].first < centroids[j].first )
		{
			std::vector<double> tempTriads;
			for (  int k=centroids[i].first;k<centroids[j].first;k++)
			{

				int temp_y=temp[temp.size()-2]*k +temp[temp.size()-1];
				int temp_depth=extractDepth(k,temp_y,DepthFrame);
				tempTriads.push_back(k);
				tempTriads.push_back(temp_y);
				tempTriads.push_back(temp_depth);
				//tempTriads.push_back()


			}
			depths.push_back(tempTriads);
			tempTriads.clear();
		}
		else
		{
			std::vector<double> tempTriads;
			for( int k=centroids[j].first;k<centroids[i].first;k++)
			{
				//std::vector<double> tempTriads;
				int temp_y=temp[temp.size()-2]*k +temp[temp.size()-1];
				int temp_depth=extractDepth(k,temp_y,DepthFrame);
				tempTriads.push_back(k);
				tempTriads.push_back(temp_y);
				tempTriads.push_back(temp_depth);


			}
			depths.push_back(tempTriads);
			tempTriads.clear();


		}





		i++;
		distances.push_back(temp);
	}
	//} 
	if (centroids.size()!=2)
	{
		std::vector<double> temp;

		temp.push_back(	std::sqrt(	(centroids[0].first-centroids[centroids.size()-1].first)*(centroids[0].first-centroids[centroids.size()-1].first)	+ (centroids[0].second-centroids[centroids.size()-1].second)*(centroids[0].second-centroids[centroids.size()-1].second)						)					);
		temp.push_back(0);
		temp.push_back(centroids.size()-1);
		if(centroids[0].first == centroids[centroids.size()-1].first)
		{
			//kane kati gia division me zero
			centroids[0].first=0;
			centroids[0].second=0;
			return centroids;

		}
		double slope_temp=(centroids[0].second-centroids[centroids.size()-1].second)/	(centroids[0].first-centroids[centroids.size()-1].first);//calculating slope

		temp.push_back(	slope_temp				);	
		temp.push_back( (centroids[centroids.size()-1].second- (slope_temp * centroids[centroids.size()-1].first)		)); //calculating intercept



		//traverse the line between the indexes from the distances vector.
		if ( centroids[0].first < centroids[centroids.size()-1].first )
		{
			std::vector<double> tempTriads;
			for (  int k=centroids[0].first;k<centroids[centroids.size()-1].first;k++)
			{

				int temp_y=temp[temp.size()-2]*k +temp[temp.size()-1];
				int temp_depth=extractDepth(k,temp_y,DepthFrame);
				tempTriads.push_back(k);
				tempTriads.push_back(temp_y);
				tempTriads.push_back(temp_depth);
				//tempTriads.push_back()


			}
			depths.push_back(tempTriads);
			tempTriads.clear();
		}
		else
		{
			std::vector<double> tempTriads;
			for( int k=centroids[centroids.size()-1].first;k<centroids[0].first;k++)
			{
				//std::vector<double> tempTriads;
				int temp_y=temp[temp.size()-2]*k +temp[temp.size()-1];
				int temp_depth=extractDepth(k,temp_y,DepthFrame);
				tempTriads.push_back(k);
				tempTriads.push_back(temp_y);
				tempTriads.push_back(temp_depth);


			}
			depths.push_back(tempTriads);
			tempTriads.clear();


		}





		distances.push_back(temp);
	}
	bool flag=0;

	//preprocessing 0 depth only rows
	for (size_t i=0;i<=depths.size()-1;i++)
	{
		double sum=0;
		for(size_t j=2;j<=depths[i].size()-1;j=j+3)
		{
			sum=sum+depths[i].at(j);

		}
		if (sum==0)
		{
			flag=1;

		}

	}

	if (	flag==1	)
	{
		//return here GOTO
		centroids[0].first=0;
		centroids[0].second=0;
		return centroids;
	}
	else{

		for (size_t i=0;i<=depths.size()-1;i++)
		{

			std::vector<double> tempz;

			int k=2;

			if(	(depths[i].size()==3)&& (depths[i].at(k)==0))
			{

				tempz.push_back(depths[i].at(k-2));
				tempz.push_back(depths[i].at(k-1));
				tempz.push_back(1);
				break;


			}
			for(size_t j=0;j<depths[i].size()-1;j++)
			{
				//std::vector<double> tempz;

				if(	depths[i].at(k) != 0 )
				{
					tempz.push_back(depths[i].at(k-2));
					tempz.push_back(depths[i].at(k-1));
					tempz.push_back(depths[i].at(k));
					//tempdepth.push_back(tempz);
					k=2;
					break;
				}
				else
				{

					k=k+3;
				}
			}

			k=depths[i].size()-1;

			for(size_t j=depths[i].size()-1;j>=0;j--)
			{
				//std::vector<double> tempz;
				//int k=2;
				if(	depths[i].at(k) != 0 )
				{
					tempz.push_back(depths[i].at(k-2));
					tempz.push_back(depths[i].at(k-1));
					tempz.push_back(depths[i].at(k));
					tempdepth.push_back(tempz);
					k=depths[i].size()-1;
					break;
				}
				else
				{

					k=k-3;
				}
			}




			k=depths[i].size()-1;

		}
	}

	for (int i=0;i<=tempdepth.size()-1;i++)
	{
		std::vector<cv::Point3d> mytemp;
		std::vector<cv::Point3d> mytemp2;
		cv::Point3d temp3dpoint;
		int kk=0;
		//temp3dpoint=convert2Dto3DOffline(tempdepth[i].at(kk),tempdepth[i].at(kk+1),tempdepth[i].at(kk+2)				);
		//temp3dpoint=convert2Dto3DOffline(tempdepth[i].at(kk+1),tempdepth[i].at(kk),tempdepth[i].at(kk+2)				);//old
		temp3dpoint=convert2Dto3DOffline(tempdepth[i].at(kk),tempdepth[i].at(kk+1),tempdepth[i].at(kk+2)				);
		mytemp2.push_back(	transformFromCameraToWorld(temp3dpoint));

		mytemp.push_back(temp3dpoint);
		
		kk=kk+3;
		//temp3dpoint=convert2Dto3DOffline(tempdepth[i].at(kk),tempdepth[i].at(kk+1),tempdepth[i].at(kk+2)				);
		//temp3dpoint=convert2Dto3DOffline(tempdepth[i].at(kk+1),tempdepth[i].at(kk),tempdepth[i].at(kk+2)				);//old
		temp3dpoint=convert2Dto3DOffline(tempdepth[i].at(kk),tempdepth[i].at(kk+1),tempdepth[i].at(kk+2)				);
		mytemp.push_back(temp3dpoint);
		mytemp2.push_back(	transformFromCameraToWorld(temp3dpoint));
		

		tempdepthCamera.push_back(mytemp);
		tempdepthWorld.push_back(mytemp2);

	}

	//tempdepthWorld

	for (int i=0;i<=tempdepthWorld.size()-1;i++)
	{
		distancesWorld.push_back	(	CalcEuclDistance(	tempdepthWorld[i].at(0),tempdepthWorld[i].at(1))		);


	}

	double maximum=-10000.0;
	int maximum_index=0;

	for (int i=0;i<=(double)distancesWorld.size()-1;i++)
	{
		if (	maximum	< (double)distancesWorld.at(i)	)
		{	
			//std::cout<<(double)distancesWorld.at(i)<<std::endl;
			maximum=(double)distancesWorld.at(i);
			maximum_index=i;
		}

	}

	//cv::Point3d midpointWorld;

	if ( maximum_index == 0 )
	{
		midpointWorld.x= (tempdepthWorld[0].at(0).x + tempdepthWorld[0].at(1).x)/2;
		midpointWorld.y= (tempdepthWorld[0].at(0).y + tempdepthWorld[0].at(1).y)/2;
		midpointWorld.z= (tempdepthWorld[0].at(0).z + tempdepthWorld[0].at(1).z)/2;

		midpointIR.x=( centroids[0].first + centroids[1].first )/2;
		midpointIR.y=( centroids[0].second + centroids[1].second )/2;

	}
	else if ( maximum_index == 1 )
	{

		midpointWorld.x= (tempdepthWorld[1].at(0).x + tempdepthWorld[1].at(1).x)/2;
		midpointWorld.y= (tempdepthWorld[1].at(0).y + tempdepthWorld[1].at(1).y)/2;
		midpointWorld.z= (tempdepthWorld[1].at(0).z + tempdepthWorld[1].at(1).z)/2;

		midpointIR.x=( centroids[1].first + centroids[2].first )/2;
		midpointIR.y=( centroids[1].second + centroids[2].second )/2;

	}
	else if ( maximum_index == 2)
	{

		midpointWorld.x= (tempdepthWorld[2].at(0).x + tempdepthWorld[2].at(1).x)/2;
		midpointWorld.y= (tempdepthWorld[2].at(0).y + tempdepthWorld[2].at(1).y)/2;
		midpointWorld.z= (tempdepthWorld[2].at(0).z + tempdepthWorld[2].at(1).z)/2;

		midpointIR.x=( centroids[0].first + centroids[2].first )/2;
		midpointIR.y=( centroids[0].second + centroids[2].second )/2;

	}

	

	//viz
	//cv::Mat VizImage=DepthFrame.clone();
	//cv::Point2d pt1,pt2;
	//pt1.x=depths[0].at(0);
	//pt1.y=depths[0].at(1);

	//pt2.x=depths[0].at(depths[0].size()-3);
	//pt2.y=depths[0].at(depths[0].size()-2);
	//cv::line(DepthFrame,pt1,pt2,cv::Scalar(255, 255, 255),8,8,0);

	//cv::namedWindow("LINES",1);
	//cv::imshow("LINES",DepthFrame);
	//cv::waitKey(10); 


	//CALCULATE MIDPOINT FROM IR
	//double max = -100000;
	//int index1=0;
	//int index2=1;

	//for ( size_t i=0;i<distances.size()-1;i++)
	//{
	//	if (distances[i][0]>max)
	//	{

	//		max=distances[i][0];
	//		index1=distances[i][1];
	//		index2=distances[i][2];
	//	}

	//}

	//midpoint.y= (centroids[index1].first+centroids[index2].first)/2;
	//midpoint.x= (centroids[index1].second+centroids[index2].second)/2;
	//end midpoint

	return centroids;

}

cv::Point test::calculateCentroidClusterCenter(std::vector<std::pair<double,double>> centroids)
{
	cv::Point point;
	double tempx=0;
	double tempy=0;

	for (size_t idx=0; idx< centroids.size() ; idx++)
	{
		tempx=centroids[idx].first+tempx;
		tempy=centroids[idx].second+tempy;

	}

	//old
	//point.x=(tempx/centroids.size())/*-5*/;
	//point.y=(tempy/centroids.size())/*+5*/;

	//testing swapped x<-->y
	point.x=(tempy/centroids.size());
	point.y=(tempx/centroids.size());


	return point;

}

int test::extractDepthFromCentroidClusterCenter(cv::Point IRcenter,cv::Mat DepthFrame)
{

	//testing_applying median filtering
	int CentroidsCenterDepth=0;
	cv::Point2d top_left,bot_right;
	std::vector<int> windowedValues;

	windowedValues.clear();


	//build the window to apply median filtering on
	top_left.x=IRcenter.x-offsetWindow.x;
	top_left.y=IRcenter.y-offsetWindow.y;
	bot_right.x=IRcenter.x+offsetWindow.x;
	bot_right.y=IRcenter.y+offsetWindow.y;

	for (int i=top_left.x; i<bot_right.x; i++)
	{
		for (int j=top_left.y; j<bot_right.y; j++)
		{

			if ((int)DepthFrame.at<unsigned short>(i,j) != 0 )
			{
				windowedValues.push_back(	(int)DepthFrame.at<unsigned short>(i,j));
			}
			//else
			//{
			//	windowedValues.push_back(4000);
			//}
		}
	}

	//sort the windowed values
	//and extract the middle one
	std::sort(windowedValues.begin(),windowedValues.end());
	CentroidsCenterDepth=windowedValues.at(windowedValues.size()/2);



	windowedValues.clear();
	return CentroidsCenterDepth;
}


void test::addToTxt(cv::Point3d WorldPoint,double theta)
{

	GroundTruth2D<<WorldPoint.x << ","<<WorldPoint.y<<","<<WorldPoint.z<<","<<theta<<std::endl;

}

void test::addToTxtComplete(cv::Point3d WorldPoint,double theta,double timestamp,int index,double theta_sequential)
{
	GroundTruth2D.precision(15);
	GroundTruth2D<<timestamp<<","<<index<<","<<WorldPoint.x << ","<<WorldPoint.y<<","<<WorldPoint.z<<","<<theta<<","<<theta_sequential<<","<<midpointWorld.x<<","<<midpointWorld.y<<","<<midpointWorld.z<</*","<<((WorldPoint.y-midpointWorld.y)/(WorldPoint.z-midpointWorld.z))<<*/std::endl;

}

void test::addToTxtComplete()
{

	GroundTruth2D<<"#"<<std::endl;

}




void test::BatchProcessing()
{

	for (index_tracking;index_tracking<SeqIndexData.size()-1;index_tracking++)
	{
		start_tracking();
	}
	std::cout<<"Done tracking"<<std::endl;
}

double test::CalculateAngleBetweenFrames(cv::Point3d pt1,cv::Point3d pt2)
{

	if (pt1!=pt2)
	{
		double theta= std::atan(		((pt2.y-pt1.y)/(pt2.x-pt1.x))		); //Angle formula
		return theta;
	}
	else
	{
		return 0;
	}
}

int test::extractDepth(int x,int y,cv::Mat DepthFrame)
{

	return (int)DepthFrame.at<unsigned short>(y,x);
}

cv::Point3d test::transformFromCameraToWorld(cv::Point3d pt3d)
{
	//open calibration file and transform
	cv::Mat tmpR, tmpT;
	QString calibration_info;
	calibration_info="F:\\gbxmlVisual\\models\\0000000000000000_calibration_info.txt";
	cv::FileStorage fs(calibration_info.toStdString(), cv::FileStorage::READ);
	cv::Point3d WorldPoint;
	fs["Rotation"] >> tmpR;
	fs["Translation"] >> tmpT;

	WorldPoint.x = (float)(pt3d.x * tmpR.at<float>(0,0) + pt3d.y * tmpR.at<float>(0,1) + pt3d.z * tmpR.at<float>(0,2)) + tmpT.at<float>(0)/1000;
	WorldPoint.y = (float)(pt3d.x * tmpR.at<float>(1,0) + pt3d.y * tmpR.at<float>(1,1) + pt3d.z * tmpR.at<float>(1,2)) + tmpT.at<float>(1)/1000;
	WorldPoint.z = (float)(pt3d.x * tmpR.at<float>(2,0) + pt3d.y * tmpR.at<float>(2,1) + pt3d.z * tmpR.at<float>(2,2)) + tmpT.at<float>(2)/1000;







	return WorldPoint;

}

double test::CalcEuclDistance(cv::Point3d pt1,cv::Point3d pt2)
{
	double euclDistance;


	euclDistance= sqrt(	(pt2.x-pt1.x)*(pt2.x-pt1.x) + (pt2.y-pt1.y)*(pt2.y-pt1.y) +	(pt2.z-pt1.z)*(pt2.z-pt1.z)												);



	return euclDistance;
}

//old working
//double test::CalculateAngle(cv::Point3d pt1,cv::Point3d pt2)
//{
//	if (pt1!=pt2)
//	{
//		double theta= std::atan(	(	(pt2.y-pt1.y)/(pt2.z-pt1.z)		)	);
//		//double theta= (	(	(pt2.y-pt1.y)/(pt2.z-pt1.z)		)	);
//		theta = theta *(180/M_PI);
//		return theta;
//	}
//	else
//	{
//		return 0;
//	}
//
//
//
//}

cv::Mat test::calculateHomography()
{
	cv::Mat H;

	return H;
}

double test::CalculateAngle(cv::Point3d pt1,cv::Point3d pt2)
{
	double theta;
	if (pt2.y==pt1.y)
	{
		theta=0;
	
	
	}
	else if(pt2.x==pt1.x)
	{
		if(pt2.y > pt1.y)
		{
			theta=90; 
		
		}
		else
		{
			theta=-90;
		
		}
	
	
	
	}
	else
	{
		
		theta= std::atan(	(	(pt2.y-pt1.y)/(pt2.x-pt1.x)		)	);
		theta=theta*(180/M_PI);

		//commented out for glory
		/*if((pt1.x>pt2.x)&&(pt1.y>pt2.y))
		{
			theta=theta*(+1);
		
		}
		else if((pt2.x>pt1.x)&&(pt2.y<pt1.y))
		{
			
			theta=theta*(+1);
		}
		else if((pt2.x>pt1.x)&&(pt2.y>pt1.y))
		{
			
			theta=theta*(-1);
		}
		else if((pt2.x<pt1.x)&&(pt2.y>pt1.y))
		{
			
			theta=theta*(-1);
		}*/

	
	}

	return theta;



}