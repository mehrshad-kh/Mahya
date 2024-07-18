#include "backend.hpp"

#include <cstdlib>
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

int Backend::first_saved_quote() 
{ 
  return first_saved_quote_;
}

int Backend::last_saved_quote() 
{ 
  return last_saved_quote_; 
}

void Backend::set_first_saved_quote(const int& value) 
{ 
  if (value == first_saved_quote_)
    return;

  first_saved_quote_ = value; 

  emit firstSavedQuoteChanged(first_saved_quote_);
}

void Backend::set_last_saved_quote(const int& value) 
{ 
  if (value == last_saved_quote_)
    return;

  last_saved_quote_ = value; 

  emit lastSavedQuoteChanged(last_saved_quote_);
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

  // TODO: Check blankness instead of emptiness.
  if (week_number.isEmpty() || text.isEmpty() || author.isEmpty()) {
    emit errorOccurred(
        "Required fields are empty.",
        "Please fill in Week #, Text, and Author.");
    return;
  }

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

  rc = sqlite3_prepare_v2(database_, str_query, -1, &statement, nullptr);
  if (!rc) {
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
    sqlite3_finalize(statement);
  } else {
    qInfo() << "Some other return code: " << step;
    emit errorOccurred(
        "Cannot save quote.", 
        "sqlite3_step return code: " + QString::number(step));

    sqlite3_finalize(statement);
    return;
  }

  emit quoteSaved();
}

void Backend::initDatabase()
{
  int rc = 0;

  std::string query =
    "CREATE TABLE IF NOT EXISTS quotes ("
    "id INTEGER PRIMARY KEY, "
    "week_number INTEGER, "
    "text TEXT, "
    "author TEXT, "
    "author_description TEXT, "
    "text_description TEXT);";

  rc = sqlite3_exec(database_, query.c_str(), nullptr, nullptr, nullptr);
  if (rc != SQLITE_OK) {
    emit errorOccurred("Cannot create table.", "");
    return;
  }
}

void Backend::openDatabase()
{
  int rc = 0;

  std::string filename = path_ + database_name_;

  rc = sqlite3_open(filename.c_str(), &database_);

  if (rc != SQLITE_OK) {
    qInfo() << "openDatabase(),148: Can't open database.";
    emit errorOccurred(
        "Can't open database.", 
        QString::fromStdString(std::string(sqlite3_errmsg(database_))));
    sqlite3_close(database_);
    return;
  }
}

void Backend::closeDatabase()
{
  int rc = 0;

  rc = sqlite3_close(database_);

  if (rc == SQLITE_BUSY) {
    emit errorOccurred("Cannot close database.", "It is busy.");
  } else if (rc != SQLITE_OK) {
    emit errorOccurred("Cannot close database.", "Unkown reason.");
  }
}

void Backend::retrieveFirstLastQuotes()
{
  int rc = 0;

  QString query = 
    "SELECT "
    "min(week_number) AS min, "
    "max(week_number) AS max "
    "FROM quotes;";
  QByteArray qba_query = query.toLocal8Bit();
  const char *str_query = qba_query.constData();

  sqlite3_exec(
      database_, 
      str_query, 
      &firstLastQuotesCallback, 
      this, 
      nullptr);
  if (rc != SQLITE_OK) {
    emit errorOccurred("Cannot retrieve first and last quotes.", "");
  }
}

int Backend::firstLastQuotesCallback(
    void *ptr, 
    int column_count, 
    char **row_data, 
    char **column_names)
{
  Backend *backend = static_cast<Backend *>(ptr);

  backend->set_first_saved_quote(static_cast<int>(std::strtol(*row_data, nullptr, 10)));
  backend->set_last_saved_quote(static_cast<int>(std::strtol(*(row_data + 1), nullptr, 10)));

  return 0;
}
