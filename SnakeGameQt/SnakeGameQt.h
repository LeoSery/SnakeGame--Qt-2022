#pragma once

#include <QtWidgets/QMainWindow>
#include <QGridLayout>

class SnakeGameQt : public QWidget
{
    Q_OBJECT

public:
    SnakeGameQt(QWidget *parent = Q_NULLPTR);
private: 
    void InitGrid();
    QGridLayout* m_GridLayout;
};
