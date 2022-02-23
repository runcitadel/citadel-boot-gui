/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This logoFile is part of the examples of the Qt Wayland module
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this logoFile in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this logoFile under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "config.h"
#include "MainWidget.h"

#include "QrCode.hpp"

#include <QFile>
#include <QExposeEvent>
#include <QResizeEvent>
#include <QOpenGLWidget>
#include <QPainter>
#include <QSvgRenderer>
#include <QHostAddress>
#include <QNetworkInterface>

MainWidget::MainWidget()
	:QOpenGLWidget()
{
}

void MainWidget::paintGL()
{
    // Get logo
    QFile logoFile(":/citadel-logo.svg");
    QFile QRLogoFile(":/citadel-qr-icon.svg");
    logoFile.open(QIODevice::ReadOnly);
    QRLogoFile.open(QIODevice::ReadOnly);
    QString logoString = QTextStream(&logoFile).readAll();
    QString qrLogoString = QTextStream(&QRLogoFile).readAll();

    // Get tor host name
    QFile torHostNamelogoFile("/home/citadel/citadel/tor/data/web/hostname");
    torHostNamelogoFile.open(QIODevice::ReadOnly);
    QTextStream hostNameTextStream(&torHostNamelogoFile);
    QString torHostName = "http://" + hostNameTextStream.readAll();

    // Get IP
    QString ipAddress;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            ipAddress = address.toString();
        }
    }

    // Prepare images
    QImage backgroundColorImage = QImage(this->geometry().size(), QImage::Format_RGB32);
    QImage backgroundImage(this->geometry().width() / 5, this->geometry().width() / 5, QImage::Format_ARGB32);
    QImage qrCenterImage(this->geometry().width() / 13, this->geometry().width() / 13, QImage::Format_ARGB32);
    QImage logoImage(68, 76.5, QImage::Format_ARGB32);
    backgroundColorImage.fill(QColor(BACKGKROUND_COLOR));
    backgroundImage.fill(Qt::white);
    qrCenterImage.fill(Qt::white);
    logoImage.fill(QColor(BACKGKROUND_COLOR));
    QPainter painter(&backgroundImage);
    QPainter QRLogoPainter(&qrCenterImage);
    QSvgRenderer *renderer = getQrCode(&torHostName);
    renderer->render(&painter);
    int deltaX = this->geometry().width() - backgroundImage.width();
    int deltaY = this->geometry().height() - backgroundImage.height() + this->geometry().width() / 9 + 50;
    int deltaX2 = backgroundImage.width() - (this->geometry().width() / 13);
    int deltaY2 = backgroundImage.height() - (this->geometry().width() / 13);
    QSvgRenderer *renderer1 = new QSvgRenderer(qrLogoString.toUtf8());
    renderer1->render(&QRLogoPainter);

    // Draw background with QR code
    QPainter textPainter(this);
    textPainter.drawImage(this->geometry(), backgroundColorImage);
    textPainter.translate(deltaX / 2, deltaY / 2);
    textPainter.drawImage(backgroundImage.rect(), backgroundImage);
    textPainter.translate(deltaX2 / 2, deltaY2 / 2);
    textPainter.drawImage(qrCenterImage.rect(), qrCenterImage);
    textPainter.resetTransform();
    textPainter.translate(0, 10);
    textPainter.setPen(QPen(QColor(TEXT_COLOR)));
    textPainter.setFont(QFont(DEFAULT_FONT, this->geometry().height() / 10, QFont::Bold));
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, "Welcome!");
    textPainter.translate(0, this->geometry().height() / 5);
    textPainter.setFont(QFont(DEFAULT_FONT, std::min(20, this->geometry().height() / 30), QFont::Bold));
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, "Your Citadel is up and running at:");
    textPainter.translate(0, std::min(40, this->geometry().height() / 15));
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, "http://citadel.local");
    textPainter.translate(0, std::min(40, this->geometry().height() / 15));
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, "http://" + ipAddress);
    textPainter.translate(0, std::min(40, this->geometry().height() / 15));
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, torHostName);
    textPainter.resetTransform();
    textPainter.translate(0, this->geometry().height() - std::min(100, this->geometry().height() / 7));
    textPainter.drawText(this->geometry(), Qt::AlignHCenter, "Thank you for using Citadel!");

    // Draw Citadel logo in the bottom left corner
    QPainter logoPainter(&logoImage);
    QSvgRenderer *renderer2 = new QSvgRenderer(logoString.toUtf8());
    renderer2->render(&logoPainter);
    int logoImageDeltaX = this->geometry().width() - logoImage.width() - 15;
    int logoImageDeltaY = this->geometry().height() - logoImage.height() - 15;
    textPainter.resetTransform();
    textPainter.translate(logoImageDeltaX, logoImageDeltaY);
    textPainter.drawImage(logoImage.rect(), logoImage);
    textPainter.end();
}

QSvgRenderer *MainWidget::getQrCode(QString *address) {
    qrcodegen::QrCode qr0 = qrcodegen::QrCode::encodeText(address->toStdString().c_str(), qrcodegen::QrCode::Ecc::HIGH);
    QString svg = QString::fromStdString(qr0.toSvgString(4));
    QSvgRenderer *renderer = new QSvgRenderer(svg.toUtf8());
    return renderer;
}
