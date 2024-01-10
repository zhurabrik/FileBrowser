#include "directory_item.h"

// конструктор класса DirectoryItem
// const QString &name - имя элемента, передаётся по ссылке
// const qint64 size - обьём, занимаемый элементом
// const qint64 total_size - общий обьём папки, может быть неизвестен на момент создания элемента
DirectoryItem::DirectoryItem(const QString &name, const qint64 size, const qint64 total_size)
	: name_(name)
	, size_(size)
	, total_size_(total_size)
{

}

// QString - возвращается размер в виде строки с единицей измерения
QString DirectoryItem::Size() const
{
	return QLocale::system().formattedDataSize(size_); // используется обозначение 1 KiB = 1,024 bytes
}

// qreal - возвращается соотношение текущего элемента к общему объёму папки, в числовом виде
qreal DirectoryItem::Ratio() const
{
	if (size_ == 0) // избежать ситуации деления на ноль в пустой папке
	{
		return 0; // так как если размер всей папки равен нулю, то отдельных элементов тем более
	}

	return (qreal)size_ / total_size_; // нахождение отношения размера элемента к общему размеру
}

// QString - возвращается процентное соотношение, в виде отформатированной строки
QString DirectoryItem::Percentage() const
{
	const qreal r = Ratio() * 100; // нахождение отношения и преобразование в проценты

	if (r == 0) // избегание лишних вычислений при объёме, равном нулю
	{
		return "0.00 %";
	}

	if (r < 0.01) // если размер файла очень мал, отображается соответствующая отметка
	{
		return "< 0.01 %";
	}

	return QString::number(floor(r * 100) / 100, 'f', 2) + " %"; // преобразование числа с плавающей точкой в строку с округлением в меньшую сторону до второго знака после запятой
}
