/*
 * This file is part of Android Shine.
 * Copyright (C) 2011 Rafael Fernández López <ereslibre@kde.org>
 *
 * Android Shine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Android Shine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Android Shine.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>

#include "mainwindow.h"

int main (int argc, char **argv)
{
    KAboutData aboutData("androidshine", 0, ki18n("Android Shine"), "0.1",
                         ki18n("Notifies events on your Android phone"),
                         KAboutData::License_GPL_V3,
                         ki18n("(c) 2011 Rafael Fernández López"),
                         ki18n("Dragons and Robots are now friends"),
                         "http://www.kde.org/",
                         "ereslibre@kde.org");
 
    KCmdLineArgs::init(argc, argv, &aboutData);    
    KApplication app;

    MainWindow *window = new MainWindow();
    window->show();

    return app.exec();
}