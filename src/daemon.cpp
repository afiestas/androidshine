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
#include "udpwireless.h"

#include <KDE/KPluginFactory>

K_PLUGIN_FACTORY(AndroidShineFactory,
                 registerPlugin<Daemon>();)
K_EXPORT_PLUGIN(AndroidShineFactory("androidshine", "androidshine"))

Daemon::Daemon(QObject *parent, const QList<QVariant>&)
    : KDEDModule(parent)
{
    m_udpWireless = new UDPWireless(this);
}

Daemon::~Daemon()
{

}


