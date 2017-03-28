#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QMainWindow>
#include <QRubberBand>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_colorButton_clicked();
    void on_fontColorButton_clicked();
    void on_comboAlpha_currentIndexChanged(int index);
    void on_sliderAlpha_sliderMoved(int position);
    void on_spinAlpha_valueChanged(double arg1);
    void on_comboSFactor_currentIndexChanged(int index);
    void on_comboDFactor_currentIndexChanged(int index);
    void setState(QAction* action);    

    void onScissorsRectChanged(const QRubberBand &rubberBand);

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
