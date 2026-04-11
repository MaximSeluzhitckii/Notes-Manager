#ifndef MANAGER_H
#define MANAGER_H

#include <QtSql>
#include <QSqlQuery>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Manager; }
QT_END_NAMESPACE

class Manager
{
public:
    Manager();

    void loadNotes(QListWidget* listWidget, const QString &search = "");
    void addNoteToDB(const QString &title);
    void deleteNoteFromDB(int id);
    void saveContentToDB(int id, const QString &content);
    QSqlQuery get_note(int id);

private:
    void setupDatabase();
    QSqlDatabase db;
};

#endif // MANAGER_H
