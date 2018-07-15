#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <QMdiSubWindow>
#include <QTreeView>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QTranslator>

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();
    void close_subwindow(QMdiSubWindow *subwin);

private slots:
    void open_file_action();
    void save_file_action();
    void make_cascade();
    void make_tile();
    void about();
    void close_all_windows();
    void activate_window();
    void set_rus();
    void set_eng();

private:
    Ui::Window *ui;
    QTranslator translator;
    std::vector<QMdiSubWindow*> subwindows;
    std::vector<QStandardItemModel*> models;
    std::vector<QAction*> subwins_acts;
    std::vector<QString> subwins_fnames;
    QObject *subwins_event_filter;

    QStandardItemModel* ReadFile(const QString &file_path);
    void SaveFile(const QString &file_path);
};

#endif // WINDOW_H
