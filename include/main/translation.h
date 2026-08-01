#pragma once

#include <QWidget>
#include "home_position/home_position.h"

class QDoubleSpinBox;
class QPushButton;

class TranslationWidget : public QWidget {
    Q_OBJECT

public:
    explicit TranslationWidget(QWidget* parent = nullptr);

    HomePosition::Position3D currentPosition() const { return currentPosition_; }

private slots:
    void onSetPosition();
    void onResetPosition();

private:
    void setupUi();

    QDoubleSpinBox* xInput_ = nullptr;
    QDoubleSpinBox* yInput_ = nullptr;
    QDoubleSpinBox* zInput_ = nullptr;
    QPushButton* resetButton_ = nullptr;

    HomePosition::Position3D currentPosition_{};
};