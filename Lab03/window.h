#ifndef WINDOW_H
#define WINDOW_H

#include <QFileSystemModel>
#include <QLabel>
#include <QComboBox>
#include <QTreeView>
#include <QStackedWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "group_by_strategies.h"
#include "list_adapter.h"
#include "chart_adapters.h"

class Window : public QWidget
{
	Q_OBJECT

public:
	Window(QWidget *parent = nullptr);
	~Window();

private:
	// указатели на модели
	QFileSystemModel *tree_model_;

	// указатели на элементы интерфейса
	QLabel *label_group_by_;
	QComboBox *combo_box_group_by_;
	QLabel *label_display_in_;
	QComboBox *combo_box_display_in_;
	QLabel *label_directory_;
	QHBoxLayout *horizontal_layout_;
	QTreeView *tree_view_;
	QStackedWidget *directory_wrapper_;
	DirectoryObserver *list_adapter_; // адаптеры отображений
	DirectoryObserver *pie_chart_adapter_;
	DirectoryObserver *bar_chart_adapter_;
	QSplitter *splitter_;
	QVBoxLayout *vertical_layout_;

	// стратегии
	GroupByStrategy *group_by_folder_;
	GroupByStrategy *group_by_type_;

	GroupByStrategy *strategy_; // текущая стратегия группировки
	DirectoryObserver *directory_view_; // текущий адаптер отображения

	void directory_Update(); // функция обновления модели

	void combo_box_group_by_OnChanged(const int index); // обработчик события выбора элемента выпадающего меню
	void combo_box_display_in_OnChanged(const int index);
	void tree_view_OnChanged(const QModelIndex &current, const QModelIndex &previous); // обработчик события выбора элемента дерева
};
#endif // WINDOW_H
