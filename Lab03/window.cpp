#include "window.h"

Window::Window(QWidget *parent)
	: QWidget(parent)
	, group_by_folder_(new GroupByFolder()) // создаем экземпляры стратегий
	, group_by_type_(new GroupByType())
	, strategy_(nullptr)
	, directory_view_(nullptr)
{
	// модели
	tree_model_ = new QFileSystemModel(); // создаём модель файловой системы, которая используется в дереве
	tree_model_->setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::AllDirs); // указываем желаемые фильтры, AllDirs требуется по документации
	tree_model_->setRootPath(QString()); // в качестве корня выбираем самый верхний, для того чтобы видеть все доступные диски

	// настройка раскладки элементов
	setWindowTitle("Lab03"); // заголовк окна
	resize(800, 600); // стандартный размер

	label_group_by_ = new QLabel("Group by:"); // подпись к выпадающему списку группировки
	label_group_by_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred); // изменяем политику ярлыка, касательно занимаемого им размера так, чтобы он занимал только необходимую ему ширину, не больше

	combo_box_group_by_ = new QComboBox(); // выпадающий список группировки
	combo_box_group_by_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	label_display_in_ = new QLabel("Display in:"); // подпись к выпадающему списку отображения
	label_display_in_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

	combo_box_display_in_ = new QComboBox(); // выпадающий список отображения
	combo_box_display_in_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	label_directory_ = new QLabel(); // изначально директория не выбирается
	label_directory_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred); // заполнять всё доступное пространство по горизонтали, но если текст не влазит, не раздвигаем окно

	horizontal_layout_ = new QHBoxLayout(); // горизонтальная раскладка
	horizontal_layout_->addWidget(label_group_by_); // добавляем на раскладку элементы
	horizontal_layout_->addWidget(combo_box_group_by_);
	horizontal_layout_->addWidget(label_display_in_);
	horizontal_layout_->addWidget(combo_box_display_in_);
	horizontal_layout_->addWidget(label_directory_);

	splitter_ = new QSplitter(); // контейнер с радзелитенем
	splitter_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // заполнять всё доступное пространство по вертикали

	tree_view_= new QTreeView(splitter_); // создаем дерево файловой системы на контейнере с разделителем
	tree_view_->setModel(tree_model_); // устанавливаем дереву модель файловой системы
	tree_view_->setHeaderHidden(true); // скрываем заголовок дерева
	tree_view_->hideColumn(1); // также выключаем ненужные столбцы
	tree_view_->hideColumn(2);
	tree_view_->hideColumn(3);

	directory_wrapper_ = new QStackedWidget(splitter_); // многостраничная раскладка

	list_adapter_ = new ListAdapter(directory_wrapper_->layout()); // создаем адаптеры и добавляем их на раскладку
	pie_chart_adapter_ = new PieChartAdapter(directory_wrapper_->layout());
	bar_chart_adapter_ = new BarChartAdapter(directory_wrapper_->layout());

	splitter_->setStretchFactor(0, 1); // устанавливаем начальное положение разделителя
	splitter_->setStretchFactor(1, 2);

	vertical_layout_ = new QVBoxLayout(this); // вертикальная раскладка
	vertical_layout_->addLayout(horizontal_layout_); // добавляем на раскладку горизонтальную раскладку
	vertical_layout_->addWidget(splitter_); // и контейнер с разделителем

	// связка событий с их обработчиками
	connect(combo_box_group_by_, &QComboBox::currentIndexChanged, this, &Window::combo_box_group_by_OnChanged); // событие выбора элемента в выпадающем меню
	connect(combo_box_display_in_, &QComboBox::currentIndexChanged, this, &Window::combo_box_display_in_OnChanged);
	connect(tree_view_->selectionModel(), &QItemSelectionModel::currentChanged, this, &Window::tree_view_OnChanged); // событие выбора элемента в дереве

	// разворачиваем элементы дерева до текущего пути
	QModelIndex index = tree_model_->index("../test/"); // получаем индекс модели, который соответствует указанному пути

	while (index.isValid()) // пока не дошли до верха дерева. если выбранного пути не существует, то цикл просто пропускается и ничего не открывается
	{
		tree_view_->expand(index); // разворачиваем элемент

		index = index.parent(); // переходим к родителю
	}

	// заполненеие выпадающих меню информацией
	combo_box_group_by_->addItem("Folder");
	combo_box_group_by_->addItem("Type");
