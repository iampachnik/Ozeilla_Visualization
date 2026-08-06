#include "main/translation.h"

#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalBlocker>
#include <QVBoxLayout>

namespace {
QDoubleSpinBox* makeAxisSpinBox() {
    auto* box = new QDoubleSpinBox();
    box->setRange(-1000.0, 1000.0);
    box->setDecimals(2);
    box->setSingleStep(0.25);
    return box;
}
}

TranslationWidget::TranslationWidget(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("background-color: #e7e7e7;");
    setupUi();
}

void TranslationWidget::setupUi() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    auto* title = new QLabel("Ozeilla Virtualization", this);
    title->setStyleSheet("color: #001D3D; font-size: 36px; font-weight: bold;");
    layout->addWidget(title);

    layout->addSpacing(16);

    auto* description = new QLabel(
        "App that collects interactive and visualization tools for various knowledge points in robotics.", this);
    description->setWordWrap(true);
    description->setAlignment(Qt::AlignCenter);
    description->setStyleSheet("color: #000814; font-size: 16px;");
    layout->addWidget(description);

    layout->addSpacing(16);

    auto* description2 = new QLabel(
        "This is the virtualization of translattion matrix. The dark axes 'O-xyz' are the shifted frame axes and the light one is the reference frame 'O-xyz'.", this);
    description2->setWordWrap(true);
    description2->setAlignment(Qt::AlignCenter);
    description2->setStyleSheet("color: #000814; font-size: 16px;");
    layout->addWidget(description2);

    layout->addSpacing(16);

    auto* description3 = new QLabel(
        "Here are the position of 'O-xyz'. Feel free to change the value:", this);
    description3->setWordWrap(true);
    description3->setAlignment(Qt::AlignCenter);
    description3->setStyleSheet("color: #000814; font-size: 16px;");
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

    resetButton_ = new QPushButton("Reset do (0, 0, 0)", this);
    resetButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #001D3D;"
        "   color: white;"
        "   border: 1px solid #003566;"
        "   border-radius: 6px;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #003566;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #003566;"
        "}"
    );

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(resetButton_);
    layout->addLayout(buttonLayout);

    layout->addStretch(1);

    connect(resetButton_, &QPushButton::clicked, this, &TranslationWidget::onResetPosition);
    connect(xInput_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TranslationWidget::onSetPosition);
    connect(yInput_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TranslationWidget::onSetPosition);
    connect(zInput_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &TranslationWidget::onSetPosition);
}

void TranslationWidget::onSetPosition() {
    currentPosition_ = HomePosition::Position3D{
        xInput_->value(),
        yInput_->value(),
        zInput_->value()
    };
    emit positionChanged(currentPosition_);
}

void TranslationWidget::onResetPosition() {
    currentPosition_ = HomePosition::START_POSITION;

    const QSignalBlocker blockX(xInput_);
    const QSignalBlocker blockY(yInput_);
    const QSignalBlocker blockZ(zInput_);
    xInput_->setValue(currentPosition_.x);
    yInput_->setValue(currentPosition_.y);
    zInput_->setValue(currentPosition_.z);

    emit positionChanged(currentPosition_);
}