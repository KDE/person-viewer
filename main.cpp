/*
    Copyright (C) 2011  Martin Klapetek <mklapetek@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <KAboutData>
#include <KApplication>
#include <KCmdLineArgs>

#include <main-window.h>

int main(int argc, char **argv) {
    KAboutData aboutData("person-viewer", 0, ki18n("PIMO:Person Viewer"), "0.1",
                         ki18n("PIMO:Person Viewer"), KAboutData::License_GPL_V2,
                         ki18n("(C) 2012, 2013, Martin Klapetek"));

    aboutData.addAuthor(ki18nc("@info:credit", "Martin Klapetek"), ki18n("Developer"),
                        "martin.klapetek@gmail.com");
    aboutData.setProductName("kdepim/person-viewer"); //set the correct name for bug reporting

    KCmdLineArgs::init(argc, argv, &aboutData);

    KApplication app;

    MainWindow *mainWindow = new MainWindow(0);
    mainWindow->show();

    app.exec();

    return 0;
}
