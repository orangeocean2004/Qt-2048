#include "database.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QCoreApplication>

Database::Database(QObject *parent) : QObject(parent)
{
    initDatabase();
}

Database::~Database()
{
    if (db.isOpen())
    {
        db.close();
    }
}

Database &Database::instance()
{
    static Database instance;
    return instance;
}

bool Database::initDatabase()
{
    // 确保db连接名称唯一
    QString connectionName = "2048_game_connection";

    // 关闭已打开的连接
    if (db.isOpen())
    {
        db.close();
    }

    // 移除已存在的连接
    if (QSqlDatabase::contains(connectionName))
    {
        QSqlDatabase::removeDatabase(connectionName);
    }

    // 打印可用的驱动程序
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();

    // 使用大写的 "QSQLITE" 而非小写的 "qsqlite"
    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);

    // 确保连接是有效的
    if (!db.isValid())
    {
        qDebug() << "Invalid database connection";
        return false;
    }

    // 使用应用程序可写的目录
    QString appDirPath = QCoreApplication::applicationDirPath();
    QString dbFilePath = appDirPath + "/2048game.db";

    qDebug() << "Database file path:" << dbFilePath;
    db.setDatabaseName(dbFilePath);

    if (!db.open())
    {
        qDebug() << "Cannot connect to database:" << db.lastError().text();
        return false;
    }

    return createTables();
}

bool Database::createTables()
{
    // 使用指定的数据库连接创建查询
    QSqlQuery query(db);

    /* 创建用户表 */
    bool success = query.exec("CREATE TABLE IF NOT EXISTS users ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "username TEXT UNIQUE NOT NULL, "
                              "password TEXT NOT NULL, "
                              "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
    if (!success)
    {
        qDebug() << "Error creating users table: " << query.lastError().text();
        qDebug() << "Is db open:" << db.isOpen() << "Is valid:" << db.isValid();
        return false;
    }

    /* 创建分数表 */
    success = query.exec("CREATE TABLE IF NOT EXISTS scores ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "user_id INTEGER NOT NULL, "
                         "score INTEGER NOT NULL, "
                         "date_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                         "FOREIGN KEY(user_id) REFERENCES users(id))");

    if (!success)
    {
        qDebug() << "Error creating scores table: " << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::registerUser(const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty())
    {
        return false;
    }

    /* 加密通话 */
    QByteArray passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    // 使用已有的db连接创建查询，而不是使用默认连接
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", QString(passwordHash));

    return query.exec();
}

bool Database::loginUser(const QString &username, const QString &password)
{
    // 加密密码
    QByteArray passwordHash = QCryptographicHash::hash(
                                  password.toUtf8(), QCryptographicHash::Sha256)
                                  .toHex();

    QSqlQuery query(db);
    query.prepare("SELECT id FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", QString(passwordHash));

    if (query.exec() && query.next())
    {
        return true;
    }

    return false;
}

//// filepath: d:\Code\Qt\2048\src\database.cpp
bool Database::updateUserScore(const QString &username, int score)
{
    // 先获取用户 ID
    QSqlQuery userQuery(db);
    userQuery.prepare("SELECT id FROM users WHERE username = :username");
    userQuery.bindValue(":username", username);

    if (!userQuery.exec() || !userQuery.next())
    {
        qDebug() << "Failed to find user:" << userQuery.lastError().text();
        return false;
    }

    int userId = userQuery.value(0).toInt();

    // 插入新的分数记录
    QSqlQuery scoreQuery(db);
    scoreQuery.prepare(
        "INSERT INTO scores (user_id, score) "
        "VALUES (:userId, :score)");
    scoreQuery.bindValue(":userId", userId);
    scoreQuery.bindValue(":score", score);

    if (!scoreQuery.exec())
    {
        qDebug() << "Failed to insert score:" << scoreQuery.lastError().text();
        return false;
    }

    // 删除超过前 30 的记录
    QSqlQuery cleanupQuery(db);
    cleanupQuery.prepare(
        "DELETE FROM scores "
        "WHERE id NOT IN ("
        "   SELECT id "
        "   FROM scores "
        "   ORDER BY score DESC "
        "   LIMIT 30)");
    if (!cleanupQuery.exec())
    {
        qDebug() << "Failed to cleanup old scores:" << cleanupQuery.lastError().text();
        // 不强制返回 false，插入成功依然返回 true
    }

    return true;
}

QList<QPair<QString, int>> Database::getTopScores(int limit)
{
    QList<QPair<QString, int>> scores;
    QSqlQuery query(db);

    query.prepare(
        "SELECT u.username, s.score "
        "FROM users u "
        "JOIN scores s ON u.id = s.user_id "
        "ORDER BY s.score DESC "
        "LIMIT :limit");
    query.bindValue(":limit", limit);

    if (!query.exec())
    {
        qDebug() << "Failed to get top scores:" << query.lastError().text();
        return scores;
    }

    while (query.next())
    {
        QString username = query.value(0).toString();
        int score = query.value(1).toInt();
        scores.append(qMakePair(username, score));
    }

    return scores;
}

int Database::getUserHighScore(const QString &username)
{
    QSqlQuery query(db); // 添加db参数
    query.prepare("SELECT MAX(s.score) "
                  "FROM scores s "
                  "INNER JOIN users u ON s.user_id = u.id "
                  "WHERE u.username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next())
    {
        return query.value(0).toInt();
    }

    return 0;
}

bool Database::resetUserPassword(const QString &username, const QString &newPassword)
{
    if (username.isEmpty() || newPassword.isEmpty())
    {
        return false;
    }

    /* 加密新密码 */
    QByteArray passwordHash = QCryptographicHash::hash(
                                  newPassword.toUtf8(), QCryptographicHash::Sha256)
                                  .toHex();

    QSqlQuery query(db); // 添加db参数
    query.prepare("UPDATE users SET password = :password WHERE username = :username");
    query.bindValue(":password", QString(passwordHash));
    query.bindValue(":username", username);

    return query.exec();
}

bool Database::changeUserPassword(const QString &username, const QString &oldPassword, const QString &newPassword)
{
    if (username.isEmpty() || oldPassword.isEmpty() || newPassword.isEmpty())
    {
        return false;
    }

    /* 验证旧密码 */
    if (!loginUser(username, oldPassword))
    {
        return false;
    }

    /* 更新密码 */
    return resetUserPassword(username, newPassword);
}