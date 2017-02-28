#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox->addItem(tr("GL_POINTS"),      int(0x0000));
    ui->comboBox->addItem(tr("GL_LINES"),       int(0x0001));
    ui->comboBox->addItem(tr("GL_LINE_LOOP"),   int(0x0002));
    ui->comboBox->addItem(tr("GL_LINE_STRIP"),  int(0x0003));
    ui->comboBox->addItem(tr("GL_TRIANGLES"),   int(0x0004));
    ui->comboBox->addItem(tr("GL_TRIANGLE_STRIP"), int(0x0005));
    ui->comboBox->addItem(tr("GL_TRIANGLE_FAN"), int(0x0006));
    ui->comboBox->addItem(tr("GL_QUADS"),       int(0x0007));
    ui->comboBox->addItem(tr("GL_QUAD_STRIP"),  int(0x0008));
    ui->comboBox->addItem(tr("GL_POLYGON"),     int(0x0009));

    connect(ui->openGLWidget, SIGNAL(frameSwapped()),
            ui->openGLWidget, SLOT(update()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    int value = ui->comboBox->itemData(index).toInt();

    ui->openGLWidget->setPrimitiveType(value);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_F1:
        qDebug() << "HELLO WORLD";
        ui->openGLWidget->clearVertices();
        break;
    default:
        break;
    }
}

