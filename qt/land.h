#ifndef LAND_H
#define LAND_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class land;
}

class land : public QDialog
{
    Q_OBJECT

public:
    explicit land(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    ~land();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::land *ui;
};

#endif // LAND_H
