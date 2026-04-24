#include "mainwindow.h"

#include <QPixmap>
#include <QCoreApplication>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QTextEdit>
#include <QLineEdit>
#include <QAction>
#include <QHeaderView>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupMenu();
    loadData();
    refreshOrdersTable();
    refreshAutoOrders();
    refreshProfile();
    refreshAddress();
    applyLightStyles();
    statusBar()->showMessage("PM Store spuštěn.");
}

MainWindow::~MainWindow() {}

QPixmap MainWindow::loadImage(const QString &fileName)
{
    QStringList paths;
    paths << fileName;
    paths << "images/" + fileName;
    paths << QCoreApplication::applicationDirPath() + "/" + fileName;
    paths << QCoreApplication::applicationDirPath() + "/images/" + fileName;
    paths << QCoreApplication::applicationDirPath() + "/../" + fileName;
    paths << QCoreApplication::applicationDirPath() + "/../images/" + fileName;
    paths << QCoreApplication::applicationDirPath() + "/../../" + fileName;
    paths << QCoreApplication::applicationDirPath() + "/../../images/" + fileName;
    paths << QCoreApplication::applicationDirPath() + "/../../../" + fileName;
    paths << QCoreApplication::applicationDirPath() + "/../../../images/" + fileName;

    for (QString path : paths) {
        QPixmap pix(path);
        if (!pix.isNull()) return pix;
    }

    return QPixmap();
}

void MainWindow::setupUi()
{
    setWindowTitle("PM Store");
    resize(1000, 600);


    tabs = new QTabWidget(this);
    storePage = new QWidget();
    accountPage = new QWidget();

    tabs->addTab(storePage, "PM Store");
    tabs->addTab(accountPage, "Osobní účet");

    setCentralWidget(tabs);

    setupStorePage();
    setupAccountPage();
}

