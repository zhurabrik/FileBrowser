#include "directory_model.h"

DirectoryModel::DirectoryModel(QObject *parent)
	: QAbstractTableModel(parent)
	, data_(std::make_unique<QList<DirectoryItem>>()) // создается пустой список
{

}

// int - возвращается количество строк таблицы
int DirectoryModel::rowCount(const QModelIndex &parent) const
{
	return data_->size(); // количество элементов QList
}

// int - возвращается количество столбцов таблицы
int DirectoryModel::columnCount(const QModelIndex &parent) const
{
	return Percentage + 1; // id последнего столбца, преобразованный в порядковый номер
}

// QVariant - возвращается ячейка таблицы с запрошенной информацией в виде аналога безтиповой переменной
QVariant DirectoryModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || data_->size() <= index.row()) // если индекс не валиден, возвращаем пустые данные
	{
		return QVariant();
	}

	switch (role) // реакция на различные запросы
	{
	case Qt::TextAlignmentRole: // если вид пытается выяснить к какому краю прижимать значения столбца
		if (index.column() == Name) // прижимаем значения, находящиеся в левом столбце к левому краю
		{
			 return Qt::AlignLeft;
		}
		else
		{
			 return Qt::AlignRight; // в остальных столбцах, к правому
		}
	case Qt::DisplayRole: // отображение данныех
	case Qt::EditRole:
		{
			const DirectoryItem &item = data_->at(index.row()); // выбираем из данных запрошенную строку

			switch (index.column()) // выбираем из строки запрошенный столбец
			{
			case Name: // если столбец это столбец имени
				return item.name(); // запрашиваем у элемента информацию об имени
			case Size:
				return item.Size(); // запрашиваем у элемента информацию об размере
			case Percentage:
				return item.Percentage(); // запрашиваем у элемента информацию об процентном соотношении
			}
		}
	}

	return QVariant();
}

// QVariant - возвращается запрошенная информация о элементе заголовка
QVariant DirectoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal || role != Qt::DisplayRole) // если запрошена информация о не поддерживаемом заголовке, или не поддерживаемый запрос, возвращаем пустые данные
	{
		return QVariant();
	}

	switch (section) // получение информации о столбце
	{
	case Name: // если запрошен столбец с именем
		return "Name"; // возвращаем его подпись
	case Size:
		return "Size";
	case Percentage:
		return "Percentage";
	}

	return QVariant();
}

// функция изменения данных модели
// std::unique_ptr<QList<DirectoryItem>> &data - ссылка на данные, не константная, так как происходит передача права владения
void DirectoryModel::set_data(std::unique_ptr<QList<DirectoryItem>> &data)
{
	if (!data) // если пришел невалидный указатель, возвращаем исключение
	{
		throw std::runtime_error("DirectoryModel::set_data: The data is nullptr.");
	}

	beginResetModel(); // уведомляем виды о намерении изменить данные модели

	data_ = std::move(data); // передаём право владения указателем нашему классу

	endResetModel(); // уведомляем виды о завершении изменения данныех модели
}
