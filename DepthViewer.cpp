#include "DepthViewer.h"
#include "QtCore/QTextStream.h"


depthViewer::depthViewer()
{
}

depthViewer::depthViewer(const QString &dir)
{
	setUp(dir);
}

depthViewer::~depthViewer()
{
}

void depthViewer::setUp(const QString &dir)
{
	m_directory = QDir(dir);
	m_name=dir;
	m_name.remove(0,dir.lastIndexOf("/")+1);
	setIndex();
}

bool depthViewer::setIndex()
{
	QFile fin(m_directory.absolutePath()+"/seq.index");
	if (!fin.exists()) return false;
	fin.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream stream(&fin);
	const QRegExp omitBlank("\\s+");
	while (!stream.atEnd()) 
	{
		bool isOk = false;
		const QStringList tokens = stream.readLine().split(omitBlank);
		const qreal time = tokens.at(0).toDouble(&isOk);
		const unsigned value = tokens.at(1).toUInt(&isOk);
		if (isOk) m_Index[time] = value;
	}
	return true;
}

const QString& depthViewer::getName() const
{
	return m_name;
}

const QMap<qreal, unsigned>& depthViewer::getIndex() const
{
	return m_Index;
}

QString depthViewer::getFrame(qreal timestamp) const
{
	QString filename;
	if (m_Index.lowerBound(timestamp)!=m_Index.end())
	{
		unsigned index=m_Index.lowerBound(timestamp).value();
		//QString file=QString().sprintf("frame_%010d.png", index);
		QString file=QString().sprintf("frame_%010d.png", index);//here QString file has the file name of the png
		return file;
		for (int i=2;i<m_directory.entryList().size();i++)
		{
			filename=m_directory.absolutePath() + "/" + m_directory.entryList()[i] + "/" + file;
			if (QFile(filename).exists())
			{
				return filename;
			}
		}
	}
	return QString();
}

