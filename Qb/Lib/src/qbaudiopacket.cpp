/* Webcamoid, webcam capture application.
 * Copyright (C) 2011-2015  Gonzalo Exequiel Pedone
 *
 * Webcamoid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamoid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamoid. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email   : hipersayan DOT x AT gmail DOT com
 * Web-Site: http://github.com/hipersayanX/webcamoid
 */

#include "qbaudiopacket.h"

class QbAudioPacketPrivate
{
    public:
        QbAudioCaps m_caps;
};

QbAudioPacket::QbAudioPacket(QObject *parent):
    QbPacket(parent)
{
    this->d = new QbAudioPacketPrivate();
}

QbAudioPacket::QbAudioPacket(const QbAudioCaps &caps,
                             const QbBufferPtr &buffer,
                             ulong bufferSize,
                             qint64 pts,
                             const QbFrac &timeBase,
                             int index,
                             qint64 id)
{
    this->d = new QbAudioPacketPrivate();
    this->d->m_caps = caps;
    this->buffer() = buffer;
    this->bufferSize() = bufferSize;
    this->pts() = pts;
    this->timeBase() = timeBase;
    this->index() = index;
    this->id() = id;
}

QbAudioPacket::QbAudioPacket(const QbPacket &other)
{
    this->d = new QbAudioPacketPrivate();
    this->d->m_caps = other.caps();
    this->data() = other.data();
    this->buffer() = other.buffer();
    this->bufferSize() = other.bufferSize();
    this->pts() = other.pts();
    this->timeBase() = other.timeBase();
    this->index() = other.index();
    this->id() = other.id();
}

QbAudioPacket::QbAudioPacket(const QbAudioPacket &other):
    QbPacket()
{
    this->d = new QbAudioPacketPrivate();
    this->d->m_caps = other.d->m_caps;
    this->data() = other.data();
    this->buffer() = other.buffer();
    this->bufferSize() = other.bufferSize();
    this->pts() = other.pts();
    this->timeBase() = other.timeBase();
    this->index() = other.index();
    this->id() = other.id();
}

QbAudioPacket::~QbAudioPacket()
{
    delete this->d;
}

QbAudioPacket &QbAudioPacket::operator =(const QbPacket &other)
{
    this->d->m_caps = other.caps();
    this->data() = other.data();
    this->buffer() = other.buffer();
    this->bufferSize() = other.bufferSize();
    this->pts() = other.pts();
    this->timeBase() = other.timeBase();
    this->index() = other.index();
    this->id() = other.id();

    return *this;
}

QbAudioPacket &QbAudioPacket::operator =(const QbAudioPacket &other)
{
    if (this != &other) {
        this->d->m_caps = other.d->m_caps;
        this->data() = other.data();
        this->buffer() = other.buffer();
        this->bufferSize() = other.bufferSize();
        this->pts() = other.pts();
        this->timeBase() = other.timeBase();
        this->index() = other.index();
        this->id() = other.id();
    }

    return *this;
}

QbAudioPacket::operator bool() const
{
    return this->d->m_caps.isValid();
}

QbAudioCaps QbAudioPacket::caps() const
{
    return this->d->m_caps;
}

QbAudioCaps &QbAudioPacket::caps()
{
    return this->d->m_caps;
}

QString QbAudioPacket::toString() const
{
    QString packetInfo;
    QDebug debug(&packetInfo);

    debug.nospace() << "Caps       : "
                    << this->d->m_caps.toString().toStdString().c_str()
                    << "\n";

    debug.nospace() << "Data       : "
                    << this->data()
                    << "\n";

    debug.nospace() << "Buffer Size: "
                    << this->bufferSize()
                    << "\n";

    debug.nospace() << "Id         : "
                    << this->id()
                    << "\n";

    debug.nospace() << "Pts        : "
                    << this->pts()
                    << " ("
                    << this->pts() * this->timeBase().value()
                    << ")\n";

    debug.nospace() << "Time Base  : "
                    << this->timeBase().toString().toStdString().c_str()
                    << "\n";

    debug.nospace() << "Index      : "
                    << this->index();

    return packetInfo;
}

QbPacket QbAudioPacket::toPacket() const
{
    QbPacket packet;
    packet.caps() =  this->d->m_caps.toCaps();
    packet.buffer() = this->buffer();
    packet.bufferSize() = this->bufferSize();
    packet.pts() = this->pts();
    packet.timeBase() = this->timeBase();
    packet.index() = this->index();
    packet.id() = this->id();

    return packet;
}

void QbAudioPacket::setCaps(const QbAudioCaps &caps)
{
    if (this->d->m_caps == caps)
        return;

    this->d->m_caps = caps;
    emit this->capsChanged(caps);
}

void QbAudioPacket::resetCaps()
{
    this->setCaps(QbAudioCaps());
}

QDebug operator <<(QDebug debug, const QbAudioPacket &packet)
{
    debug.nospace() << packet.toString().toStdString().c_str();

    return debug.space();
}
