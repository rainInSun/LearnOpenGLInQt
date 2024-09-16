#ifndef PROGRESS_EXAMPLE_H
#define PROGRESS_EXAMPLE_H


#include <QWidget>

class CoolProgress;

class ProgressExample : public QWidget
{
	Q_OBJECT

public:
	ProgressExample(QWidget *parent = nullptr);
	~ProgressExample();

private:
	CoolProgress *m_progressRing_1 = nullptr;
	CoolProgress *m_progressRing_3 = nullptr;
    CoolProgress *m_progressFlashDot_3 = nullptr;
};
#endif