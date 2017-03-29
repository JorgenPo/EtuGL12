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
    m_eraseAction(),
    m_selectAction()
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


    ui->comboAlpha->addItem(tr("GL_NEVER"),    0x0200);
    ui->comboAlpha->addItem(tr("GL_LESS"),     0x0201);
    ui->comboAlpha->addItem(tr("GL_EQUAL"),    0x0202);
    ui->comboAlpha->addItem(tr("GL_LEQUAL"),   0x0203);
    ui->comboAlpha->addItem(tr("GL_GREATER"),  0x0204);
    ui->comboAlpha->addItem(tr("GL_NOTEQUAL"), 0x0205);
    ui->comboAlpha->addItem(tr("GL_GEQUAL"),   0x0206);
    ui->comboAlpha->addItem(tr("GL_ALWAYS"),   0x0207);

    ui->comboSFactor->addItem(tr("GL_ZERO"),                 0x0000);
    ui->comboSFactor->addItem(tr("GL_ONE"),                  0x0001);
    ui->comboSFactor->addItem(tr("GL_SRC_COLOR"),            0x0300);
    ui->comboSFactor->addItem(tr("GL_ONE_MINUS_SRC_COLOR"),  0x0301);
    ui->comboSFactor->addItem(tr("GL_SRC_ALPHA"),            0x0302);
    ui->comboSFactor->addItem(tr("GL_ONE_MINUS_SRC_ALPHA"),  0x0303);
    ui->comboSFactor->addItem(tr("GL_DST_ALPHA"),            0x0304);
    ui->comboSFactor->addItem(tr("GL_ONE_MINUS_DST_ALPHA"),  0x0305);
    ui->comboSFactor->addItem(tr("GL_DST_COLOR"),            0x0306);
    ui->comboSFactor->addItem(tr("GL_ONE_MINUS_DST_COLOR"),  0x0307);
    ui->comboSFactor->addItem(tr("GL_SRC_ALPHA_SATURATE"),   0x0308);

    ui->comboDFactor->addItem(tr("GL_ZERO"),                 0x0000);
    ui->comboDFactor->addItem(tr("GL_ONE"),                  0x0001);
    ui->comboDFactor->addItem(tr("GL_SRC_COLOR"),            0x0300);
    ui->comboDFactor->addItem(tr("GL_ONE_MINUS_SRC_COLOR"),  0x0301);
    ui->comboDFactor->addItem(tr("GL_SRC_ALPHA"),            0x0302);
    ui->comboDFactor->addItem(tr("GL_ONE_MINUS_SRC_ALPHA"),  0x0303);
    ui->comboDFactor->addItem(tr("GL_DST_ALPHA"),            0x0304);
    ui->comboDFactor->addItem(tr("GL_ONE_MINUS_DST_ALPHA"),  0x0305);

    ui->colorButton->setPalette(Qt::black);
    ui->fontColorButton->setPalette(Qt::white);
    ui->colorButton->setAutoFillBackground(true);

    /* Toolbar set up */
    m_drawAction     = new QAction (QIcon(":/images/icon_draw.svg"),     tr(""), this);
    m_scissorsAction = new QAction (QIcon(":/images/icon_scissors.svg"), tr(""), this);
    m_eraseAction    = new QAction (QIcon(":/images/icon_erase.svg"),    tr(""), this);
    m_selectAction   = new QAction (QIcon(":/images/004-select.png"),    tr(""), this);

    m_drawAction->setData(static_cast<int>(GLView::STATE_DRAW));
    m_scissorsAction->setData(static_cast<int>(GLView::STATE_SCISSORS));
    m_eraseAction->setData(static_cast<int>(GLView::STATE_ERASE));
    m_selectAction->setData(static_cast<int>(GLView::STATE_SELECT));

    m_toolBarActionGroup = new QActionGroup(this);
    m_toolBarActionGroup->addAction(m_drawAction);
    m_toolBarActionGroup->addAction(m_scissorsAction);
    m_toolBarActionGroup->addAction(m_eraseAction);
    m_toolBarActionGroup->addAction(m_selectAction);
    m_toolBarActionGroup->setExclusive(true);

    ui->mainToolBar->insertAction(nullptr, m_drawAction);
    ui->mainToolBar->insertAction(nullptr, m_scissorsAction);
    ui->mainToolBar->insertAction(nullptr, m_eraseAction);
    ui->mainToolBar->insertAction(nullptr, m_selectAction);

    m_drawAction->setToolTip(tr("Режим рисования"));
    m_scissorsAction->setToolTip(tr("Режим отсечения"));
    m_eraseAction->setToolTip(tr("Режим стирания"));
    m_selectAction->setToolTip(tr("Режим выделения"));

    m_drawAction->setShortcut(QKeySequence("Ctrl+D"));
    m_scissorsAction->setShortcut(QKeySequence("Ctrl+S"));
    m_eraseAction->setShortcut(QKeySequence("Ctrl+E"));
    m_selectAction->setShortcut(QKeySequence("Ctrl+A"));

    m_drawAction->setCheckable(true);
    m_scissorsAction->setCheckable(true);
    m_eraseAction->setCheckable(true);
    m_selectAction->setCheckable(true);

    /* Default */
    m_drawAction->setChecked(true);

    ui->comboAlpha->setCurrentIndex(7);

    ui->comboSFactor->setCurrentIndex(1);
    ui->comboDFactor->setCurrentIndex(0);


    connect(m_toolBarActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(setState(QAction*)));

    ui->openGLWidget->setScissorTestEnabled(true, 0, 0,
                                            this->size().width(), this->size().height());

    connect(ui->openGLWidget, SIGNAL(scissorsRectChanged(const QRubberBand&)), this, SLOT(onScissorsRectChanged(const QRubberBand&)));

    connect(ui->clearAction, SIGNAL(triggered(bool)), ui->openGLWidget, SLOT(clearVertices()));
    connect(ui->actionCopy, SIGNAL(triggered(bool)), ui->openGLWidget, SLOT(copyVertices()));
    connect(ui->actionPaste, SIGNAL(triggered(bool)), ui->openGLWidget, SLOT(pasteVertices()));
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
    qDebug() << "Key main";
    switch(event->key()) {
    case Qt::Key_F1:
        qDebug() << "F1 Pressed";
        ui->openGLWidget->clearVertices();
        break;
    default:
        break;
    }
    ui->openGLWidget->keyPressEvent(event);
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

void MainWindow::on_comboAlpha_currentIndexChanged(int index)
{
    int func = ui->comboAlpha->itemData(index).toInt();

    if ( func == GL_ALWAYS ) {
        ui->sliderAlpha->setEnabled(false);
        ui->spinAlpha->setEnabled(false);
        ui->openGLWidget->setAlphaTestEnabled(false);
    } else {
        ui->sliderAlpha->setEnabled(true);
        ui->spinAlpha->setEnabled(true);
        ui->openGLWidget->setAlphaTestEnabled(true);
        ui->openGLWidget->setAlphaFunction(func);
    }

}

void MainWindow::on_sliderAlpha_sliderMoved(int position)
{
    float clampedValue = position / 100.0f;

    ui->openGLWidget->setAlphaRef(clampedValue);

    ui->sliderAlpha->setToolTip(QString("Value = %1").arg(clampedValue));

    ui->spinAlpha->setValue(clampedValue);
}

void MainWindow::on_spinAlpha_valueChanged(double arg1)
{
    ui->sliderAlpha->setValue(arg1 * 100);
}

void MainWindow::on_comboSFactor_currentIndexChanged(int index)
{
    int value = ui->comboSFactor->itemData(index).toInt();

    ui->openGLWidget->setBlendingSfactor(value);
}

void MainWindow::on_comboDFactor_currentIndexChanged(int index)
{
    int value = ui->comboDFactor->itemData(index).toInt();

    ui->openGLWidget->setBlendingDfactor(value);
}

void MainWindow::setState(QAction *action)
{
    GLView::State state = static_cast<GLView::State>(action->data().toInt());
    switch (state) {
    case GLView::STATE_DRAW:
        ui->openGLWidget->setState(GLView::STATE_DRAW);
        break;
    case GLView::STATE_SCISSORS:
        ui->openGLWidget->setState(GLView::STATE_SCISSORS);
        break;
    case GLView::STATE_ERASE:
        ui->openGLWidget->setState(GLView::STATE_ERASE);
        break;
    case GLView::STATE_SELECT:
        ui->openGLWidget->setState(GLView::STATE_SELECT);
        break;
    default:
        qDebug() << "Error unknown state: " << state << "\n";
        break;
    }
}

void MainWindow::onScissorsRectChanged(const QRubberBand& rubberBand)
{
    ui->scissorX->setValue(rubberBand.x());
    ui->scissorY->setValue(rubberBand.y());
    ui->scissorWidth->setValue(rubberBand.width());
    ui->scissorHeight->setValue(rubberBand.height());
}
