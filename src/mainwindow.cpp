#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFont>
#include <QTableWidget>
#include <QHeaderView>
#include <QDialogButtonBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameBoard(nullptr), currentUser("")
{
    /* 窗口大小 */
    setWindowTitle("2048 Game");
    resize(600, 600);

    /* 初始化数据库 */
    if (!Database::instance().initDatabase())
    {
        QMessageBox::critical(this, "Database Error", "Failed to initialize database. The application will now close.");
        QTimer::singleShot(0, qApp, &QApplication::quit);
        return;
    }

    /* 中央堆叠部件 */
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    /* 主菜单页面 */
    menuWidget = new QWidget();
    setupMenuWidget();

    /* 登录页面 */
    setupLoginWidget();

    stackedWidget->addWidget(loginWidget);
    stackedWidget->addWidget(menuWidget);
    stackedWidget->setCurrentWidget(loginWidget);

    setStyleSheet("background-color: #faf8ef;");
}

MainWindow::~MainWindow()
{
    if (gameBoard)
    {
        delete gameBoard;
    }
}

void MainWindow::setupMenuWidget()
{
    centralWidget = menuWidget;

    /* 创建主布局 */
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setAlignment(Qt::AlignCenter);

    titleLabel = new QLabel("2048", centralWidget);
    QFont titleFont("Arial", 48, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #776e65;");

    startGameButton = new QPushButton("start!", centralWidget);
    leaderboardButton = new QPushButton("ranks.", centralWidget);
    changePasswordButton = new QPushButton("change password", centralWidget);
    logoutButton = new QPushButton("logout", centralWidget);
    quitButton = new QPushButton("quit?", centralWidget);

    QString buttonStyle = "QPushButton {"
                          "background-color: #8f7a66;"
                          "color: white;"
                          "font-weight: bold;"
                          "border-radius: 10px;"
                          "padding: 15px;"
                          "font-size: 18px;"
                          "min-width: 200px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #9f8a76;"
                          "}";

    startGameButton->setStyleSheet(buttonStyle);
    leaderboardButton->setStyleSheet(buttonStyle);
    changePasswordButton->setStyleSheet(buttonStyle);
    logoutButton->setStyleSheet(buttonStyle);
    quitButton->setStyleSheet(buttonStyle);

    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(leaderboardButton, &QPushButton::clicked, this, &MainWindow::showLeaderboard);
    connect(changePasswordButton, &QPushButton::clicked, this, &MainWindow::showChangePasswordDialog);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::logoutUser);
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::quitGame);

    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(40);
    mainLayout->addWidget(startGameButton);
    mainLayout->addWidget(leaderboardButton);
    mainLayout->addWidget(changePasswordButton);
    mainLayout->addWidget(logoutButton);
    mainLayout->addWidget(quitButton);
    mainLayout->addStretch();
}

void MainWindow::setupLoginWidget()
{
    loginWidget = new Login();

    // 修改处：直接调用 onLoginSuccess，以便更新 currentUser
    connect(loginWidget, &Login::loginSuccess, this,
            [this](const QString &username)
            {
                onLoginSuccess(username);
            });

    stackedWidget->addWidget(loginWidget);
}

void MainWindow::startGame()
{

    if (!gameBoard)
    {
        gameBoard = new GameBoard(currentUser, this); // 传递当前用户

        connect(gameBoard, &GameBoard::returnToMainMenu, this,
                [this]()
                { stackedWidget->setCurrentWidget(menuWidget); });

        stackedWidget->addWidget(gameBoard);
    }

    gameBoard->resetGame();
    stackedWidget->setCurrentWidget(gameBoard);
}

void MainWindow::showLeaderboard()
{
    auto scores = Database::instance().getTopScores();

    QDialog dialog(this);
    dialog.setWindowTitle(tr("ranks"));
    dialog.resize(350, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *titleLabel = new QLabel(tr("2048 ranks"), &dialog);
    QFont titleFont("Arial", 16, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    QTableWidget *table = new QTableWidget(&dialog);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({tr("rank"), tr("player"), tr("score")});
    table->setRowCount(scores.size());
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i = 0; i < scores.size(); ++i)
    {
        QTableWidgetItem *rankItem = new QTableWidgetItem(QString::number(i + 1));
        rankItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 0, rankItem);

        QTableWidgetItem *nameItem = new QTableWidgetItem(scores[i].first);
        nameItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 1, nameItem);

        QTableWidgetItem *scoreItem = new QTableWidgetItem(QString::number(scores[i].second));
        scoreItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 2, scoreItem);
    }

    QPushButton *closeButton = new QPushButton(tr("close"), &dialog);
    closeButton->setStyleSheet("background-color: #8f7a66; color: white; padding: 8px; border-radius: 5px;");
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    layout->addWidget(titleLabel);
    layout->addWidget(table);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    dialog.setStyleSheet("background-color: #faf8ef;");
    dialog.exec();
}

void MainWindow::quitGame()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Exit Game");
    msgBox.setText("Are you sure you want to quit the game?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #faf8ef;"
        "    color: #776e65;"
        "}"
        "QMessageBox QLabel {"
        "    color: #776e65;"
        "    font-size: 14px;"
        "}"
        "QPushButton {"
        "    background-color: #8f7a66;"
        "    color: white;"
        "    border-radius: 5px;"
        "    padding: 8px 16px;"
        "    font-weight: bold;"
        "    min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #9f8a76;"
        "}");

    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}

void MainWindow::userLogin()
{
    stackedWidget->setCurrentWidget(loginWidget);
}

