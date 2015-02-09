#ifndef LIVEVIEW_H
#define LIVEVIEW_H

#include <QUdpSocket>

#include "camera.h"

namespace Oi {
    class Camera;
    class LiveView;
}

class Oi::LiveView
{
    public:
        LiveView(Oi::Camera *c);

        void start();
        void stop();

    protected:
        int          port = 48482;
        QUdpSocket   socket;

        Camera *cam;
};

#endif // LIVEVIEW_H
