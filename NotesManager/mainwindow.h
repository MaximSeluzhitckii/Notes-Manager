#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_lineEdit_textChanged(const QString &text);

private:
    Ui::MainWindow *ui;

    void setupDatabase();
    void loadNotes(const QString &search = "");
    void addNoteToDB(const QString &title);
    void deleteNoteFromDB(int id);
    void saveContentToDB(int id, const QString &content);
};

#endif // MAINWINDOW_H