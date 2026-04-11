#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupDatabase();
    loadNotes();

    ui->lineEdit->setPlaceholderText("Поиск...");
    ui->lineEdit->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("notes.db");

    if (!db.open()) {
        qDebug() << "Ошибка открытия БД";
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS notes ("
               "id      INTEGER PRIMARY KEY AUTOINCREMENT, "
               "title   TEXT, "
               "content TEXT, "
               "created TEXT)");
}

void MainWindow::loadNotes(const QString &search)
{
    ui->listWidget->clear();

    QSqlQuery query;

    if (search.isEmpty()) {
        query.exec("SELECT id, title FROM notes ORDER BY id DESC");
    } else {
        query.prepare("SELECT id, title FROM notes "
                      "WHERE title LIKE :s ORDER BY id DESC");
        query.bindValue(":s", "%" + search + "%");
        query.exec();
    }

    while (query.next()) {
        int     id    = query.value("id").toInt();
        QString title = query.value("title").toString();

        QListWidgetItem *item = new QListWidgetItem(title);
        item->setData(Qt::UserRole, id);
        ui->listWidget->addItem(item);
    }
}

void MainWindow::addNoteToDB(const QString &title)
{
    QSqlQuery query;
    query.prepare("INSERT INTO notes (title, content, created) "
                  "VALUES (:title, '', datetime('now'))");
    query.bindValue(":title", title);
    query.exec();
}

void MainWindow::deleteNoteFromDB(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
}

void MainWindow::saveContentToDB(int id, const QString &content)
{
    QSqlQuery query;
    query.prepare("UPDATE notes SET content = :content WHERE id = :id");
    query.bindValue(":content", content);
    query.bindValue(":id", id);
    query.exec();
}

void MainWindow::on_lineEdit_textChanged(const QString &text)
{
    loadNotes(text);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString title = ui->lineEdit->text().trimmed();
    if (title.isEmpty()) return;

    addNoteToDB(title);
    ui->lineEdit->clear();
    ui->textEdit->clear();
}

void MainWindow::on_pushButton_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if (!item) return;

    int id = item->data(Qt::UserRole).toInt();
    deleteNoteFromDB(id);
    loadNotes(ui->lineEdit->text());
    ui->textEdit->clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if (!item) return;

    int id = item->data(Qt::UserRole).toInt();
    saveContentToDB(id, ui->textEdit->toPlainText());
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int id = item->data(Qt::UserRole).toInt();

    QSqlQuery query;
    query.prepare("SELECT content FROM notes WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        ui->textEdit->setPlainText(query.value("content").toString());
    }
}