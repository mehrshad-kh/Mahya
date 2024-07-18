#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <string>

#include <sqlite3.h>

#include <QObject>
#include <QString>

#include <QtQml>

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Backend(QObject *parent = nullptr);

    ~Backend();

    int first_saved_quote();
    int last_saved_quote();

    void set_first_saved_quote(const int& value);
    void set_last_saved_quote(const int& value);

    Q_INVOKABLE void saveQuote(
        QString week_number,
        QString text,
        QString author,
        QString author_description,
        QString text_description);

    Q_INVOKABLE void retrieveFirstLastQuotes();

signals:
    void errorOccurred(QString error_message);
    void firstSavedQuoteChanged(int value);
    void lastSavedQuoteChanged(int value);
    void quoteSaved();

private:
    sqlite3 *database_;

    int first_saved_quote_ = 0;
    int last_saved_quote_ = 0;
    std::string database_name_ = "quotes.db";
    std::string path_ = "/Users/mehrshadkh./Documents/db/Mahya/";


    void initDatabase();
    void openDatabase();
    void closeDatabase();

    static int firstLastQuotesCallback(
        void *ptr, 
        int column_count, 
        char **row_data, 
        char **column_names);
};

#endif // BACKEND_HPP