//	combo_box_group_by_->addItem("Unknown"); // для проверки исключения

	combo_box_display_in_->addItem("List");
	combo_box_display_in_->addItem("Pie Chart");
	combo_box_display_in_->addItem("Bar Chart");
//	combo_box_display_in_->addItem("Unknown"); // для проверки исключения
}

Window::~Window()
{
	// удаление стратегий
	delete group_by_folder_;
	delete group_by_type_;

	// удаление адаптеров
	delete list_adapter_;
	delete pie_chart_adapter_;
	delete bar_chart_adapter_;

	// удаление моделей
	delete tree_model_;

	// удаление графических элементов
	delete label_group_by_;
	delete combo_box_group_by_;
	delete label_display_in_;
	delete combo_box_display_in_;
	delete label_directory_;
	delete horizontal_layout_;
	delete tree_view_;
	delete directory_wrapper_;
	delete splitter_;
	delete vertical_layout_;
}

// функция обновления отображения директории
void Window::directory_Update()
{
	QModelIndex index = tree_view_->currentIndex(); // получаем текущий открытый элемент в дереве

	if (index.isValid()) // если в дереве что-то выбрано
	{
//		index = QModelIndex(); // проверка исключительной ситуации

		try
		{
			const QString path = tree_model_->filePath(index); // получаем у модели путь, соответствующий текущему индексу

			strategy_->Explore(path); // запускаем сканирование директории текущей стратегией группировки

			label_directory_->setText(path); // вписываем в подпись путь директории, открытой на данный момент; если возникло исключение, текст изменён не будет
		}
		catch (const std::runtime_error &e)
		{
			QMessageBox::about(this, "Error", e.what()); // если в процессе группировки элементов директории возникло исключение, покажется уведомление, но текущая информация потеряна не будет
		}
	}
}

// обработчик события выпадающего меню группировки
void Window::combo_box_group_by_OnChanged(const int index)
{
	try
	{
		switch (index) // получаем из выбранного индекса тип группировки
		{
		case 0:
			strategy_ = group_by_folder_; // устанавливаем текущую группировку по папкам
			break;
		case 1:
			strategy_ = group_by_type_; // устанавливаем текущую группировку по типам
			break;
		default:
			throw std::runtime_error("Unknown grouping type selected."); // такой ситуации быть не должно, по этому сообщаем о ней
			break;
		}

		if (directory_view_ != nullptr)
		{
			strategy_->Attach(directory_view_); // перепривязываем наблюдатель к стратегии

			directory_Update(); // после смены типа, обновляем данные директории
		}
	}
	catch (const std::runtime_error &e)
	{
		QMessageBox::about(this, "Error", e.what()); // сообщаем что в процессе выбора вида группировки что-то пошло не так
	}
}

void Window::combo_box_display_in_OnChanged(const int index)
{
	try
	{
		switch (index) // получаем из выбранного индекса тип отображения
		{
		case 0:
			directory_view_ = list_adapter_; // устанавливаем активный адаптер на адаптер списка
			break;
		case 1:
			directory_view_ = pie_chart_adapter_; // устанавливаем активный адаптер на адаптер круговой диаграммы
			break;
		case 2:
			directory_view_ = bar_chart_adapter_; // устанавливаем активный адаптер на адаптер вертикальной диаграммы
			break;
		default:
			throw std::runtime_error("Unknown display type selected."); // такой ситуации быть не должно, по этому сообщаем о ней
			break;
		}

		strategy_->Attach(directory_view_); // привязываем новый наблюдатель к стратегии

		directory_Update(); // обновляем данные директории

		directory_wrapper_->setCurrentIndex(index); // отображаем настроенный адаптер в качестве текущего
	}
	catch (const std::runtime_error &e)
	{
		QMessageBox::about(this, "Error", e.what()); // сообщаем что в процессе выбора вида отображения что-то пошло не так
	}
}

// обработчик события выбора элемента дерева
// const QModelIndex &current - текущий индекс, выбранного элемента в дереве
void Window::tree_view_OnChanged(const QModelIndex &current, const QModelIndex &previous)
{
	if (current.isValid()) // если изменение индекса не привело к его отсутствию
	{
		directory_Update(); // обновляем данные директории, так как изменилась отображаемая папка
	}
}
