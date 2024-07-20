#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <ctime>
#include <memory>
#include <string>

#include <SQLiteCpp/SQLiteCpp.h>

#include <QObject>
#include <QString>

#include <QtQml>

class Backend : public QObject
{
  Q_OBJECT
  QML_ELEMENT

public:
  explicit Backend(QObject *parent = nullptr);

  int first_saved_quote();
  int last_saved_quote();

  void set_first_saved_quote(const int& value);
  void set_last_saved_quote(const int& value);

  Q_INVOKABLE void init();
  Q_INVOKABLE void saveQuote(
      QString week_number,
      QString text,
      QString author,
      QString author_description,
      QString text_description);
  Q_INVOKABLE void retrieveFirstLastSavedQuotes();

signals:
  void errorOccurred(QString text, QString informative_text);
  void firstSavedQuoteChanged(int value);
  void lastSavedQuoteChanged(int value);
  void quoteSaved();

private:
  std::unique_ptr<SQLite::Database> db_;

  int first_saved_quote_ = 0;
  int last_saved_quote_ = 0;
  std::string database_name_ = "database.db";
  std::string relative_path_ = "/Documents/Databases/mahya/";

  void openDatabase();
};

#endif // BACKEND_HPP
