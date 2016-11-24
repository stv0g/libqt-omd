/** Oi LiveView Class
 *
 * @copyright	2015 Steffen Vogel
 * @license     http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author      Steffen Vogel <post@steffenvogel.de>
 * @link        https://dev.0l.de/wiki/projects/omd/start
 * @package     libqt-omd
 */
/*
 * This file is part of libqt-omd
 *
 * libqt-omd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libqt-omd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libqt-omd. If not, see <http://www.gnu.org/licenses/>.
 */

#include "liveview.h"

using namespace Oi;

LiveView::LiveView(Camera *c) :
    mCam(c),
    mSocket(this)
{ }

void LiveView::start()
{
    mSocket->bind(QHostAddress::LocalHost, mPort);

    mCam->get()

    connect(mSocket, &QUdpSocket::readyRead, this, &LiveView::readPendingChunks);
}

void LiveView::stop()
{

}

void LiveView::readPendingChunks()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;

        datagram.resize(udpSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        mSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        processChunk(datagram);
    }
}

void LiveView::processChunk(QByteArray chunk)
{
    const char *data = chunk.constData();
    const char *payload = data + 12;

    const quint16 *packetType = reinterpret_cast<const quint16 *>(data + 0);
    const quint16 *chunkNo    = reinterpret_cast<const quint16 *>(data + 2);
    const quint32 *frameId    = reinterpret_cast<const quint32 *>(data + 4);
    const quint32 *streamId   = reinterpret_cast<const quint16 *>(data + 8);

    qDebug() << "packetType: " << *packetType << " chunkNo: " << *chunkNo << " frameId: " << *frameId << " streamId: " << *streamId;

    switch (*packetType) {
        case 0x9060: /* Start of Frame */
            break;

        case 0x8060: /* Middle of Frame */
            break;

        case 0x80e0: /* End of Frame */
            break;
    }
}

void LiveView::processFrame()
{

}

