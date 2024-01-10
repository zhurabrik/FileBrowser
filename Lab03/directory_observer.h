#ifndef DIRECTORY_OBSERVER_H
#define DIRECTORY_OBSERVER_H

#include "directory_item.h"

// наблюдатель директории
class DirectoryObserver
{
public:
	virtual ~DirectoryObserver() = default; // деструктор по умолчанию объявлен виртуальным, для корректного вызова деструкторов конкретных реализаций

	// обработчик события обновления информации в директории
	virtual void Update(std::unique_ptr<QList<DirectoryItem>> &items) const = 0;
};

#endif // DIRECTORY_OBSERVER_H