void MainWindow::setupMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("Soubor");
    QAction *exitAction = new QAction("Konec", this);

    QMenu *settingsMenu = menuBar()->addMenu("Nastavení");
    QAction *themeAction = new QAction("Vzhled aplikace", this);

    QMenu *helpMenu = menuBar()->addMenu("Nápověda");
    QAction *aboutAction = new QAction("O aplikaci", this);

    fileMenu->addAction(exitAction);
    settingsMenu->addAction(themeAction);
    helpMenu->addAction(aboutAction);

    connect(exitAction, &QAction::triggered, this, &MainWindow::confirmExit);
    connect(themeAction, &QAction::triggered, this, &MainWindow::openSettings);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::setupStorePage()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QFrame *headerFrame = new QFrame();
    headerFrame->setObjectName("card");

    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);

    QLabel *logoLabel = new QLabel("PM Store");
    logoLabel->setObjectName("mainTitle");

    QPushButton *productsButton = new QPushButton("Produkty");
    QPushButton *ordersButton = new QPushButton("Moje objednávky");
    QPushButton *autoOrdersButton = new QPushButton("Automatická objednávka");
    QPushButton *supportButton = new QPushButton("Podpora");

    QLabel *loginLabel = new QLabel("LOGIN: KHA0047");
    loginLabel->setObjectName("loginLabel");

    headerLayout->addWidget(logoLabel);
    headerLayout->addSpacing(20);
    headerLayout->addWidget(productsButton);
    headerLayout->addWidget(ordersButton);
    headerLayout->addWidget(autoOrdersButton);
    headerLayout->addWidget(supportButton);
    headerLayout->addStretch();


    QPushButton *accountBtn = new QPushButton("👤");
    accountBtn->setFixedSize(40, 35);

    cartButton = new QPushButton("🛒 0");
    cartButton->setFixedSize(70, 35);

    headerLayout->addWidget(accountBtn);
    headerLayout->addWidget(cartButton);
    headerLayout->addWidget(loginLabel);

    QHBoxLayout *contentLayout = new QHBoxLayout();

    QFrame *leftFrame = new QFrame();
    leftFrame->setObjectName("card");
    leftFrame->setMaximumWidth(280);

    QVBoxLayout *leftLayout = new QVBoxLayout(leftFrame);

    QLabel *accountTitle = new QLabel("OSOBNÍ ÚČET");
    accountTitle->setObjectName("sectionTitle");

    QLabel *userName = new QLabel("Shakira Khamzina");
    QLabel *userInfo = new QLabel("Dobrý den, Shakira Khamzina.\nPřehled vašich objednávek a\nautomatických dodávek.");
    userInfo->setWordWrap(true);

    QPushButton *goAccountButton = new QPushButton("Přejít na účet");

    QLabel *activeOrders = new QLabel("AKTIVNÍ AUTOMATICKÉ OBJEDNÁVKY");
    activeOrders->setObjectName("smallTitle");

    autoOrdersLabel = new QLabel();
    autoOrdersLabel->setStyleSheet("font-family: monospace;");
    autoOrdersLabel->setWordWrap(true);

    QPushButton *processButton = new QPushButton("Zpracovat vše");

    leftLayout->addWidget(accountTitle);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(userName);
    leftLayout->addWidget(userInfo);
    leftLayout->addWidget(goAccountButton);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(activeOrders);
    leftLayout->addWidget(autoOrdersLabel);
    leftLayout->addStretch();
    leftLayout->addWidget(processButton);

    QVBoxLayout *centerLayout = new QVBoxLayout();

    QFrame *quickFrame = new QFrame();
    quickFrame->setObjectName("card");
    QVBoxLayout *quickMainLayout = new QVBoxLayout(quickFrame);

    QLabel *quickTitle = new QLabel("RYCHLÉ AKCE");
    quickTitle->setObjectName("sectionTitle");

    QPushButton *createAutoButton = new QPushButton("Vytvořit novou automatickou objednávku");
    QPushButton *editAddressButtonQuick = new QPushButton("Upravit adresu doručení");
    QPushButton *paymentButtonQuick = new QPushButton("Nastavení plateb");
    QPushButton *cancelAutoButton = new QPushButton("Zrušit automatickou objednávku");

    QGridLayout *quickGrid = new QGridLayout();
    quickGrid->addWidget(createAutoButton, 0, 0);
    quickGrid->addWidget(editAddressButtonQuick, 0, 1);
    quickGrid->addWidget(paymentButtonQuick, 1, 0);
    quickGrid->addWidget(cancelAutoButton, 1, 1);

    quickMainLayout->addWidget(quickTitle);
    quickMainLayout->addLayout(quickGrid);

    QFrame *ordersFrame = new QFrame();
    ordersFrame->setObjectName("card");
    QVBoxLayout *ordersLayout = new QVBoxLayout(ordersFrame);

    QLabel *ordersTitle = new QLabel("OBJEDNÁVKY");
    ordersTitle->setObjectName("sectionTitle");

    QHBoxLayout *orderButtonsLayout = new QHBoxLayout();
    QPushButton *editOrderButton = new QPushButton("Upravit vybranou");
    QPushButton *newOrderButton = new QPushButton("Nová objednávka");
    QPushButton *deleteOrderButton = new QPushButton("Smazat vybranou");

    orderButtonsLayout->addStretch();
    orderButtonsLayout->addWidget(editOrderButton);
    orderButtonsLayout->addWidget(newOrderButton);
    orderButtonsLayout->addWidget(deleteOrderButton);

    ordersTable = new QTableWidget();
    ordersTable->setColumnCount(5);
    ordersTable->setHorizontalHeaderLabels({"ID", "Datum", "Stav", "Cena", "Produkt"});
    ordersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ordersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ordersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ordersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ordersTable->setMinimumHeight(145);

    ordersLayout->addWidget(ordersTitle);
    ordersLayout->addLayout(orderButtonsLayout);
    ordersLayout->addWidget(ordersTable);

    QFrame *productsFrame = new QFrame();
    productsFrame->setObjectName("card");
    QVBoxLayout *productsLayout = new QVBoxLayout(productsFrame);

    QLabel *productsTitle = new QLabel("NEDÁVNO PROHLÍŽENÉ PRODUKTY");
    productsTitle->setObjectName("sectionTitle");

    QHBoxLayout *cardsLayout = new QHBoxLayout();

    auto makeProductCard = [&](QString image, QString name, QString price) {
        QFrame *product = new QFrame();
        product->setObjectName("productCard");
        product->setFixedSize(230, 140);

        QVBoxLayout *layout = new QVBoxLayout(product);
        layout->setContentsMargins(8, 6, 8, 6);
        layout->setSpacing(4);

        QLabel *nameLabel = new QLabel(name);

        QLabel *img = new QLabel();
        img->setAlignment(Qt::AlignCenter);
        img->setFixedSize(180, 50);

        QPixmap pix = loadImage(image);
        if (pix.isNull()) pix = loadImage("iqos.jpg");

        if (!pix.isNull()) {
            img->setPixmap(pix.scaled(160, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            img->setText("Obrázek nenalezen");
        }

        QLabel *priceLabel = new QLabel(price);

        QPushButton *button = new QPushButton("Přidat do košíku");
        button->setFixedSize(140, 25);
        button->setStyleSheet("font-size: 10px; padding: 1px;");

        layout->addWidget(nameLabel);
        layout->addWidget(img, 0, Qt::AlignCenter);
        layout->addWidget(priceLabel);
        layout->addWidget(button, 0, Qt::AlignCenter);

        connect(button, &QPushButton::clicked, this, &MainWindow::addToCart);


        return product;
    };

    cardsLayout->addWidget(makeProductCard("r1.jpg", "IQOS ILUMA", "1590 Kč"));
    cardsLayout->addWidget(makeProductCard("r2.jpg", "IQOS ILUMA PRIME", "2490 Kč"));
    cardsLayout->addWidget(makeProductCard("r4.jpg", "TEREA Bronze", "890 Kč"));

    productsLayout->addWidget(productsTitle);
    productsLayout->addLayout(cardsLayout);

    centerLayout->addWidget(quickFrame);
    centerLayout->addWidget(ordersFrame);
    centerLayout->addWidget(productsFrame);

    QVBoxLayout *rightLayout = new QVBoxLayout();

    QFrame *searchFrame = new QFrame();
    searchFrame->setObjectName("card");
    QVBoxLayout *searchLayout = new QVBoxLayout(searchFrame);

    QLabel *searchTitle = new QLabel("Hledat objednávku podle ID:");
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("např. 2451");
    QPushButton *searchButton = new QPushButton("Najít");

    searchLayout->addWidget(searchTitle);
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);

    QFrame *promoFrame = new QFrame();
    promoFrame->setObjectName("card");
    QVBoxLayout *promoLayout = new QVBoxLayout(promoFrame);

    QLabel *promoText = new QLabel("NOVINKA\nIQOS ILUMA PRIME\nObjevte prémiovou verzi s moderním designem.");
    promoText->setWordWrap(true);

    QLabel *promoImage = new QLabel();
    promoImage->setAlignment(Qt::AlignCenter);
    promoImage->setMinimumHeight(260);
    promoImage->setObjectName("promoImage");

    QPixmap promoPixmap = loadImage("iqos.jpg");
    if (!promoPixmap.isNull()) {
        promoImage->setPixmap(promoPixmap.scaled(250, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        promoImage->setText("Obrázek nenalezen");
    }

    QPushButton *moreButton = new QPushButton("Zjistit více");

    promoLayout->addWidget(promoText);
    promoLayout->addWidget(promoImage);
    promoLayout->addWidget(moreButton);

    rightLayout->addWidget(searchFrame);
    rightLayout->addWidget(promoFrame);
    rightLayout->addStretch();

    contentLayout->addWidget(leftFrame, 1);
    contentLayout->addLayout(centerLayout, 3);
    contentLayout->addLayout(rightLayout, 1);

    mainLayout->addWidget(headerFrame);
    mainLayout->addLayout(contentLayout);

    storePage->setLayout(mainLayout);

    connect(productsButton, &QPushButton::clicked, this, &MainWindow::openProducts);
    connect(ordersButton, &QPushButton::clicked, this, &MainWindow::openOrders);
    connect(autoOrdersButton, &QPushButton::clicked, this, &MainWindow::createAutoOrder);
    connect(supportButton, &QPushButton::clicked, this, &MainWindow::openSupport);

    connect(goAccountButton, &QPushButton::clicked, this, &MainWindow::goToAccountPage);
    connect(processButton, &QPushButton::clicked, this, &MainWindow::processAllAutoOrders);

    connect(createAutoButton, &QPushButton::clicked, this, &MainWindow::createAutoOrder);
    connect(editAddressButtonQuick, &QPushButton::clicked, this, &MainWindow::editAddress);
    connect(paymentButtonQuick, &QPushButton::clicked, this, &MainWindow::paymentSettings);
    connect(cancelAutoButton, &QPushButton::clicked, this, &MainWindow::cancelAutoOrder);

    connect(newOrderButton, &QPushButton::clicked, this, &MainWindow::createOrder);
    connect(editOrderButton, &QPushButton::clicked, this, &MainWindow::editSelectedOrder);
    connect(deleteOrderButton, &QPushButton::clicked, this, &MainWindow::deleteSelectedOrder);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchOrder);
    connect(moreButton, &QPushButton::clicked, this, &MainWindow::showPromo);

    connect(ordersTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::showOrderDetail);
    connect(accountBtn, &QPushButton::clicked, this, &MainWindow::goToAccountPage);
}

void MainWindow::setupAccountPage()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QFrame *headerFrame = new QFrame();
    headerFrame->setObjectName("card");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);

    QLabel *title = new QLabel("Osobní účet");
    title->setObjectName("mainTitle");

    QLabel *loginLabel = new QLabel("LOGIN: KHA0047");
    loginLabel->setObjectName("loginLabel");

    headerLayout->addWidget(title);
    headerLayout->addStretch();
    headerLayout->addWidget(loginLabel);

    QHBoxLayout *contentLayout = new QHBoxLayout();

    QVBoxLayout *leftColumn = new QVBoxLayout();

    QFrame *profileFrame = new QFrame();
    profileFrame->setObjectName("card");
    QVBoxLayout *profileLayout = new QVBoxLayout(profileFrame);

    QLabel *profileTitle = new QLabel("PROFIL UŽIVATELE");
    profileTitle->setObjectName("sectionTitle");

    profileInfoLabel = new QLabel();
    profileInfoLabel->setWordWrap(true);

    QPushButton *editProfileButton = new QPushButton("Upravit profil");

    profileLayout->addWidget(profileTitle);
    profileLayout->addWidget(profileInfoLabel);
    profileLayout->addStretch();
    profileLayout->addWidget(editProfileButton);

    QFrame *addressFrame = new QFrame();
    addressFrame->setObjectName("card");
    QVBoxLayout *addressLayout = new QVBoxLayout(addressFrame);

    QLabel *addressTitle = new QLabel("DORUČOVACÍ ADRESA");
    addressTitle->setObjectName("sectionTitle");

    addressInfoLabel = new QLabel();
    addressInfoLabel->setWordWrap(true);

    QPushButton *editAddressButton = new QPushButton("Upravit adresu");

    addressLayout->addWidget(addressTitle);
    addressLayout->addWidget(addressInfoLabel);
    addressLayout->addStretch();
    addressLayout->addWidget(editAddressButton);

    leftColumn->addWidget(profileFrame);
    leftColumn->addWidget(addressFrame);

    QVBoxLayout *rightColumn = new QVBoxLayout();

    QFrame *settingsFrame = new QFrame();
    settingsFrame->setObjectName("card");
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsFrame);

    QLabel *settingsTitle = new QLabel("NASTAVENÍ ÚČTU");
    settingsTitle->setObjectName("sectionTitle");

    QPushButton *changePassButton = new QPushButton("Změnit heslo");
    QPushButton *appSettingsButton = new QPushButton("Nastavení aplikace");
    QPushButton *paymentButton = new QPushButton("Správa platebních metod");
    QPushButton *logoutButton = new QPushButton("Odhlásit se");

    settingsLayout->addWidget(settingsTitle);
    settingsLayout->addWidget(changePassButton);
    settingsLayout->addWidget(appSettingsButton);
    settingsLayout->addWidget(paymentButton);
    settingsLayout->addWidget(logoutButton);
    settingsLayout->addStretch();

    QFrame *historyFrame = new QFrame();
    historyFrame->setObjectName("card");
    QVBoxLayout *historyLayout = new QVBoxLayout(historyFrame);

    QLabel *historyTitle = new QLabel("HISTORIE AKTIVITY");
    historyTitle->setObjectName("sectionTitle");

    historyText = new QTextEdit();
    historyText->setReadOnly(true);
    historyText->setText("• Aplikace byla spuštěna.\n• Uživatelský profil načten.\n• Objednávky připraveny.");

    historyLayout->addWidget(historyTitle);
    historyLayout->addWidget(historyText);

    rightColumn->addWidget(settingsFrame);
    rightColumn->addWidget(historyFrame);

    contentLayout->addLayout(leftColumn, 1);
    contentLayout->addLayout(rightColumn, 3);

    mainLayout->addWidget(headerFrame);
    mainLayout->addLayout(contentLayout);

    accountPage->setLayout(mainLayout);

    connect(editProfileButton, &QPushButton::clicked, this, &MainWindow::editProfile);
    connect(editAddressButton, &QPushButton::clicked, this, &MainWindow::editAddress);
    connect(changePassButton, &QPushButton::clicked, this, &MainWindow::changePassword);
    connect(appSettingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);
    connect(paymentButton, &QPushButton::clicked, this, &MainWindow::paymentSettings);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::logoutUser);
}

