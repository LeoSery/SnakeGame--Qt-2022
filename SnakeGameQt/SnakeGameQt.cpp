#include "SnakeGameQt.h"

SnakeGameQt::SnakeGameQt(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(680, 680);
	m_GridLayout = new QGridLayout(this);
	InitGrid();

	setLayout(m_GridLayout);
}

void SnakeGameQt::InitGrid()
{
	m_GridLayout->setHorizontalSpacing(0);
	m_GridLayout->setVerticalSpacing(0);

	for (size_t i = 1; i < 18; i++)
	{
		m_GridLayout->setColumnMinimumWidth(i, 40);
		for (size_t j = 1; j < 18; j++)
		{
			m_GridLayout->setColumnMinimumWidth(j, 40);
			QWidget* w = new QWidget(this);

			if ((i + j) % 2 == 0)
				w->setStyleSheet("background-color: #A7D948");
			else
				w->setStyleSheet("background-color: #8ECC39");
			
			m_GridLayout->addWidget(w, j, i);
		}
	}
}