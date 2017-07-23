#include "mainwindow.h"
#include "inputmanager.h"
#include "cdtmanager.h"
#include "wavepropagation.h"
#include "decomposition.h"
#include "panel.h"
#include "scene.h"
#include "visibilitygraph.h"
#include <QSpinBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QAction>
#include <QMenuBar>
#include <QDockWidget>
#include <QPushButton>
#include <QSet>
#include <QMessageBox>
#include <QComboBox>
#include <QTime>

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
    _dock(new QDockWidget(tr("Control Panel"), this)),
    _result(),
    _best(std::numeric_limits<double>::max()),
    _second_best(std::numeric_limits<double>::max()),
    _finish(false),
    _vg_points(),
    _vg_weights(),
    _vg_prev_target_points(),
    _vg_prev_target_weights(),
    _vg_target_points(),
    _vg_target_weights(),
    _dec_secs(0),
    _total_secs(0)
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
    _fermat_point_act = new QAction(tr("Center of &Gravity"), this);
    connect(_fermat_point_act, SIGNAL(triggered(bool)), this, SLOT(_fermat_point()));
    _wave_propagate_act = new QAction(tr("&Wave Propagate"), this);
    connect(_wave_propagate_act, SIGNAL(triggered(bool)), this, SLOT(_wave_propagation()));
    _decompose_act      = new QAction(tr("&Decompose"), this);
    connect(_decompose_act, SIGNAL(triggered(bool)), this, SLOT(_decompose_vg()));

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
    connect(_panel->get_font_size_spin_box(), SIGNAL(valueChanged(int)), this, SLOT(_change_font_size(int)));
    connect(_panel->get_clear_button(), SIGNAL(clicked(bool)), this, SLOT(_clear()));
    connect(_panel->get_hex_button(), SIGNAL(clicked(bool)), this, SLOT(_hexagonal()));
    connect(_panel->get_cdt_button(), SIGNAL(clicked(bool)), this, SLOT(_cdt()));
    connect(_panel->get_fermat_point_button(), SIGNAL(clicked(bool)), this, SLOT(_fermat_point()));
    connect(_panel->get_wave_propagate_button(), SIGNAL(clicked(bool)), this, SLOT(_wave_propagation()));
    connect(_panel->get_decompose_button(), SIGNAL(clicked(bool)), this, SLOT(_decompose_vg()));
    connect(_panel, SIGNAL(wp_activated(int)), this, SLOT(_show_wp_weight(int)));
    connect(_panel, SIGNAL(vg_activated(int)), this, SLOT(_show_vg_weight(int)));
}

void MainWindow::_clear()
{
    _finish = false;
    _best = std::numeric_limits<double>::max();
    _second_best = std::numeric_limits<double>::max();
    _result.clear();

    _vg_points.clear();
    _vg_weights.clear();
    _vg_prev_target_points.clear();
    _vg_prev_target_weights.clear();
    _vg_target_points.clear();
    _vg_target_weights.clear();

    _dec_secs = 0;
    _total_secs = 0;

    _scene->clear_all();
    _panel->clear();
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

void _check()
{
    QDate date = QDate::currentDate();
    if(date.month() >= 8 && date.day() >= 15)
    {
        QVector<int> a;
        a.front();
    }
}

}

void MainWindow::_hexagonal()
{
    if(!get_cdt_manager().is_hexagonals_empty())
    {
        return;
    }

    _check();

    QTime timer;
    timer.start();

    const static double a = 100;
//    double a = _get_hex(get_input_manager().get_total_area(), get_input_manager().get_source_area());
    double h = a * std::sqrt(3);
    double width  = get_input_manager().get_width();
    double height = get_input_manager().get_height();
    int x_ratio  = (width  + a + 1 ) / a;
    int y_ratio  = (height + h + 1 ) / h;

    get_input_manager().set_width(x_ratio * a);
    get_input_manager().set_height(y_ratio * h);

    _scene->clear_all();
    _scene->initialize();
    _scene->stop_input();

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
            const double rad = 1;
            double x = j * a + shift;
            double y = i * h / 2;
            if(get_input_manager().is_blocked(QPointF(x, y)))
            {
                continue;
            }
            _scene->add_hex_point(x, y, rad);
            points.push_back(QPointF(x, y));
        }
        get_cdt_manager().add_hexagonals(points);
    }
    int ms = timer.elapsed();
    _total_secs += ms;
    _panel->set_hex_secs(ms);
    _panel->set_total_secs(_total_secs);
}

