#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <btree.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Btree bt;
    int tamano;
    int grosor;
    int altura;

    void calculatePadding();
    //void printTree(node * r, int x, int y);

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushAgregar_clicked();

    void on_pushBorrar_clicked();

    void on_pushBuscar_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
