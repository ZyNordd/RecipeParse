#include "mainwindow.h"
#include "parser.h"
#include <QApplication>
#include <QNetworkAccessManager>
#include <QObject>
#include <iostream>


int main(int argc, char* argv[]) {
    //qDebug() << "SslSupport: " << QSslSocket::supportsSsl();
    //qDebug() << "SslLibraryBuildVersion: " << QSslSocket::sslLibraryBuildVersionString();
    //qDebug() << "SslLibraryRuntimeVersion: " << QSslSocket::sslLibraryVersionString();
    auto* a = new QApplication(argc, argv);
    MainWindow w;
    w.show();
    return (*a).exec();
}
