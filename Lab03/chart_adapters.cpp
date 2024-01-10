#include "chart_adapters.h"

// конструктор
// std::unique_ptr<Chart> &&chart - r-value ссылка на указатель на экземпляр класса, создающего диаграмму
// QLayout *layout - разметка, на которую следуюет нанести вид, создаваемый адаптером
ChartAdapter::ChartAdapter(std::unique_ptr<Chart> &&chart, QLayout *layout)
	: chart_(std::move(chart)) // забираем право владения классом создания диаграмм
{
	view_ = new QChartView(chart_->CreateChart(std::make_unique<QList<DirectoryItem>>())); // инициализация пустым списком
	view_->setFrameShape(QFrame::StyledPanel); // возвращаем виду рамку
	view_->resize(256, 0); // задаем стандартную ширину, для корректного вычисления положения разделителя
	view_->setSceneRect(0, 0, 256, 0); // так как положение разделителя основано на размере сцены, меняем и его

	layout->addWidget(view_); // добавляем созданный вид на разметку
}

ChartAdapter::~ChartAdapter()
{
	delete view_; // удаляем вид
}

// обработчик события обновления информации
// std::unique_ptr<QList<DirectoryItem>> &items - ссылка на умный указатель на новые данные
void ChartAdapter::Update(std::unique_ptr<QList<DirectoryItem>> &items) const
{
	QChart *previous = view_->chart(); // запоминаем предыдущий график

	view_->setChart(chart_->CreateChart(items)); // создаем новый график с указанными данными, и устанавливаем его текущим

	delete previous; // удаляем предыдущий график
}

// конструктор адаптера круговой диаграммы
PieChartAdapter::PieChartAdapter(QLayout *layout)
	: ChartAdapter(std::make_unique<PieChart>(), layout) // создаём экземпляр класса создания круговой диаграммы
{

}

// конструктор адаптера вертикальной диаграммы
BarChartAdapter::BarChartAdapter(QLayout *layout)
	: ChartAdapter(std::make_unique<BarChart>(), layout) // создаём экземпляр класса создания вертикальной диаграммы
{

}
