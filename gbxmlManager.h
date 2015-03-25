#pragma once
#include <QtWidgets/QGraphicsScene>
#include "Definitions.h"


#define _AFXDLL

class gbxmlManager : public QGraphicsScene
{
	Q_OBJECT
public:
	gbxmlManager(void);
	~gbxmlManager(void);
	void drawBuildingLayout(int floor /*= 0*/);
	bool parsegbXMLFile(std::string file);
	void setSceneSize(int w, int h);
	void createSpaceList();
	void addSurfaceToSpace(CSurface surf, std::string spaceID);
	void findGeomertyLimits();
	void createFloorsList();
	

	void updateViewMode();
	void calculatePixels();

	int m_ActiveFloor;
	int getFloor(float lev);


	bool b_IsBuildingDrawn;

	CvPoint3D32f gbxmlManager::convert_Millimeters2Units( CvPoint3D32f p );

	GeometryLimits gbxmlManager::getBuildingGeometry();
	QMutex mMutexShow;

	cv::Point3f m_Translation;
	std::vector<CSpace> getSpaces();
	std::vector<CSpace> m_SpacesList;
	std::vector<PolygonHolder> m_Polygons;
	std::vector<FloorInfo> m_BuildingFloors;
	CvSize m_Offset;
	CvPoint getPixelForPoint(CvPoint3D32f p);
	GeometryLimits m_GlobalGeometry;
	CvSize  getOffset();
	schema::gbXML m_gbXMLStruct;
	schema::lengthUnitEnum m_LenghtUnit;
	CvSize m_SceneSize;

};