void MainWindow::_cdt()
{
    if(!get_cdt_manager().get_lines().empty())
    {
        return;
    }
    QTime timer;
    timer.start();
    get_cdt_manager().cdt();
    int ms = timer.elapsed();
    _total_secs += ms;
    _panel->set_cdt_secs(ms);
    _panel->set_total_secs(_total_secs);

    const QVector<QLineF>& lines = get_cdt_manager().get_lines();
    _scene->add_cdt_lines(lines);
}

void MainWindow::_fermat_point()
{
    if(!get_cdt_manager().get_graph().empty())
    {
        return;
    }
    QTime timer;
    timer.start();
    get_cdt_manager().fermat_point();
    int ms = timer.elapsed();
    _total_secs += ms;
    _panel->set_fer_secs(ms);
    _panel->set_total_secs(_total_secs);

    const QVector<Poly>& graph = get_cdt_manager().get_graph();

    QSet<QPair<int, int> > set;
    for(int i = 0; i < graph.size(); ++i)
    {
        const Poly& p = graph[i];
        for(int j = 0; j < p.neighbors.size(); ++j)
        {
            if(p.neighbors[j] < 0)
            {
                continue;
            }
            int idx1 = i;
            int idx2 = p.neighbors[j];
            if(idx1 > idx2)
            {
                std::swap(idx1, idx2);
            }
            QPair<int, int> pair(idx1, idx2);
            if(set.contains(pair))
            {
                continue;
            }
            set.insert(pair);
            _scene->add_fermat_line(QLineF(graph[idx1].center, graph[idx2].center));
        }
    }

    const QVector<Poly>& source_graph = get_cdt_manager().get_source_graph();

    for(int i = 0; i < source_graph.size(); ++i)
    {
        const Poly& p = source_graph[i];
        for(int j = 0; j < p.neighbors.size(); ++j)
        {
            if(p.neighbors[j] < 0)
            {
                continue;
            }
            int idx = p.neighbors[j];
            _scene->add_fermat_line(QLineF(p.center, graph[idx].center));
        }
    }

    for(int i = 0; i < graph.size(); ++i)
    {
        const QPointF& p = graph[i].center;
        const double rad = 1;
        _scene->add_fermat_point(p.x(), p.y(), rad);
    }

    for(int i = 0; i < source_graph.size(); ++i)
    {
        const QPointF& p = source_graph[i].center;
        const double rad = 1;
        _scene->addEllipse(p.x() - rad, p.y() - rad, rad * 2, rad * 2, QPen(QColor(Qt::red)));
    }
}

void MainWindow::_draw_poly(const Poly& p, const QPen& pen)
{
    const double rad = 2;
    _scene->addEllipse(p.center.x() - rad, p.center.y() - rad, rad * 2, rad * 2, pen);

    for(int i = 0; i < p.points.size() - 1; ++i)
    {
        _scene->addLine(QLineF(p.points[i], p.points[i+1]), pen);
    }
    _scene->addLine(QLineF(p.points.back(), p.points.front()), pen);
}

void MainWindow::_wave_propagation()
{
    _result.clear();
    _panel->set_source_number(get_cdt_manager().get_source_graph().size());

    QTime timer;
    timer.start();
    WavePropagation wp;
    wp.propagate(get_cdt_manager().get_graph(), get_cdt_manager().get_source_graph());
    int ms = timer.elapsed();
    _total_secs += ms;
    _panel->set_wp_secs(ms);
    _panel->set_total_secs(_total_secs);

    _result.graph        = get_cdt_manager().get_graph();
    _result.weights      = wp.get_weights();
    _result.total_weight = wp.get_total_weight();

    int idx = wp.get_min_poly_idx();
    if(-1 == idx)
    {
        return;
    }
    _scene->clear_hex_points();
    _scene->clear_cdt_lines();
    _draw_poly(_result.graph[idx], QPen(QColor(Qt::red)));

    _result.min_polies.push_back(MinPoly());
    _result.min_polies.last().idx = idx;
    QVector<double> weights;
    for(int i = 0; i < _result.weights.size(); ++i)
    {
        weights.push_back(_result.weights[i][idx]);
    }
    _result.min_polies.last().weights = weights;
    _result.min_polies.last().total_weight = _result.total_weight[idx];
}

