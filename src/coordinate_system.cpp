#include "coordinate_system/coordinate_system.h"

#include <QMouseEvent>
#include <QWheelEvent>

#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace {
constexpr float kMinZoom = 0.25f;
constexpr float kMaxZoom = 6.0f;
constexpr float kMinSceneExtent = 6.0f;
}

CoordinateSystemWidget::CoordinateSystemWidget(QWidget* parent)
    : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(200, 200);
}

void CoordinateSystemWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.118f, 0.118f, 0.118f, 1.0f); // matches panel's #1e1e1e
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Simple directional light + color-tracked material so glColor4f still
    // works for tinting while the sphere gets proper shading (depth cue).
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);

    const GLfloat lightPos[] = {0.4f, 1.0f, 0.6f, 0.0f}; // directional
    const GLfloat lightAmbient[] = {0.35f, 0.35f, 0.35f, 1.0f};
    const GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glDisable(GL_LIGHTING); // off by default; enabled only while drawing the sphere
}

void CoordinateSystemWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void CoordinateSystemWidget::applyProjection() {
    // Recomputed every frame (cheap) because the far plane must grow with the
    // scene: without this, large positions (e.g. Z=100) would get clipped.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const int w = width();
    const int h = height();
    const float aspect = h == 0 ? 1.0f : static_cast<float>(w) / static_cast<float>(h);

    const float fov = 45.0f;
    const float nearPlane = 0.1f;
    const float farPlane = std::max(200.0f, sceneExtent_ * 10.0f);

    const float top = std::tan(fov * 0.5f * static_cast<float>(M_PI) / 180.0f) * nearPlane;
    const float right = top * aspect;
    glFrustum(-right, right, -top, top, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
}

void CoordinateSystemWidget::applyCamera(float effectiveDistance) {
    glLoadIdentity();

    const float yawRad = yaw_ * static_cast<float>(M_PI) / 180.0f;
    const float pitchRad = pitch_ * static_cast<float>(M_PI) / 180.0f;

    const float eyeX = effectiveDistance * std::cos(pitchRad) * std::sin(yawRad);
    const float eyeY = effectiveDistance * std::sin(pitchRad);
    const float eyeZ = effectiveDistance * std::cos(pitchRad) * std::cos(yawRad);

    auto normalize = [](float& x, float& y, float& z) {
        const float len = std::sqrt(x * x + y * y + z * z);
        if (len > 1e-6f) {
            x /= len;
            y /= len;
            z /= len;
        }
    };

    // Forward vector (camera -> origin)
    float fx = -eyeX, fy = -eyeY, fz = -eyeZ;
    normalize(fx, fy, fz);

    const float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

    // Right = forward x up
    float sx = fy * upZ - fz * upY;
    float sy = fz * upX - fx * upZ;
    float sz = fx * upY - fy * upX;
    normalize(sx, sy, sz);

    // True up = right x forward
    const float ux = sy * fz - sz * fy;
    const float uy = sz * fx - sx * fz;
    const float uz = sx * fy - sy * fx;

    const GLfloat m[16] = {
        sx,  ux,  -fx, 0.0f,
        sy,  uy,  -fy, 0.0f,
        sz,  uz,  -fz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    glMultMatrixf(m);
    glTranslatef(-eyeX, -eyeY, -eyeZ);
}

void CoordinateSystemWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    applyProjection();

    const float effectiveDistance = sceneExtent_ * 2.2f * zoomFactor_;
    applyCamera(effectiveDistance);

    drawGrid(sceneExtent_, 20);

    // Reference frame O-xyz spans the full scene extent, vivid and opaque.
    glPushMatrix();
    drawAxes(sceneExtent_);
    glPopMatrix();

    // Translated position shown as a solid, lit red sphere (no connecting line).
    glPushMatrix();
    glTranslated(translatedPosition_.x, translatedPosition_.y, translatedPosition_.z);
    glEnable(GL_LIGHTING);
    glColor4f(1.0f, 0.15f, 0.15f, 1.0f);
    const float sphereRadius = std::clamp(sceneExtent_ * 0.035f, 0.2f, 1.2f);
    drawSphere(sphereRadius, 28, 18);
    glDisable(GL_LIGHTING);
    glPopMatrix();
}

void CoordinateSystemWidget::drawGrid(float size, int divisions) {
    glColor4f(0.35f, 0.35f, 0.35f, 0.4f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    const float step = (size * 2.0f) / divisions;
    for (int i = 0; i <= divisions; ++i) {
        const float pos = -size + i * step;
        glVertex3f(pos, 0.0f, -size);
        glVertex3f(pos, 0.0f, size);
        glVertex3f(-size, 0.0f, pos);
        glVertex3f(size, 0.0f, pos);
    }
    glEnd();
}

void CoordinateSystemWidget::drawAxes(float length) {
    const float coneLength = length * 0.08f;
    const float shaftLength = length - coneLength;
    const float coneRadius = length * 0.025f;

    glLineWidth(6.0f);

    // X - vivid red
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(shaftLength, 0, 0);
    glEnd();
    glPushMatrix();
    glTranslatef(shaftLength, 0, 0);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawArrowhead(coneRadius, coneLength);
    glPopMatrix();

    // Y - vivid green
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, shaftLength, 0);
    glEnd();
    glPushMatrix();
    glTranslatef(0, shaftLength, 0);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawArrowhead(coneRadius, coneLength);
    glPopMatrix();

    // Z - vivid blue
    glColor4f(0.15f, 0.45f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, shaftLength);
    glEnd();
    glPushMatrix();
    glTranslatef(0, 0, shaftLength);
    drawArrowhead(coneRadius, coneLength);
    glPopMatrix();
}

// Draws a simple cone pointing along +Z, base at the origin of the current
// matrix. Used as an arrowhead at each axis tip for a more "engineered" look.
void CoordinateSystemWidget::drawArrowhead(float baseRadius, float coneLength) {
    constexpr int kSlices = 12;

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, coneLength);
    for (int i = 0; i <= kSlices; ++i) {
        const float angle = 2.0f * static_cast<float>(M_PI) * static_cast<float>(i) / kSlices;
        glVertex3f(baseRadius * std::cos(angle), baseRadius * std::sin(angle), 0.0f);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = kSlices; i >= 0; --i) {
        const float angle = 2.0f * static_cast<float>(M_PI) * static_cast<float>(i) / kSlices;
        glVertex3f(baseRadius * std::cos(angle), baseRadius * std::sin(angle), 0.0f);
    }
    glEnd();
}

