
#include <KAboutData>
#include <KApplication>
#include <KCmdLineArgs>

#include <main-window.h>

int main(int argc, char **argv) {
    KAboutData aboutData("person-viewer", 0, ki18n("PIMO:Person Viewer"), "0.1",
                         ki18n("PIMO:Person Viewer"), KAboutData::License_GPL_V2,
                         ki18n("(C) 2011, Martin Klapetek"));

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
