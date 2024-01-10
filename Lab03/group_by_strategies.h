#ifndef GROUP_BY_STRATEGIES_H
#define GROUP_BY_STRATEGIES_H

#include <QList>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>

#include "directory_item.h"
#include "directory_observer.h"

// общий класс стратегии группировки файлов
class GroupByStrategy
{
public:
	virtual ~GroupByStrategy() = default; // деструктор по умолчанию объявлен виртуальным, для корректного вызова деструкторов конкретных реализаций

	virtual void Explore(const QString &path) const = 0; // метод, производящий обзор файлов и возвращающий список элментов

	// функция привязки нового наблюдателя
	void Attach(DirectoryObserver *observer);

	// событие окончания обработки директории
	void OnFinish(std::unique_ptr<QList<DirectoryItem>> &items) const;

private:
	DirectoryObserver *observer_; // наблюдатель события
};

// реализация группировки файлов по папкам
class GroupByFolder : public GroupByStrategy
{
public:
	void Explore(const QString &path) const;
};

// реализация группировки файлов по типу
class GroupByType : public GroupByStrategy
{
public:
	void Explore(const QString &path) const;
};

#endif // GROUP_BY_STRATEGIES_H
