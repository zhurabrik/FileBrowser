#ifndef DIRECTORY_ITEM_H
#define DIRECTORY_ITEM_H

#include <QString>
#include <QLocale>

// хранилище информации об элементе директории. он может быть как папкой, так и типом файла
class DirectoryItem
{
public:
	// конструктор
	DirectoryItem(const QString &name, const qint64 size, const qint64 total_size = 0);

	// информация, хранимая в явном виде
	const QString &name() const { return name_; }
	qint64 size() const { return size_; }
	qint64 total_size() const { return total_size_; }

	// изменение данных
	void set_name(const QString &name) { name_ = name; }
	void set_size(const qint64 size) { size_ = size; }
	void set_total_size(const qint64 total_size) { total_size_ = total_size; }

	// вычисляемые значения
	QString Size() const;
	qreal Ratio() const;
	QString Percentage() const;

	// оператор сравнения
	bool operator>(const DirectoryItem &obj) const
	{
		return size_ > obj.size_;
	}

private:
	QString name_; // имя элемента
	qint64 size_; // размеры хранятся в числовом виде, для удобства предварительных вычислений; по запросу преобразуются в отформатированные строки
	qint64 total_size_;
};

#endif // DIRECTORY_ITEM_H
