#include "login.h"
#include "database.h"
#include <QMessageBox>
#include <QApplication>
#include <QKeyEvent>

Login::Login(QWidget *parent) : QWidget(parent)
{
    setupUI();

    /* 登陆界面的信号与槽 */
    connect(loginButton, &QPushButton::clicked, this, &Login::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &Login::showRegisterPage);
    connect(forgotPasswordButton, &QPushButton::clicked, this, &Login::showForgotPasswordPage);
    connect(backButton, &QPushButton::clicked, this, &Login::onBackClicked);

    /* 注册界面的信号与槽 */
    connect(createAccountButton, &QPushButton::clicked, this, &Login::onRegisterClicked);
    connect(backToLoginButton, &QPushButton::clicked, this, &Login::showLoginPage);

    /* 忘记密码界面的信号与槽 */
    connect(resetPasswordButton, &QPushButton::clicked, this, &Login::resetPassword);
    connect(backToLoginFromForgotButton, &QPushButton::clicked, this, &Login::showLoginPage);

    /* 修改密码界面的信号与槽 */
    connect(updatePasswordButton, &QPushButton::clicked, this, &Login::changePassword);
    connect(backToLoginFromChangeButton, &QPushButton::clicked, this, &Login::showLoginPage);

    usernameInput->installEventFilter(this);
    passwordInput->installEventFilter(this);
}

void Login::setupUI()
{
    setWindowTitle(tr("Login"));

    stackedWidget = new QStackedWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);

    setupLoginPage();
    setupRegisterPage();
    setupForgotPasswordPage();
    setupChangePasswordPage();

    stackedWidget->setCurrentWidget(loginPage);

    setStyleSheet("background-color: #faf8ef;");
    setMinimumSize(400, 500);
}

void Login::setupLoginPage()
{
    loginPage = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout(loginPage);
    layout->setSpacing(15);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel(tr("2048 login"), loginPage);
    QFont titleFont("Arial", 18, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #776e65;");

    usernameInput = new QLineEdit(loginPage);
    usernameInput->setPlaceholderText(tr("usrname"));
    usernameInput->setMinimumWidth(250);

    passwordInput = new QLineEdit(loginPage);
    passwordInput->setPlaceholderText(tr("password"));
    passwordInput->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton(tr("login"), loginPage);
    registerButton = new QPushButton(tr("register"), loginPage);
    forgotPasswordButton = new QPushButton(tr("forget your password?"), loginPage);
    backButton = new QPushButton(tr("Exit"), loginPage);

    statusLabel = new QLabel(loginPage);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: red;");

    /* 按钮样式 */
    loginButton->setStyleSheet(getButtonStyle());
    registerButton->setStyleSheet(getButtonStyle());
    forgotPasswordButton->setStyleSheet("color: #8f7a66; background: none; border: none;");
    backButton->setStyleSheet(getButtonStyle(false));

    /* 添加到布局 */
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(statusLabel);
    layout->addSpacing(10);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);

    layout->addLayout(buttonLayout);
    layout->addWidget(forgotPasswordButton, 0, Qt::AlignCenter);
    layout->addWidget(backButton);
    layout->addStretch();

    stackedWidget->addWidget(loginPage);
}

