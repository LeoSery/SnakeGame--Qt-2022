#include <QApplication>
#include <QKeyEvent>
#include <random>

#include "SnakeGameQt.h"

SnakeGameQt::SnakeGameQt(QWidget* parent) : QWidget(parent)
{
	setFixedSize(730, 730);

	m_gridLayout = new QGridLayout(this);
	m_snakePicture = new QPixmap("Assets/Snake/snakePicture.png");

	InitGrid();
	InitSnake();
	setLayout(m_gridLayout);
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
		Move(Direction::Up);
		break;
	case Qt::Key_Left:
		Move(Direction::Left);
		break;
	case Qt::Key_Down:
		Move(Direction::Down);
		break;
	case Qt::Key_Right:
		Move(Direction::Right);
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
	std::default_random_engine generator(static_cast<unsigned int>(time(0)));
	std::uniform_int_distribution<int> distribution(1, GridSize);

	m_actualPos.posX = distribution(generator);
	m_actualPos.posY = distribution(generator);

	qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_actualPos.posX, m_actualPos.posY)->widget())->setPixmap(*m_snakePicture);
}

void SnakeGameQt::Move(const Direction& side)
{
	qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_actualPos.posX, m_actualPos.posY)->widget())->clear();
	m_oldPos = m_actualPos;

	switch (side)
	{
	case Direction::Up:
		if (m_actualPos.posX == 1) m_actualPos.posX = GridSize; else m_actualPos.posX--;
		break;
	case Direction::Left:
		if (m_actualPos.posY == 1) m_actualPos.posY = GridSize; else m_actualPos.posY--;
		break;
	case Direction::Down:
		if (m_actualPos.posX == 18) m_actualPos.posX = 1; else m_actualPos.posX++;
		break;
	case Direction::Right:
		if (m_actualPos.posY == 18) m_actualPos.posY = 1; else m_actualPos.posY++;
		break;
	}

	qobject_cast<QLabel*>(m_gridLayout->itemAtPosition(m_actualPos.posX, m_actualPos.posY)->widget())->setPixmap(*m_snakePicture);
}