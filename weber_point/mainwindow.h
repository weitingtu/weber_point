#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QAction;
class QGraphicsScene;
class QGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

private slots:
    void _initialize();
    void _hexagonal();

private:
    void _create_actions();
    void _create_menus();

    QMenu*          _file_menu;
    QAction*        _initialize_act;
    QAction*        _hexagonal_act;
    QAction*        _accumulation_act;
    QAction*        _decompose_act;
    QGraphicsScene* _scene;
    QGraphicsView*  _view;
};

#endif // MAINWINDOW_H
