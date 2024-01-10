#include "charts.h"

// функция, создающая диаграмму
// const std::unique_ptr<QList<DirectoryItem>> &items - элементы которые требуется отрисовать, const, так как они не сохраняются в исходном виде
// QChart * - возврат созданной диаграммы
QChart *Chart::CreateChart(const std::unique_ptr<QList<DirectoryItem>> &items) const
{
	QChart *chart = new QChart(); // создаем диаграмму
	chart->setBackgroundRoundness(0); // убираем скругление краев фона
	chart->layout()->setContentsMargins(0, 0, 0, 0); // убераем отступы сцены
	chart->setMargins({ 20, 20, 0, 20 }); // убираем отступы по бокам диаграммы

	// предварительная подготовка данных к добавлению на диаграмму
	// сортировка обьектов по занимаемому объему
	std::sort(items->begin(), items->end(), std::greater<DirectoryItem>()); // по убыванию

	// проверка общего размера директории
	auto item = items->cbegin(); // получаем итератор на первый элемент массива

	qint64 total_size = item ? item->total_size() : 0; // исключение ситуации пустого массива

	if (total_size == 0) // если общий размер папки равен нулю, или папка пуста
	{
		chart->setTitle("The folder is empty."); // выводим надпись о том что папка пуста

		return chart;
	}

	// группировка маленьких элементов в один
	if (items->size() > 8) // маленькими считаются все элементы, идущие после 7-го
	{
		std::advance(item, 7); // смещаем указатель, используемый для проверки размера папки

		qint64 size = 0; // накопитель суммарного размера всех элементов категории Others

		while (item != items->cend()) // перебор массива до конца
		{
			size += item->size(); // добавляем размер элемента в сумму

			item = items->erase(item); // удаление элемента на который указывает итератор item, возвращает указатель на следующий элемент
		}

		items->push_back(DirectoryItem("Others", size, total_size)); // создаем элемент Others, в котором содержится информация полученная из всех удаленных элементов
	}

	DrawLegend(chart->legend()); // настройка легенды
	DrawChart(chart, items); // создаем series с заданными данными

	return chart;
}

// функция, настраивающая легенду
// QLegend *legend - легенда, которую требуется настроить
void Chart::DrawLegend(QLegend *legend) const
{
	legend->setAlignment(Qt::AlignRight); // легенда отрисовывается справа от диаграммы
}

// функция, заполняющая круговую диаграмму данными
// QChart *chart - диаграмма, на которую выводится информация
// const std::unique_ptr<QList<DirectoryItem>> &items - элементы которые требуется отрисовать
void PieChart::DrawChart(QChart *chart, const std::unique_ptr<QList<DirectoryItem>> &items) const
{
	QPieSeries *series = new QPieSeries(); // создаем набор данных для круговой диаграммы
	series->setPieSize(1); // масштаб диаграммы, устанавливаем 100 %

	for (const auto &item : *items) // перебираем все элементы, требующие отрисовки
	{
		// добавляем в набор отформатированные данные, полученные из элемента
		// так как в процентах может содержаться спец символ <, кодируем его
		series->append(item.name() + " (" + item.Percentage().toHtmlEscaped() + ")", item.Ratio());
	}

	chart->addSeries(series); // устанавливаем диаграмме созданный набор данных
}

// функция, заполняющая вертикальную диаграмму данными
// построена аналогично предыдущей
void BarChart::DrawChart(QChart *chart, const std::unique_ptr<QList<DirectoryItem>> &items) const
{
	QBarSeries *series = new QBarSeries(); // создаем набор элементов данных вертикальной диаграммы
	series->setBarWidth(1); // масштаб диаграммы, устанавливаем 100 %

	for (const auto &item : *items)
	{
		QBarSet *set = new QBarSet(item.name() + " (" + item.Percentage().toHtmlEscaped() + ")"); // создаем элемент данных

		set->append(item.Ratio()); // присваиваем элементу занимаемую им долю диаграммы

		series->append(set); // добавляем элемент данных в набор
	}

	chart->addSeries(series); // устанавливаем диаграмме созданный набор элементов
}
