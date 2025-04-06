#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QMessageBox>

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = nullptr);
signals:
    void loginSuccess(const QString &username);
    void registerSuccess();
    void backToMainMenu();
private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onBackClicked();
    void onForgotPasswordClicked();
    void onChangePasswordClicked();
    void showLoginPage();
    void showRegisterPage();
    void showForgotPasswordPage();
    void showChangePasswordPage();
    void resetPassword();
    void changePassword();

private:
    QStackedWidget *stackedWidget;

    /* 登录界面 */
    QWidget *loginPage;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *forgotPasswordButton;
    QPushButton *backButton;
    QLabel *statusLabel;

    /* 注册页面 */
    QWidget *registerPage;
    QLineEdit *regUsernameInput;
    QLineEdit *regPasswordInput;
    QLineEdit *regConfirmPasswordInput;
    QPushButton *createAccountButton;
    QPushButton *backToLoginButton;
    QLabel *regStatusLabel;

    /* 忘记密码页面 */
    QWidget *forgotPasswordPage;
    QLineEdit *fpUsernameInput;
    QLineEdit *fpNewPasswordInput;
    QLineEdit *fpConfirmPasswordInput;
    QPushButton *resetPasswordButton;
    QPushButton *backToLoginFromForgotButton;
    QLabel *fpStatusLabel;

    /* 修改密码页面 */
    QWidget *changePasswordPage;
    QLineEdit *cpUsernameInput;
    QLineEdit *cpOldPasswordInput;
    QLineEdit *cpNewPasswordInput;
    QLineEdit *cpConfirmPasswordInput;
    QPushButton *updatePasswordButton;
    QPushButton *backToLoginFromChangeButton;
    QLabel *cpStatusLabel;

    void setupUI();
    void setupLoginPage();
    void setupRegisterPage();
    void setupForgotPasswordPage();
    void setupChangePasswordPage();
    QString getButtonStyle(bool isAccent = true);
    bool eventFilter(QObject *obj, QEvent *event) override;
    void setupMessageBoxStyle(QMessageBox &msgBox);
};

#endif // LOGIN_H
