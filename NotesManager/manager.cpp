#include "mainwindow.h"
#include "manager.h"
#include "ui_mainwindow.h"

Manager::Manager()
{
    setupDatabase();
    //loadNotes();
}

void Manager::setupDatabase() //
{
    db = QSqlDatabase::addDatabase("QSQLITE");
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

void Manager::loadNotes(QListWidget* listWidget, const QString &search) //
{
    listWidget->clear();

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
        listWidget->addItem(item);
    }
}

void Manager::addNoteToDB(const QString &title) //
{
    QSqlQuery query;
    query.prepare("INSERT INTO notes (title, content, created) "
                  "VALUES (:title, '', datetime('now'))");
    query.bindValue(":title", title);
    query.exec();
}

void Manager::deleteNoteFromDB(int id) //
{
    QSqlQuery query;
    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
}

void Manager::saveContentToDB(int id, const QString &content) //
{
    QSqlQuery query;
    query.prepare("UPDATE notes SET content = :content WHERE id = :id");
    query.bindValue(":content", content);
    query.bindValue(":id", id);
    query.exec();
}

QSqlQuery Manager::get_note(int id) {
    QSqlQuery query;
    query.prepare("SELECT content FROM notes WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    return query;
}
