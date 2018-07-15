#include <iostream>

#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStandardItemModel>
#include <QtXml/QDomElement>
#include <QMessageBox>

#include "window.h"
#include "ui_window.h"

/**
 * @brief parse any xml file. recursive funtion.
 * parsed xml file contains in QStandardItemModel.
 *
 * @param node: root QDomNode, its childs are handled.
 * @param model: model pointer.
 * @param parent: pointer on the parent item if it exist for cur node.
 */
void parse_xml(const QDomNode &node, QStandardItemModel *model,
               QStandardItem* parent);


/**
 * @brief creates QDomDocument from model. recursive funtion.
 *
 * @param document: pointer to the QDomDocument
 * @param parent_dom: parent element if exists
 * @param model: model from which we construct
 * @param parent_item: parent item, can be null
 */
void dump_xml(QDomDocument *document, QDomNode *parent_dom,
              const QStandardItemModel * const model,
              const QStandardItem * const parent_item);


class SubwindowsEventFilter : public QObject {
    Window *w;

public:
    SubwindowsEventFilter(Window *w) {
        this->w = w;
    }

private:
    bool eventFilter(QObject * obj, QEvent * e)
    {
        switch (e->type())
        {
            case QEvent::Close:
            {
                QMdiSubWindow * subWindow = dynamic_cast<QMdiSubWindow*>(obj);
                w->close_subwindow(subWindow);
                break;
            }
            default:
                qt_noop();
        }
        return QObject::eventFilter(obj, e);
    }
};


Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    subwins_event_filter = new SubwindowsEventFilter(this);

    connect(ui->actionOpen, SIGNAL (triggered()), this,
            SLOT (open_file_action()));
    connect(ui->actionSave, SIGNAL (triggered()), this,
            SLOT (save_file_action()));
    connect(ui->actionExit, SIGNAL (triggered()), this,
            SLOT (close()));
    connect(ui->actionCascade, SIGNAL (triggered()), this,
            SLOT (make_cascade()));
    connect(ui->actionTile, SIGNAL (triggered()), this,
            SLOT (make_tile()));
    connect(ui->actionAbout, SIGNAL (triggered()), this,
            SLOT (about()));
    connect(ui->actionClose_all_windows, SIGNAL (triggered()), this,
            SLOT (close_all_windows()));
    connect(ui->actionEnglish, SIGNAL (triggered()), this,
            SLOT (set_eng()));
    connect(ui->actionRussian, SIGNAL (triggered()), this,
            SLOT (set_rus()));

    this->setWindowTitle(tr("XML-editor"));
    this->move(100, 100);
}

Window::~Window()
{
    delete ui;
    delete subwins_event_filter;
}

void Window::open_file_action()
{
    QString file_path = QFileDialog::getOpenFileName(
        this,
        tr("Open Document"),
        QDir::homePath(),
        tr("XML-files (*.xml);;All files (*.*)"));
    QStandardItemModel *model = ReadFile(file_path);
    if (model == nullptr) {
        return;
    }

    QMdiSubWindow *sub = new QMdiSubWindow();
    sub->setAttribute(Qt::WA_DeleteOnClose, true);
    sub->installEventFilter(subwins_event_filter);
    QTreeView *tree_view = new QTreeView();

    QFileInfo fileInfo(file_path);
    model->setHeaderData(0, Qt::Horizontal, fileInfo.fileName());

    tree_view->setModel(model);
    sub->setWidget(tree_view);

    this->subwindows.push_back(sub);
    this->models.push_back(model);
    this->ui->mdiArea->addSubWindow(sub);

    QAction *new_subwindow_act = ui->menuWindow->addAction(
                fileInfo.fileName());
    connect(new_subwindow_act, SIGNAL (triggered()), this,
            SLOT (activate_window()));
    subwins_acts.push_back(new_subwindow_act);
    this->subwins_fnames.push_back(fileInfo.fileName());

    sub->show();
}

void Window::save_file_action() {
    if (subwindows.size() < 1) {
        return;
    }
    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Save Document"),
            QDir::currentPath(),
            tr("XML-files (*.xml);;All files (*.*)"));
    SaveFile(filename);
}

void Window::make_cascade()
{
    ui->mdiArea->cascadeSubWindows();
}

void Window::make_tile()
{
    ui->mdiArea->tileSubWindows();
}

void Window::close_all_windows()
{
    auto subwindows = ui->mdiArea->subWindowList();
    for(int i = 0; i < subwindows.count(); i++) {
        subwindows[i]->close();
    }
}

void Window::set_rus() {
    std::vector<QString> dirs = {".", "../translations", ":/translations"};
    for (int i = 0; i < 3; i++) {
        bool loaded = translator.load(dirs[i] + QDir::separator() +
                                      "lang_ru_RU.qm");
        if (loaded) {
            auto app = QCoreApplication::instance();
            app->installTranslator(&translator);
            ui->retranslateUi(this);
            return;
        }
    }
}

