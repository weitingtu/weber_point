#include "mainwindow.h"
#include "inputmanager.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QAction>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    _file_menu(nullptr),
    _initialize_act(nullptr),
    _hexagonal_act(nullptr),
    _accumulation_act(nullptr),
    _decompose_act(nullptr),
    _scene(new QGraphicsScene(this)),
    _view(new QGraphicsView(_scene, this))
{
    _create_actions();
    _create_menus();
    setCentralWidget(_view);
}

MainWindow::~MainWindow()
{
}

QSize MainWindow::minimumSizeHint() const
{
    return QSize(400, 200);
}

QSize MainWindow::sizeHint() const
{
    return QSize(800, 600);
}

void MainWindow::_create_actions()
{
    _initialize_act   = new QAction(tr("&Initialize"), this);
    _initialize_act->setCheckable(true);
    connect(_initialize_act, SIGNAL(toggled(bool)), this, SLOT(_initialize()));
    _hexagonal_act    = new QAction(tr("&Hexagonal"), this);
    _accumulation_act = new QAction(tr("&Accumulation"), this);
    _decompose_act    = new QAction(tr("&Decompose"), this);
}

void MainWindow::_create_menus()
{
    _file_menu = menuBar()->addMenu(tr("File"));
    _file_menu->addAction(_initialize_act);
    _file_menu->addAction(_hexagonal_act);
    _file_menu->addAction(_accumulation_act);
    _file_menu->addAction(_decompose_act);
}

void MainWindow::_initialize()
{
    // boundary
    int width  = get_input_manager().get_width();
    int height = get_input_manager().get_height();
    int w = width * 0.1;
    int h = height * 0.1;
    _scene->setSceneRect(0 - w, 0 - h, width + 2 * w, height + 2 * h);

    _scene->addRect(0, 0, width, height);

    const QVector<QPolygonF>& sources = get_input_manager().get_sources();
    for(int i = 0; i < sources.size(); ++i)
    {
        _scene->addPolygon(sources[i], QPen(), QBrush(Qt::gray));
    }
    const QVector<QPolygonF>& obstacles = get_input_manager().get_obstacles();
    for(int i = 0; i < obstacles.size(); ++i)
    {
        _scene->addPolygon(obstacles[i], QPen(), QBrush(Qt::black));
    }
    _view->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
    _view->centerOn(_scene->sceneRect().center());
}
