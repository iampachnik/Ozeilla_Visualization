#pragma once

#include <QWidget>

class TranslationWidget;
class CoordinateSystemWidget;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    TranslationWidget* dataPanel_ = nullptr;
    CoordinateSystemWidget* visualizationPanel_ = nullptr;
};