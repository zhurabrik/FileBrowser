#ifndef LIST_ADAPTER_H
#define LIST_ADAPTER_H

#include <QLayout>
#include <QHeaderView>
#include <QTreeView>

#include "directory_observer.h"
#include "directory_model.h"

// адаптер для отображения директории в виде таблицы
class ListAdapter : public DirectoryObserver
{
public:
	explicit ListAdapter(QLayout *layout);
	~ListAdapter();

	void Update(std::unique_ptr<QList<DirectoryItem>> &items) const;

private:
	DirectoryModel *model_; // модель данных, используемая в виде

	QTreeView *view_; // вид
};

#endif // LIST_ADAPTER_H
