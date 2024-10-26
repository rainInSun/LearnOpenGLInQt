#include "progressexample.h"
#include "coolprogress.h"

ProgressExample::ProgressExample(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(600, 200);
	setWindowTitle("LearnOpenGLInQt-CoolProgress");

	m_progressRing_1 = new CoolProgress(false, CoolProgress::Ring_1, 200, 200, this);
	m_progressRing_1->move(QPoint(0, 0));
	m_progressRing_1->startProgress(1.0);

	m_progressRing_3 = new CoolProgress(false, CoolProgress::Ring_3, 200, 200, this);
	m_progressRing_3->move(QPoint(200, 0));
	m_progressRing_3->startProgress(1.0);

	m_progressFlashDot_3 = new CoolProgress(false, CoolProgress::FlashDot_3, 200, 200, this);
	m_progressFlashDot_3->move(QPoint(400, 0));
	m_progressFlashDot_3->startProgress(1.0);
}

ProgressExample::~ProgressExample()
{
}