#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Scene;
class Panel;
class QMenu;
class QAction;
class QGraphicsScene;
class QGraphicsView;
class QDockWdiget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

private slots:
    void _clear();
    void _hexagonal();
    void _cdt();
    void _fermat_point();
    void _zoom_in();
    void _zoom_out();
    void _zoom_fit();

private:
    void _create_dock_widget();
    void _create_actions();
    void _create_menus();
    void _connect_panel();

    QMenu*          _file_menu;
    QMenu*          _view_menu;
    QAction*        _clear_act;
    QAction*        _hexagonal_act;
    QAction*        _cdt_act;
    QAction*        _fermat_point_act;
    QAction*        _decompose_act;
    QAction*        _zoom_in_act;
    QAction*        _zoom_out_act;
    QAction*        _zoom_fit_act;
    Scene*          _scene;
    QGraphicsView*  _view;
    Panel*          _panel;
    QDockWidget*    _dock;
};

#endif // MAINWINDOW_H
