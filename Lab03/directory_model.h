#ifndef DIRECTORY_MODEL_H
#define DIRECTORY_MODEL_H

#include <stdexcept>

#include <QAbstractTableModel>

#include "directory_item.h"

// модель директории. используется для отображения информации о текущей директории в виде папок или типов файлов
class DirectoryModel : public QAbstractTableModel
{
public:
	enum Column // перечисление имеющихся столбцов
	{
		Name, // имя элемента
		Size, // размер
		Percentage // процентное соотношение по отношению к общему размеру директории
	};

	// конструктор
	explicit DirectoryModel(QObject *parent = nullptr); // по умолчанию инициализируется без данных

	// набор необходимых унаследованных функций
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	// функция установки элементов
	void set_data(std::unique_ptr<QList<DirectoryItem>> &data);

private:
	std::unique_ptr<QList<DirectoryItem>> data_; // уникальный умный указатель на массив элементов, так как данными должна владеть только модель
};

#endif // DIRECTORY_MODEL_H