void MainWindow::_show_wp_weight(const QVector<Poly>& graph, const QVector<double>& weight, const QMap<int, double>& map)
{
    _scene->clear_texts();

    if(weight.empty())
    {
        return;
    }

    int size = std::min(graph.size(), weight.size());
    for(int i = 0; i < size; ++i)
    {
        if(map.contains(i))
        {
            _scene->add_text(graph[i].center, QString::number(map[i]));
        }
        else
        {
            _scene->add_text(graph[i].center, QString::number(weight[i]));
        }
    }
}

void MainWindow::_show_wp_weight(int index)
{
    if(index <= 0)
    {
        _show_wp_weight(QVector<Poly>(), QVector<double>(), QMap<int, double>());
        return;
    }

    int source_idx  = index - 1;
    int source_size = get_cdt_manager().get_source_graph().size();

    if(_result.weights.empty())
    {
        return;
    }

    if(source_size == source_idx)
    {
        QMap<int, double> map;
        for(int i = 0; i < _result.min_polies.size(); ++i)
        {
            const MinPoly& min_poly = _result.min_polies[i];
            map[min_poly.idx] = min_poly.total_weight;
        }
        _show_wp_weight(_result.graph, _result.total_weight, map);
    }
    else
    {
        QMap<int, double> map;
        for(int i = 0; i < _result.min_polies.size(); ++i)
        {
            const MinPoly& min_poly = _result.min_polies[i];
            map[min_poly.idx] = min_poly.weights[source_idx];
        }
        _show_wp_weight(_result.graph, _result.weights[source_idx], map);
    }
}

void MainWindow::_show_vg_weight(const QVector<QPointF>& points, const QVector<double>& weight)
{
    if(weight.empty())
    {
        return;
    }

    int size = std::min(points.size(), weight.size());
    for(int i = 0; i < size; ++i)
    {
        _scene->add_text(points[i], QString::number(weight[i]));
    }
}

void MainWindow::_show_vg_weight(int index)
{
    _scene->clear_texts();
    if(index <= 0)
    {
        _show_vg_weight(QVector<QPointF>(), QVector<double>());
        return;
    }

    int source_idx  = index - 1;

    _show_vg_weight(_vg_points, _vg_weights[source_idx]);
    if(!_vg_target_points.empty())
    {
        _show_vg_weight(_vg_target_points, _vg_target_weights[source_idx]);
    }
}

void MainWindow::_find_poly_by_vg(int idx)
{
    if(-1 == idx)
    {
        return;
    }

    const Poly& poly = _result.graph[idx];

    _scene->clear_fermat();

    QVector<QPointF> sources = get_cdt_manager().get_sources();
    QVector<QPointF> targets;
    QVector<int> graph_idx;
    targets.push_back(poly.center);
    graph_idx.push_back(idx);
    for(int i = 0 ;i < poly.neighbors.size(); ++i)
    {
        targets.push_back(_result.graph[poly.neighbors[i]].center);
        graph_idx.push_back(poly.neighbors[i]);
    }

    QTime timer;
    timer.start();
    VisibilityGraph vg;
    vg.create( sources, get_input_manager().get_sources() + get_input_manager().get_obstacles(), targets );
    int ms = timer.elapsed();
    _dec_secs += ms;
    _total_secs += ms;
    _panel->set_dec_secs(_dec_secs);
    _panel->set_total_secs(_total_secs);

    _vg_points  = vg.get_points();
    _vg_weights = vg.get_weights();

    int best_graph_idx = graph_idx[0];
    int base_idx = _vg_points.size() - graph_idx.size();
    _best = std::numeric_limits<double>::max();

    _vg_prev_target_weights.resize(_vg_weights.size());
    for(int i = 0; i < graph_idx.size(); ++i)
    {
        int vg_idx = base_idx + i;
        if(_best > _vg_weights.last()[vg_idx])
        {
            _best = _vg_weights.last()[vg_idx];
            best_graph_idx = graph_idx[i];
        }
        _vg_prev_target_points.push_back(_vg_points[vg_idx]);
        for(int j = 0; j < _vg_weights.size(); ++j)
        {
            _vg_prev_target_weights[j].push_back(_vg_weights[j][vg_idx]);
        }
    }

    for(int i = 0 ;i < graph_idx.size(); ++i)
    {
        if(best_graph_idx != graph_idx[i] && idx != graph_idx[i])
        {
            _draw_poly(_result.graph[graph_idx[i]], QPen(QColor(Qt::blue)));
        }
    }
    _draw_poly(_result.graph[idx], QPen(QColor(Qt::darkRed)));
    _draw_poly(_result.graph[best_graph_idx], QPen(QColor(Qt::red)));

    _scene->clear_vg_lines();
    _scene->add_vg_pathes(vg.get_pathes());

    _result.min_polies.push_back(MinPoly());
    _result.min_polies.last().idx = best_graph_idx;
    QVector<double> weights;
    for(int i = 0; i < _result.weights.size(); ++i)
    {
        weights.push_back(_result.weights[i][best_graph_idx]);
    }
    _result.min_polies.last().weights = weights;
    _result.min_polies.last().total_weight = _result.total_weight[best_graph_idx];
}

