#include "glsl_02.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSurfaceFormat fmt;
    // 指定OpenGL版本
	fmt.setVersion(3, 3);
	fmt.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(fmt);
	GLSL_02 w;
	w.show();
	return a.exec();
}