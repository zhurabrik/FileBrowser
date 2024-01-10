#ifndef CHARTS_H
#define CHARTS_H

#include <QPieSeries>
#include <QBarSet>
#include <QBarSeries>
#include <QGraphicsLayout>
#include <QChart>

#include "directory_item.h"

// класс, создающий диаграмму, шаблонный метод
class Chart
{
public:
	virtual ~Chart() = default;

	// функция, создающая диаграмму
	QChart *CreateChart(const std::unique_ptr<QList<DirectoryItem>> &items) const;

	virtual void DrawChart(QChart *chart, const std::unique_ptr<QList<DirectoryItem>> &items) const = 0; // уникальный шаг алгоритма
	void DrawLegend(QLegend *legend) const; // настройки легенды
};

// релаизация шага создания круговой диаграммы
class PieChart : public Chart
{
public:
	void DrawChart(QChart *chart, const std::unique_ptr<QList<DirectoryItem>> &items) const;
};

// релаизация шага создания вертикальной диаграммы
class BarChart : public Chart
{
public:
	void DrawChart(QChart *chart, const std::unique_ptr<QList<DirectoryItem>> &items) const;
};

#endif // CHARTS_H
