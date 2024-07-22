#include "backend.hpp"

#include "utility.hpp"

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
  if (Utility::isBlank(week_number)
      || Utility::isBlank(text)
      || Utility::isBlank(author)) {
    emit errorOccurred(
        "Required fields are blank.",
        "Please fill in Week #, Text, and Author.");
    return;
  }

  if (!Utility::isNumber(week_number)) {
    emit errorOccurred(
        "Invalid Week #", 
        "Please enter a valid number.");
    emit weekNumberFocused();
    return;
  }

  std::string query = 
    "SELECT id "
    "FROM quotes "
    "WHERE week_number = ?;";

  try {
    SQLite::Statement statement(*db_, query);
    statement.bind(1, week_number.toInt());
    statement.executeStep();
    if (statement.hasRow()) {
      emit errorOccurred(
          "Repetitive Week #",
          "Week #" + week_number + " already exists.");
      return;
    }
  } catch (const SQLite::Exception& e) {
    QString what = QString::fromLocal8Bit(e.what(), -1);
    emit errorOccurred("Internal Error", what);
    return; 
  }

  query =
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
    statement.bind(1, week_number.toInt());
    statement.bind(2, text.toStdString());
    statement.bind(3, author.toStdString());
    statement.bind(4, author_description.toStdString());
    statement.bind(5, text_description.toStdString());
    statement.bind(6, Utility::nowInUtc());
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
  emit errorOccurred("Success!", "Quote saved successfully.");
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

