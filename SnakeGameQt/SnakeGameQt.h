#pragma once

#include <QtWidgets/QWidget>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <list>
#include <thread>
#include <random>

class SnakeGameQt : public QWidget
{
	Q_OBJECT

public:
	SnakeGameQt(QWidget* parent = Q_NULLPTR);
	~SnakeGameQt();
	bool IsRunning() const { return m_gameIsRunning; }

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
	void InitUI();

	void CheckFood();
	void CheckScore(unsigned int score);
	void CheckPosition();

	void RenderSnake();
	void Die();

	std::default_random_engine m_generator;
	std::mt19937 m_randomEngine;
	std::uniform_int_distribution<unsigned int> m_distribution;
	std::list<snakeCordonate> m_snake;

	foodCordonate m_foodActualPos;
	foodCordonate m_foodOldPos;

	QWidget* m_GameWindow;
	QGridLayout* m_gridLayout;
	QPixmap* m_snakePicture;
	QPixmap* m_fruitsArray[11];
	QLabel* m_snakeTitle;
	QLabel* m_snakeScore;
	QLabel* m_snakeBestScore;

	static constexpr size_t GridSize = 18;

	unsigned int m_score = 0;
	unsigned int m_bestScore = 0;

	std::thread snakeThread;

	bool m_gameIsRunning = true;
};
