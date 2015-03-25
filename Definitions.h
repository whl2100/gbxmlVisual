#pragma once

//#include <opencv\cv.h>
#include <cv.h>
#include "gb_xsd.hxx"

struct GeometryLimits
{
	double m_minX;
	double m_minY;
	double m_maxX;
	double m_maxY;
	double m_minZ;
	double m_maxZ;
	double m_pixelX;
	double m_pixelY;
	bool m_useX;
	CvPoint getPixel_X(CvPoint2D32f p)
	{
		return cvPoint( cvRound((p.x - m_minX) * m_pixelX), cvRound((p.y - m_minY) * m_pixelX));
	}
	CvPoint getPixel_Y(CvPoint2D32f p)
	{
		return cvPoint( cvRound((p.x - m_minX) * m_pixelY), cvRound((p.y - m_minY) * m_pixelY));
	}
	CvPoint getPixel_X(CvPoint3D32f p)
	{
		return cvPoint( cvRound((p.x - m_minX) * m_pixelX), cvRound((p.y - m_minY) * m_pixelX));
	}
	CvPoint getPixel_Y(CvPoint3D32f p)
	{
		return cvPoint( cvRound((p.x - m_minX) * m_pixelY), cvRound((p.y - m_minY) * m_pixelY));
	}
	CvPoint2D32f getPoint_X(CvPoint p)
	{
		return cvPoint2D32f( (p.x / m_pixelX) + m_minX, (p.y / m_pixelX) + m_minY);
	}
	CvPoint2D32f getPoint_Y(CvPoint p)
	{
		return cvPoint2D32f( (p.x / m_pixelY) + m_minX, (p.y / m_pixelY) + m_minY);
	}
	void calculatePixelDims(CvSize imSize)
	{
		m_pixelX = (double)(imSize.width / (m_maxX - m_minX));
		m_pixelY = (double)(imSize.height / (m_maxY - m_minY));
		m_useX = (m_pixelX < m_pixelY);
	}
	int pixelateDistance(float d)
	{
		if (m_useX == true)
		{
			return cvRound(d * m_pixelX);
		}
		else
		{
			return cvRound(d * m_pixelY);
		}
	}
};
struct ComplexPoint
{
	CvPoint3D32f point;
	CvPoint pixel;
	ComplexPoint()
	{
	}
	ComplexPoint(CvPoint p)
	{
		pixel = p;
	}
	ComplexPoint(CvPoint3D32f p)
	{
		point = p;
	}
	ComplexPoint(CvPoint pix, CvPoint3D32f p)
	{
		pixel = pix;
		point = p;
	}

};
struct CLine
{
	ComplexPoint start;
	ComplexPoint end;
	CLine(ComplexPoint s, ComplexPoint e)
	{
		start = s;
		end = e;
	}
};
struct CSurfaceOpening
{
	std::string id;
	schema::openingTypeEnum::value type;
	std::vector<ComplexPoint> points;
};
//////////////////////////////////////////////////////////////////////////
struct CPolugon
{
	std::vector<ComplexPoint> points;
};
struct CSpace
{
	std::string id;
	std::vector<CLine> edges;
	std::vector<CSurfaceOpening> openings;
	GeometryLimits spaceGeometry;
	std::vector<CPolugon> polygons;
	CvPoint3D32f spaceCenter;
	int floor;
};
//////////////////////////////////////////////////////////////////////////

struct CSurface
{
	std::string id;
	schema::surfaceTypeEnum::value type;
	std::vector<CSurfaceOpening> openings;
	std::vector<ComplexPoint> points;
};



struct PolygonHolder
{
	QVector<QPolygonF> polygons;
	std::string spaceid;
	QPointF center;
	int floor;
	PolygonHolder(QPolygonF p, std::string s, QPointF c, int f)
	{
		polygons.push_back(p);
		spaceid = s;
		center = c;
		floor = f;
	}
	void addPolygon(QPolygonF p)
	{
		polygons.push_front(p);
	}
};

static bool sorterComplexPoint3D_x(const ComplexPoint &a, const ComplexPoint &b)
{
	return a.point.x < b.point.x;
};
struct FloorInfo
{
	int floorID;
	float floorLevel;
	FloorInfo(int i, float l)
	{
		floorID = i;
		floorLevel = l;
	}
};


struct Vertex3D
{
	Vertex3D() : x(0), y(0), z(0), r(0), g(0), b(0), a(1) {}

	float x, y, z;
	unsigned char r, g, b, a;
};

/////////////////////////////////////////////////////////////

