#include "list_adapter.h"

// конструктор
// QLayout *layout - разметка, на которую следуюет нанести вид, создаваемый адаптером
ListAdapter::ListAdapter(QLayout *layout)
{
	model_ = new DirectoryModel(); // создаем модель директории

	view_= new QTreeView(); // текущая директория также отображается через дерево
	view_->setModel(model_); // устанавливаем виду директории модель
	view_->setRootIsDecorated(false); // но без возможности разворачивать его элементы
	view_->header()->setStretchLastSection(false); // отключаем заполнение последним столбцом
	view_->header()->setMinimumSectionSize(75); // устанавливаем столбцам минимальный размер
	view_->header()->setSectionResizeMode(QHeaderView::ResizeToContents); // столбцы изменяются по содержимому
	view_->header()->setSectionResizeMode(DirectoryModel::Name, QHeaderView::Stretch); // включаем заполнение столбцом с именем

	layout->addWidget(view_); // добавляем созданный вид на разметку
}

// деструктор
ListAdapter::~ListAdapter()
{
	delete model_; // удаление модели
	delete view_; // удаление вида
}

// обработчик события обновления информации
// std::unique_ptr<QList<DirectoryItem>> &items - ссылка на умный указатель новых данных
void ListAdapter::Update(std::unique_ptr<QList<DirectoryItem>> &items) const
{
	model_->set_data(items); // передача новых данных в модель, где происходит передача права владения
}
