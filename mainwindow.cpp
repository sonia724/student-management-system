#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtCore>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Buttons initial state
    ui->pushButtonAdd->setEnabled(true);
    ui->pushButtonUpdate->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->pushButtonNEXT, &QPushButton::clicked, this, &MainWindow::onNextClicked);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &MainWindow::onUpdateClicked);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);

    currentIndex = -1;

    // Load CSV
    QFile file("students.csv");
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString header = in.readLine(); // skip header

        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList f = line.split(",");

            if(f.size() == 6) {
                students.append({f[0], f[1], f[2], f[3], f[4], f[5]});
            }
        }
        file.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearFields() {
    ui->textEditName->clear();
    ui->textEditRoll->clear();
    ui->textEditFather->clear();
    ui->textEditcaste->clear();
    ui->textEditDistrict->clear();
    ui->textEditGPA->clear();
}

bool MainWindow::validateFields() {
    if (ui->textEditName->toPlainText().isEmpty()) return fieldError("Name");
    if (ui->textEditRoll->toPlainText().isEmpty()) return fieldError("Roll Number");
    if (ui->textEditFather->toPlainText().isEmpty()) return fieldError("Father Name");
    if (ui->textEditcaste->toPlainText().isEmpty()) return fieldError("Caste");
    if (ui->textEditDistrict->toPlainText().isEmpty()) return fieldError("District");
    if (ui->textEditGPA->toPlainText().isEmpty()) return fieldError("GPA");
    return true;
}

bool MainWindow::fieldError(QString field) {
    QMessageBox::warning(this, "Missing Field", field + " is required!");
    return false;
}

void MainWindow::saveAll() {
    QFile file("students.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&file);

        out << "Name,Roll,Father,Caste,District,GPA\n";
        for (auto &s : students)
            out << s.name << "," << s.roll << "," << s.father << ","
                << s.caste << "," << s.district << "," << s.gpa << "\n";

        file.close();
    }
}

void MainWindow::onAddClicked() {
    if (!validateFields()) return;

    // Add new record
    students.append({
        ui->textEditName->toPlainText(),
        ui->textEditRoll->toPlainText(),
        ui->textEditFather->toPlainText(),
        ui->textEditcaste->toPlainText(),
        ui->textEditDistrict->toPlainText(),
        ui->textEditGPA->toPlainText()
    });

    saveAll();
    QMessageBox::information(this, "Added", "Record added successfully!");

    // Clear fields for next input
    clearFields();
    currentIndex = -1;

    // Buttons state
    ui->pushButtonAdd->setEnabled(true);      // always enabled
    ui->pushButtonUpdate->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);
}

void MainWindow::onNextClicked() {
    if (students.isEmpty()) {
        QMessageBox::information(this, "Info", "No records available!");
        return;
    }

    currentIndex = (currentIndex + 1) % students.size();
    Student s = students[currentIndex];

    ui->textEditName->setText(s.name);
    ui->textEditRoll->setText(s.roll);
    ui->textEditFather->setText(s.father);
    ui->textEditcaste->setText(s.caste);
    ui->textEditDistrict->setText(s.district);
    ui->textEditGPA->setText(s.gpa);

    // Buttons state
    ui->pushButtonAdd->setEnabled(true);      // always enabled
    ui->pushButtonUpdate->setEnabled(true);
    ui->pushButtonDelete->setEnabled(true);
}

void MainWindow::onUpdateClicked() {
    if (currentIndex < 0 || currentIndex >= students.size()) return;
    if (!validateFields()) return;

    Student &s = students[currentIndex];

    // Do not change Roll Number
    QString oldRoll = s.roll;
    QString newRoll = ui->textEditRoll->toPlainText();
    if (oldRoll != newRoll) {
        QMessageBox::warning(this,
                             "Primary Key Error",
                             "You cannot change Roll Number!");
        ui->textEditRoll->setText(oldRoll);
        return;
    }

    s.name = ui->textEditName->toPlainText();
    s.father = ui->textEditFather->toPlainText();
    s.caste = ui->textEditcaste->toPlainText();
    s.district = ui->textEditDistrict->toPlainText();
    s.gpa = ui->textEditGPA->toPlainText();

    saveAll();
    QMessageBox::information(this, "Updated", "Record updated successfully!");

    // Buttons state
    ui->pushButtonAdd->setEnabled(true);      // always enabled
    ui->pushButtonUpdate->setEnabled(true);
    ui->pushButtonDelete->setEnabled(true);
}

void MainWindow::onDeleteClicked() {
    if (currentIndex < 0 || currentIndex >= students.size()) return;

    // Delete current record
    students.removeAt(currentIndex);
    saveAll();

    QMessageBox::information(this, "Deleted", "Record deleted successfully!");

    if (students.isEmpty()) {
        clearFields();
        currentIndex = -1;

        ui->pushButtonAdd->setEnabled(true);
        ui->pushButtonUpdate->setEnabled(false);
        ui->pushButtonDelete->setEnabled(false);
        return;
    }

    // Show next record after deletion
    currentIndex %= students.size();
    Student s = students[currentIndex];

    ui->textEditName->setText(s.name);
    ui->textEditRoll->setText(s.roll);
    ui->textEditFather->setText(s.father);
    ui->textEditcaste->setText(s.caste);
    ui->textEditDistrict->setText(s.district);
    ui->textEditGPA->setText(s.gpa);

    ui->pushButtonAdd->setEnabled(true);
    ui->pushButtonUpdate->setEnabled(true);
    ui->pushButtonDelete->setEnabled(true);
}
