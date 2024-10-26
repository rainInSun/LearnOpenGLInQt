#ifndef SCENETRANSITION_EXAMPLE_H
#define SCENETRANSITION_EXAMPLE_H

#include <QWidget>
#include <QTimer>

class SceneTransition;

class SceneTransitionExample : public QWidget
{
	Q_OBJECT

public:
	SceneTransitionExample(QWidget *parent = nullptr);
	~SceneTransitionExample();

	void onTimer();

private:
	SceneTransition *m_sceneTransition = nullptr;
	QTimer m_timer;
	int m_transitionType = 0;
	int m_imgCnt = 0;
	int m_curImg = 0;
	int m_direction = 0;
};
#endif