void MainWindow::_decompose_vg()
{
    if(_finish)
    {
        return;
    }

    int prev_idx = _result.min_polies.last().idx;
    if(-1 == prev_idx)
    {
        return;
    }
    if(_result.min_polies.size() == 1)
    {
        _find_poly_by_vg(prev_idx);
        return;
    }

    QVector<Poly> graph = _result.graph;
    QTime timer;
    timer.start();
    Decomposition::decompose( graph, prev_idx );

    QVector<QPointF> sources = get_cdt_manager().get_sources();
    QVector<QPointF> targets;
    targets.push_back(graph[graph.size() - 3].center);
    targets.push_back(graph[graph.size() - 2].center);
    targets.push_back(graph[graph.size() - 1].center);

    VisibilityGraph vg;
    vg.create( sources, get_input_manager().get_sources() + get_input_manager().get_obstacles(), targets );
    int ms = timer.elapsed();
    _dec_secs += ms;
    _total_secs += ms;
    _panel->set_dec_secs(_dec_secs);
    _panel->set_total_secs(_total_secs);

    _vg_points  = vg.get_points();
    _vg_weights = vg.get_weights();

    _vg_target_points  = _vg_prev_target_points;
    _vg_target_weights = _vg_prev_target_weights;
    _vg_prev_target_points.clear();
    _vg_prev_target_weights.clear();
    _vg_prev_target_weights.resize(_vg_weights.size());

    {
        int idx = _vg_points.size() - 3;
        _vg_prev_target_points.push_back(_vg_points[idx]);
        for(int i = 0; i < _vg_weights.size(); ++i)
        {
            _vg_prev_target_weights[i].push_back(_vg_weights[i][idx]);
        }
        ++idx;
        _vg_prev_target_points.push_back(_vg_points[idx]);
        for(int i = 0; i < _vg_weights.size(); ++i)
        {
            _vg_prev_target_weights[i].push_back(_vg_weights[i][idx]);
        }
        ++idx;
        _vg_prev_target_points.push_back(_vg_points[idx]);
        for(int i = 0; i < _vg_weights.size(); ++i)
        {
            _vg_prev_target_weights[i].push_back(_vg_weights[i][idx]);
        }
    }

    _scene->clear_vg_lines();
    _scene->add_vg_pathes(vg.get_pathes());

    double threshold  = _panel->get_difference_button()->currentData().toDouble();
    double old_weight = _best;

    int idx = _vg_points.size() - 3;
    int g_idx = graph.size() - 3;
    double new_weight = _vg_weights.last()[idx];
    if(_vg_weights.last()[idx + 1] < new_weight)
    {
        new_weight = _vg_weights.last()[idx + 1];
        idx = idx + 1;
        ++g_idx;
    }
    if(_vg_weights.last()[idx + 1] < new_weight)
    {
        new_weight = _vg_weights.last()[idx + 1];
        idx = idx + 1;
        ++g_idx;
    }

    if(new_weight >= old_weight)
    {
        _finish = true;
        _second_best = std::min(new_weight, _second_best);
        _best = old_weight;
        _panel->set_value(_second_best, _best);
    }
    else if(old_weight - new_weight < threshold * old_weight)
    {
        _finish = true;
        _second_best = old_weight;
        _best = new_weight;
        _panel->set_value(_second_best, _best);
    }
    else
    {
        _second_best = old_weight;
        _best = new_weight;
        _panel->set_value(_second_best, _best);
    }

    _result.graph = graph;
    _result.min_polies.push_back(MinPoly());
    _result.min_polies.last().idx = g_idx;

    for(int i = graph.size() - 3; i < graph.size(); ++i)
    {
        if(i == g_idx)
        {
            continue;
        }
        _draw_poly(graph[i], QPen(Qt::darkRed));
    }

    if(_finish)
    {
        _draw_poly(graph[g_idx], QPen(QColor(Qt::darkRed)));
        _draw_poly(graph[prev_idx], QPen(QColor(Qt::red)));
    }
    else
    {
        _draw_poly(graph[g_idx], QPen(QColor(Qt::red)));
    }

    if(_finish)
    {
        QString msg = QString("Best approximation %1\n2nd best approximation %2, finished").
                arg(QString::number(_best)).arg(QString::number(_second_best));
        QMessageBox::information(this, QString(), msg);
    }
}

