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
        int          mPort = 48482;
        QUdpSocket   mSocket;

        Camera *mCam;
};

#endif // LIVEVIEW_H
