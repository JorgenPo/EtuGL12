#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QDebug>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_drawAction(),
    m_scissorsAction(),
    m_eraseAction()
{
    ui->setupUi(this);

    ui->comboBox->addItem(tr("GL_POINTS"),         int(0x0000));
    ui->comboBox->addItem(tr("GL_LINES"),          int(0x0001));
    ui->comboBox->addItem(tr("GL_LINE_LOOP"),      int(0x0002));
    ui->comboBox->addItem(tr("GL_LINE_STRIP"),     int(0x0003));
    ui->comboBox->addItem(tr("GL_TRIANGLES"),      int(0x0004));
    ui->comboBox->addItem(tr("GL_TRIANGLE_STRIP"), int(0x0005));
    ui->comboBox->addItem(tr("GL_TRIANGLE_FAN"),   int(0x0006));
    ui->comboBox->addItem(tr("GL_QUADS"),          int(0x0007));
    ui->comboBox->addItem(tr("GL_QUAD_STRIP"),     int(0x0008));
    ui->comboBox->addItem(tr("GL_POLYGON"),        int(0x0009));

    connect(ui->openGLWidget, SIGNAL(frameSwapped()),
            ui->openGLWidget, SLOT(update()));

    ui->colorButton->setPalette(Qt::black);
    ui->fontColorButton->setPalette(Qt::white);
    ui->colorButton->setAutoFillBackground(true);

    /* Toolbar set up */
    m_drawAction     = new QAction (QIcon(":/images/icon_draw.svg"),     tr(""), this);
    m_scissorsAction = new QAction (QIcon(":/images/icon_scissors.svg"), tr(""), this);
    m_eraseAction    = new QAction (QIcon(":/images/icon_erase.svg"),    tr(""), this);

    ui->mainToolBar->insertAction(nullptr, m_drawAction);
    ui->mainToolBar->insertAction(nullptr, m_scissorsAction);
    ui->mainToolBar->insertAction(nullptr, m_eraseAction);

    m_drawAction->setToolTip(tr("Режим рисования"));
    m_scissorsAction->setToolTip(tr("Режим отсечения"));
    m_eraseAction->setToolTip(tr("Режим стирания"));

    m_drawAction->setShortcut(QKeySequence("Ctrl+D"));
    m_scissorsAction->setShortcut(QKeySequence("Ctrl+S"));
    m_eraseAction->setShortcut(QKeySequence("Ctrl+E"));

    m_drawAction->setCheckable(true);
    m_scissorsAction->setCheckable(true);
    m_eraseAction->setCheckable(true);

    /* Default */
    m_drawAction->setChecked(true);

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


// Color button clicked
void MainWindow::on_colorButton_clicked()
{
    QColor color = QColorDialog::getColor(
                Qt::black, this, tr("Выберите цвет точек"),
                QColorDialog::ShowAlphaChannel |
                QColorDialog::DontUseNativeDialog);

    ui->colorButton->setPalette(QPalette(color));

    ui->openGLWidget->setColor(color);
}

void MainWindow::on_fontColorButton_clicked()
{
    QColor color = QColorDialog::getColor(
                Qt::black, this, tr("Выберите цвет точек"),
                QColorDialog::ShowAlphaChannel |
                QColorDialog::DontUseNativeDialog);

    ui->fontColorButton->setPalette(QPalette(color));

    ui->openGLWidget->setBackgroundColor(color);
}
