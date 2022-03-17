#include "interface.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Interface interfaceObject;
    QPalette palette = app.palette();
    QColor gainsboro(222, 226, 230);
    palette.setColor(QPalette::Window, gainsboro);
    app.setPalette(palette);

    interfaceObject.show();
    return app.exec();
}
