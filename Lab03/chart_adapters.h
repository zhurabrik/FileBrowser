#ifndef CHART_ADAPTERS_H
#define CHART_ADAPTERS_H

#include <QLayout>
#include <QChartView>

#include "directory_observer.h"
#include "charts.h"

// адаптер для отображения директории в виде диаграммы
class ChartAdapter : public DirectoryObserver
{
public:
	ChartAdapter(std::unique_ptr<Chart> &&chart, QLayout *layout);
	~ChartAdapter();

	void Update(std::unique_ptr<QList<DirectoryItem>> &items) const;

private:
	std::unique_ptr<Chart> chart_; // класс, создающий диаграмму

	QChartView *view_; // вид
};

// реализация адаптера круговой диаграммы
class PieChartAdapter : public ChartAdapter
{
public:
	explicit PieChartAdapter(QLayout *layout);
};

// реализация адаптера вертикальной диаграммы
class BarChartAdapter : public ChartAdapter
{
public:
	explicit BarChartAdapter(QLayout *layout);
};

#endif // CHART_ADAPTERS_H
