#include "mainwindow.h"
#include "inputmanager.h"
#include "cdtmanager.h"
#include "fermatpoint.h"
#include "panel.h"
#include "scene.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QAction>
#include <QMenuBar>
#include <QDockWidget>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    _file_menu(nullptr),
    _view_menu(nullptr),
    _clear_act(nullptr),
    _hexagonal_act(nullptr),
    _cdt_act(nullptr),
    _fermat_point_act(nullptr),
    _wave_propagate_act(nullptr),
    _decompose_act(nullptr),
    _zoom_in_act(nullptr),
    _zoom_out_act(nullptr),
    _zoom_fit_act(nullptr),
    _scene(new Scene(this)),
    _view(new QGraphicsView(_scene, this)),
    _panel(new Panel(this)),
    _dock(new QDockWidget(tr("Control Panel"), this))
{
    connect(_panel, SIGNAL(mode_changed(MODE)), _scene, SLOT(set_mode(MODE)));
    setCentralWidget(_view);
    _create_dock_widget();
    _create_actions();
    _create_menus();
    _connect_panel();
}

MainWindow::~MainWindow()
{
}

QSize MainWindow::minimumSizeHint() const
{
    return QSize(800, 600);
}

QSize MainWindow::sizeHint() const
{
    return QSize(1024, 768);
}

void MainWindow::_create_dock_widget()
{
    _dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _dock->setWidget(_panel);
    addDockWidget(Qt::RightDockWidgetArea, _dock);
}

void MainWindow::_create_actions()
{
    _clear_act   = new QAction(tr("&Clear"), this);
    connect(_clear_act, SIGNAL(triggered(bool)), this, SLOT(_clear()));

    _hexagonal_act    = new QAction(tr("&Hexagonal"), this);
    connect(_hexagonal_act, SIGNAL(triggered(bool)), this, SLOT(_hexagonal()));
    _cdt_act    = new QAction(tr("&CDT"), this);
    connect(_cdt_act, SIGNAL(triggered(bool)), this, SLOT(_cdt()));
    _fermat_point_act = new QAction(tr("&Fermat Point"), this);
    connect(_fermat_point_act, SIGNAL(triggered(bool)), this, SLOT(_fermat_point()));
    _wave_propagate_act = new QAction(tr("&Wave Propagate"), this);
    _decompose_act      = new QAction(tr("&Decompose"), this);

    _zoom_in_act      = new QAction(tr("Zoom in"), this);
    _zoom_in_act->setShortcut(tr("Z"));
    connect(_zoom_in_act, SIGNAL(triggered(bool)), this, SLOT(_zoom_in()));
    _zoom_out_act     = new QAction(tr("Zoom out"), this);
    _zoom_out_act->setShortcut(tr("Shift+Z"));
    connect(_zoom_out_act, SIGNAL(triggered(bool)), this, SLOT(_zoom_out()));
    _zoom_fit_act     = new QAction(tr("Zoom fit"), this);
    _zoom_fit_act->setShortcut(tr("F"));
    connect(_zoom_fit_act, SIGNAL(triggered(bool)), this, SLOT(_zoom_fit()));
}

void MainWindow::_create_menus()
{
    _file_menu = menuBar()->addMenu(tr("File"));
    _file_menu->addAction(_clear_act);
    _file_menu->addSeparator();
    _file_menu->addAction(_hexagonal_act);
    _file_menu->addAction(_cdt_act);
    _file_menu->addAction(_fermat_point_act);
    _file_menu->addAction(_wave_propagate_act);
    _file_menu->addAction(_decompose_act);
    _view_menu = menuBar()->addMenu(tr("View"));
    _view_menu->addAction(_dock->toggleViewAction());
    _view_menu->addSeparator();
    _view_menu->addAction(_zoom_in_act);
    _view_menu->addAction(_zoom_out_act);
    _view_menu->addAction(_zoom_fit_act);
}

void MainWindow::_connect_panel()
{
    connect(_panel->get_clear_button(), SIGNAL(clicked(bool)), this, SLOT(_clear()));
    connect(_panel->get_hex_button(), SIGNAL(clicked(bool)), this, SLOT(_hexagonal()));
    connect(_panel->get_cdt_button(), SIGNAL(clicked(bool)), this, SLOT(_cdt()));
    connect(_panel->get_fermat_point_button(), SIGNAL(clicked(bool)), this, SLOT(_fermat_point()));
}

void MainWindow::_clear()
{
    _scene->clear();
    get_input_manager().clear();
    get_cdt_manager().clear();
    _scene->initialize();
}

namespace  {

//double _get_hex( double total_area, double source_area )
//{
//    double x = 1.0;
//    double ratio = total_area / source_area;
//    return sqrt( ( ratio * 2 ) / ( x * 3 * sqrt( 3.0 ) )  );
//}

}

void MainWindow::_hexagonal()
{
    const static double a = 80;
//    double a = _get_hex(get_input_manager().get_total_area(), get_input_manager().get_source_area());
    double h = a * std::sqrt(3);
    double width  = get_input_manager().get_width();
    double height = get_input_manager().get_height();
    int x_ratio  = (width  + a + 1 ) / a;
    int y_ratio  = (height + h + 1 ) / h;

    get_input_manager().set_width(x_ratio * a);
    get_input_manager().set_height(y_ratio * h);

    _scene->clear();
    _scene->initialize();

    get_cdt_manager().clear();
    get_cdt_manager().add_sources(get_input_manager().get_sources());
    get_cdt_manager().add_obstacles(get_input_manager().get_obstacles());

    for(int i = 0; i <  2 * y_ratio + 1; ++i)
    {
        QVector<QPointF> points;
        for(int j = 0; j < x_ratio + 1; ++j)
        {
            double shift = 0;
            if(i % 2 == 1)
            {
                if(j == x_ratio)
                {
                    continue;
                }
                shift = a / 2;
            }
            double rad = 1;
            double x = j * a + shift;
            double y = i * h / 2;
            if(get_input_manager().is_blocked(QPointF(x, y)))
            {
                continue;
            }
            _scene->addEllipse(x - rad, y - rad, rad * 2, rad * 2);
            points.push_back(QPointF(x, y));
        }
        get_cdt_manager().add_hexagonals(points);
    }
}

void MainWindow::_cdt()
{
    get_cdt_manager().cdt();

    const QVector<QLineF>& lines = get_cdt_manager().get_lines();
    for(int i = 0; i < lines.size(); ++i)
    {
        _scene->addLine(lines[i]);
    }
}

void MainWindow::_fermat_point()
{
    get_cdt_manager().fermat_point();
    const QVector<QPointF>& points = get_cdt_manager().get_f_points();
    for(int i = 0; i < points.size(); ++i)
    {
        const QPointF& p = points[i];
        const double rad = 1;
        _scene->addEllipse(p.x() - rad, p.y() - rad, rad * 2, rad * 2);
    }

    const QVector<QLineF>& lines = get_cdt_manager().get_f_lines();
    for(int i = 0; i < lines.size(); ++i)
    {
        _scene->addLine(lines[i], QPen(QColor(Qt::green)));
    }
}

void MainWindow::_zoom_in()
{
    _view->scale(2.0, 2.0);
}

void MainWindow::_zoom_out()
{
    _view->scale(1.0 / 2.0, 1.0 / 2.0);
}

void MainWindow::_zoom_fit()
{
    _view->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
    _view->centerOn(_scene->sceneRect().center());
}
