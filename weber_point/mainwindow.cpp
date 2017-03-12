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
    _hexagonal_act->setCheckable(true);
    connect(_hexagonal_act, SIGNAL(toggled(bool)), this, SLOT(_hexagonal()));
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
    double width  = get_input_manager().get_width();
    double height = get_input_manager().get_height();
    double w = width * 0.1;
    double h = height * 0.1;
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

void MainWindow::_hexagonal()
{
    double a = 40;
    double h = a * std::sqrt(3);
    double width  = get_input_manager().get_width();
    double height = get_input_manager().get_height();
    int x_ratio  = (width  + a + 1 ) / a;
    int y_ratio  = (height + h + 1 ) / h;

    get_input_manager().set_width(x_ratio * a);
    get_input_manager().set_height(y_ratio * h);

    _scene->clear();
    _initialize();

    for(int i = 0; i < x_ratio + 1; ++i)
    {
        for(int j = 0; j < 2 * y_ratio + 1; ++j)
        {
            double shift = 0;
            if(j % 2 == 1)
            {
                if(i == x_ratio)
                {
                    continue;
                }
                shift = a / 2;
            }
            double rad = 1;
            double x = i * a + shift;
            double y = j * h / 2;
            if(get_input_manager().is_blocked(QPointF(x, y)))
            {
                continue;
            }
            _scene->addEllipse(x - rad, y - rad, rad * 2, rad * 2);
        }
    }
}
