#include <QApplication>
#include <QKeyEvent>
#include <chrono>
#include "SnakeGameQt.h"

void onMove(SnakeGameQt* thread)
{
	while (true)
	{
		if (!thread->IsRunning())
			continue;

		thread->MoveDirection(thread->LastKeyPressed);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

SnakeGameQt::SnakeGameQt(QWidget* parent) : QWidget(parent)
{
	setFixedSize(900, 750);
	setWindowTitle("Snake");

	m_GameWindow = new QWidget(this);
	m_GameWindow->setFixedSize(730, 730);
	m_GameWindow->setGeometry(10, 10, 730, 730);

	m_gridLayout = new QGridLayout(m_GameWindow);
	m_snakePicture = new QPixmap("Assets/Snake/snakePicture.png");

	for (int i = 0; i < 11; i++)
	{
		m_fruitsArray[i] = new QPixmap(std::string("Assets/Food/" + std::to_string(i) + ".png").c_str());
	}

	m_generator = *(new std::default_random_engine());
	m_randomEngine = *(new std::mt19937(m_generator()));
	m_distribution = *(new std::uniform_int_distribution<unsigned int>(1, GridSize));

	m_randomEngine.seed(std::chrono::system_clock::now().time_since_epoch().count());

	InitUI();
	InitGrid();
	InitSnake();
	InitFood();

	snakeThread = std::thread(onMove, this);
}

SnakeGameQt::~SnakeGameQt()
{
	snakeThread.join();
	for (size_t i = 0; i < GridSize * GridSize; i++)
	{
		QLayoutItem* item = m_gridLayout->itemAt(i);
		if (item)
			delete qobject_cast<QLabel*>(item->widget());
	}
}

void SnakeGameQt::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Up:
		LastKeyPressed = Direction::Up;
		break;
	case Qt::Key_Left:
		LastKeyPressed = Direction::Left;
		break;
	case Qt::Key_Down:
		LastKeyPressed = Direction::Down;
		break;
	case Qt::Key_Right:
		LastKeyPressed = Direction::Right;
		break;
	case Qt::Key_Z:
		LastKeyPressed = Direction::Up;
		break;
	case Qt::Key_Q:
		LastKeyPressed = Direction::Left;
		break;
	case Qt::Key_S:
		LastKeyPressed = Direction::Down;
		break;
	case Qt::Key_D:
		LastKeyPressed = Direction::Right;
		break;
	}
	repaint();
}

void SnakeGameQt::InitGrid()
{
	m_gridLayout->setHorizontalSpacing(0);
	m_gridLayout->setVerticalSpacing(0);

	for (size_t i = 1; i <= GridSize; i++)
	{
		m_gridLayout->setColumnMinimumWidth(i, 40);
		for (size_t j = 1; j <= GridSize; j++)
		{
			m_gridLayout->setColumnMinimumWidth(j, 40);
			QLabel* label = new QLabel(this);

			if ((i + j) % 2 == 0)
				label->setStyleSheet("background-color: #A7D948");
			else
				label->setStyleSheet("background-color: #8ECC39");

			m_gridLayout->addWidget(label, j, i);
		}
	}
}

void SnakeGameQt::InitSnake()
{
	snakeCordonate elem = { m_distribution(m_generator), m_distribution(m_generator) };
	m_snake.push_front(elem);
	qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_snake.front().posX, m_snake.front().posY)->widget())->setPixmap(*m_snakePicture);
}

void SnakeGameQt::InitFood()
{
	m_foodActualPos.posX = m_distribution(m_generator);
	m_foodActualPos.posY = m_distribution(m_generator);

	if (m_foodActualPos.posX != m_snake.front().posX || m_foodActualPos.posY != m_snake.front().posY)
		qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_foodActualPos.posX, m_foodActualPos.posY)->widget())->setPixmap(*m_fruitsArray[m_distribution(m_generator) % 11]);
	else
		InitFood();
}

void SnakeGameQt::InitUI()
{
	m_snakeTitle = new QLabel(this);
	m_snakeTitle->setText("Snake");
	m_snakeTitle->setGeometry(670, 0, 300, 100);
	m_snakeTitle->setAlignment(Qt::AlignCenter);
	m_snakeTitle->setStyleSheet("font-size: 40px");
	m_snakeTitle->show();

	m_snakeScore = new QLabel(this);
	m_snakeScore->setText("Score : " + QString::number(m_score));
	m_snakeScore->setGeometry(740, 200, 200, 100);
	m_snakeScore->setAlignment(Qt::AlignLeft);
	m_snakeScore->setStyleSheet("font-size: 20px");
	m_snakeScore->show();

	m_snakeBestScore = new QLabel(this);
	m_snakeBestScore->setText("Best score : " + QString::number(m_bestScore));
	m_snakeBestScore->setGeometry(740, 300, 200, 100);
	m_snakeBestScore->setAlignment(Qt::AlignLeft);
	m_snakeBestScore->setStyleSheet("font-size: 20px");
	m_snakeBestScore->show();
}

void SnakeGameQt::MoveDirection(const Direction& side)
{
	qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_snake.front().posX, m_snake.front().posY)->widget())->clear();

	snakeCordonate newCoordinates = m_snake.front();
	switch (side)
	{
	case Direction::Up:
		if (m_snake.front().posX == 1) newCoordinates.posX = GridSize; else newCoordinates.posX--;
		break;
	case Direction::Left:
		if (m_snake.front().posY == 1) newCoordinates.posY = GridSize; else newCoordinates.posY--;
		break;
	case Direction::Down:
		if (m_snake.front().posX == 18) newCoordinates.posX = 1; else newCoordinates.posX++;
		break;
	case Direction::Right:
		if (m_snake.front().posY == 18) newCoordinates.posY = 1; else newCoordinates.posY++;
		break;
	}
	m_snake.push_front(newCoordinates);
	CheckFood();

	qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_snake.front().posX, m_snake.front().posY)->widget())->setPixmap(*m_snakePicture);
	RenderSnake();
}

void SnakeGameQt::CheckFood()
{
	if (m_snake.front().posX == m_foodActualPos.posX && m_snake.front().posY == m_foodActualPos.posY)
	{
		m_score++;
		CheckScore(m_score);
		InitFood();
	}
	else
	{
		qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_snake.back().posX, m_snake.back().posY)->widget())->clear();
		m_snake.pop_back();
	}
}

void SnakeGameQt::CheckScore(unsigned int score)
{
	if (m_score > m_bestScore)
	{
		m_bestScore = m_score;
		m_snakeBestScore->setText("Best score : " + QString::number(m_bestScore));
	}
	m_snakeScore->setText("Score : " + QString::number(m_score));
}

void SnakeGameQt::CheckPosition()
{
	for (auto element : m_snake)
	{
		if (element.posX != m_snake.front().posX && element.posY != m_snake.front().posY)
		{
			if (m_snake.front().posX == element.posX && m_snake.front().posY == element.posY)
			{
				Die();
			}
		}
	}
}

void SnakeGameQt::RenderSnake()
{
	for (auto element : m_snake)
	{
		qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(element.posX, element.posY)->widget())->setPixmap(*m_snakePicture);
	}
}

void SnakeGameQt::Die()
{
	CheckScore(m_score);
}
