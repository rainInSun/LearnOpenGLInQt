/*
MIT License  
  
Copyright (c) 2024 rainInSun 
  
Permission is hereby granted, free of charge, to any person obtaining a copy  
of this software and associated documentation files (the "Software"), to deal  
in the Software without restriction, including without limitation the rights  
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell  
copies of the Software, and to permit persons to whom the Software is  
furnished to do so, subject to the following conditions:  
  
The above copyright notice and this permission notice shall be included in all  
copies or substantial portions of the Software.  
  
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,  
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  
SOFTWARE.
*/

#ifndef SCENETRANSITION_H
#define SCENETRANSITION_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <memory>
#include "scenetransitiondef.h"

class SceneTransitionImpl;

class SceneTransition : public QOpenGLWidget
{
	Q_OBJECT

public:
	SceneTransition(QWidget *parent = nullptr);
	virtual ~SceneTransition();

	void goNext();
	void goPrev();
	void setTransitionType(TransitionType type);
	void setTransitionDuration(int duration);
	void setImgList(const QVector<QImage>& imgList);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

	void onTimer();

signals:
	void initialized();

private:
	std::unique_ptr<SceneTransitionImpl> m_impl;
};
#endif