void MainWindow::loadData()
{
    orders.append({2451, "12.03.2026", "Doručeno", "1590 Kč", "IQOS ILUMA", "Kurýr",
                   "Hlavní 25, Ostrava, 702 00", "Karta online",
                   "Objednávka byla doručena v pořádku."});

    orders.append({2448, "01.03.2026", "Odesláno", "890 Kč", "TEREA Bronze", "Zásilkovna",
                   "Masarykova 18, Brno, 602 00", "Dobírka",
                   "Balík je na cestě."});

    autoOrders.append({"IQOS ILUMA", "30 dní", "15.04.2026"});
    autoOrders.append({"TEREA Bronze", "14 dní", "02.04.2026"});
}

void MainWindow::refreshOrdersTable()
{
    ordersTable->setRowCount(orders.size());

    for (int i = 0; i < orders.size(); i++) {
        ordersTable->setItem(i, 0, new QTableWidgetItem(QString::number(orders[i].id)));
        ordersTable->setItem(i, 1, new QTableWidgetItem(orders[i].date));
        ordersTable->setItem(i, 2, new QTableWidgetItem(orders[i].state));
        ordersTable->setItem(i, 3, new QTableWidgetItem(orders[i].price));
        ordersTable->setItem(i, 4, new QTableWidgetItem(orders[i].product));
    }
}