void Login::setupRegisterPage()
{
    registerPage = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout(registerPage);
    layout->setSpacing(15);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel(tr("create a new account!"), registerPage);
    QFont titleFont("Arial", 18, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #776e65;");

    regUsernameInput = new QLineEdit(registerPage);
    regUsernameInput->setPlaceholderText(tr("usrname"));
    regUsernameInput->setMinimumWidth(250);

    regPasswordInput = new QLineEdit(registerPage);
    regPasswordInput->setPlaceholderText(tr("password"));
    regPasswordInput->setEchoMode(QLineEdit::Password);

    regConfirmPasswordInput = new QLineEdit(registerPage);
    regConfirmPasswordInput->setPlaceholderText(tr("confirm your password"));
    regConfirmPasswordInput->setEchoMode(QLineEdit::Password);

    createAccountButton = new QPushButton(tr("create an account"), registerPage);
    backToLoginButton = new QPushButton(tr("back to login"), registerPage);

    regStatusLabel = new QLabel(registerPage);
    regStatusLabel->setAlignment(Qt::AlignCenter);
    regStatusLabel->setStyleSheet("color: red;");

    createAccountButton->setStyleSheet(getButtonStyle());
    backToLoginButton->setStyleSheet(getButtonStyle(false));

    regUsernameInput->installEventFilter(this);
    regPasswordInput->installEventFilter(this);
    regConfirmPasswordInput->installEventFilter(this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(regUsernameInput);
    layout->addWidget(regPasswordInput);
    layout->addWidget(regConfirmPasswordInput);
    layout->addWidget(regStatusLabel);
    layout->addSpacing(10);

    layout->addWidget(createAccountButton);
    layout->addWidget(backToLoginButton);
    layout->addStretch();

    stackedWidget->addWidget(registerPage);
}

void Login::setupForgotPasswordPage()
{
    forgotPasswordPage = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout(forgotPasswordPage);
    layout->setSpacing(15);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel(tr("reset your password"), forgotPasswordPage);
    QFont titleFont("Arial", 18, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #776e65;");

    fpUsernameInput = new QLineEdit(forgotPasswordPage);
    fpUsernameInput->setPlaceholderText(tr("usrname"));
    fpUsernameInput->setMinimumWidth(250);

    fpNewPasswordInput = new QLineEdit(forgotPasswordPage);
    fpNewPasswordInput->setPlaceholderText(tr("new password"));
    fpNewPasswordInput->setEchoMode(QLineEdit::Password);

    fpConfirmPasswordInput = new QLineEdit(forgotPasswordPage);
    fpConfirmPasswordInput->setPlaceholderText(tr("confirm your new password"));
    fpConfirmPasswordInput->setEchoMode(QLineEdit::Password);

    resetPasswordButton = new QPushButton(tr("reset password"), forgotPasswordPage);
    backToLoginFromForgotButton = new QPushButton(tr("back to login"), forgotPasswordPage);

    fpStatusLabel = new QLabel(forgotPasswordPage);
    fpStatusLabel->setAlignment(Qt::AlignCenter);
    fpStatusLabel->setStyleSheet("color: red;");

    resetPasswordButton->setStyleSheet(getButtonStyle());
    backToLoginFromForgotButton->setStyleSheet(getButtonStyle(false));
    fpUsernameInput->installEventFilter(this);
    fpNewPasswordInput->installEventFilter(this);
    fpConfirmPasswordInput->installEventFilter(this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(fpUsernameInput);
    layout->addWidget(fpNewPasswordInput);
    layout->addWidget(fpConfirmPasswordInput);
    layout->addWidget(fpStatusLabel);
    layout->addSpacing(10);

    layout->addWidget(resetPasswordButton);
    layout->addWidget(backToLoginFromForgotButton);
    layout->addStretch();

    stackedWidget->addWidget(forgotPasswordPage);
}

void Login::setupChangePasswordPage()
{
    changePasswordPage = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout(changePasswordPage);
    layout->setSpacing(15);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel(tr("change password"), changePasswordPage);
    QFont titleFont("Arial", 18, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #776e65;");

    cpUsernameInput = new QLineEdit(changePasswordPage);
    cpUsernameInput->setPlaceholderText(tr("usrname"));
    cpUsernameInput->setMinimumWidth(250);

    cpOldPasswordInput = new QLineEdit(changePasswordPage);
    cpOldPasswordInput->setPlaceholderText(tr("old password"));
    cpOldPasswordInput->setEchoMode(QLineEdit::Password);

    cpNewPasswordInput = new QLineEdit(changePasswordPage);
    cpNewPasswordInput->setPlaceholderText(tr("new password"));
    cpNewPasswordInput->setEchoMode(QLineEdit::Password);

    cpConfirmPasswordInput = new QLineEdit(changePasswordPage);
    cpConfirmPasswordInput->setPlaceholderText(tr("confirm your new password"));
    cpConfirmPasswordInput->setEchoMode(QLineEdit::Password);

    updatePasswordButton = new QPushButton(tr("save your new password"), changePasswordPage);
    backToLoginFromChangeButton = new QPushButton(tr("back to login"), changePasswordPage);

    cpStatusLabel = new QLabel(changePasswordPage);
    cpStatusLabel->setAlignment(Qt::AlignCenter);
    cpStatusLabel->setStyleSheet("color: red;");

    updatePasswordButton->setStyleSheet(getButtonStyle());
    backToLoginFromChangeButton->setStyleSheet(getButtonStyle(false));

    cpUsernameInput->installEventFilter(this);
    cpOldPasswordInput->installEventFilter(this);
    cpNewPasswordInput->installEventFilter(this);
    cpConfirmPasswordInput->installEventFilter(this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(cpUsernameInput);
    layout->addWidget(cpOldPasswordInput);
    layout->addWidget(cpNewPasswordInput);
    layout->addWidget(cpConfirmPasswordInput);
    layout->addWidget(cpStatusLabel);
    layout->addSpacing(10);

    layout->addWidget(updatePasswordButton);
    layout->addWidget(backToLoginFromChangeButton);
    layout->addStretch();

    stackedWidget->addWidget(changePasswordPage);
}

QString Login::getButtonStyle(bool isAccent)
{
    if (isAccent)
    {
        return "QPushButton {"
               "background-color: #8f7a66;"
               "color: white;"
               "border-radius: 5px;"
               "padding: 8px;"
               "font-weight: bold;"
               "min-width: 100px;"
               "}"
               "QPushButton:hover {"
               "background-color: #9f8a76;"
               "}";
    }
    else
    {
        return "QPushButton {"
               "background-color: #bbada0;"
               "color: white;"
               "border-radius: 5px;"
               "padding: 8px;"
               "font-weight: bold;"
               "min-width: 100px;"
               "}"
               "QPushButton:hover {"
               "background-color: #cbbfb5;"
               "}";
    }
}

/* 显示不同的页面 */
void Login::showLoginPage()
{
    stackedWidget->setCurrentWidget(loginPage);
    statusLabel->clear();
    usernameInput->clear();
    passwordInput->clear();
}

void Login::showRegisterPage()
{
    stackedWidget->setCurrentWidget(registerPage);
    regStatusLabel->clear();
    regUsernameInput->clear();
    regPasswordInput->clear();
    regConfirmPasswordInput->clear();
}

void Login::showForgotPasswordPage()
{
    stackedWidget->setCurrentWidget(forgotPasswordPage);
    fpStatusLabel->clear();
    fpUsernameInput->clear();
    fpNewPasswordInput->clear();
    fpConfirmPasswordInput->clear();
}

void Login::showChangePasswordPage()
{
    stackedWidget->setCurrentWidget(changePasswordPage);
    cpStatusLabel->clear();
    cpUsernameInput->clear();
    cpOldPasswordInput->clear();
    cpNewPasswordInput->clear();
    cpConfirmPasswordInput->clear();
}

/* 登录功能 */
void Login::onLoginClicked()
{
    QString username = usernameInput->text().trimmed();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty())
    {
        statusLabel->setText(tr("username or password can not be empty"));
        return;
    }

    if (Database::instance().loginUser(username, password))
    {
        statusLabel->clear();
        emit loginSuccess(username);
    }
    else
    {
        statusLabel->setText(tr("usrname or password is wrong"));
    }
}

/* 注册功能 */
void Login::onRegisterClicked()
{
    QString username = regUsernameInput->text().trimmed();
    QString password = regPasswordInput->text();
    QString confirmPassword = regConfirmPasswordInput->text();

    if (username.isEmpty() || password.isEmpty())
    {
        regStatusLabel->setText(tr("username or password can not be empty"));
        return;
    }

    if (password != confirmPassword)
    {
        regStatusLabel->setText(tr("two password is not same"));
        return;
    }

    if (Database::instance().registerUser(username, password))
    {
        QMessageBox msgBox;
        setupMessageBoxStyle(msgBox);
        msgBox.setWindowTitle(tr("register successful"));
        msgBox.setText(tr("usr %1 you could login now").arg(username));
        msgBox.exec();

        regStatusLabel->clear();
        emit registerSuccess();
        showLoginPage();
    }
    else
    {
        regStatusLabel->setText(tr("the usrname already be used"));
    }
}

void Login::onBackClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Exit Confirmation");
    msgBox.setText("Are you sure you want to exit the application?");
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
        // 退出
        QApplication::quit();
    }
}

void Login::resetPassword()
{
    QString username = fpUsernameInput->text().trimmed();
    QString newPassword = fpNewPasswordInput->text();
    QString confirmPassword = fpConfirmPasswordInput->text();

    if (username.isEmpty() || newPassword.isEmpty())
    {
        fpStatusLabel->setText(tr("usrnname or new password can not be empty"));
        return;
    }

    if (newPassword != confirmPassword)
    {
        fpStatusLabel->setText(tr("two new password is not same"));
        return;
    }

    if (Database::instance().resetUserPassword(username, newPassword))
    {
        QMessageBox::information(this, tr("reset successful"),
                                 tr("usr %1 reset password successful").arg(username));
        fpStatusLabel->clear();
        showLoginPage();
    }
    else
    {
        fpStatusLabel->setText(tr("usrname not exist"));
    }
}

// 修改密码功能
void Login::changePassword()
{
    QString username = cpUsernameInput->text().trimmed();
    QString oldPassword = cpOldPasswordInput->text();
    QString newPassword = cpNewPasswordInput->text();
    QString confirmPassword = cpConfirmPasswordInput->text();

    if (username.isEmpty() || oldPassword.isEmpty() || newPassword.isEmpty())
    {
        cpStatusLabel->setText(tr("this can not be empty"));
        return;
    }

    if (newPassword != confirmPassword)
    {
        cpStatusLabel->setText(tr("two new password is not same"));
        return;
    }

    if (Database::instance().changeUserPassword(username, oldPassword, newPassword))
    {
        QMessageBox::information(this, tr("reset successful"),
                                 tr("usr %1 reset password successful").arg(username));
        cpStatusLabel->clear();
        showLoginPage();
    }
    else
    {
        cpStatusLabel->setText(tr("reset fail! check your usrname or password"));
    }
}

void Login::onForgotPasswordClicked()
{
    // 显示忘记密码页面
    showForgotPasswordPage();
}

void Login::onChangePasswordClicked()
{
    // 显示修改密码页面
    showChangePasswordPage();
}

bool Login::eventFilter(QObject *obj, QEvent *event)
{
    // 检查事件类型是否为按键事件
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // 检查是否按下了回车键
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            // 确保是从登录页面的输入框发出的
            if (stackedWidget->currentWidget() == loginPage &&
                (obj == usernameInput || obj == passwordInput))
            {
                // 触发登录操作
                onLoginClicked();
                return true; // 事件已处理
            }

            // 注册页面也可以添加回车快捷键
            if (stackedWidget->currentWidget() == registerPage &&
                (obj == regUsernameInput || obj == regPasswordInput || obj == regConfirmPasswordInput))
            {
                onRegisterClicked();
                return true;
            }

            // 忘记密码页面
            if (stackedWidget->currentWidget() == forgotPasswordPage &&
                (obj == fpUsernameInput || obj == fpNewPasswordInput || obj == fpConfirmPasswordInput))
            {
                resetPassword();
                return true;
            }

            // 修改密码页面
            if (stackedWidget->currentWidget() == changePasswordPage &&
                (obj == cpUsernameInput || obj == cpOldPasswordInput || obj == cpNewPasswordInput || obj == cpConfirmPasswordInput))
            {
                changePassword();
                return true;
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void Login::setupMessageBoxStyle(QMessageBox &msgBox)
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