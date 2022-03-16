#pragma once

#include <QtWidgets/QWidget>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

class SnakeGameQt : public QWidget
{
	Q_OBJECT

public:
	SnakeGameQt(QWidget* parent = Q_NULLPTR);
	~SnakeGameQt();
protected:
	void keyPressEvent(QKeyEvent* event);

	struct snakeCordonate { int posX = -1; int posY = -1; };
	enum class Direction { Up, Left, Down, Right, };
private:
	void InitGrid();
	void InitSnake();
	void Move(const Direction&);

	snakeCordonate m_actualPos;
	snakeCordonate m_oldPos;
	QGridLayout* m_gridLayout;
	QPixmap* m_snakePicture;

	static constexpr size_t GridSize = 18;
};
