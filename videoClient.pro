# -------------------------------------------------
# Project created by QtCreator 2011-01-26T15:43:30
# -------------------------------------------------

QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = videoClient
TEMPLATE = app

CONFIG += mobility
MOBILITY =

SOURCES += \
    main.cpp \
    tcpclient.cpp \
    videoClient.cpp
HEADERS += videoClient.h \
    spcaframe.h \
	tcpclient.h
#INCLUDEPATH +=	/usr/include/opencv
#LIBS += -lopencv_core -lopencv_ml -lopencv_highgui -lopencv_imgproc -lopencv_objdetect
FORMS += videoClient.ui
RESOURCES += videoClient.qrc
QT *= svg
OTHER_FILES += \
    android/res/layout/splash.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-es/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-es/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-es/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-es/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-es/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/version.xml \
    android/AndroidManifest.xml

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat
