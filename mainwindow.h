#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QMainWindow>
#include <QRubberBand>
#include <glview.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void stateChanged(GLView::State);
    void labChanged(GLView::Labs);

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_colorButton_clicked();
    void on_fontColorButton_clicked();
    void on_comboAlpha_currentIndexChanged(int index);
    void on_sliderAlpha_sliderMoved(int position);
    void on_spinAlpha_valueChanged(double arg1);
    void on_comboSFactor_currentIndexChanged(int index);
    void on_comboDFactor_currentIndexChanged(int index);
    void setStateFromAction(QAction* action);

    void onScissorsRectChanged(const QRubberBand &rubberBand);
    void on_comboBox_splineTypes_activated(const QString &arg1);

    void on_pushButton_actionSpline_clicked();

    void on_tabWidget_currentChanged(int index);

    void isSplineActivated(bool activated);
private:
    Ui::MainWindow *ui;

    QActionGroup   *m_toolBarActionGroup;
    QAction        *m_drawAction;
    QAction        *m_scissorsAction;
    QAction        *m_eraseAction;
    QAction        *m_selectAction;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
