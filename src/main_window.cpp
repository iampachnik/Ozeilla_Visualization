#include "main_window.h"

#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace {
QDoubleSpinBox* makeAxisSpinBox() {
    auto* box = new QDoubleSpinBox();
    box->setRange(-1000.0, 1000.0);
    box->setDecimals(2);
    box->setSingleStep(0.5);
    return box;
}
} 

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Ozeilla Visualization");
    showMaximized();

    dataPanel_ = new QWidget();
    dataPanel_->setStyleSheet("background-color: #2b2b2b;");
    setupDataPanel();

    visualizationPanel_ = new QWidget();
    visualizationPanel_->setStyleSheet("background-color: #1e1e1e;");

    auto* divider = new QFrame(this);
    divider->setFixedWidth(6);
    divider->setStyleSheet("background-color: #888;");

    auto* contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(dataPanel_);
    contentLayout->addWidget(divider);
    contentLayout->addWidget(visualizationPanel_);
    contentLayout->setStretch(0, 1);
    contentLayout->setStretch(2, 2);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(contentLayout);
}

void MainWindow::setupDataPanel() {
    auto* layout = new QVBoxLayout(dataPanel_);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    auto* title = new QLabel("Ozeilla Virtualization", dataPanel_);
    title->setStyleSheet("color: white; font-size: 36px; font-weight: bold;");
    layout->addWidget(title);

    layout->addSpacing(16);

    auto* description = new QLabel(
        "App that collects interactive and visualization tools for various knowledge points in robotics.", dataPanel_);
    description->setWordWrap(true);
    description->setAlignment(Qt::AlignCenter);
    description->setStyleSheet("color: white; font-size: 16px;");
    layout->addWidget(description);

    layout->addSpacing(16);

    auto* description2 = new QLabel(
        "This is the virtualization of translattion matrix. The dark axes 'O-xyz' are the shifted frame axes and the light one is the reference frame 'O-xyz'.", dataPanel_);
    description2->setWordWrap(true);
    description2->setAlignment(Qt::AlignCenter);
    description2->setStyleSheet("color: white; font-size: 16px;");
    layout->addWidget(description2);

    layout->addSpacing(16);

    auto* description3 = new QLabel(
        "Here are the position of 'O-xyz'. Feel free to change the value:", dataPanel_);
    description3->setWordWrap(true);
    description3->setAlignment(Qt::AlignCenter);
    description3->setStyleSheet("color: white; font-size: 16px;");
    layout->addWidget(description3);

    
    xInput_ = makeAxisSpinBox();
    yInput_ = makeAxisSpinBox();
    zInput_ = makeAxisSpinBox();

    auto* formLayout = new QFormLayout();
    formLayout->addRow("X:", xInput_);
    formLayout->addRow("Y:", yInput_);
    formLayout->addRow("Z:", zInput_);
    layout->addLayout(formLayout);

    layout->addSpacing(16);
    
    // setButton_ = new QPushButton("Ustaw pozycję", dataPanel_);
    resetButton_ = new QPushButton("Reset do (0, 0, 0)", dataPanel_);
    resetButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #444;"
        "   color: white;"
        "   border: 1px solid #666;"
        "   border-radius: 6px;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #555;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #333;"
        "}"
    );

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(resetButton_);
    layout->addLayout(buttonLayout);

    // statusLabel_ = new QLabel(dataPanel_);
    // statusLabel_->setAlignment(Qt::AlignCenter);
    // tatusLabel_->setStyleSheet("font-weight: bold; padding: 8px; color: white;");
    // layout->addWidget(statusLabel_);

    layout->addStretch(1);

    // connect(setButton_, &QPushButton::clicked, this, &MainWindow::onSetPosition);
    connect(resetButton_, &QPushButton::clicked, this, &MainWindow::onResetPosition);

    //updateStatusLabel();
}

void MainWindow::onSetPosition() {
    currentPosition_ = HomePosition::Position3D{
        xInput_->value(),
        yInput_->value(),
        zInput_->value()
    };
    // updateStatusLabel();
}

void MainWindow::onResetPosition() {
    HomePosition::resetButtonToStart(currentPosition_);
    xInput_->setValue(currentPosition_.x);
    yInput_->setValue(currentPosition_.y);
    zInput_->setValue(currentPosition_.z);
    // updateStatusLabel();
}

/*
void MainWindow::updateStatusLabel() {
    statusLabel_->setText(QString("Pozycja: (%1, %2, %3)")
        .arg(currentPosition_.x, 0, 'f', 2)
        .arg(currentPosition_.y, 0, 'f', 2)
        .arg(currentPosition_.z, 0, 'f', 2));
}
*/