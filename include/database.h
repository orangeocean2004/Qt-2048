#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

class Database : public QObject
{
    Q_OBJECT
public:
    static Database &instance();

    /* 账户管理 */
    bool initDatabase();
    bool registerUser(const QString &username, const QString &password);
    bool loginUser(const QString &username,const QString &password);

    /* 分数记录 */
    bool updateUserScore(const QString &username, int score);
    QList<QPair<QString, int>> getTopScores(int limit = 10);
    int getUserHighScore(const QString &username);

    bool resetUserPassword(const QString &username, const QString &newPassword);
    bool changeUserPassword(const QString &username,const QString &oldPassword, const QString &newPassword);

private:
    Database(QObject *parent = nullptr);
    ~Database();

    QSqlDatabase db;
    bool createTables();
};

#endif // DATABASE_H
