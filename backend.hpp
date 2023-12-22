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
    // Possible problem: different order on the web.
    Q_PROPERTY(int first_saved_quote READ first_saved_quote WRITE set_first_saved_quote NOTIFY firstSavedQuoteChanged)
    Q_PROPERTY(int last_saved_quote READ last_saved_quote WRITE set_last_saved_quote NOTIFY lastSavedQuoteChanged)


public:
    /*
     * @throws std::runtime_error
     */
    explicit Backend(QObject *parent = nullptr);

    /*
     * @throws std::runtime_error
     */
    ~Backend();

    // Getters
    int first_saved_quote();
    int last_saved_quote();

    // Setters
    void set_first_saved_quote(const int& value);
    void set_last_saved_quote(const int& value);

    /*
     * @throws std::runtime_error
     */
    Q_INVOKABLE void saveQuote(
        QString week_number,
        QString text,
        QString author,
        QString author_description,
        QString text_description);

    Q_INVOKABLE void retrieveFirstLastQuotes();

signals:
    void firstSavedQuoteChanged(int value);
    void lastSavedQuoteChanged(int value);
    void quoteSaved(int week_number);

private:
    sqlite3 *database_;

    int first_saved_quote_ = 0;
    int last_saved_quote_ = 0;
    std::string database_name_ = "quotes.db";
    std::string path_ = "/Users/mehrshadkh./Documents/Mahya/";


    /*
     * @throws std::runtime_error
     */
    void initDatabase();
    /*
     * @throws std::runtime_error
     */
    void openDatabase();
    /*
     * @throws std::runtime_error
     */
    void closeDatabase();

    static int firstLastQuotesCallback(
        void *ptr, 
        int column_count, 
        char **row_data, 
        char **column_names);
};

#endif // BACKEND_HPP