void MainWindow::refreshAutoOrders()
{
    QString text = "Produkt       Interval    Datum\n\n";

    for (const AutoOrder &a : autoOrders) {
        text += a.product.leftJustified(13)
        + a.interval.leftJustified(12)
            + a.nextDate + "\n";
    }

    autoOrdersLabel->setText(text);
}

void MainWindow::refreshProfile()
{
    profileInfoLabel->setText(
        "Jméno: " + profileName +
        "\nEmail: " + profileEmail +
        "\nTelefon: " + profilePhone +
        "\nStatus: " + profileStatus +
        "\nInterval objednávek: " + profileInterval
        );
}

void MainWindow::refreshAddress()
{
    addressInfoLabel->setText(
        "Ulice: " + street +
        "\nMěsto: " + city +
        "\nPSČ: " + psc +
        "\nZemě: " + country
        );
}

QDialog* MainWindow::createOrderDialog(Order &order, bool editMode)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(editMode ? "Upravit objednávku" : "Nová objednávka");

    QFormLayout *form = new QFormLayout(dialog);

    QLineEdit *idEdit = new QLineEdit(QString::number(order.id));
    QLineEdit *dateEdit = new QLineEdit(order.date);
    QLineEdit *stateEdit = new QLineEdit(order.state);
    QLineEdit *priceEdit = new QLineEdit(order.price);
    QLineEdit *productEdit = new QLineEdit(order.product);
    QLineEdit *deliveryEdit = new QLineEdit(order.delivery);
    QLineEdit *addressEdit = new QLineEdit(order.address);
    QLineEdit *paymentEdit = new QLineEdit(order.payment);
    QTextEdit *noteEdit = new QTextEdit(order.note);

    form->addRow("ID:", idEdit);
    form->addRow("Datum:", dateEdit);
    form->addRow("Stav:", stateEdit);
    form->addRow("Cena:", priceEdit);
    form->addRow("Produkt:", productEdit);
    form->addRow("Doručení:", deliveryEdit);
    form->addRow("Adresa:", addressEdit);
    form->addRow("Platba:", paymentEdit);
    form->addRow("Poznámka:", noteEdit);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    form->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, dialog, [=, &order]() {
        bool ok;
        int id = idEdit->text().toInt(&ok);

        if (!ok) {
            QMessageBox::warning(dialog, "Chyba", "ID musí být číslo.");
            return;
        }

        order.id = id;
        order.date = dateEdit->text();
        order.state = stateEdit->text();
        order.price = priceEdit->text();
        order.product = productEdit->text();
        order.delivery = deliveryEdit->text();
        order.address = addressEdit->text();
        order.payment = paymentEdit->text();
        order.note = noteEdit->toPlainText();

        dialog->accept();
    });

    connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    return dialog;
}

