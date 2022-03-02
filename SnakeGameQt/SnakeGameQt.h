#pragma once

#include <QtWidgets/QMainWindow>
#include <QGridLayout>

class SnakeGameQt : public QWidget
{
	Q_OBJECT

public:
	SnakeGameQt(QWidget* parent = Q_NULLPTR);
private:
	void InitGrid();
	void InitSnake();
	QGridLayout* m_GridLayout;

	struct snakeCordonate { int posX; int posY; };
};
