#include <QApplication>
#include <QKeyEvent>
#include <chrono>

#include "SnakeGameQt.h"

void onMove(SnakeGameQt* t)
{
	while (true)
	{
		t->MoveDirection(t->LastKeyPressed);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

SnakeGameQt::SnakeGameQt(QWidget* parent) : QWidget(parent)
{
	setFixedSize(730, 730);

	m_gridLayout = new QGridLayout(this);
	m_snakePicture = new QPixmap("Assets/Snake/snakePicture.png");

	for (int i = 0; i < 11; i++)
	{
		m_fruitsArray[i] = new QPixmap(std::string("Assets/Food/" + std::to_string(i) + ".png").c_str());
	}

	m_generator = *(new std::default_random_engine());
	m_distribution = *(new std::uniform_int_distribution<unsigned int>(1, GridSize));

	InitGrid();
	InitSnake();
	InitFood();
	setLayout(m_gridLayout);

	th = std::thread(&onMove, this);
}

SnakeGameQt::~SnakeGameQt()
{
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
	m_snakeActualPos.posX = m_distribution(m_generator);
	m_snakeActualPos.posY = m_distribution(m_generator);

	qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_snakeActualPos.posX, m_snakeActualPos.posY)->widget())->setPixmap(*m_snakePicture);
}

void SnakeGameQt::InitFood()
{
	m_foodActualPos.posX = m_distribution(m_generator);
	m_foodActualPos.posY = m_distribution(m_generator);

	if (m_foodActualPos.posX != m_snakeActualPos.posX || m_foodActualPos.posY != m_snakeActualPos.posY)
		qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_foodActualPos.posX, m_foodActualPos.posY)->widget())->setPixmap(*m_fruitsArray[m_distribution(m_generator) % 11]);
	else
		InitFood();
}

void SnakeGameQt::MoveDirection(const Direction& side)
{
	qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_snakeActualPos.posX, m_snakeActualPos.posY)->widget())->clear();
	m_snakeOldPos = m_snakeActualPos;

	switch (side)
	{
	case Direction::Up:
		if (m_snakeActualPos.posX == 1) m_snakeActualPos.posX = GridSize; else m_snakeActualPos.posX--;
		break;
	case Direction::Left:
		if (m_snakeActualPos.posY == 1) m_snakeActualPos.posY = GridSize; else m_snakeActualPos.posY--;
		break;
	case Direction::Down:
		if (m_snakeActualPos.posX == 18) m_snakeActualPos.posX = 1; else m_snakeActualPos.posX++;
		break;
	case Direction::Right:
		if (m_snakeActualPos.posY == 18) m_snakeActualPos.posY = 1; else m_snakeActualPos.posY++;
		break;
	}
	checkFood();

	qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_snakeActualPos.posX, m_snakeActualPos.posY)->widget())->setPixmap(*m_snakePicture);
}

void SnakeGameQt::checkFood()
{
	if (m_snakeActualPos.posX == m_foodActualPos.posX && m_snakeActualPos.posY == m_foodActualPos.posY)
	{
		score++;
		//checkSnake();
		InitFood();
	}
}

//void SnakeGameQt::checkSnake()
//{
//
//}