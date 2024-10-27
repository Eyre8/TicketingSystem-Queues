#include <QApplication>
#include "ticketsystemwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TicketSystemWindow window;
    window.show();

    return app.exec();
}