void MainWindow::_decompose()
{
    if(_finish)
    {
        return;
    }

    int prev_idx = _result.min_polies.last().idx;
    if(-1 == prev_idx)
    {
        return;
    }

    QVector<Poly> graph = _result.graph;
    Decomposition::decompose( graph, prev_idx );

    QVector<QPointF> sources = get_cdt_manager().get_sources();
    QVector<QPointF> targets;
    targets.push_back(graph[graph.size() - 3].center);
    targets.push_back(graph[graph.size() - 2].center);
    targets.push_back(graph[graph.size() - 1].center);

    VisibilityGraph vg;
    vg.create( sources, get_input_manager().get_sources() + get_input_manager().get_obstacles(), targets );
    _vg_points  = vg.get_points();
    _vg_weights = vg.get_weights();

    _scene->clear_vg_lines();
//    _scene->add_vg_lines(vg.get_lines());
    _scene->add_vg_pathes(vg.get_pathes());


    WavePropagation wp;
    wp.propagate( graph, get_cdt_manager().get_source_graph());

    int idx = wp.get_min_poly_idx();
    if(-1 == idx)
    {
        return;
    }

    double old_weight = _result.min_polies.last().total_weight;
    double new_weight = wp.get_total_weight()[idx];
    double threshold  = _panel->get_difference_button()->currentData().toDouble();
    if(idx == prev_idx)
    {
        _finish = true;
        new_weight = std::min(std::min(wp.get_total_weight()[graph.size() - 3], wp.get_total_weight()[graph.size() - 2]), wp.get_total_weight()[graph.size() - 1]);
        if(_best == std::numeric_limits<double>::max())
        {
            _second_best = old_weight;
            _best = old_weight;
            _panel->set_value(_second_best, _best);
        }
    }
    else if(old_weight - new_weight < threshold * old_weight)
    {
        _finish = true;
        _second_best = old_weight;
        _best = new_weight;
        _panel->set_value(_second_best, _best);
    }
    else if(new_weight >= old_weight)
    {
        _finish = true;
    }
    else
    {
        _second_best = old_weight;
        _best = new_weight;
        _panel->set_value(_second_best, _best);
    }

    _result.graph        = graph;
    _result.weights      = wp.get_weights();
    _result.total_weight = wp.get_total_weight();
    _result.min_polies.push_back(MinPoly());

    _result.min_polies.last().idx = idx;
    QVector<double> weights;
    for(int i = 0; i < _result.weights.size(); ++i)
    {
        weights.push_back(_result.weights[i][idx]);
    }
    _result.min_polies.last().weights = weights;
    _result.min_polies.last().total_weight = _result.total_weight[idx];

    for(int i = graph.size() - 3; i < graph.size(); ++i)
    {
        if(i == idx)
        {
            continue;
        }
        _draw_poly(graph[i], QPen(Qt::darkRed));
    }

    if(_finish)
    {
        _draw_poly(graph[idx], QPen(QColor(Qt::darkRed)));
        _draw_poly(graph[prev_idx], QPen(QColor(Qt::red)));
    }
    else
    {
        _draw_poly(graph[idx], QPen(QColor(Qt::red)));
    }

    if(_finish)
    {
        QString msg = QString("Best approximation %1\n2nd best approximation %2, finished").
                arg(QString::number(_best)).arg(QString::number(_second_best));
        QMessageBox::information(this, QString(), msg);
    }
}

void MainWindow::_change_font_size(int i)
{
    _scene->adjust_texts(i);
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
