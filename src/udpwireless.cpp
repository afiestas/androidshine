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

#include "udpwireless.h"

#include <QtNetwork/QUdpSocket>

#include <KDE/KIcon>
#include <KDE/KNotification>

UDPWireless::UDPWireless(QObject* parent)
    : QObject(parent)
    , m_udpSocket(new QUdpSocket(this))
    , m_extractor("^v(\\d+)/([^/]+)/([^/]+)/([^/]+)/([^/]*)/(.*)$")
{
    qDebug() << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    m_udpSocket->bind(10600);
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingNotifications()));
}

UDPWireless::~UDPWireless()
{

}

void UDPWireless::readPendingNotifications()
{
    qDebug() << "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
    while (m_udpSocket->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(m_udpSocket->pendingDatagramSize());
         QHostAddress sender;
         quint16 senderPort;
         m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
         const int pos = m_extractor.indexIn(datagram);
         if (pos > -1) {
             const QString version = m_extractor.cap(1);
             const QString deviceId = m_extractor.cap(2);
             const QString notificationId = m_extractor.cap(3);
             const QString eventType = m_extractor.cap(4);
             const QString data = m_extractor.cap(5);
             const QString eventContents = m_extractor.cap(6);

             QString type = "unknownEvent";
             QString icon = "pda";
             if (eventType == "RING") {
                 type = "callReceived";
                 icon = "call-start";
             } else if (eventType == "SMS") {
                 type = "smsReceived";
                 icon = "mail-receive";
             } else if (eventType == "MMS") {
                 type = "mmsReceived";
                 icon = "mail-receive";
             } else if (eventType == "BATTERY") {
                 type = "battery100";
                 icon = "battery-100"; // Here we need to take all different cases into account. All
                                       // possible steps on battery charge level and state (discharging
                                       // or charging)
             } else if (eventType == "PING") {
                 type = "pingReceived";
                 icon = "dialog-ok";
             }

            KComponentData componentData("androidshine", "androidshine");
            KNotification *notification = new KNotification(type);
            notification->setPixmap(KIcon(icon).pixmap(48, 48));
            notification->setComponentData(componentData);
            notification->setText(eventContents);
            notification->sendEvent();
         }
     }
}