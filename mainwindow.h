#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class HeroItems;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_CalcX2_clicked();

private:
    Ui::MainWindow *ui;

    void showItemsX2(const HeroItems &items);
};

#endif // MAINWINDOW_H
