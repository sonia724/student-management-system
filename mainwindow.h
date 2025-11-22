#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Student {
    QString name;
    QString roll;
    QString father;
    QString caste;
    QString district;
    QString gpa;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QList<Student> students;
    int currentIndex;

    void clearFields();
    bool validateFields();
    bool fieldError(QString field);
    void saveAll();

private slots:
    void onAddClicked();
    void onNextClicked();
    void onUpdateClicked();
    void onDeleteClicked();
};

#endif // MAINWINDOW_H
