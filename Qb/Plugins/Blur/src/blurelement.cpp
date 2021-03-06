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

#include "blurelement.h"

BlurElement::BlurElement(): QbElement()
{
    this->m_radius = 5;
}

QObject *BlurElement::controlInterface(QQmlEngine *engine, const QString &controlId) const
{
    Q_UNUSED(controlId)

    if (!engine)
        return NULL;

    // Load the UI from the plugin.
    QQmlComponent component(engine, QUrl(QStringLiteral("qrc:/Blur/share/qml/main.qml")));

    // Create a context for the plugin.
    QQmlContext *context = new QQmlContext(engine->rootContext());
    context->setContextProperty("Blur", (QObject *) this);
    context->setContextProperty("controlId", this->objectName());

    // Create an item with the plugin context.
    QObject *item = component.create(context);
    context->setParent(item);

    return item;
}

int BlurElement::radius() const
{
    return this->m_radius;
}

void BlurElement::integralImage(const QImage &image,
                                int oWidth, int oHeight,
                                PixelU32 *integral)
{
    for (int y = 1; y < oHeight; y++) {
        const QRgb *line = (const QRgb *) image.constScanLine(y - 1);

        // Reset current line summation.
        PixelU32 sum;

        for (int x = 1; x < oWidth; x++) {
            QRgb pixel = line[x - 1];

            // Accumulate pixels in current line.
            sum += pixel;

            // Offset to the current line.
            int offset = x + y * oWidth;

            // Offset to the previous line.
            // equivalent to x + (y - 1) * oWidth;
            int offsetPrevious = offset - oWidth;

            // Accumulate current line and previous line.
            integral[offset] = sum + integral[offsetPrevious];
        }
    }
}

void BlurElement::setRadius(int radius)
{
    if (this->m_radius == radius)
        return;

    this->m_radius = radius;
    emit this->radiusChanged(radius);
}

void BlurElement::resetRadius()
{
    this->setRadius(5);
}

QbPacket BlurElement::iStream(const QbPacket &packet)
{
    QImage src = QbUtils::packetToImage(packet);

    if (src.isNull())
        return QbPacket();

    src = src.convertToFormat(QImage::Format_ARGB32);
    QImage oFrame(src.size(), src.format());

    int oWidth = src.width() + 1;
    int oHeight = src.height() + 1;
    PixelU32 *integral = new PixelU32[oWidth * oHeight];
    this->integralImage(src, oWidth, oHeight, integral);

    int radius = this->m_radius;

    for (int y = 0; y < src.height(); y++) {
        const QRgb *iLine = (const QRgb *) src.constScanLine(y);
        QRgb *oLine = (QRgb *) oFrame.scanLine(y);
        int yp = qMax(y - radius, 0);
        int kh = qMin(y + radius, src.height() - 1) - yp + 1;

        for (int x = 0; x < src.width(); x++) {
            int xp = qMax(x - radius, 0);
            int kw = qMin(x + radius, src.width() - 1) - xp + 1;

            PixelU32 sum = integralSum(integral, oWidth, xp, yp, kw, kh);
            PixelU32 mean = sum / quint32(kw * kh);

            oLine[x] = qRgba(mean.r, mean.g, mean.b, qAlpha(iLine[x]));
        }
    }

    delete [] integral;

    QbPacket oPacket = QbUtils::imageToPacket(oFrame, packet);
    qbSend(oPacket)
}
