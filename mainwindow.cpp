#include "mainwindow.h"
#include "ui_mainwindow.h"

#define view ui->graphicsView
#define intOnly setValidator(new QIntValidator(INT_MIN, INT_MAX, this))
#define getNum text().toInt()

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    tamano = 0;

    grosor = 0;
    altura = 0;

    ui->lineAgregar->intOnly;
    ui->lineBorrar->intOnly;
    ui->lineBuscar->intOnly;

    bt.scene = new QGraphicsScene(10, 71, 600 , 500);
    view->setScene(bt.scene);
//    view->centerOn(310, 321);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushAgregar_clicked()
{
    int x = ui->lineAgregar->getNum;

    if(bt.insert(x))
        tamano++;

    altura  = bt.altura(bt.root, -1);
    grosor  = bt.numHojas(bt.root, -1, -1);

    cout<<"Grosor: "<<grosor<<endl;
    cout<<"Altura: "<<altura<<endl;
    cout<<"Size: "<<tamano<<endl;

    bt.scene->clear();
    bt.printTree(bt.root, grosor, altura);

}

void MainWindow::on_pushBorrar_clicked()
    {
    int x = ui->lineBorrar->getNum;

    if(bt.DelNode(x))
        tamano--;

    altura  = bt.altura(bt.root, -1);
    grosor  = bt.numHojas(bt.root, -1, -1);

    cout<<"Grosor: "<<grosor<<endl;
    cout<<"Altura: "<<altura<<endl;
    cout<<"Size: "<<tamano<<endl;

    bt.scene->clear();
    bt.printTree(bt.root, grosor, altura);

}

void MainWindow::on_pushBuscar_clicked()
{
    ui->label->clear();

    QString s;
    int x = ui->lineBuscar->getNum;
    s = bt.buscar(x);

    ui->label->setText(s);
}
