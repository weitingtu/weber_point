#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "poly.h"

class Scene;
class Panel;
class QMenu;
class QAction;
class QGraphicsScene;
class QGraphicsView;
class QDockWdiget;
class QPen;
class Poly;

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
    void _wave_propagation();
    void _show_wp_weight(int index);
    void _show_vg_weight(int index);
    void _decompose_vg();
    void _decompose();
    void _change_font_size(int i);
    void _zoom_in();
    void _zoom_out();
    void _zoom_fit();

private:
    void _create_dock_widget();
    void _create_actions();
    void _create_menus();
    void _connect_panel();
    void _show_wp_weight(const QVector<Poly>& graph, const QVector<double>& weight, const QMap<int, double> &map);
    void _show_vg_weight(const QVector<QPointF>& points, const QVector<double>& weight);
    void _draw_poly(const Poly& p, const QPen &pen);

    struct MinPoly {
        MinPoly() : idx(-1), weights(), total_weight() {}
        int idx;
        QVector<double> weights;
        double total_weight;
    };

    struct Result {
        QVector<Poly>             graph;
        QVector<QVector<double> > weights;
        QVector<double>           total_weight;
        QVector<MinPoly>          min_polies;
        void clear() { graph.clear(); weights.clear(); total_weight.clear(); min_polies.clear();}
    };

    QMenu*          _file_menu;
    QMenu*          _view_menu;
    QAction*        _clear_act;
    QAction*        _hexagonal_act;
    QAction*        _cdt_act;
    QAction*        _fermat_point_act;
    QAction*        _wave_propagate_act;
    QAction*        _decompose_act;
    QAction*        _zoom_in_act;
    QAction*        _zoom_out_act;
    QAction*        _zoom_fit_act;
    Scene*          _scene;
    QGraphicsView*  _view;
    Panel*          _panel;
    QDockWidget*    _dock;
    Result          _result;
    double          _best;
    double          _second_best;
    bool            _finish;
    QVector<QPointF> _vg_points;
    QVector<QVector<double>> _vg_weights;
    QVector<QPointF>         _vg_prev_target_points;
    QVector<QVector<double>> _vg_prev_target_weights;
    QVector<QPointF>         _vg_target_points;
    QVector<QVector<double>> _vg_target_weights;
    int _dec_secs;
    int _total_secs;
};

#endif // MAINWINDOW_H
