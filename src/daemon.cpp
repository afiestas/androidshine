/**
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

#include "daemon.h"

#include <QtNetwork/QUdpSocket>

#include <KDE/KPluginFactory>
#include <KDE/KNotification>

K_PLUGIN_FACTORY(AndroidShineFactory,
                 registerPlugin<Daemon>();)
K_EXPORT_PLUGIN(AndroidShineFactory("androidshine", "androidshine"))

Daemon::Daemon(QObject *parent, const QList<QVariant>&)
    : KDEDModule(parent)
    , m_udpSocket(new QUdpSocket(this))
{
    m_udpSocket->bind(10600);
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingNotifications()));
}

Daemon::~Daemon()
{

}

void Daemon::readPendingNotifications()
{
    while (m_udpSocket->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(m_udpSocket->pendingDatagramSize());
         QHostAddress sender;
         quint16 senderPort;
         m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

         const QRegExp extractor("^v(\\d+)/([^/]+)/([^/]+)/([^/]+)/([^/]*)/(.*)$");
         const int pos = extractor.indexIn(datagram);
         if (pos > -1) {
             const QString version = extractor.cap(1);
             const QString deviceId = extractor.cap(2);
             const QString notificationId = extractor.cap(3);
             const QString eventType = extractor.cap(4);
             const QString data = extractor.cap(5);
             const QString eventContents = extractor.cap(6);

             QString type = "unknownEvent";
             if (eventType == "RING") {
                 type = "callReceived";
             } else if (eventType == "SMS") {
                 type = "smsReceived";
             } else if (eventType == "MMS") {
                 type = "mmsReceived";
             } else if (eventType == "BATTERY") {
                 type = "batteryChanged";
             } else if (eventType == "PING") {
                 type = "pingReceived";
             }

            KComponentData componentData("androidshine", "androidshine");
            KNotification *notification = new KNotification(type);
            notification->setComponentData(componentData);
            notification->setText(eventContents);
            notification->sendEvent();
         }
     }
}
