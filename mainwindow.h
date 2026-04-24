#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QPixmap>
#include <QDialog>

struct Order {
    int id;
    QString date;
    QString state;
    QString price;
    QString product;
    QString delivery;
    QString address;
    QString payment;
    QString note;
};

struct AutoOrder {
    QString product;
    QString interval;
    QString nextDate;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTabWidget *tabs;
    QWidget *storePage;
    QWidget *accountPage;

    QTableWidget *ordersTable;
    QLineEdit *searchEdit;
    QLabel *autoOrdersLabel;
    QTextEdit *historyText;

    QPushButton *cartButton;
    int cartCount = 0;

    QList<Order> orders;
    QList<AutoOrder> autoOrders;

    bool darkTheme = false;

    QString profileName = "Shakira Khamzina";
    QString profileEmail = "shakira@email.com";
    QString profilePhone = "+420 777 123 456";
    QString profileStatus = "Premium zákazník";
    QString profileInterval = "14 dní";
    QString street = "Hlavní 25";
    QString city = "Ostrava";
    QString psc = "702 00";
    QString country = "Česká republika";

    QLabel *profileInfoLabel;
    QLabel *addressInfoLabel;

    void setupUi();
    void setupMenu();
    void setupStorePage();
    void setupAccountPage();

    void loadData();
    void refreshOrdersTable();
    void refreshAutoOrders();
    void refreshProfile();
    void refreshAddress();

    void applyLightStyles();
    void applyDarkStyles();

    QPixmap loadImage(const QString &fileName);
    QDialog* createOrderDialog(Order &order, bool editMode);

private slots:
    void showAbout();
    void confirmExit();
    void openSettings();

    void goToAccountPage();
    void openProducts();
    void openOrders();
    void openSupport();

    void createOrder();
    void editSelectedOrder();
    void deleteSelectedOrder();
    void showOrderDetail(int row, int column);
    void searchOrder();

    void createAutoOrder();
    void cancelAutoOrder();
    void processAllAutoOrders();

    void addToCart();
    void showPromo();

    void editProfile();
    void editAddress();
    void changePassword();
    void paymentSettings();
    void logoutUser();
};

#endif