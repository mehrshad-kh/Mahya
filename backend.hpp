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
    /*
     * @throws std::runtime_error
     */
    explicit Backend(QObject *parent = nullptr);

    /*
     * @throws std::runtime_error
     */
    ~Backend();

    /*
     * @throws std::runtime_error
     */
    Q_INVOKABLE void saveQuote(
        QString week_number,
        QString text,
        QString author,
        QString author_description,
        QString text_description);

signals:
    void quoteSaved(int week_number);

private:
    sqlite3 *database;

    std::string database_name = "quotes.db";
    std::string path = "/Users/mehrshadkh./Documents/Mahya/";

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
};

#endif // BACKEND_HPP
