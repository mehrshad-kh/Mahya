#include "backend.hpp"

#include <stdexcept>

#include <QByteArray>

#define MY_DEBUG

#ifdef MY_DEBUG
#include <QDebug>
#endif

Backend::Backend(QObject *parent) : QObject(parent)
{
    openDatabase();
    initDatabase();
}

Backend::~Backend()
{
    closeDatabase();
}

void Backend::saveQuote(
    QString week_number,
    QString text,
    QString author,
    QString author_description,
    QString text_description)
{
    int rc = 0;
    int step = 0;
    char *sql_error = nullptr;

    sqlite3_stmt *statement = nullptr;

    QByteArray qba_text = text.toLocal8Bit();
    const char *str_text = qba_text.constData();
    QByteArray qba_author = author.toLocal8Bit();
    const char *str_author = qba_author.constData();
    QByteArray qba_author_description = author_description.toLocal8Bit();
    const char *str_author_description = qba_author_description.constData();
    QByteArray qba_text_description = text_description.toLocal8Bit();
    const char *str_text_description = qba_text_description.constData();

    QString query =
        "INSERT INTO quotes ("
        "week_number, "
        "text, "
        "author, "
        "author_description, "
        "text_description) "
        "VALUES (?, ?, ?, ?, ?);";

    QByteArray qba_query = query.toLocal8Bit();
    const char *str_query = qba_query.constData();

    rc = sqlite3_prepare_v2(database, str_query, -1, &statement, nullptr);

    if (rc == SQLITE_OK) {
        sqlite3_bind_int(statement, 1, week_number.toInt());
        // SQLITE_STATIC indicates that the application remains
        // responsible for disposing of the third argument.
        sqlite3_bind_text(statement, 2, str_text, -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 3, str_author, -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 4, str_author_description, -1, SQLITE_STATIC);
        sqlite3_bind_text(statement, 5, str_text_description, -1, SQLITE_STATIC);
    }

    step = sqlite3_step(statement);
    if (step == SQLITE_DONE) {
        qInfo() << "Step done.";
    } else {
        qInfo() << "Some other return code: " << step;
    }

    sqlite3_finalize(statement);

    emit quoteSaved(week_number.toInt());
    return;
}

void Backend::initDatabase()
{
    int rc = 0;
    char *sql_error = nullptr;

    std::string query =
        "CREATE TABLE IF NOT EXISTS quotes ("
        "id INTEGER PRIMRAY KEY, "
        "week_number INTEGER, "
        "text TEXT, "
        "author TEXT, "
        "author_description TEXT, "
        "text_description TEXT"
        ");";

    rc = sqlite3_exec(database, query.c_str(), nullptr, nullptr, &sql_error);
    // SQLITE_OK is 0.
    if (rc != SQLITE_OK) {
        std::string message = "Cannot create table: "
            + std::string(sql_error);
        sqlite3_free(sql_error);
        throw std::runtime_error(message);
    }
}

void Backend::openDatabase()
{
    int rc = 0;

    std::string filename = path + database_name;

    rc = sqlite3_open(filename.c_str(), &database);

    if (rc != SQLITE_OK) {
        std::string message = "Can't open database: " 
            + std::string(sqlite3_errmsg(database));
        sqlite3_close(database);
        throw std::runtime_error(message);
    }
}

void Backend::closeDatabase()
{
    int rc = 0;

    rc = sqlite3_close(database);

    if (rc == SQLITE_BUSY) {
        throw std::runtime_error("Cannot close database: it is busy.");
    } else if (rc != SQLITE_OK) {
        throw std::runtime_error("Cannot close database.");
    }
}