void MainWindow::createOrder()
{
    Order order = {0, "24.04.2026", "Nová", "0 Kč", "IQOS ILUMA", "Kurýr",
                   "Hlavní 25, Ostrava, 702 00", "Karta online", ""};

    QDialog *dialog = createOrderDialog(order, false);

    if (dialog->exec() == QDialog::Accepted) {
        orders.append(order);
        refreshOrdersTable();
        QMessageBox::information(this, "Uloženo", "Objednávka byla vytvořena.");
        statusBar()->showMessage("Nová objednávka byla vytvořena.", 3000);
        historyText->append("• Vytvořena objednávka ID " + QString::number(order.id));
    }

    dialog->deleteLater();
}

void MainWindow::editSelectedOrder()
{
    int row = ordersTable->currentRow();

    if (row < 0 || row >= orders.size()) {
        QMessageBox::warning(this, "Chyba", "Nejdřív vyber objednávku.");
        return;
    }

    QDialog *dialog = createOrderDialog(orders[row], true);

    if (dialog->exec() == QDialog::Accepted) {
        refreshOrdersTable();
        QMessageBox::information(this, "Uloženo", "Objednávka byla upravena.");
        statusBar()->showMessage("Objednávka byla upravena.", 3000);
        historyText->append("• Upravena objednávka ID " + QString::number(orders[row].id));
    }

    dialog->deleteLater();
}

void MainWindow::deleteSelectedOrder()
{
    int row = ordersTable->currentRow();

    if (row < 0 || row >= orders.size()) {
        QMessageBox::warning(this, "Chyba", "Nejdřív vyber objednávku.");
        return;
    }

    if (QMessageBox::question(this, "Smazat?", "Opravdu chceš smazat vybranou objednávku?") == QMessageBox::Yes) {
        int id = orders[row].id;
        orders.removeAt(row);
        refreshOrdersTable();
        QMessageBox::information(this, "Smazáno", "Objednávka byla smazána.");
        statusBar()->showMessage("Objednávka byla smazána.", 3000);
        historyText->append("• Smazána objednávka ID " + QString::number(id));
    }
}

void MainWindow::showOrderDetail(int row, int)
{
    if (row < 0 || row >= orders.size()) return;

    Order order = orders[row];

    QDialog dialog(this);
    dialog.setWindowTitle("Detail objednávky");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *info = new QLabel(
        "ID: " + QString::number(order.id) +
        "\nDatum: " + order.date +
        "\nStav: " + order.state +
        "\nCena: " + order.price +
        "\nProdukt: " + order.product +
        "\nDoručení: " + order.delivery +
        "\nAdresa: " + order.address +
        "\nPlatba: " + order.payment +
        "\nPoznámka: " + order.note
        );

    QPushButton *editButton = new QPushButton("Upravit");
    QPushButton *deleteButton = new QPushButton("Smazat");
    QPushButton *closeButton = new QPushButton("Zavřít");

    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addWidget(editButton);
    buttons->addWidget(deleteButton);
    buttons->addWidget(closeButton);

    layout->addWidget(info);
    layout->addLayout(buttons);

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::close);

    connect(editButton, &QPushButton::clicked, &dialog, [&]() {
        ordersTable->selectRow(row);
        dialog.close();
        editSelectedOrder();
    });

    connect(deleteButton, &QPushButton::clicked, &dialog, [&]() {
        ordersTable->selectRow(row);
        dialog.close();
        deleteSelectedOrder();
    });

    dialog.exec();
}

