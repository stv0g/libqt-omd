#ifndef LIVEVIEW_H
#define LIVEVIEW_H

#include <QUdpSocket>

class LiveView
{
public:
    LiveView();

    void start();
    void stop();

protected:
    int          port = 48482;
    QUdpSocket   socket;
};

#endif // LIVEVIEW_H
