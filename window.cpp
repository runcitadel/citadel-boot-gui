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
#include "window.h"
#include "MainWidget.h"

#include "QrCode.hpp"

#include <QWidget>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>

Window::Window()
	:QWidget()
{
}

void Window::handleButton() {
    this->m_layout->currentIndex() == 0 ? this->m_layout->setCurrentIndex(1) : this->m_layout->setCurrentIndex(0);
    this->m_button->setText(this->m_layout->currentIndex() == 0 ? "Open the Umbrel dashboard" : "Show QR Code");
    this->m_webView->setUrl(QUrl("http://localhost"));
}


void Window::init() {
    QVBoxLayout *layout = new QVBoxLayout;
    QStackedWidget *stackedWidget = new QStackedWidget;
    MainWidget *mainWidget = new MainWidget();
    QWebEngineView *webView = new QWebEngineView();
    webView->setUrl(QUrl("http://localhost"));
    layout->setSpacing(0);
    layout->setMargin(0);
    QPushButton *button = new QPushButton("Open the Umbrel dashboard");
    this->m_button = button;
    this->m_layout = stackedWidget;
    this->m_webView = webView;
    button->setMinimumHeight(25);
    button->setFlat(true);
    stackedWidget->addWidget(mainWidget);
    stackedWidget->addWidget(webView);
    layout->addWidget(stackedWidget);
    layout->addWidget(button);
    setLayout(layout);
    connect(button, &QPushButton::clicked, this, &Window::handleButton);
}
