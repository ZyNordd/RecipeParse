#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qmessagebox.h>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QTextCodec>
#include <qsqldatabase.h>
#include <qtimer.h>
#include <QPalette>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->radioButtonName->setChecked(true);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./Recipes.db");
    db.open();

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE Recipes(Name TEXT,Recipe TEXT, Ingredients TEXT,Calories INT,Protein INT,Fat INT,Carbohydrate INT, URL TEXT);"); //Создание базы данных с полями: имя, текст рецепта, ингредиенты, калории, БЖУ и уникального URL, который нужен для избежания добавления повторного рецепта.
    model = new QSqlTableModel(this, db);
    model->setTable("Recipes");
    model->select();

    ui->tableView->setModel(model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    for (const auto& file : fs::directory_iterator("./dll")) {
        if (!fs::is_directory(file)) {
            if (fs::path(file).extension() == ".dll")
            {
                fs::path p = fs::path(file);
                openDll(p.string());
            }
        }
    }

    std::ifstream fin("URList.txt");
    for (std::string line; std::getline(fin, line); )
        URList.insert(line);
    ui->labelCount->setText(QString::number(URList.size()));
    query->exec("SELECT COUNT(*) FROM Recipes");
    query->first();
    ui->labelCountRecipes->setText(QString::number(query->value(0).toInt()));
    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(managerFinished(QNetworkReply*)));

    URLfinder = new QNetworkAccessManager();
    QObject::connect(URLfinder, SIGNAL(finished(QNetworkReply*)), this, SLOT(URLfinderFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete query;
    delete ui;
    delete manager;
    delete model;
    delete URLfinder;

    closeDll();
}

#ifdef _WIN32
void MainWindow::insertDllIntoList(const HINSTANCE& mem, const fs::path& filepath) {
    std::pair elem = std::make_pair(mem, filepath);
    dynLibsList.push_back(elem);
}
#else
void MainWindow::insertDllIntoList(void* mem, fs::path& filepath) {
    std::pair elem = std::make_pair(mem, filepath);
    dynLibsList.push_back(elem);
#endif

void MainWindow::openDll(fs::path path) {
#ifdef _WIN32
    HINSTANCE load;
    load = LoadLibrary(path.string().c_str());
    MainWindow::insertDllIntoList(load, path);

#else
    void* load;
    load = dlopen(path.c_str(), RTLD_LAZY);
    insertDllIntoList(load, path);
#endif
}

void MainWindow::closeDll() {
#ifdef _WIN32
    for (auto iter = dynLibsList.begin(); iter != dynLibsList.end(); iter++) {
        FreeLibrary((*iter).first);
    }
#else
    for (auto iter = dynLibsList.begin(); iter != dynLibsList.end(); iter++) {
        dlclose((*iter).first);
    }
#endif
}

std::string MainWindow::getDomain(QString URL) {
    std::string bad_domain = URL.toStdString();
    std::string good_domain;
    std::size_t found = bad_domain.find("www.");
    if (found != std::string::npos) {
        found += 4;
        while (bad_domain[found] != '.') {
            good_domain += bad_domain[found];
            found++;
        }
    }
    else {
        std::size_t found = bad_domain.find("://");
        if (found != std::string::npos) {
            found += 3;
            while (bad_domain[found] != '.') {
                good_domain += bad_domain[found];
                found++;
            }
        }
        else {
            found = 0;
            while (bad_domain[found] != '.' && found != bad_domain.size()) {
                good_domain += bad_domain[found];
                found++;
            }
        }
    }
    return good_domain;
}

#ifdef _WIN32
HINSTANCE MainWindow::findDllInList(std::string name) {
    auto goal = name;
    goal.insert(goal.size(), ".dll", 0, 4);
    for (auto i = 0; i < dynLibsList.size(); ++i) {
        if (dynLibsList[i].second.filename() == goal) {
            return dynLibsList[i].first;
        }
    }
}
#else
void* MainWindow::findDllInList(std::string name) {
    auto goal = name;
    goal.insert(goal.size(), ".so", 0, 4);
    for (auto i = 0; i < dynLibsList.size(); ++i) {
        if (dynLibsList[i].second.filename() == goal) {
            return dynLibsList[i].first;
        }
    }
}
#endif

void MainWindow::on_pushButtonSingleGet_clicked() {
    isSingle = true;
    auto domainDLL = getDomain(ui->lineEditURL->text());
    domainDLL.insert(domainDLL.size(), ".dll", 0, 4);
    for (auto i = 0; i < dynLibsList.size(); ++i) {
        if (dynLibsList[i].second.filename() == domainDLL) {
            query->prepare("SELECT * FROM Recipes WHERE URL = :URL");
            query->bindValue(":URL", ui->lineEditURL->text());
            query->exec();
            bool isReplica = false;
            while (query->next()) {
                QString tmp_url = query->value("URL").toString();
                if (tmp_url == ui->lineEditURL->text()) {
                    isReplica = true;
                }
            }
            if (!isReplica) {
                URList.insert(ui->lineEditURL->text().toStdString());
                ui->labelCount->setText(QString::number(URList.size()));
                std::ofstream fout;
                fout.open("URList.txt");
                for (auto i = URList.begin(); i != URList.end(); i++) {
                    fout << *i << "\n";
                }
                fout.close();
                request.setUrl(QUrl(ui->lineEditURL->text()));
                manager->get(request);
                return;
            }
            else {
                QMessageBox::information(this, QString::fromLocal8Bit("Предупреждение"), QString::fromLocal8Bit("Уже есть в базе данных"));
                return;
            }
        }
    }
    QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Ссылка не исправна"));
}

void MainWindow::on_pushButtonFind_clicked() {
    if (ui->radioButtonIngredients->isChecked()) {
        QString tmpIngr;
        std::vector<QString> Ingrs;
        tmpIngr = ui->lineEditSearch->text();
        QString tmp;
        for (auto it = tmpIngr.begin(); it != tmpIngr.end(); ++it) {          
            if (*it == '&' || *it == ';' || *it == '.' || *it == ',') {
                Ingrs.push_back(tmp);
                tmp = "";
            }
            else if (*it == ' ') {
                if (!tmp.isEmpty()) {
                    tmp.resize(tmp.size() - 1);
                }
                continue;
            }

            else if (it == tmpIngr.end() - 1) {
                Ingrs.push_back(tmp);
            }
            else tmp += *it;   
        }
        QString filter;
        for (int a = 0; a < Ingrs.size(); a++) {
            QString Ingredients = Ingrs[a];
            QString Ingredients2 = Ingredients;
            //Ingredients2.replace(0, 1, QString(*Ingredients.begin()).toUpper());
            if (*Ingredients.begin() == QString(*Ingredients.begin()).toUpper())
                Ingredients2.replace(0,1, QString(*Ingredients2.begin()).toLower());
            else
                Ingredients2.replace(0,1, QString(*Ingredients2.begin()).toUpper());
            filter += " ((Ingredients LIKE '%"+Ingredients+"%') OR (Ingredients LIKE '%"+Ingredients2+"%')) ";
            if (a != Ingrs.size() - 1) filter += "AND";
        }
        //QString filter = "Ingredients LIKE '%яйцо%'";
        //QMessageBox::information(this, QString::fromLocal8Bit("ОШИБКА"), filter);
        model->setFilter(filter);
        model->select();
        ui->tableView->setModel(model);
    }
    else if (ui->radioButtonName->isChecked()){
        QString Name = ui->lineEditSearch->text();
        QString Name2 = Name;
        Name2.replace(*Name.begin(), QString(*Name.begin()).toUpper());
        if (*Name.begin() == QString(*Name.begin()).toUpper())
            Name2.replace(*Name2.begin(), QString(*Name2.begin()).toLower());
        else
            Name2.replace(*Name2.begin(), QString(*Name2.begin()).toUpper());
        QString filter = "Name LIKE '%" + Name + "%'"
            "OR Name LIKE '%" + Name2 + "%'";
        model->setFilter(filter);
        model->select();
        ui->tableView->setModel(model);
    }
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::on_pushButtonMultiDelete_clicked() {
    if (ui->lineEditSearch->text().isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QString::fromLocal8Bit("ОШИБКА"), QString::fromLocal8Bit("Вы хотите очистить всю таблицу?"), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            query->exec("DELETE FROM Recipes");
            query->exec("VACUUM;");
            model->select();
            ui->tableView->setModel(model);
        }
    }
    else if (ui->radioButtonIngredients->isChecked()) {
        QString tmpIngr;
        std::vector<QString> Ingrs;
        tmpIngr = ui->lineEditSearch->text();
        QString tmp;
        for (auto it = tmpIngr.begin(); it != tmpIngr.end(); ++it) {
            if (*it == '&' || *it == ';' || *it == '.' || *it == ',') {
                Ingrs.push_back(tmp);
                tmp = "";
            }
            else if (*it == ' ') {
                if (!tmp.isEmpty()) {
                    tmp.resize(tmp.size() - 1);
                }
                continue;
            }

            else if (it == tmpIngr.end() - 1) {
                Ingrs.push_back(tmp);
            }
            else tmp += *it;
        }
        QString filter;
        for (int a = 0; a < Ingrs.size(); a++) {
            QString Ingredients = Ingrs[a];
            QString Ingredients2 = Ingredients;
            //Ingredients2.replace(0, 1, QString(*Ingredients.begin()).toUpper());
            if (*Ingredients.begin() == QString(*Ingredients.begin()).toUpper())
                Ingredients2.replace(0, 1, QString(*Ingredients2.begin()).toLower());
            else
                Ingredients2.replace(0, 1, QString(*Ingredients2.begin()).toUpper());
            filter += " ((Ingredients LIKE '%" + Ingredients + "%') OR (Ingredients LIKE '%" + Ingredients2 + "%')) ";
            if (a != Ingrs.size() - 1) filter += "AND";
        }
        //QString filter = "Ingredients LIKE '%яйцо%'";
        //QMessageBox::information(this, QString::fromLocal8Bit("ОШИБКА"), filter);
        filter = "DELETE FROM Recipes WHERE " + filter;
        query->exec(filter);
        model->select();
        ui->tableView->setModel(model);
    }
    else if (ui->radioButtonName->isChecked()) {
        QString Name = ui->lineEditSearch->text();
        QString Name2 = Name;
        Name2.replace(*Name.begin(), QString(*Name.begin()).toUpper());
        if (*Name.begin() == QString(*Name.begin()).toUpper())
            Name2.replace(*Name2.begin(), QString(*Name2.begin()).toLower());
        else
            Name2.replace(*Name2.begin(), QString(*Name2.begin()).toUpper());
        QString filter = "Name LIKE '%" + Name + "%'"
            "OR Name LIKE '%" + Name2 + "%'";
        filter = "DELETE FROM Recipes WHERE " + filter;
        QMessageBox::information(this, QString::fromLocal8Bit("ОШИБКА"), filter);
        query->exec(filter);
        model->select();
        ui->tableView->setModel(model);
    }
    query->exec("SELECT COUNT(*) FROM Recipes");
    query->first();
    ui->labelCountRecipes->setText(QString::number(query->value(0).toInt()));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::on_pushButtonDelete_clicked() {
    model->removeRow(row);
    query->exec("VACUUM;");
    model->setTable("Recipes");
    model->select();
    ui->tableView->setModel(model);
    query->exec("SELECT COUNT(*) FROM Recipes");
    query->first();
    ui->labelCountRecipes->setText(QString::number(query->value(0).toInt()));
}

void MainWindow::on_pushButtonScanURLs_clicked() {
    isGoing = true;
    ui->pushButtonScanURLs->setStyleSheet("background-color:lightgreen");
    ui->pushButtonStop->setStyleSheet("background-color:red");
    oldValueURL = URList.size();
    for (int i = 1; i < 715; ++i) {
        if (isGoing) {
            tmpURL = "https://eda.ru/recepty?page=";
            std::string i_str = std::to_string(i);
            tmpURL.insert(28, i_str, 0, i_str.size());
            request.setUrl(QUrl(QString::fromStdString(tmpURL)));
            QNetworkReply* reply = URLfinder->get(request);
            QEventLoop loop;
            connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
        }
    }
    std::ofstream fout;          
    fout.open("URList.txt");
    for (auto i = URList.begin(); i != URList.end(); i++) {
        fout << *i << "\n";
    }
    fout.close();
    newValueURL = URList.size();
    int diff = newValueURL - oldValueURL;
    QMessageBox::information(this, QString::fromLocal8Bit("Успешно"), (QString::fromLocal8Bit("Список ссылок обновлен/создан. Добавлено ") + QString::number(diff) + QString::fromLocal8Bit(" ссылок.")));
    ui->pushButtonScanURLs->setStyleSheet("background-color:midlight");
    ui->pushButtonStop->setStyleSheet("background-color:midlight");
}

void MainWindow::on_pushButtonMultiGet_clicked() {
    isSingle = false;
    isGoing = true;
    ui->pushButtonMultiGet->setStyleSheet("background-color:lightgreen");
    ui->pushButtonStop->setStyleSheet("background-color:red");
    for (auto i = URList.begin(); i != URList.end(); ++i) {
        if (isGoing) {
            query->prepare("SELECT * FROM Recipes WHERE URL = :URL");
            query->bindValue(":URL", QString::fromStdString(*i));
            query->exec();
            bool isReplica = false;
            while (query->next()) {
                QString tmp_url = query->value("URL").toString();
                if (tmp_url == QString::fromStdString(*i)) {
                    isReplica = true;
                }
            }
            if (!isReplica) {
                request.setUrl(QUrl(QString::fromStdString(*i)));
                QNetworkReply* reply = manager->get(request);
                QEventLoop loop;
                connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();
            }
        }
    }
}

void MainWindow::on_pushButtonStop_clicked() {
    isGoing = false;
    ui->pushButtonMultiGet->setStyleSheet("background-color:midlight");
    ui->pushButtonStop->setStyleSheet("background-color:midlight");
    ui->pushButtonScanURLs->setStyleSheet("background-color:midlight");
}

void MainWindow::URLfinderFinished(QNetworkReply* reply) {
    QString html_code = reply->readAll();
    typedef std::string(*parser_str) (std::string);
    parser_str URLs;

#ifdef _WIN32
    HINSTANCE load = findDllInList(getDomain(reply->url().toString()));
    URLs = (parser_str)*GetProcAddress(load, "URLs");
#else
    void* load = findDllInList(getDomain(reply->url().toString()));
    URLs = (std::string(*)(std::string))dlsym(load, "URLs");
#endif
    std::string tmpURLs = URLs(html_code.toStdString());
    std::string singleURL = "";
    for (auto i = 0; i < tmpURLs.size(); ++i) {
        if(tmpURLs[i] != '\n') singleURL.push_back(tmpURLs[i]);
        else {
            URList.insert(singleURL);
            singleURL = "";
        }
    }
    ui->labelCount->setText(QString::number(URList.size()));
}

void MainWindow::managerFinished(QNetworkReply* reply) {
    QString html_code = reply->readAll();

    typedef std::string(*parser_str) (std::string);
    typedef int(*parser_num) (std::string);

    parser_str ingredients, name, recipe;
    parser_num calories, protein, fat, carbohydrate;
#ifdef _WIN32
    HINSTANCE load = findDllInList(getDomain(reply->url().toString()));
    ingredients = (parser_str)*GetProcAddress(load, "ingredients");
    name = (parser_str)*GetProcAddress(load, "name");
    calories = (parser_num)*GetProcAddress(load, "calories");
    protein = (parser_num)*GetProcAddress(load, "protein");
    fat = (parser_num)*GetProcAddress(load, "fat");
    carbohydrate = (parser_num)*GetProcAddress(load, "carbohydrate");
    recipe = (parser_str)*GetProcAddress(load, "recipe");
#else
    void* load = findDllInList(getDomain(reply->url().toString()));
    URLs = (std::string(*)(std::string))dlsym(load, "URLs");
    ingredients = (std::string(*)(std::string))dlsym(load, "URLs");
    name = (std::string(*)(std::string))dlsym(load, "URLs");
    calories = (int(*)(std::string))dlsym(load, "URLs");
    protein = (int(*)(std::string))dlsym(load, "URLs");
    fat = (int(*)(std::string))dlsym(load, "URLs");
    carbohydrate = (int(*)(std::string))dlsym(load, "URLs");
    recipe = (std::string(*)(std::string))dlsym(load, "URLs");
#endif

    QString ingredients_date = QString::fromStdString(ingredients(html_code.toStdString()));
    QString name_date = QString::fromStdString(name(html_code.toStdString()));
    qint64 calories_date = (qint64)calories(html_code.toStdString());
    qint64 protein_date = (qint64)protein(html_code.toStdString());
    qint64 fat_date = (qint64)fat(html_code.toStdString());
    qint64 carbohydrate_date = (qint64)carbohydrate(html_code.toStdString());
    QString recipe_date = QString::fromStdString(recipe(html_code.toStdString()));

    if (ingredients_date == "" && 
        name_date == "" && 
        calories_date == 0 && 
        protein_date == 0 && 
        fat_date == 0 && 
        carbohydrate_date == 0 && 
        recipe_date == "") {
        if(isSingle)
            QMessageBox::information(this, QString::fromLocal8Bit("Ошибка"), QString::fromLocal8Bit("Ссылка устарела, на странице нет данных."));
        else {
        }
    }
    else {      
        query->prepare("INSERT INTO Recipes (Name, Recipe, Ingredients, Calories, Protein, Fat, Carbohydrate, URL)"
            "VALUES"
            "(:Name, :Recipe, :Ingredients, :Calories, :Protein, :Fat, :Carbohydrate, :URL)");
        query->bindValue(":Name", name_date);
        query->bindValue(":Recipe", recipe_date);
        query->bindValue(":Ingredients", ingredients_date);
        query->bindValue(":Calories", calories_date);
        query->bindValue(":Protein", protein_date);
        query->bindValue(":Fat", fat_date);
        query->bindValue(":Carbohydrate", carbohydrate_date);
        query->bindValue(":URL", reply->url().toString());
        query->exec();

        model->select();
        ui->tableView->setModel(model);
    }
    query->exec("SELECT COUNT(*) FROM Recipes");
    query->first();
    ui->labelCountRecipes->setText(QString::number(query->value(0).toInt()));
}

void MainWindow::on_tableView_clicked(const QModelIndex& index) {
    row = index.row(); 
}