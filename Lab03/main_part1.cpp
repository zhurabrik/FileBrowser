#include "group_by_folder.h"
#include "group_by_type.h"

// работоспособность проверена на пяти предложенных примерах + три дополнительных примера
// test/1/ - пустая папка
// test/2/ - не пустая, без подпапок
// test/3/ - не пустая, с подпапкой 50/50
// test/4/ - пустая, с подпапкой
// test/5/ - не пустая, уровень вложенности > 1

// test/6/ - не пустая, с пустой подпапкой
// test/7/ - не пустая, с подпапкой, имеет файл нулевого размера и очень маленький файл в подпапке
// test/X/ - не существующая директория

// вспомогательная функция вывода в поток списка элементов
// QTextStream &out - текстовый поток, в который выводятся элементы
// const QList<ItemInfo> &item_list - массив элементов, которые необходимо вывести
void Print(QTextStream &out, const QList<ItemInfo> &item_list)
{
	for (auto &item : item_list)
	{
		out << "    "; // отступ, для повышения удобства чтения

		// форматирование вывода по полям
		out.setFieldAlignment(QTextStream::AlignLeft); // имя, по аналогии с File Explorer, прежимается к левому краю
		out.setFieldWidth(50); // указание статической ширины поля (динамическое изменение размера столбца будет реализовано в следующей части лабораторной работы)
		out << item.name(); // вывод имени элемента. это может быть название папки, или расширение файла

		out.setFieldAlignment(QTextStream::AlignRight);
		out.setFieldWidth(10);
		out << item.size();

		out.setFieldWidth(10);
		out << item.percentage();

		out.setFieldWidth(0); // сброс настроек ширины поля после использования
		out << Qt::endl; // вывод конца строки
	}
}

// вспомогательная функция вывода в поток конкретного примера, сгруппированного по некоторому правилу
// QTextStream &out - текстовый поток, в который выводятся элементы
// const std::unique_ptr<GroupByStrategy> &strategy - умный указатель на стратегию, передаваемый по ссылке
// const QString &path - директория, которую требуется просканировать
void PrintExample(QTextStream &out, const std::unique_ptr<GroupByStrategy> &strategy, const QString &path)
{
	out << "  " << path << Qt::endl; // выводится путь

	try
	{
		Print(out, strategy->Explore(path)); // затем выводятся элементы, сгруппированные по некоторому правилу
	}
	catch (const std::runtime_error &error) // обработка исключений. разбито на две части, для того чтобы возникновение исключения при одном типе группировки, у других типов был шанс быть выполненными
	{
		out << error.what() << Qt::endl;
	}
}

// вспомогательная функция вывода в поток примеров сгруппированных по некоторому правилу
// QTextStream &out - текстовый поток, в который выводятся элементы
// const std::unique_ptr<GroupByStrategy> &strategy - умный указатель на стратегию, передаваемый по ссылке
void PrintExamples(QTextStream &out, const std::unique_ptr<GroupByStrategy> &strategy)
{
	PrintExample(out, strategy, "../test/X/");
	PrintExample(out, strategy, "../test/1/");
	PrintExample(out, strategy, "../test/2/");
	PrintExample(out, strategy, "../test/3/");
	PrintExample(out, strategy, "../test/4/");
	PrintExample(out, strategy, "../test/5/");
	PrintExample(out, strategy, "../test/6/");
	PrintExample(out, strategy, "../test/7/");
}

int main(int argc, char *argv[])
{
	QTextStream cout(stdout); // поток вывода информации, по аналогии с std::cout, но с поддержкой вывода QString

	// группировка по папкам
	cout << "GroupByFolder" << Qt::endl; // вывод типа группироваки

	try
	{
		PrintExamples(cout, std::make_unique<GroupByFolder>()); // создание стратегии и вызов функции вывода примера
	}
	catch (const std::runtime_error &error) // обработка исключений. разбито на две части, для того чтобы возникновение исключения при одном типе группировки, у других типов был шанс быть выполненными
	{
		cout << error.what() << Qt::endl;
	}

	// группировака по типам
	cout << "GroupByFolder" << Qt::endl;

	try
	{
		PrintExamples(cout, std::make_unique<GroupByType>());
	}
	catch (const std::runtime_error &error)
	{
		cout << error.what() << Qt::endl;
	}

	return 0;
}
