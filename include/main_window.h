#pragma once

#include <QWidget>
#include "home_position/home_position.h"

class QDoubleSpinBox;
class QPushButton;
class QLabel;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onSetPosition();
    void onResetPosition();

private:
    void setupDataPanel();
    // void updateStatusLabel();

    QWidget* dataPanel_ = nullptr;
    QWidget* visualizationPanel_ = nullptr;

    QDoubleSpinBox* xInput_ = nullptr;
    QDoubleSpinBox* yInput_ = nullptr;
    QDoubleSpinBox* zInput_ = nullptr;

    // QPushButton* setButton_ = nullptr;
    QPushButton* resetButton_ = nullptr;

    // QLabel* statusLabel_ = nullptr;

    HomePosition::Position3D currentPosition_{};
};