void MainWindow::searchOrder()
{
    QString idText = searchEdit->text().trimmed();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Chyba", "Zadej ID objednávky.");
        return;
    }

    bool ok;
    int searchId = idText.toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Chyba", "ID musí být číslo.");
        return;
    }

    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].id == searchId) {
            ordersTable->selectRow(i);
            statusBar()->showMessage("Objednávka nalezena.", 3000);

            showOrderDetail(i, 0);
            return;
        }
    }

    QMessageBox::information(this, "Výsledek", "Objednávka s tímto ID nebyla nalezena.");
    statusBar()->showMessage("Objednávka nebyla nalezena.", 3000);
}

void MainWindow::createAutoOrder()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nová automatická objednávka");

    QFormLayout *form = new QFormLayout(&dialog);

    QLineEdit *productEdit = new QLineEdit("IQOS ILUMA");
    QLineEdit *intervalEdit = new QLineEdit("30 dní");
    QLineEdit *nextDateEdit = new QLineEdit("15.04.2026");

    form->addRow("Produkt:", productEdit);
    form->addRow("Interval:", intervalEdit);
    form->addRow("Další odeslání:", nextDateEdit);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    form->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (productEdit->text().isEmpty() || intervalEdit->text().isEmpty() || nextDateEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Chyba", "Vyplň všechna pole.");
            return;
        }

        autoOrders.append({
            productEdit->text(),
            intervalEdit->text(),
            nextDateEdit->text()
        });

        refreshAutoOrders();

        QMessageBox::information(this, "Hotovo", "Automatická objednávka byla vytvořena.");
        statusBar()->showMessage("Auto-order vytvořen.", 3000);
        historyText->append("• Vytvořen auto-order: " + productEdit->text());
    }
}


void MainWindow::cancelAutoOrder()
{
    if (autoOrders.isEmpty()) {
        QMessageBox::information(this, "Info", "Neexistuje žádná automatická objednávka.");
        return;
    }

    QStringList items;
    for (const AutoOrder &a : autoOrders) {
        items << a.product + " / " + a.interval + " / " + a.nextDate;
    }

    bool ok;
    QString selected = QInputDialog::getItem(this, "Zrušit auto-order", "Vyber objednávku:", items, 0, false, &ok);

    if (!ok) return;

    int index = items.indexOf(selected);

    if (index >= 0) {
        QString product = autoOrders[index].product;
        autoOrders.removeAt(index);
        refreshAutoOrders();
        QMessageBox::information(this, "Zrušeno", "Automatická objednávka byla zrušena.");
        statusBar()->showMessage("Auto-order zrušen.", 3000);
        historyText->append("• Zrušen auto-order: " + product);
    }
}

void MainWindow::processAllAutoOrders()
{
    QMessageBox::information(this, "Zpracování", "Všechny automatické objednávky byly zpracovány.");
    statusBar()->showMessage("Automatické objednávky zpracovány.", 3000);
    historyText->append("• Zpracovány všechny automatické objednávky.");
}

void MainWindow::editProfile()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Upravit profil");

    QFormLayout *form = new QFormLayout(&dialog);

    QLineEdit *nameEdit = new QLineEdit(profileName);
    QLineEdit *emailEdit = new QLineEdit(profileEmail);
    QLineEdit *phoneEdit = new QLineEdit(profilePhone);
    QLineEdit *statusEdit = new QLineEdit(profileStatus);
    QLineEdit *intervalEdit = new QLineEdit(profileInterval);

    form->addRow("Jméno:", nameEdit);
    form->addRow("Email:", emailEdit);
    form->addRow("Telefon:", phoneEdit);
    form->addRow("Status:", statusEdit);
    form->addRow("Interval objednávek:", intervalEdit);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    form->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (nameEdit->text().isEmpty() || emailEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Chyba", "Jméno a email musí být vyplněné.");
            return;
        }

        profileName = nameEdit->text();
        profileEmail = emailEdit->text();
        profilePhone = phoneEdit->text();
        profileStatus = statusEdit->text();
        profileInterval = intervalEdit->text();

        refreshProfile();

        QMessageBox::information(this, "Uloženo", "Profil byl upraven.");
        statusBar()->showMessage("Profil upraven.", 3000);
        historyText->append("• Upraven profil uživatele.");
    }
}

