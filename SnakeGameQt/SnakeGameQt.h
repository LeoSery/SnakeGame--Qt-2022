#pragma once

#include <QtWidgets/QWidget>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <thread>

#include <random>

class SnakeGameQt : public QWidget
{
	Q_OBJECT

public:
	SnakeGameQt(QWidget* parent = Q_NULLPTR);
	~SnakeGameQt();
	enum class Direction { Up, Left, Down, Right, };
	void MoveDirection(const Direction&);
	Direction LastKeyPressed = Direction::Right;

protected:
	void keyPressEvent(QKeyEvent* event);

	struct snakeCordonate { int posX = -1; int posY = -1; };
	struct foodCordonate { int posX = -1; int posY = -1; };
private:
	void InitGrid();
	void InitSnake();
	void InitFood();


	void checkFood();
	//void checkSnake();

	std::default_random_engine m_generator;
	std::uniform_int_distribution<unsigned int> m_distribution;


	snakeCordonate m_snakeActualPos;
	snakeCordonate m_snakeOldPos;

	foodCordonate m_foodActualPos;
	foodCordonate m_foodOldPos;

	QGridLayout* m_gridLayout;
	QPixmap* m_snakePicture;
	QPixmap* m_fruitsArray[11];

	static constexpr size_t GridSize = 18;
	unsigned int score = 0;
	std::thread th;
};