void MainWindow::onLoginSuccess(const QString &username)
{
    currentUser = username; 

    if (!titleLabel->text().contains(username))
    {
        titleLabel->setText(QString("2048\nWelcome, %1").arg(username));
    }

    stackedWidget->setCurrentWidget(menuWidget);

    QMessageBox::information(this, "Welcome", QString("Welcome back, %1!").arg(username));
}

void MainWindow::logoutUser()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Logout Confirmation");
    msgBox.setText("Are you sure you want to logout?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #faf8ef;"
        "    color: #776e65;"
        "}"
        "QMessageBox QLabel {"
        "    color: #776e65;"
        "    font-size: 14px;"
        "}"
        "QPushButton {"
        "    background-color: #8f7a66;"
        "    color: white;"
        "    border-radius: 5px;"
        "    padding: 8px 16px;"
        "    font-weight: bold;"
        "    min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #9f8a76;"
        "}");

    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        currentUser = "";

        titleLabel->setText("2048");

        stackedWidget->setCurrentWidget(loginWidget);

        QMessageBox successMsg;
        successMsg.setWindowTitle("Logged Out");
        successMsg.setText("You have been logged out successfully.");
        successMsg.setStyleSheet(msgBox.styleSheet());
        successMsg.exec();
    }
}

void MainWindow::showChangePasswordDialog()
{
    if (currentUser.isEmpty())
    {
        QMessageBox msgBox;
        setupMessageBoxStyle(msgBox);
        msgBox.setWindowTitle("Login Required");
        msgBox.setText("Please login first.");
        msgBox.exec();
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Change Password");
    dialog.setFixedSize(400, 300);
    dialog.setStyleSheet("background-color: #faf8ef;");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setSpacing(15);

    QLabel *titleLabel = new QLabel("Change Your Password", &dialog);
    QFont titleFont("Arial", 16, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #776e65;");

    QLabel *usernameLabel = new QLabel("Username:", &dialog);
    usernameLabel->setStyleSheet("color: #776e65;");
    QLabel *usernameValue = new QLabel(currentUser, &dialog);
    usernameValue->setStyleSheet("font-weight: bold; color: #776e65;");

    QLabel *oldPassLabel = new QLabel("Current Password:", &dialog);
    oldPassLabel->setStyleSheet("color: #776e65;");
    QLineEdit *oldPassInput = new QLineEdit(&dialog);
    oldPassInput->setEchoMode(QLineEdit::Password);

    QLabel *newPassLabel = new QLabel("New Password:", &dialog);
    newPassLabel->setStyleSheet("color: #776e65;");
    QLineEdit *newPassInput = new QLineEdit(&dialog);
    newPassInput->setEchoMode(QLineEdit::Password);

    QLabel *confirmPassLabel = new QLabel("Confirm New Password:", &dialog);
    confirmPassLabel->setStyleSheet("color: #776e65;");
    QLineEdit *confirmPassInput = new QLineEdit(&dialog);
    confirmPassInput->setEchoMode(QLineEdit::Password);

    QLabel *statusLabel = new QLabel(&dialog);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: red;");

    QPushButton *changeButton = new QPushButton("Change Password", &dialog);
    QPushButton *cancelButton = new QPushButton("Cancel", &dialog);

    changeButton->setStyleSheet(
        "QPushButton {"
        "background-color: #8f7a66;"
        "color: white;"
        "border-radius: 5px;"
        "padding: 8px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #9f8a76;"
        "}");

    cancelButton->setStyleSheet(
        "QPushButton {"
        "background-color: #bbada0;"
        "color: white;"
        "border-radius: 5px;"
        "padding: 8px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #cbbfb5;"
        "}");

    layout->addWidget(titleLabel);
    layout->addSpacing(10);

    QHBoxLayout *userLayout = new QHBoxLayout();
    userLayout->addWidget(usernameLabel);
    userLayout->addWidget(usernameValue);
    layout->addLayout(userLayout);

    layout->addWidget(oldPassLabel);
    layout->addWidget(oldPassInput);
    layout->addWidget(newPassLabel);
    layout->addWidget(newPassInput);
    layout->addWidget(confirmPassLabel);
    layout->addWidget(confirmPassInput);
    layout->addWidget(statusLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(changeButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(changeButton, &QPushButton::clicked, [=, &dialog]()
            {
        QString oldPass = oldPassInput->text();
        QString newPass = newPassInput->text();
        QString confirmPass = confirmPassInput->text();

        if (oldPass.isEmpty() || newPass.isEmpty() || confirmPass.isEmpty()) {
            statusLabel->setText("All fields must be filled");
            return;
        }
        
        if (newPass != confirmPass) {
            statusLabel->setText("New passwords do not match");
            return;
        }
        
        if (Database::instance().changeUserPassword(currentUser, oldPass, newPass)) {
            QMessageBox msgBox;
            setupMessageBoxStyle(msgBox);
            msgBox.setWindowTitle("Success");
            msgBox.setText("Your password has been changed successfully.");
            msgBox.exec();
            dialog.accept();
        } else {
            statusLabel->setText("Current password is incorrect");
        } });

    dialog.exec();
}

void MainWindow::setupMessageBoxStyle(QMessageBox &msgBox)
{
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #faf8ef;"
        "    color: #776e65;"
        "}"
        "QMessageBox QLabel {"
        "    color: #776e65;"
        "    font-size: 14px;"
        "}"
        "QPushButton {"
        "    background-color: #8f7a66;"
        "    color: white;"
        "    border-radius: 5px;"
        "    padding: 8px 16px;"
        "    font-weight: bold;"
        "    min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #9f8a76;"
        "}");
}