void MainWindow::editAddress()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Upravit adresu");

    QFormLayout *form = new QFormLayout(&dialog);

    QLineEdit *streetEdit = new QLineEdit(street);
    QLineEdit *cityEdit = new QLineEdit(city);
    QLineEdit *pscEdit = new QLineEdit(psc);
    QLineEdit *countryEdit = new QLineEdit(country);

    form->addRow("Ulice:", streetEdit);
    form->addRow("Město:", cityEdit);
    form->addRow("PSČ:", pscEdit);
    form->addRow("Země:", countryEdit);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    form->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (streetEdit->text().isEmpty() || cityEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Chyba", "Ulice a město musí být vyplněné.");
            return;
        }

        street = streetEdit->text();
        city = cityEdit->text();
        psc = pscEdit->text();
        country = countryEdit->text();

        refreshAddress();

        QMessageBox::information(this, "Uloženo", "Adresa byla upravena.");
        statusBar()->showMessage("Adresa upravena.", 3000);
        historyText->append("• Upravena doručovací adresa.");
    }
}

void MainWindow::paymentSettings()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Správa platebních metod");

    QFormLayout *form = new QFormLayout(&dialog);

    QComboBox *paymentType = new QComboBox();
    paymentType->addItems({"Karta online", "Dobírka", "Bankovní převod"});

    QLineEdit *cardNumberEdit = new QLineEdit("**** **** **** 1234");
    QLineEdit *cardHolderEdit = new QLineEdit(profileName);
    QLineEdit *validEdit = new QLineEdit("12/28");

    form->addRow("Typ platby:", paymentType);
    form->addRow("Číslo karty:", cardNumberEdit);
    form->addRow("Držitel karty:", cardHolderEdit);
    form->addRow("Platnost:", validEdit);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    form->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QMessageBox::information(this, "Uloženo", "Platební metoda byla uložena.");
        statusBar()->showMessage("Platební metoda upravena.", 3000);
        historyText->append("• Upraveny platební metody.");
    }
}

void MainWindow::changePassword()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Změnit heslo");

    QFormLayout *form = new QFormLayout(&dialog);

    QLineEdit *oldPassEdit = new QLineEdit();
    QLineEdit *newPassEdit = new QLineEdit();
    QLineEdit *confirmPassEdit = new QLineEdit();

    oldPassEdit->setEchoMode(QLineEdit::Password);
    newPassEdit->setEchoMode(QLineEdit::Password);
    confirmPassEdit->setEchoMode(QLineEdit::Password);

    form->addRow("Staré heslo:", oldPassEdit);
    form->addRow("Nové heslo:", newPassEdit);
    form->addRow("Potvrdit heslo:", confirmPassEdit);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    form->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (oldPassEdit->text().isEmpty() || newPassEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Chyba", "Vyplň všechna pole.");
            return;
        }

        if (newPassEdit->text() != confirmPassEdit->text()) {
            QMessageBox::warning(this, "Chyba", "Nová hesla se neshodují.");
            return;
        }

        QMessageBox::information(this, "Uloženo", "Heslo bylo změněno.");
        statusBar()->showMessage("Heslo změněno.", 3000);
        historyText->append("• Změněno heslo.");
    }
}

void MainWindow::logoutUser()
{
    if (QMessageBox::question(this, "Odhlásit se?", "Opravdu se chceš odhlásit?") == QMessageBox::Yes) {

        profileName = "Host";
        profileEmail = "—";
        profilePhone = "—";
        profileStatus = "Odhlášený uživatel";
        profileInterval = "—";

        street = "—";
        city = "—";
        psc = "—";
        country = "—";

        orders.clear();
        autoOrders.clear();

        refreshProfile();
        refreshAddress();
        refreshOrdersTable();
        refreshAutoOrders();

        historyText->clear();
        historyText->setText("• Uživatel byl odhlášen.");

        searchEdit->clear();

        QMessageBox::information(this, "Odhlášeno", "Uživatel byl odhlášen.");
        statusBar()->showMessage("Uživatel odhlášen. Data byla skryta.", 3000);
    }
}

void MainWindow::goToAccountPage()
{
    tabs->setCurrentWidget(accountPage);
    statusBar()->showMessage("Otevřen osobní účet.", 3000);
}

void MainWindow::openProducts()
{
    QMessageBox::information(this, "Produkty", "Zobrazeny produkty PM Store.");
    statusBar()->showMessage("Otevřena sekce Produkty.", 3000);
}

void MainWindow::openOrders()
{
    QMessageBox::information(this, "Objednávky", "Objednávky jsou zobrazeny v hlavní tabulce.");
    statusBar()->showMessage("Otevřena sekce objednávek.", 3000);
}

void MainWindow::openSupport()
{
    QMessageBox::information(this, "Podpora", "Kontakt podpory: support@pmstore.cz");
    statusBar()->showMessage("Otevřena podpora.", 3000);
}

void MainWindow::addToCart()
{
    cartCount++;
    cartButton->setText("🛒 " + QString::number(cartCount));

    QMessageBox::information(this, "Košík", "Produkt byl přidán do košíku.");
    statusBar()->showMessage("Produkt přidán do košíku.", 3000);
    historyText->append("• Produkt přidán do košíku.");
}

void MainWindow::showPromo()
{
    QMessageBox::information(this, "Novinka", "IQOS ILUMA PRIME\nPrémiová verze s moderním designem.");
    statusBar()->showMessage("Zobrazena novinka.", 3000);
}

