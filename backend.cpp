#include "backend.hpp"

#include <cstdlib>
#include <stdexcept>

#include <QByteArray>

#define LETS_DEBUG
#undef LETS_DEBUG

#ifdef LETS_DEBUG
#include <QDebug>
#endif

Backend::Backend(QObject *parent) : QObject(parent) {}

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
  if (value == first_saved_quote_) {
    return;
  }
  first_saved_quote_ = value; 
  emit firstSavedQuoteChanged(first_saved_quote_);
}

void Backend::set_last_saved_quote(const int& value) 
{ 
  if (value == last_saved_quote_) {
    return;
  }
  last_saved_quote_ = value; 
  emit lastSavedQuoteChanged(last_saved_quote_);
}

void Backend::init()
{
  openDatabase();
}

void Backend::saveQuote(
    QString week_number,
    QString text,
    QString author,
    QString author_description,
    QString text_description)
{
  if (Backend::isBlank(week_number)
      || Backend::isBlank(text)
      || Backend::isBlank(author)) {
    emit errorOccurred(
        "Required fields are blank.",
        "Please fill in Week #, Text, and Author.");
    return;
  }

  std::string query =
    "INSERT INTO quotes ("
    "week_number, "
    "text, "
    "author, "
    "author_description, "
    "text_description, "
    "date_time_submitted) "
    "VALUES (?, ?, ?, ?, ?, ?);";

  try {
    SQLite::Statement statement(*db_, query);
    statement.bind(1, week_number.toStdString());
    statement.bind(2, text.toStdString());
    statement.bind(3, author.toStdString());
    statement.bind(4, author_description.toStdString());
    statement.bind(5, text_description.toStdString());
    statement.bind(6, Backend::nowInUtc());
    statement.exec();
  } catch (const SQLite::Exception& e) {
    QString what = QString::fromLocal8Bit(e.what(), -1);
    emit errorOccurred("Cannot save quote.", what);
    return;
  } catch (const std::runtime_error& e) {
    QString what = QString::fromLocal8Bit(e.what(), -1);
    emit errorOccurred("Error occurred.", what);
    return;
  }

  emit quoteSaved();
  emit errorOccurred("Success!", "Saved successfully.");
}

void Backend::retrieveFirstLastSavedQuotes()
{
  std::string query = 
    "SELECT "
    "min(week_number) AS min, "
    "max(week_number) AS max "
    "FROM quotes;";

  try {
    SQLite::Statement statement(*db_, query);
    while (statement.executeStep()) {
      int min = statement.getColumn(0);
      int max = statement.getColumn(1);
      set_first_saved_quote(min);
      set_last_saved_quote(max);
    }
  } catch (const SQLite::Exception& e) {
    QString what = QString::fromLocal8Bit(e.what(), -1);
    emit errorOccurred("Cannot retrieve first/last quotes.", what);
    return;
  }
}

std::string Backend::nowInUtc()
{
  char buf[21] = {0};
  std::time_t now = std::time(0);
  std::tm *now_in_utc = std::gmtime(&now);
  if (std::strftime(buf, 42, "%FT%TZ", now_in_utc) == 0) {
    throw std::runtime_error("Cannot get current date time in UTC.");
  }
  return std::string(buf);
}

bool Backend::isBlank(const QString& str)
{
  return str.trimmed().isEmpty();
}

void Backend::openDatabase()
{
  std::string path = 
      std::string(std::getenv("HOME")) + relative_path_ + database_name_;

  try {
    db_ = std::make_unique<SQLite::Database>(path, SQLite::OPEN_READWRITE);
  } catch (const SQLite::Exception& e) {
    QString what = QString::fromLocal8Bit(e.what(), -1);
    emit errorOccurred("Cannot open database.", what);
    return;
  }
}

