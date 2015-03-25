#include "QtCore/QDir.h"
#include "QtCore/QMap.h"


class depthViewer
{
public:
	depthViewer();
	depthViewer(const QString& dir);
	~depthViewer();
	QString getFrameIR(qreal timestamp) const;

	void setUp(const QString& dir);
	const QString& getName() const;
	const QMap<qreal, unsigned>& getIndex() const;
	QString getFrame(qreal timestamp) const;

private:
	bool setIndex();
	
	QDir m_directory;
	QString m_name;
	QMap<qreal, unsigned> m_Index;
};