void Window::set_eng() {
    std::vector<QString> dirs = {".", "../translations", ":/translations"};
    for (int i = 0; i < 3; i++) {
        bool loaded = translator.load(dirs[i] + QDir::separator() +
                                      "lang_en_US.qm");
        if (loaded) {
            auto app = QCoreApplication::instance();
            app->installTranslator(&translator);
            ui->retranslateUi(this);
            return;
        }
    }
}

void Window::close_subwindow(QMdiSubWindow *subwin) {
    int need_i = -1;
    for (int i = 0; i < subwindows.size(); i++) {
        if (subwindows[i] == subwin) {
            need_i = i;
            break;
        }
    }
    if (need_i < 0) {
        return;
    }
    subwin->close();
    ui->menuWindow->removeAction(subwins_acts[need_i]);
    subwindows.erase(subwindows.begin() + need_i);
    subwins_fnames.erase(subwins_fnames.begin() + need_i);
    subwins_acts.erase(subwins_acts.begin() + need_i);
    models.erase(models.begin() + need_i);
}

void Window::activate_window()
{
    auto action = dynamic_cast<QAction*>(sender());
    QString subwindow_name = action->text();
    int need_index = -1;
    for (int i = 0; i < subwins_fnames.size(); i++) {
        if (subwins_fnames[i] == subwindow_name) {
            need_index = i;
            break;
        }
    }
    if (need_index >= 0) {
        ui->mdiArea->setActiveSubWindow(subwindows[need_index]);
    }
}

void Window::about()
{
    QMessageBox::about(this, tr("About"), tr("XML-editor."));
}

QStandardItemModel* Window::ReadFile(const QString &file_path)
{
    QStandardItemModel *model = new QStandardItemModel(0, 1, this);
    QDomDocument document;

    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return nullptr;
    }

    document.setContent(&file);
    file.close();
    parse_xml(document, model, nullptr);
    return model;
}

void Window::SaveFile(const QString &file_path)
{
    QFile file(file_path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QDomDocument document;
    int index = -1;
    auto active_subwindow = ui->mdiArea->activeSubWindow();
    for (int i = 0; i < subwindows.size(); i++) {
        QMdiSubWindow *cur_sub_w = subwindows[i];
        if (cur_sub_w == active_subwindow) {
            index = i;
            break;
        }
    }
    dump_xml(&document, nullptr, models[index], nullptr);
    QTextStream stream(&file);
    stream << document.toString();
    file.close();
}


void parse_xml(const QDomNode &node, QStandardItemModel *model,
               QStandardItem* parent) {
    auto nodes = node.childNodes();
    for (int i = 0; i < nodes.length(); i++) {
        auto item = nodes.at(i);
        if (item.isElement()) {
            auto cur_elem = nodes.at(i).toElement();
            auto tag_name = cur_elem.tagName();
            if (tag_name == "") {
                continue;
            }
            QStandardItem *row = new QStandardItem(tag_name);

            auto attributes = cur_elem.attributes();
            if (attributes.length() > 0) {
                QStandardItem *attrs_row = new QStandardItem("<attributes>");
                row->appendRow(attrs_row);
                for (int j = 0; j < attributes.length(); j++) {
                    auto attr = attributes.item(j).toAttr();
                    QStandardItem *attr_row = new QStandardItem(attr.name());
                    QStandardItem *attr_val = new QStandardItem(attr.value());
                    attr_row->appendRow(attr_val);
                    attrs_row->appendRow(attr_row);
                }
            }

            if (parent == nullptr) {
                model->appendRow(row);
            } else {
                parent->appendRow(row);
            }
            parse_xml(cur_elem, model, row);
        } else if (item.isText()) {
            auto text = nodes.at(i).toText().data();
            QStandardItem *row = new QStandardItem(text);

            if (parent == nullptr) {
                model->appendRow(row);
            } else {
                parent->appendRow(row);
            }
        }
    }
}

void dump_xml(QDomDocument *document, QDomNode *parent_dom,
              const QStandardItemModel * const model,
              const QStandardItem * const parent_item) {
    int count = 0;
    if (parent_item == nullptr) {
        count = model->rowCount();
    } else {
        count = parent_item->rowCount();
    }
    for (int i = 0; i < count; i++) {
        QStandardItem *item;
        QDomNode node;
        if (parent_item == nullptr) {
            item = model->item(i, 0);
        } else {
            item = parent_item->child(i);
        }
        auto item_name = item->text();
        if (item->hasChildren()) {
            node = document->createElement(item_name);
        } else {
            node = document->createTextNode(item_name);
        }

        if (item->text() == "<attributes>") {
            for (int j = 0; j < item->rowCount(); j++) {
                auto attr_name = item->child(j)->text();
                auto attr_value = item->child(j)->child(0)->text();
                if (parent_item != nullptr) {
                    QDomElement el = parent_dom->toElement();
                    el.setAttribute(attr_name, attr_value);
                }
            }
        } else {
            dump_xml(document, &node, model, item);
            if (parent_item == nullptr) {
                document->appendChild(node);
            } else {
                parent_dom->appendChild(node);
            }
        }
    }
}