void CoordinateSystemWidget::drawSphere(float radius, int slices, int stacks) {
    for (int i = 0; i < stacks; ++i) {
        const float lat0 = static_cast<float>(M_PI) * (-0.5f + static_cast<float>(i) / stacks);
        const float lat1 = static_cast<float>(M_PI) * (-0.5f + static_cast<float>(i + 1) / stacks);

        const float y0 = std::sin(lat0);
        const float r0 = std::cos(lat0);
        const float y1 = std::sin(lat1);
        const float r1 = std::cos(lat1);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            const float lng = 2.0f * static_cast<float>(M_PI) * static_cast<float>(j) / slices;
            const float x = std::cos(lng);
            const float z = std::sin(lng);

            glNormal3f(x * r0, y0, z * r0);
            glVertex3f(radius * x * r0, radius * y0, radius * z * r0);
            glNormal3f(x * r1, y1, z * r1);
            glVertex3f(radius * x * r1, radius * y1, radius * z * r1);
        }
        glEnd();
    }
}

void CoordinateSystemWidget::setTranslatedPosition(const HomePosition::Position3D& pos) {
    translatedPosition_ = pos;

    // Auto-scale the whole scene (grid, axes, camera distance) so the ball
    // and axes always stay in view, no matter how large the coordinates get.
    const double maxAbs = std::max({std::abs(pos.x), std::abs(pos.y), std::abs(pos.z)});
    sceneExtent_ = std::max(kMinSceneExtent, static_cast<float>(maxAbs) * 1.4f);

    update();
}

void CoordinateSystemWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        dragging_ = true;
        lastMousePos_ = event->pos();
    }
}

void CoordinateSystemWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!dragging_) {
        return;
    }

    const QPoint delta = event->pos() - lastMousePos_;
    lastMousePos_ = event->pos();

    yaw_ += delta.x() * 0.4f;
    pitch_ += delta.y() * 0.4f;
    pitch_ = std::clamp(pitch_, -89.0f, 89.0f);

    update();
}

void CoordinateSystemWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        dragging_ = false;
    }
}

void CoordinateSystemWidget::wheelEvent(QWheelEvent* event) {
    const float steps = event->angleDelta().y() / 120.0f;
    zoomFactor_ *= std::pow(0.9f, steps);
    zoomFactor_ = std::clamp(zoomFactor_, kMinZoom, kMaxZoom);
    update();
}
