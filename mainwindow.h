#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

    QAction        *m_drawAction;
    QAction        *m_scissorsAction;
    QAction        *m_eraseAction;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
