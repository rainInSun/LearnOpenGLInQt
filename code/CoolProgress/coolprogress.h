#ifndef COOL_PROGRESS_H
#define COOL_PROGRESS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <memory>

class CoolProgressImpl;

class CoolProgress : public QOpenGLWidget
{
	Q_OBJECT

public:
	enum ProgressStyle
	{
		Ring_1 = 0,
		Ring_2,
		Ring_3,
		FlashDot_1,
		FlashDot_2,
		FlashDot_3,
		FlashDot_4,
		Rect_1,
		Rect_2,
		Rect_3,
		Polygon_1,
		WaterWave_1
	};
public:
	CoolProgress(bool bTransparent, ProgressStyle style, int width = 100, int height = 100, QWidget *parent = nullptr);
	~CoolProgress();

	void startProgress(float speed = 1.0);
	void stopProgress();
	void updateProgress();
	// only valid in Ring style
	void setRingRadius(float r1, float r2);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

private:
	void initShader();
	void setShaderUniform();

private:
	std::unique_ptr<CoolProgressImpl> m_impl;
};
#endif