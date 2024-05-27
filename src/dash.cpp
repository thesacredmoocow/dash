#include <QApplication>
#include <QStringList>
#include <QWindow>

#include "app/window.hpp"
#include "app/action.hpp"
#include <unistd.h>

bool is_interface_online(std::string interface) {
    struct ifreq ifr;
    int sock = socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP);
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, interface.c_str());
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
            perror("SIOCGIFFLAGS");
    }
    close(sock);
    return !!(ifr.ifr_flags & IFF_UP);
}

int main(int argc, char *argv[])
{
    //for(int i = 0; i < 50; i++) {
        //system("ip link add dev vcan0 type vcan");
        //system("ip link set vcan0 mtu 16");
        //system("ip link set up vcan0");
        system("ip link set can0 type can bitrate 500000");
        system("ip link set up can0");
    //    sleep(10);
    //    if(is_interface_online("vcan0")) {
    //        break;
    //    }
    //}
    

    QApplication dash(argc, argv);

    dash.setOrganizationName("openDsh");
    dash.setApplicationName("dash");
    dash.installEventFilter(ActionEventFilter::get_instance());

    QSize size = dash.primaryScreen()->size();
    QPoint pos = dash.primaryScreen()->geometry().topLeft();
    bool fullscreen = true;

    QSettings settings;
    DASH_LOG(info) << "loaded config: " << settings.fileName().toStdString();

    QStringList args = dash.arguments();
    if (args.size() > 2) {
        size = QSize(args.at(1).toInt(), args.at(2).toInt());
        if (args.size() > 4)
            pos = QPoint(args.at(3).toInt(), args.at(4).toInt());
        fullscreen = false;
    }
    else {
        settings.beginGroup("Window");
        if (settings.contains("size")) {
            size = settings.value("size").toSize();
            if (settings.contains("pos"))
                pos = settings.value("pos").toPoint();
            fullscreen = false;
        }
    }

    QPixmap pixmap(QPixmap(":/splash.png").scaledToHeight(size.height() / 2));
    QSplashScreen splash(pixmap);
    splash.setMask(pixmap.mask());
    splash.move(pos.x() + ((size.width() / 2) - (splash.width() / 2)), pos.y() + ((size.height() / 2) - (splash.height() / 2)));
    splash.show();
    dash.processEvents();

    MainWindow window(QRect(pos, size));
    window.setWindowIcon(QIcon(":/logo.png"));
    window.setWindowFlags(Qt::FramelessWindowHint);
    if (fullscreen)
        window.setWindowState(Qt::WindowFullScreen);

    window.show();
    splash.finish(&window);

    return dash.exec();
}
