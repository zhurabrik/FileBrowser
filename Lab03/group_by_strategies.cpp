#include "group_by_strategies.h"

// функция привязки нового наблюдателя
// DirectoryObserver *observer - новый наблюдатель
void GroupByStrategy::Attach(DirectoryObserver *observer)
{
	if (observer == nullptr) // если передан невалидный наблюдатель, выбрасываем исключение
	{
		throw std::runtime_error("The observer is nullptr.");
	}

	observer_ = observer; // устанавливаем нового наблюдателя
}

// событие окончания обработки директории
// std::unique_ptr<QList<DirectoryItem>> &items - ссылка на умный указатель новых данных
void GroupByStrategy::OnFinish(std::unique_ptr<QList<DirectoryItem>> &items) const
{
	observer_->Update(items); // вызывается обработчик события наблюдателя, с передачей ему данных
	// наблюдатель может быть только один, так как по заданию требовалось передавать значения в модель без копирования
	// но модели нужна гарантия что её внутренние данные не будут изменяться из вне, по этому в событие передаётся не константная ссылка
}

// метод, производящий обзор файлов
// const QString &path - директория, которую требуется сгруппировать по папкам
void GroupByFolder::Explore(const QString &path) const
{
	QFileInfo directory_info(path); // получаем информацию о пределенном пути

	if (!directory_info.isDir()) // если в метод передан не путь до папки, выбрасывается исключение
	{
		throw std::runtime_error("The directory is invalid.");
	}

	std::unique_ptr<QList<DirectoryItem>> item_list = std::make_unique<QList<DirectoryItem>>(); // результирующий массив элементов

	qint64 total_size = 0; // накопительный общий размер всех файлов в директории

	QDir directory(directory_info.absoluteFilePath()); // создаем экземпляр класса QDir

	// подсчет размера текущей папки
	for (const QFileInfo &file_info : directory.entryInfoList(QDir::Files)) // проход по всем файлам текущей папки
	{
		total_size += file_info.size(); // учет в общий размер
	}

	item_list->append(DirectoryItem("(Current Directory)", total_size)); // так как до данной операции общий размер был равен 0, его можно использовать в качестве размера элемента

	// подсчет размера подпапок
	for (const QFileInfo &file_info : directory.entryInfoList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name)) // перебор всех папок в даннй директории
	{
		qint64 size = 0; // размер всех файлов в папке

		QDirIterator it(file_info.absoluteFilePath(), QDir::Files, QDirIterator::Subdirectories); // итератор директории, для вычисления размера файлов во всех подпапках

		while (it.hasNext()) // цикл основанный на итераторе директории
		{
			it.next(); // следующий файл

			size += it.fileInfo().size(); // размер файла добавляется к общему размеру данной папки
		}

		total_size += size; // размер папки добавляется к общему размеру

		item_list->append(DirectoryItem(file_info.fileName(), size)); // добавляем папку в массив
	}

	// вычисление процентов
	for (auto &item : *item_list) // перебор всех элементов массива
	{
		item.set_total_size(total_size); //установка общего размера, для последующего вычисления процентного соотношения
	}

	OnFinish(item_list); // генерируем событие с новым массивом элементов
}

// метод, производящий обзор файлов
// const QString &path - директория, которую требуется сгруппировать по типам файлов
void GroupByType::Explore(const QString &path) const
{
	QFileInfo directory_info(path);

	if (!directory_info.isDir())
	{
		throw std::runtime_error("The directory is invalid.");
	}

	qint64 total_size = 0;

	QHash<QString, qint64> hash; // для группировки по типу используется словарь на основе хеша; ключ - тип файла, значение - размер всех файлов данного типа

	QDirIterator it(directory_info.absoluteFilePath(), QDir::Files, QDirIterator::Subdirectories); // перебор всех файлов всех подпапок

	while (it.hasNext())
	{
		it.next();

		const QFileInfo &file_info = it.fileInfo(); // текущий файл

		qint64 size = file_info.size(); // размер текущего файла

		hash[file_info.suffix()] += size; // находим в словаре данный тип файла и увеличиваем размер файлов данного типа

		total_size += size; // увеличиваем общий размер
	}

	// преобразование хеш таблицы в массив наборов информации
	std::unique_ptr<QList<DirectoryItem>> item_list = std::make_unique<QList<DirectoryItem>>(); // массив элементов

	QHashIterator hash_it(hash); // итератор по словарю

	while (hash_it.hasNext()) // цикл на основе итератора словаря
	{
		hash_it.next();

		item_list->append(DirectoryItem("*." + hash_it.key(), hash_it.value(), total_size)); // на этот раз на момент добавления элемента в массив общий размер папки известен
	}

	OnFinish(item_list);
}
