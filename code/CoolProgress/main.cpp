#include "progressexample.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSurfaceFormat fmt;
    // 指定OpenGL版本
	fmt.setVersion(3, 3);
	fmt.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(fmt);
	ProgressExample w;
	w.show();
	return a.exec();
}