void MainWindow::openSettings()
{
    QStringList themes;
    themes << "Light theme" << "Dark theme";

    bool ok;
    QString theme = QInputDialog::getItem(this, "Nastavení aplikace", "Vyber téma:", themes, darkTheme ? 1 : 0, false, &ok);

    if (!ok) return;

    if (theme == "Dark theme") {
        darkTheme = true;
        applyDarkStyles();
        statusBar()->showMessage("Tmavý režim zapnut.", 3000);
    } else {
        darkTheme = false;
        applyLightStyles();
        statusBar()->showMessage("Světlý režim zapnut.", 3000);
    }
}

void MainWindow::confirmExit()
{
    if (QMessageBox::question(this, "Konec", "Opravdu chceš ukončit aplikaci?") == QMessageBox::Yes) {
        close();
    }
}

void MainWindow::showAbout()
{
    QMessageBox::about(
        this,
        "O aplikaci",
        "PM Store\nGUI aplikace pro správu e-shopu.\nAutor: Shakira Khamzina\nQt projekt."
        );
}

void MainWindow::applyLightStyles()
{
    this->setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #F4E6EC;
        }

        QTabWidget::pane {
            border: none;
            background: #F4E6EC;
        }

        QTabBar::tab {
            background: #E3C5D5;
            color: #4A3B43;
            padding: 10px 18px;
            margin-right: 4px;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
        }

        QTabBar::tab:selected {
            background: #D9A7A7;
            font-weight: bold;
        }

        QMenuBar {
            background-color: #EADDD1;
            color: #4A3B43;
        }

        QMenuBar::item:selected {
            background: #E3C5D5;
        }

        QMenu {
            background-color: #F4E6EC;
            color: #4A3B43;
        }

        QMenu::item:selected {
            background-color: #E3C5D5;
        }

        QFrame#card {
            background-color: #EADDD1;
            border: 1px solid #D9A7A7;
            border-radius: 12px;
            padding: 12px;
        }

        QFrame#productCard {
            background-color: #F4E6EC;
            border: 1px solid #B58DAF;
            border-radius: 10px;
            padding: 10px;
        }

        QLabel {
            color: #4A3B43;
            font-size: 14px;
            background: transparent;
        }

        QLabel#mainTitle {
            font-size: 22px;
            font-weight: bold;
            color: #7B5A6A;
        }

        QLabel#sectionTitle {
            font-size: 18px;
            font-weight: bold;
            color: #7B5A6A;
        }

        QLabel#smallTitle {
            font-size: 13px;
            font-weight: bold;
            color: #7B5A6A;
        }

        QLabel#loginLabel {
            font-size: 16px;
            font-weight: bold;
            color: #8A5C6A;
        }

        QLabel#promoImage {
            background-color: #F8F1F4;
            border: 1px dashed #B58DAF;
            border-radius: 10px;
            color: #8A5C6A;
        }

        QPushButton {
            background-color: #E3C5D5;
            color: #4A3B43;
            border: 1px solid #B58DAF;
            border-radius: 8px;
            padding: 10px 14px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #D9A7A7;
        }

        QPushButton:pressed {
            background-color: #B58DAF;
            color: white;
        }

        QLineEdit, QTextEdit, QTableWidget {
            background-color: #F8F1F4;
            border: 1px solid #D9A7A7;
            border-radius: 8px;
            padding: 8px;
            color: #4A3B43;
        }
    )");
}

void MainWindow::applyDarkStyles()
{
    this->setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #2B252A;
            color: #F4E6EC;
        }

        QFrame#card {
            background-color: #3A3038;
            border: 1px solid #B58DAF;
            border-radius: 12px;
            padding: 12px;
        }

        QFrame#productCard {
            background-color: #443842;
            border: 1px solid #B58DAF;
            border-radius: 10px;
            padding: 8px;
        }

        QLabel {
            color: #F4E6EC;
            font-size: 14px;
            background: transparent;
        }

        QLabel#mainTitle, QLabel#sectionTitle, QLabel#smallTitle, QLabel#loginLabel {
            color: #E3C5D5;
            font-weight: bold;
        }

        QPushButton {
            background-color: #B58DAF;
            color: white;
            border: 1px solid #E3C5D5;
            border-radius: 8px;
            padding: 6px 10px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #D9A7A7;
        }

        QLineEdit, QTextEdit, QTableWidget {
            background-color: #4A3B43;
            border: 1px solid #B58DAF;
            border-radius: 8px;
            padding: 6px;
            color: white;
        }

        QTabBar::tab {
            background: #4A3B43;
            color: #F4E6EC;
            padding: 8px 14px;
            margin-right: 4px;
        }

        QTabBar::tab:selected {
            background: #B58DAF;
            font-weight: bold;
        }

        QMenuBar {
            background-color: #3A3038;
            color: #F4E6EC;
        }

        QMenu {
            background-color: #3A3038;
            color: #F4E6EC;
        }
    )");
}