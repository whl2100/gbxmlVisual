#pragma once
#include "gbxmlManager.h"
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QMainWindow>
#include <fstream>

//#include <opencv\cv.h>
#include <cv.h>
#define Z_VALUE_OPENING 500
#define Z_VALUE_SPACE	100
#define Z_VALUE_TEXT	10
#define Z_VALUE_SENSOR	1000


gbxmlManager::gbxmlManager(void)
{
}
//////////////////////////////////////////////////////////////////////////
gbxmlManager::~gbxmlManager(void)
{
}
//////////////////////////////////////////////////////////////////////////
std::vector<CSpace> gbxmlManager::getSpaces()
{
	return m_SpacesList;
}
//////////////////////////////////////////////////////////////////////////
CvPoint gbxmlManager::getPixelForPoint(CvPoint3D32f p)
{
	if (m_GlobalGeometry.m_useX == true)
	{
		return m_GlobalGeometry.getPixel_X(p);
	}
	else
	{
		return m_GlobalGeometry.getPixel_Y(p);
	}
}
//////////////////////////////////////////////////////////////////////////
CvSize gbxmlManager::getOffset()
{
	return m_Offset;
}
//////////////////////////////////////////////////////////////////////////
void gbxmlManager::calculatePixels()
{
	for (int i=0; i<(int)m_SpacesList.size(); i++)
	{
		for (int j=0; j<(int)m_SpacesList[i].edges.size(); j++)
		{
			if (m_GlobalGeometry.m_useX == true)
			{
				m_SpacesList[i].edges[j].start.pixel = m_GlobalGeometry.getPixel_X(m_SpacesList[i].edges[j].start.point);
				m_SpacesList[i].edges[j].start.pixel.x = m_SpacesList[i].edges[j].start.pixel.x + m_Offset.width;
				m_SpacesList[i].edges[j].start.pixel.y = m_SpacesList[i].edges[j].start.pixel.y + m_Offset.height;
				m_SpacesList[i].edges[j].end.pixel = m_GlobalGeometry.getPixel_X(m_SpacesList[i].edges[j].end.point);
				m_SpacesList[i].edges[j].end.pixel.x = m_SpacesList[i].edges[j].end.pixel.x + m_Offset.width;
				m_SpacesList[i].edges[j].end.pixel.y = m_SpacesList[i].edges[j].end.pixel.y + m_Offset.height;
			}
			else
			{
				m_SpacesList[i].edges[j].start.pixel = m_GlobalGeometry.getPixel_Y(m_SpacesList[i].edges[j].start.point);
				m_SpacesList[i].edges[j].start.pixel.x = m_SpacesList[i].edges[j].start.pixel.x + m_Offset.width;
				m_SpacesList[i].edges[j].start.pixel.y = m_SpacesList[i].edges[j].start.pixel.y + m_Offset.height;
				m_SpacesList[i].edges[j].end.pixel = m_GlobalGeometry.getPixel_Y(m_SpacesList[i].edges[j].end.point);
				m_SpacesList[i].edges[j].end.pixel.x = m_SpacesList[i].edges[j].end.pixel.x + m_Offset.width;
				m_SpacesList[i].edges[j].end.pixel.y = m_SpacesList[i].edges[j].end.pixel.y + m_Offset.height;
			}
		}
		for (int p=0; p<(int)m_SpacesList[i].openings.size(); p++)
		{
			for (int z=0; z<(int)m_SpacesList[i].openings[p].points.size(); z++)
			{
				if (m_GlobalGeometry.m_useX == true)
				{
					m_SpacesList[i].openings[p].points[z].pixel = m_GlobalGeometry.getPixel_X(m_SpacesList[i].openings[p].points[z].point);
					m_SpacesList[i].openings[p].points[z].pixel.x = m_SpacesList[i].openings[p].points[z].pixel.x + m_Offset.width;
					m_SpacesList[i].openings[p].points[z].pixel.y = m_SpacesList[i].openings[p].points[z].pixel.y + m_Offset.height;
				}
				else
				{
					m_SpacesList[i].openings[p].points[z].pixel = m_GlobalGeometry.getPixel_Y(m_SpacesList[i].openings[p].points[z].point);
					m_SpacesList[i].openings[p].points[z].pixel.x = m_SpacesList[i].openings[p].points[z].pixel.x + m_Offset.width;
					m_SpacesList[i].openings[p].points[z].pixel.y = m_SpacesList[i].openings[p].points[z].pixel.y + m_Offset.height;
				}
			}
		}
		for (int j=0; j<(int)m_SpacesList[i].polygons.size(); j++)
		{
			for (int z=0; z<(int)m_SpacesList[i].polygons[j].points.size(); z++)
			{
				if (m_GlobalGeometry.m_useX == true)
				{
					m_SpacesList[i].polygons[j].points[z].pixel = m_GlobalGeometry.getPixel_X(m_SpacesList[i].polygons[j].points[z].point);
					m_SpacesList[i].polygons[j].points[z].pixel.x = m_SpacesList[i].polygons[j].points[z].pixel.x + m_Offset.width;
					m_SpacesList[i].polygons[j].points[z].pixel.y = m_SpacesList[i].polygons[j].points[z].pixel.y + m_Offset.height;
				}
				else
				{
					m_SpacesList[i].polygons[j].points[z].pixel = m_GlobalGeometry.getPixel_Y(m_SpacesList[i].polygons[j].points[z].point);
					m_SpacesList[i].polygons[j].points[z].pixel.x = m_SpacesList[i].polygons[j].points[z].pixel.x + m_Offset.width;
					m_SpacesList[i].polygons[j].points[z].pixel.y = m_SpacesList[i].polygons[j].points[z].pixel.y + m_Offset.height;
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void gbxmlManager::drawBuildingLayout(int floor /*= 0*/)
{

	clear();
	update();
	m_ActiveFloor = floor;
	blockSignals(true);
	if ((int)views().size() > 0)
	{
		views().at(0)->setUpdatesEnabled(false);
	}

	//	m_Building.setSceneSize(width(), height());
	setSceneSize(width(), height());
	std::vector<CSpace> spaces = gbxmlManager::getSpaces();
	QGraphicsItem *newItem;

	for (int i=0; i<(int)spaces.size(); i++)
	{
		QVector<QPointF> p;
		for (int j=0; j<(int)spaces[i].polygons.size(); j++)
		{
			p.clear();
			for (int k=0; k<(int)spaces[i].polygons[j].points.size(); k++)
			{
				p.push_back(QPointF(spaces[i].polygons[j].points[k].pixel.x, spaces[i].polygons[j].points[k].pixel.y));
			}
			if (j == 0)
			{
				m_Polygons.push_back(PolygonHolder(QPolygonF(p), spaces[i].id, QPolygonF(p).boundingRect().center(), spaces[i].floor));
			}
			else
			{
				m_Polygons[(int)m_Polygons.size()-1].addPolygon(p);
			}
		}

		if (spaces[i].floor == m_ActiveFloor)
		{
			for (int j=0; j<(int)spaces[i].edges.size(); j++)
			{
				newItem = addLine(spaces[i].edges[j].start.pixel.x,spaces[i].edges[j].start.pixel.y, spaces[i].edges[j].end.pixel.x, spaces[i].edges[j].end.pixel.y, QPen(QBrush(QColor(Qt::black)), 2));
				newItem->setZValue(Z_VALUE_SPACE);
				newItem->setToolTip(QString::fromStdString(spaces[i].id));
			}
			for (int k=0; k<(int)spaces[i].openings.size(); k++)
			{
				if (spaces[i].openings[k].type == schema::openingTypeEnum::SlidingDoor ||
					spaces[i].openings[k].type == schema::openingTypeEnum::NonSlidingDoor)
				{
					newItem = addLine(spaces[i].openings[k].points[0].pixel.x, spaces[i].openings[k].points[0].pixel.y, spaces[i].openings[k].points[1].pixel.x, spaces[i].openings[k].points[1].pixel.y, QPen(QBrush(QColor(Qt::red)), 4));
					newItem->setZValue(Z_VALUE_OPENING);
					newItem->setToolTip(QString::fromStdString(spaces[i].openings[k].id));
				}
				else if (spaces[i].openings[k].type == schema::openingTypeEnum::FixedWindow || 
					spaces[i].openings[k].type == schema::openingTypeEnum::OperableWindow)
				{
					newItem = addLine(spaces[i].openings[k].points[0].pixel.x, spaces[i].openings[k].points[0].pixel.y, spaces[i].openings[k].points[1].pixel.x, spaces[i].openings[k].points[1].pixel.y, QPen(QBrush(QColor(Qt::blue)), 4));
					newItem->setZValue(Z_VALUE_OPENING);
					newItem->setToolTip(QString::fromStdString(spaces[i].openings[k].id));
				}
			}
			// Write space name at the center of the space
				//QGraphicsTextItem* ti = addText(QString::fromStdString(spaces[i].id));
				//CvPoint c = getPixelForPoint(spaces[i].spaceCenter);
				//c.x = c.x + getOffset().width;
				//c.y = c.y + getOffset().height;
				//ti->setPos(QPointF(c.x, c.y));
				//ti->setZValue(Z_VALUE_TEXT);
				//ti->setToolTip(QString::fromStdString(spaces[i].id));
				//ti->scale(1,-1);
		}
	}



	if ((int)views().size() > 0)
	{
		views().at(0)->setUpdatesEnabled(true);
	}
	b_IsBuildingDrawn = true;
}
//////////////////////////////////////////////////////////////////////////
void gbxmlManager::addSurfaceToSpace(CSurface surf, std::string spaceID)
{
	for (int i=0; i<(int)m_SpacesList.size(); i++)
	{
		if (m_SpacesList[i].id == spaceID)
		{
			if (surf.type == schema::surfaceTypeEnum::InteriorWall ||
				surf.type == schema::surfaceTypeEnum::ExteriorWall)
			{
				for (int j=1; j<(int)surf.points.size(); j++)
				{
					m_SpacesList[i].edges.push_back(CLine(ComplexPoint(surf.points[j-1].point),ComplexPoint(surf.points[j].point)));
				}
				for (int j=0; j<(int)surf.openings.size(); j++)
				{
					m_SpacesList[i].openings.push_back(surf.openings[j]);
				}
				break;
			}
			else if (surf.type == schema::surfaceTypeEnum::InteriorFloor ||
				surf.type == schema::surfaceTypeEnum::UndergroundSlab ||
				surf.type == schema::surfaceTypeEnum::RaisedFloor ||
				surf.type == schema::surfaceTypeEnum::SlabOnGrade)
			{
				CPolugon p;
				for (int j=0; j<(int)surf.points.size(); j++)
				{
					p.points.push_back(surf.points[j]);
				}
				m_SpacesList[i].polygons.push_back(p);
				break;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void gbxmlManager::findGeomertyLimits()
{
	bool firstLoop = true;
	bool firstSpaceLoop;
	for (int i=0; i<(int)m_SpacesList.size(); i++)
	{
		firstSpaceLoop = true;
		for (int z=0; z<(int)m_SpacesList[i].polygons.size(); z++)
		{
			for (int j=0; j<(int)m_SpacesList[i].polygons[z].points.size(); j++)
			{

				if (firstLoop == true)
				{
					m_GlobalGeometry.m_minX = m_SpacesList[i].polygons[z].points.at(j).point.x;
					m_GlobalGeometry.m_minY = m_SpacesList[i].polygons[z].points.at(j).point.y;
					m_GlobalGeometry.m_minZ = m_SpacesList[i].polygons[z].points.at(j).point.z;
					m_GlobalGeometry.m_maxX = m_SpacesList[i].polygons[z].points.at(j).point.x;
					m_GlobalGeometry.m_maxY = m_SpacesList[i].polygons[z].points.at(j).point.y;
					m_GlobalGeometry.m_maxZ = m_SpacesList[i].polygons[z].points.at(j).point.z;
					firstLoop = false;
				}

				if (firstSpaceLoop == true)
				{
					m_SpacesList[i].spaceGeometry.m_minX = m_SpacesList[i].polygons[z].points.at(j).point.x;
					m_SpacesList[i].spaceGeometry.m_minY = m_SpacesList[i].polygons[z].points.at(j).point.y;
					m_SpacesList[i].spaceGeometry.m_minZ = m_SpacesList[i].polygons[z].points.at(j).point.z;
					m_SpacesList[i].spaceGeometry.m_maxX = m_SpacesList[i].polygons[z].points.at(j).point.x;
					m_SpacesList[i].spaceGeometry.m_maxY = m_SpacesList[i].polygons[z].points.at(j).point.y;
					m_SpacesList[i].spaceGeometry.m_maxZ = m_SpacesList[i].polygons[z].points.at(j).point.z;
					firstSpaceLoop = false;
				}
				//global geometry
				if (m_GlobalGeometry.m_minX > m_SpacesList[i].polygons[z].points.at(j).point.x)
				{
					m_GlobalGeometry.m_minX = m_SpacesList[i].polygons[z].points.at(j).point.x;
				}
				if (m_GlobalGeometry.m_minY > m_SpacesList[i].polygons[z].points.at(j).point.y)
				{
					m_GlobalGeometry.m_minY = m_SpacesList[i].polygons[z].points.at(j).point.y;
				}
				if (m_GlobalGeometry.m_minZ > m_SpacesList[i].polygons[z].points.at(j).point.z)
				{
					m_GlobalGeometry.m_minZ = m_SpacesList[i].polygons[z].points.at(j).point.z;
				}
				if (m_GlobalGeometry.m_maxX < m_SpacesList[i].polygons[z].points.at(j).point.x)
				{
					m_GlobalGeometry.m_maxX = m_SpacesList[i].polygons[z].points.at(j).point.x;
				}
				if (m_GlobalGeometry.m_maxY < m_SpacesList[i].polygons[z].points.at(j).point.y)
				{
					m_GlobalGeometry.m_maxY = m_SpacesList[i].polygons[z].points.at(j).point.y;
				}			
				if (m_GlobalGeometry.m_maxZ < m_SpacesList[i].polygons[z].points.at(j).point.z)
				{
					m_GlobalGeometry.m_maxZ = m_SpacesList[i].polygons[z].points.at(j).point.z;
				}
				//space geometry
				if (m_SpacesList[i].spaceGeometry.m_minX > m_SpacesList[i].polygons[z].points.at(j).point.x)
				{
					m_SpacesList[i].spaceGeometry.m_minX = m_SpacesList[i].polygons[z].points.at(j).point.x;
				}
				if (m_SpacesList[i].spaceGeometry.m_minY > m_SpacesList[i].polygons[z].points.at(j).point.y)
				{
					m_SpacesList[i].spaceGeometry.m_minY = m_SpacesList[i].polygons[z].points.at(j).point.y;
				}
				if (m_SpacesList[i].spaceGeometry.m_minZ > m_SpacesList[i].polygons[z].points.at(j).point.z)
				{
					m_SpacesList[i].spaceGeometry.m_minZ = m_SpacesList[i].polygons[z].points.at(j).point.z;
				}
				if (m_SpacesList[i].spaceGeometry.m_maxX < m_SpacesList[i].polygons[z].points.at(j).point.x)
				{
					m_SpacesList[i].spaceGeometry.m_maxX = m_SpacesList[i].polygons[z].points.at(j).point.x;
				}
				if (m_SpacesList[i].spaceGeometry.m_maxY < m_SpacesList[i].polygons[z].points.at(j).point.y)
				{
					m_SpacesList[i].spaceGeometry.m_maxY = m_SpacesList[i].polygons[z].points.at(j).point.y;
				}			
				if (m_SpacesList[i].spaceGeometry.m_maxZ < m_SpacesList[i].polygons[z].points.at(j).point.z)
				{
					m_SpacesList[i].spaceGeometry.m_maxZ = m_SpacesList[i].polygons[z].points.at(j).point.z;
				}
			}
		}
		m_SpacesList[i].spaceCenter.x = (float)((m_SpacesList[i].spaceGeometry.m_maxX + m_SpacesList[i].spaceGeometry.m_minX) / 2.);
		m_SpacesList[i].spaceCenter.y = (float)((m_SpacesList[i].spaceGeometry.m_maxY + m_SpacesList[i].spaceGeometry.m_minY) / 2.);
		m_SpacesList[i].spaceCenter.z = (float)((m_SpacesList[i].spaceGeometry.m_maxZ + m_SpacesList[i].spaceGeometry.m_minZ) / 2.);
	}
}
///////////////////////////////////////////////////////////////////////////
int gbxmlManager::getFloor(float lev)
{
	for (int i=0; i<(int)m_BuildingFloors.size(); i++)
	{
		if (fabs(m_BuildingFloors[i].floorLevel - lev) < 0.001)
		{
			return i;
		}
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////
void gbxmlManager::createSpaceList()
{
	schema::Campus::Building_type mainBuilding = m_gbXMLStruct.Campus().at(0).Building().at(0);
	for (int i=0; i<(int)mainBuilding.Space().size(); i++)
	{
		CSpace newSpace;
		newSpace.id = mainBuilding.Space()[i].id();
		m_SpacesList.push_back(newSpace);
	}
	for (int i=0; i<(int)m_gbXMLStruct.Campus().at(0).Surface().size(); i++)
	{
		CSurface newSurface;
		schema::Surface curSurface = m_gbXMLStruct.Campus().at(0).Surface()[i];
		if (curSurface.surfaceType() == schema::surfaceTypeEnum::InteriorFloor ||
			curSurface.surfaceType() == schema::surfaceTypeEnum::UndergroundSlab ||
			curSurface.surfaceType() == schema::surfaceTypeEnum::RaisedFloor ||
			curSurface.surfaceType() == schema::surfaceTypeEnum::SlabOnGrade)
		{
			newSurface.id = curSurface.id();
			newSurface.type = curSurface.surfaceType();
			assert(curSurface.PlanarGeometry().size() == 1);
			assert(curSurface.PlanarGeometry().at(0).PolyLoop().size() == 1);
			schema::PolyLoop curLoop = curSurface.PlanarGeometry().at(0).PolyLoop().at(0);
			for (int j=0; j<(int)curLoop.CartesianPoint().size(); j++)
			{
				ComplexPoint p;
				p.point.x = (float)curLoop.CartesianPoint().at(j).Coordinate().at(0);
				p.point.y = (float)curLoop.CartesianPoint().at(j).Coordinate().at(1);
				p.point.z = (float)curLoop.CartesianPoint().at(j).Coordinate().at(2);
				newSurface.points.push_back(p);
			}
			for (int j=0; j<(int)curSurface.Opening().size(); j++)
			{
				CSurfaceOpening newOpening;
				newOpening.id = curSurface.Opening().at(j).id();
				newOpening.type = curSurface.Opening().at(j).openingType();
				assert(curSurface.Opening().at(j).PlanarGeometry().size() == 1);
				assert(curSurface.Opening().at(j).PlanarGeometry().at(0).PolyLoop().size() == 1);
				schema::PolyLoop curLoop = curSurface.Opening().at(j).PlanarGeometry().at(0).PolyLoop().at(0);
				for (int j=0; j<(int)curLoop.CartesianPoint().size(); j++)
				{
					ComplexPoint p;
					p.point.x = (float)curLoop.CartesianPoint().at(j).Coordinate().at(0);
					p.point.y = (float)curLoop.CartesianPoint().at(j).Coordinate().at(1);
					p.point.z = (float)curLoop.CartesianPoint().at(j).Coordinate().at(2);
					newOpening.points.push_back(p);
				}
				sort(newOpening.points.begin(), newOpening.points.end(), sorterComplexPoint3D_x);
				for (int k=1; k<(int)newOpening.points.size(); k++)
				{
					if (newOpening.points[k-1].point.x == newOpening.points[k].point.x && newOpening.points[k-1].point.y == newOpening.points[k].point.y)
					{
						newOpening.points.erase(newOpening.points.begin()+k);
						k--;
					}
				}
				newSurface.openings.push_back(newOpening);
			}

			for (int j=0; j<(int)curSurface.AdjacentSpaceId().size(); j++)
			{
				addSurfaceToSpace(newSurface, curSurface.AdjacentSpaceId().at(j).spaceIdRef());
			}
		}
		else if (curSurface.surfaceType() == schema::surfaceTypeEnum::InteriorWall ||
			curSurface.surfaceType() == schema::surfaceTypeEnum::ExteriorWall)
		{
			CSurface newSurface;
			newSurface.id = curSurface.id();
			newSurface.type = curSurface.surfaceType();
			assert(curSurface.PlanarGeometry().size() == 1);
			assert(curSurface.PlanarGeometry().at(0).PolyLoop().size() == 1);
			schema::PolyLoop curLoop = curSurface.PlanarGeometry().at(0).PolyLoop().at(0);
			for (int j=0; j<(int)curLoop.CartesianPoint().size(); j++)
			{
				ComplexPoint p;
				p.point.x = (float)curLoop.CartesianPoint().at(j).Coordinate().at(0);
				p.point.y = (float)curLoop.CartesianPoint().at(j).Coordinate().at(1);
				p.point.z = (float)curLoop.CartesianPoint().at(j).Coordinate().at(2);
				newSurface.points.push_back(p);
			}
			sort(newSurface.points.begin(), newSurface.points.end(), sorterComplexPoint3D_x);
			for (int k=1; k<(int)newSurface.points.size(); k++)
			{
				if (newSurface.points[k-1].point.x == newSurface.points[k].point.x && newSurface.points[k-1].point.y == newSurface.points[k].point.y)
				{
					newSurface.points.erase(newSurface.points.begin()+k);
					k--;
				}
			}
			for (int j=0; j<(int)curSurface.Opening().size(); j++)
			{
				CSurfaceOpening newOpening;
				newOpening.id = curSurface.Opening().at(j).id();
				newOpening.type = curSurface.Opening().at(j).openingType();
				assert(curSurface.Opening().at(j).PlanarGeometry().size() == 1);
				assert(curSurface.Opening().at(j).PlanarGeometry().at(0).PolyLoop().size() == 1);
				schema::PolyLoop curLoop = curSurface.Opening().at(j).PlanarGeometry().at(0).PolyLoop().at(0);
				for (int j=0; j<(int)curLoop.CartesianPoint().size(); j++)
				{
					ComplexPoint p;
					p.point.x = (float)curLoop.CartesianPoint().at(j).Coordinate().at(0);
					p.point.y = (float)curLoop.CartesianPoint().at(j).Coordinate().at(1);
					p.point.z = (float)curLoop.CartesianPoint().at(j).Coordinate().at(2);
					newOpening.points.push_back(p);
				}
				sort(newOpening.points.begin(), newOpening.points.end(), sorterComplexPoint3D_x);
				for (int k=1; k<(int)newOpening.points.size(); k++)
				{
					if (newOpening.points[k-1].point.x == newOpening.points[k].point.x && newOpening.points[k-1].point.y == newOpening.points[k].point.y)
					{
						newOpening.points.erase(newOpening.points.begin()+k);
						k--;
					}
				}
				newSurface.openings.push_back(newOpening);
			}
			for (int j=0; j<(int)curSurface.AdjacentSpaceId().size(); j++)
			{
				addSurfaceToSpace(newSurface, curSurface.AdjacentSpaceId().at(j).spaceIdRef());
			}
		}
	}
	findGeomertyLimits();
	float minLev;
	for (int i=0; i<(int)m_SpacesList.size(); i++)
	{
		for (int j=0; j<(int)m_SpacesList[i].polygons.size(); j++)
		{
			for (int z=0; z<(int)m_SpacesList[i].polygons[j].points.size(); z++)
			{
				if (j == 0 || minLev > m_SpacesList[i].polygons[j].points[z].point.z)
				{
					minLev = m_SpacesList[i].polygons[j].points[z].point.z;
				}
			}
		}
		m_SpacesList[i].floor = getFloor(minLev);
		for (int j=0; j<(int)m_SpacesList[i].polygons.size(); j++)
		{
			for (int z=0; z<(int)m_SpacesList[i].polygons[j].points.size(); z++)
			{
				if (minLev < m_SpacesList[i].polygons[j].points[z].point.z)
				{
					m_SpacesList[i].polygons[j].points.erase(m_SpacesList[i].polygons[j].points.begin() + z);
					z--;
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
void gbxmlManager::createFloorsList()
{
	std::vector<float> floorLevels;
	for (int i=0; i<(int)m_gbXMLStruct.Campus().at(0).Surface().size(); i++)
	{
		schema::Surface curSurface = m_gbXMLStruct.Campus()[0].Surface()[i];
		if (curSurface.surfaceType() == schema::surfaceTypeEnum::InteriorFloor ||
			curSurface.surfaceType() == schema::surfaceTypeEnum::UndergroundSlab ||
			curSurface.surfaceType() == schema::surfaceTypeEnum::RaisedFloor ||
			curSurface.surfaceType() == schema::surfaceTypeEnum::SlabOnGrade)
		{
			float curLevel = (float)curSurface.PlanarGeometry()[0].PolyLoop()[0].CartesianPoint()[0].Coordinate()[2];

			if (floorLevels.size() == 0 /*|| std::find(floorLevels.begin(), floorLevels.end(), curLevel) == floorLevels.end()*/)
			{
				floorLevels.push_back(curLevel);
			}
			else
			{
				bool found = false;
				for (int j=0; j<(int)floorLevels.size(); j++)
				{
					if (fabs(curLevel - floorLevels[j]) < 0.001)
					{
						found = true;
						break;
					}
				}
				if (found == false)
				{
					floorLevels.push_back(curLevel);
				}
			}
		}
	}
	for (int i=0; i<(int)floorLevels.size(); i++)
	{
		m_BuildingFloors.push_back(FloorInfo(i, floorLevels[i]));
	}
	floorLevels.clear();
}
///////////////////////////////////////////////////////////////////////////////
bool gbxmlManager::parsegbXMLFile(std::string file)
{
	try
	{
		m_gbXMLStruct = *schema::gbXML_(file, xsd::cxx::tree::flags::dont_validate);
		m_LenghtUnit = m_gbXMLStruct.lengthUnit();
		createFloorsList();
		createSpaceList();
		return true;
	}
	catch (const xml_schema::exception& e)
	{
		std::ofstream ofs;
		ofs.open("log.txt", std::ios::app);
		ofs << e << std::endl;
		ofs.close();
		ofs.clear();
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////
void gbxmlManager::setSceneSize(int w, int h)
{
	//m_SceneSize =  cvSize(w,h);
	w=571;
	//w=m_rotatedBackgroundSize.x * 1,25;
	h=511;
	//h=m_rotatedBackgroundSize.y * 1,25;
	m_GlobalGeometry.m_pixelX = (double)(w) / (m_GlobalGeometry.m_maxX - m_GlobalGeometry.m_minX);
	m_GlobalGeometry.m_pixelY = (double)(h) / (m_GlobalGeometry.m_maxY - m_GlobalGeometry.m_minY);
	m_GlobalGeometry.m_useX = (m_GlobalGeometry.m_pixelX < m_GlobalGeometry.m_pixelY);
	if (m_GlobalGeometry.m_useX == true)
	{
		m_Offset.width = 0;
		m_Offset.height = cvRound((float)(h - m_GlobalGeometry.getPixel_X(cvPoint2D32f(0, m_GlobalGeometry.m_maxY)).y) / 2.);
	}
	else
	{
		m_Offset.width = cvRound((float)(w - m_GlobalGeometry.getPixel_Y(cvPoint2D32f(m_GlobalGeometry.m_maxX,0)).x) / 2.);
		m_Offset.height = 0;
	}
	calculatePixels();
}
//////////////////////////////////////////////////////////////////////////
CvPoint3D32f gbxmlManager::convert_Millimeters2Units( CvPoint3D32f p )
{
	CvPoint3D32f p_correctUnit;
	switch( m_LenghtUnit )
	{
	case schema::lengthUnitEnum::Kilometers:
		{
			p_correctUnit.x = p.x / 1000000.0;
			p_correctUnit.y = p.y / 1000000.0;
			p_correctUnit.z = p.z / 1000000.0;
			break;
		}
	case schema::lengthUnitEnum::Meters:
		{
			p_correctUnit.x = p.x / 1000.0;
			p_correctUnit.y = p.y / 1000.0;
			p_correctUnit.z = p.z / 1000.0;
			break;
		}
	case schema::lengthUnitEnum::Centimeters:
		{
			p_correctUnit.x = p.x / 10.0;
			p_correctUnit.y = p.y / 10.0;
			p_correctUnit.z = p.z / 10.0;
			break;
		}
	case schema::lengthUnitEnum::Millimeters:
		{
			p_correctUnit.x = p.x;
			p_correctUnit.y = p.y;
			p_correctUnit.z = p.z;
			break;
		}
	case schema::lengthUnitEnum::Miles:
		{
			p_correctUnit.x = p.x / 1609344.0;
			p_correctUnit.y = p.y / 1609344.0;
			p_correctUnit.z = p.z / 1609344.0;
			break;
		}
	case schema::lengthUnitEnum::Yards:
		{
			p_correctUnit.x = p.x / 914.4;
			p_correctUnit.y = p.y / 914.4;
			p_correctUnit.z = p.z / 914.4;
			break;
		}
	case schema::lengthUnitEnum::Feet:
		{
			p_correctUnit.x = p.x / 305.0;
			p_correctUnit.y = p.y / 305.0;
			p_correctUnit.z = p.z / 305.0;
			break;
		}
	case schema::lengthUnitEnum::Inches:
		{
			p_correctUnit.x = p.x / 25.4;
			p_correctUnit.y = p.y / 25.4;
			p_correctUnit.z = p.z / 25.4;
			break;
		}
	}
	return p_correctUnit;
}
GeometryLimits gbxmlManager::getBuildingGeometry()
{
	return m_GlobalGeometry;
}

