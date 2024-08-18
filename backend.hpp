#ifndef BACKEND_HPP
#define BACKEND_HPP

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

  /**
   * @brief Initialize the Backend object.
   *
   * Must be called after each object creation.
   * Otherwise, exceptions will be thrown at each function call.
   */
  Q_INVOKABLE void init();
  Q_INVOKABLE void saveQuote(
      QString week_number,
      QString text,
      QString author,
      QString author_description,
      QString text_description,
      QString date_intended);
  Q_INVOKABLE void retrieveFirstLastSavedQuotes();

signals:
  void errorOccurred(QString text, QString informative_text);
  void firstSavedQuoteChanged(int value);
  void lastSavedQuoteChanged(int value);
  void quoteSaved();
  void weekNumberFocused();

private:
  std::unique_ptr<SQLite::Database> db_;

  int first_saved_quote_ = 0;
  int last_saved_quote_ = 0;
  std::string database_name_ = "database.db";
  std::string relative_path_ = "Documents/Databases/mahya";

  void openDatabase();
};

#endif // BACKEND_HPP
