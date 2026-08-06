#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_1>
#include <QPoint>

#include "home_position/home_position.h"

// QOpenGLFunctions_2_1 (not plain QOpenGLFunctions) is required because we use
// the legacy fixed-function pipeline (glBegin/glEnd, glMatrixMode, glFrustum...,
// glLight/glMaterial), which the ES2-style QOpenGLFunctions subset does not
// expose. This matches the QSurfaceFormat::CompatibilityProfile / OpenGL 2.1
// requested in main.cpp.
class CoordinateSystemWidget : public QOpenGLWidget, protected QOpenGLFunctions_2_1 {
    Q_OBJECT

public:
    explicit CoordinateSystemWidget(QWidget* parent = nullptr);

public slots:
    void setTranslatedPosition(const HomePosition::Position3D& pos);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void applyProjection();
    void applyCamera(float effectiveDistance);
    void drawGrid(float size, int divisions);
    void drawAxes(float length);
    void drawArrowhead(float baseRadius, float coneLength);
    void drawSphere(float radius, int slices, int stacks);

    HomePosition::Position3D translatedPosition_{};

    // How far the scene needs to reach to keep the ball + axes fully in view.
    // Recomputed automatically every time the translated position changes.
    float sceneExtent_ = 6.0f;

    // User-controlled zoom, applied on top of the auto-computed scene distance,
    // so scrolling still feels consistent no matter how large the scene gets.
    float zoomFactor_ = 1.0f;

    float yaw_ = 35.0f;
    float pitch_ = 32.0f;

    QPoint lastMousePos_;
    bool dragging_ = false;
};
