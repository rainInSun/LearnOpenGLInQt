#include "scenetransitionexample.h"
#include "scenetransition.h"
#include <QRandomGenerator>

SceneTransitionExample::SceneTransitionExample(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(1000, 580);
	setWindowTitle("LearnOpenGLInQt-SceneTransition");
	m_sceneTransition = new SceneTransition(this);
	m_sceneTransition->setFixedSize(1000, 580);
	

	QStringList list = {
	"img/01.jpg",
	"img/02.jpg",
	"img/03.jpg",
	"img/04.jpg",
	"img/05.png",
	};
	m_imgCnt = list.size();
	QVector<QImage> imgVec;
	for (int i = 0; i < list.size(); i++)
	{
		QImage img(list[i]);
		imgVec.push_back(img);
	}

	m_sceneTransition->setImgList(imgVec);
	m_sceneTransition->setTransitionDuration(2000);
	connect(&m_timer, &QTimer::timeout, this, &SceneTransitionExample::onTimer);
	m_timer.start(3000);
}

SceneTransitionExample::~SceneTransitionExample()
{
}

void SceneTransitionExample::onTimer()
{
	if (0 == m_curImg)
		m_direction = 0;
	if (m_imgCnt - 1 == m_curImg)
		m_direction = 1;

	m_transitionType = QRandomGenerator::global()->bounded(0, TransitionType::MaxCnt);
	m_sceneTransition->setTransitionType((TransitionType)m_transitionType);

	if (0 == m_direction)
	{
		m_sceneTransition->goNext();
		m_curImg++;
	}
	else if (1 == m_direction)
	{
		m_sceneTransition->goPrev();
		m_curImg--;
	}
	update();
}