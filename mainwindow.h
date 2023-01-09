#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqltablemodel.h>
#include <QTableView>
#include <filesystem>
#include <fstream>
#include <qthread.h>
#include <unordered_set>
#include <qmessagebox.h>
#include <QFile>
#include <QDataStream>
#include <QTextCodec>
#include <qtimer.h>
#include <QPalette>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace fs = std::filesystem;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        /// Конструктор, в котором создаются соединения сетевых менеджеров с методами, отвечающими за обработку ответов. Также происходит открытие базы данных и поиск доступных динамических библиотек.
        MainWindow(QWidget *parent = nullptr);
        /// Деструктор, в котором происходит очистка занятой памяти и закрытие динамических библиотек.
        ~MainWindow();

        /// Метод, отвечающий за создание вектора, хранящего дескрипторы и пути динамических библиотек.
    #ifdef _WIN32
        static void insertDllIntoList(const HINSTANCE& mem, const fs::path& filepath);
    #else
        static void insertDllIntoList(void* mem, fs::path& filepath);
    #endif
        /// Метод, осуществляющий открытие динамической библиотеки.
        void openDll(fs::path path);
        /// Метод, осуществляющий закрытие всех динамических библиотек.
        void closeDll();
        /// Метод, определяющий домен ссылки.
        std::string getDomain(QString URL);

        /// Метод находит нужную динамическую библиотеку из вектора по домену.
    #ifdef _WIN32
        HINSTANCE findDllInList(std::string name);
    #else
        void* findDllInList(std::string name);
    #endif

    private:
        /// Oбъект, который предоставляет код для настройки и управления пользовательским интерфейсом.
        Ui::MainWindow *ui;
        /// Менеджер предоставляет возможность делать сетевые запросы.
        QNetworkAccessManager* manager;
        /// Еще один менеджер предоставляющий возможность делать сетевые запросы.
        QNetworkAccessManager* URLfinder;
        /// Объект, содержащий запрос, который нужно отрпавить с помощью QNetworkAccessManager.
        QNetworkRequest request;
        /// Объект для обработки соединения с базой данных.
        QSqlDatabase db;
        /// Объект предоставляет средства для выполнения операторов SQL и управления ими.
        QSqlQuery* query;
        /// Объект, позволяющий выводить базу данных в окно приложения.
        QSqlTableModel* model;
        /// Вектор, хранящий все уникальные ссылки с рецептами, по которым будут произвеедны запросы с помощью manager.
        std::unordered_set<std::string> URList;
        ///Номер строки таблицы, на которую нажали.
        int row; 
        /// Строка, в которой хранятся ссылки главной страницы сайта для последующих запросов с помощью URLfinder с целью получения ссылок на рецепты.
        std::string tmpURL;
        /// Переменная, определяющая множественный или единичный запрос был сделан.
        bool isSingle = false;
        /// Переменная для остановки процесса парсинга.
        bool isGoing = true;
        /// Переменная, хранящая количество ссылок до обновления списка.
        int oldValueURL = 0;
        /// Переменная, хранящая количество ссылок после обновления списка.
        int newValueURL = 0;


        ///Вектор, хранящий дескрипторы и пути динамических библиотек.
    #ifdef _WIN32
        inline static std::vector<std::pair<HINSTANCE, fs::path>> dynLibsList;
    #else
        inline static std::vector<std::pair<void*, fs::path>> dynLibsList;
    #endif

    private slots:
        /// Метод нажатия на кнопку для запроса по введенной ссылке.
        void on_pushButtonSingleGet_clicked(); 
        /// Метод нажатия на кнопку для поиска нужных рецептов в таблице.
        void on_pushButtonFind_clicked();
        /// Метод нажатия на кнопку для удаления рецептов по фильтру.
        void on_pushButtonMultiDelete_clicked();
        /// Метод нажатия на кнопку для удаления выделенной строки.
        void on_pushButtonDelete_clicked();
        /// Метод нажатия на кнопку для запуска процесса получения всех ссылок на рецепты с главного меню сайта.
        void on_pushButtonScanURLs_clicked();
        /// Метод нажатия на кнопку для запуска процесса получения всех рецептов по ссылкам, хранящимся в векторе URLs.
        void on_pushButtonMultiGet_clicked();
        /// Метод нажатия на кнопку для остановки процессов.
        void on_pushButtonStop_clicked();
        /// Метод обработки запроса URLfinder.
        void URLfinderFinished(QNetworkReply* reply);
        /// Метод обработки запроса manager.
        void managerFinished(QNetworkReply* reply);
        /// Метод для выделения строки в таблице.
        void on_tableView_clicked(const QModelIndex &index);
};
#endif // MAINWINDOW_H
