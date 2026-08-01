#include "main/main_window.h"
#include "main/translation.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Ozeilla Visualization");
    showMaximized();

    dataPanel_ = new TranslationWidget();

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