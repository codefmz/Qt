/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
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

#include "norwegianwoodstyle.h"

#include <QComboBox>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QRadioButton>
#include <QStyleFactory>
NorwegianWoodStyle::NorwegianWoodStyle()
    : QProxyStyle(QStyleFactory::create("windows"))
{
    setObjectName("NorwegianWood");
}

/**
 * @brief: 设置自定义的 QPalette
 * @param:
 * @return:
 */
QPalette NorwegianWoodStyle::standardPalette() const
{
    if (!m_standardPalette.isBrushSet(QPalette::Disabled, QPalette::Mid))
    {

        // 1.预定义几个要使用的颜色
        QColor brown(212, 140, 95);
        //        QColor brown(0, 0, 0);
        QColor beige(236, 182, 120);             //米黄色
                                                 //        QColor beige(255, 255, 255);             //米黄色
        QColor slightlyOpaqueBlack(0, 0, 0, 63); //轻微不透明黑色

        // 2.laod 背景图片
        QImage backgroundImage(":/images/woodbackground.png"); //横纹
        QImage buttonImage(":/images/woodbutton.png");         //竖纹
        QImage midImage = buttonImage.convertToFormat(QImage::Format_RGB32);

        QPalette palette(brown);
        // 3.设置 QPalette 的 active inactive disabled 三个group中角色的状态
        palette.setBrush(QPalette::BrightText, Qt::cyan);    //按钮按下后文本的颜色
        palette.setBrush(QPalette::Base, beige);             //文本窗口的背景色
        palette.setBrush(QPalette::Highlight, Qt::darkCyan); // list tree table 选中后的 颜色
        // 3个状态都是
        setTexture(palette, QPalette::Button, buttonImage); //按钮按
        setTexture(palette, QPalette::Mid, midImage);
        setTexture(palette, QPalette::Window, backgroundImage); //窗口
        // disable 时的状态，将颜色变得更深
        QBrush brush = palette.window();
        brush.setColor(brush.color().darker());
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush);

        QPainter painter;
        painter.begin(&midImage);
        painter.setPen(Qt::NoPen);
        painter.fillRect(midImage.rect(), slightlyOpaqueBlack);
        painter.end();
        m_standardPalette = palette;
    }
    return m_standardPalette;
}
/**
 * @brief:使用 这个style 的时候会用到，
 * @param:
 * @return:
 */
void NorwegianWoodStyle::polish(QWidget* widget)
{
    if (qobject_cast<QPushButton*>(widget) || qobject_cast<QComboBox*>(widget))
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- polish";
        widget->setAttribute(Qt::WA_Hover, true);
    }
}

void NorwegianWoodStyle::unpolish(QWidget* widget)
{
    if (qobject_cast<QPushButton*>(widget) || qobject_cast<QComboBox*>(widget))
    {
        widget->setAttribute(Qt::WA_Hover, false);
    }
}

/**
 * @brief:  修改控件的默认像素，影响大小
 * @param:
 * @return:
 */
int NorwegianWoodStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case PM_ComboBoxFrameWidth: //将ComBox的宽度变大，默认值是2
        return 8;
    case PM_ScrollBarExtent:
        return QProxyStyle::pixelMetric(metric, option, widget);
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}

int NorwegianWoodStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const
{
    switch (hint)
    {
    case SH_DitherDisabledText:
        return int(false);
    case SH_EtchDisabledText:
        return int(true);
    default:
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
}


/**
* @brief: 用来绘制各种各样的基本图形元素， 这个例子将button 的边框该为了圆形，同时鼠标进入，选中或者不选中会有一个动态的3D变换
* @param:
* @return:
*/
void NorwegianWoodStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case PE_PanelButtonCommand:   //重新绘制 部分button
    case PE_PanelItemViewItem:
    {
        int    delta = (option->state & State_MouseOver) ? 64 : 0;
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- delta = " << delta ;
        QColor slightlyOpaqueBlack(0, 0, 0, 63);
        QColor semiTransparentWhite(255, 255, 255, 127 + delta);
        QColor semiTransparentBlack(0, 0, 0, 127 - delta);
        int x, y, width, height;
        option->rect.getRect(&x, &y, &width, &height);
        QPainterPath              roundRect = roundRectPath(option->rect);
        int                       radius = qMin(width, height) / 2;

        QBrush                    brush;
        bool                      darker;
        const QStyleOptionButton* buttonOption = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (buttonOption && (buttonOption->features & QStyleOptionButton::Flat))
        {
            brush = option->palette.window();
            darker = (option->state & (State_Sunken | State_On));
        }
        else
        {
            if (option->state & (State_Sunken | State_On))
            {
                brush = option->palette.mid();
                darker = !(option->state & State_Sunken);
            }
            else
            {
                brush = option->palette.button();
                darker = false;
            }
        }
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->fillPath(roundRect, brush);
        if (darker)
            painter->fillPath(roundRect, slightlyOpaqueBlack);
        int penWidth;
        if (radius < 10)
            penWidth = 3;
        else if (radius < 20)
            penWidth = 5;
        else
            penWidth = 7;

        QPen topPen(semiTransparentWhite, penWidth); //白色画笔
        QPen bottomPen(semiTransparentBlack, penWidth); //黑色画笔
        if (option->state & (State_Sunken | State_On))   //选中的时候交换
        {
            qSwap(topPen, bottomPen);
        }

        //绘制左上方的边界线
        int x1 = x;
        int x2 = x + radius;
        int x3 = x + width - radius;
        int x4 = x + width;
        if (option->direction == Qt::RightToLeft)
        {
            qSwap(x1, x4);
            qSwap(x2, x3);
        }
        QPolygon topHalf;
        topHalf << QPoint(x1, y) << QPoint(x4, y) << QPoint(x3, y + radius) << QPoint(x2, y + height - radius) << QPoint(x1, y + height);
        painter->setClipPath(roundRect);
        painter->setClipRegion(topHalf, Qt::IntersectClip);
        painter->setPen(topPen);
        painter->drawPath(roundRect);


        QPolygon bottomHalf = topHalf;
        bottomHalf[0] = QPoint(x4, y + height);

        painter->setClipPath(roundRect);
        painter->setClipRegion(bottomHalf, Qt::IntersectClip);
        painter->setPen(bottomPen);
        painter->drawPath(roundRect);
        painter->setPen(option->palette.windowText().color());
        painter->setClipping(false);
        painter->drawPath(roundRect);

        painter->restore();
    }
    break;
    default:
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}

/**
* @brief: 冲新绘制控件，使得 QPushButo选中时文本颜色变换
* @param:
* @return:
*/
void NorwegianWoodStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case CE_PushButtonLabel:  //绘制pushButton 的文本，改变 Push
    {
        QStyleOptionButton        myButtonOption;
        const QStyleOptionButton* buttonOption = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (buttonOption)
        {
            myButtonOption = *buttonOption;
            if (myButtonOption.palette.currentColorGroup() != QPalette::Disabled)
            {
                if (myButtonOption.state & (State_Sunken | State_On))
                {
                    myButtonOption.palette.setBrush(QPalette::ButtonText, myButtonOption.palette.brightText());
                }
            }
        }
        QProxyStyle::drawControl(element, &myButtonOption, painter, widget);
    }
    break;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}
/**
* @brief: 设置所有 QPalette 组中每个角色的状态
* @param:
* @return:
*/
void NorwegianWoodStyle::setTexture(QPalette& palette, QPalette::ColorRole role, const QImage& image)
{
    for (int i = 0; i < QPalette::NColorGroups; ++i)
    {
        qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << i;
        QBrush brush(image);
        brush.setColor(palette.brush(QPalette::ColorGroup(i), role).color());
        palette.setBrush(QPalette::ColorGroup(i), role, brush);
    }
}


QPainterPath NorwegianWoodStyle::roundRectPath(const QRect& rect)
{
    int radius = qMin(rect.width(), rect.height()) / 2;
    int diam = 2 * radius;
    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);
    QPainterPath path;
    path.moveTo(x2, y1 + radius);
    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0); //绘制圆角
    path.lineTo(x1 + radius, y1);
    path.arcTo(QRect(x1, y1, diam, diam), 90.0, +90.0);
    path.lineTo(x1, y2 - radius);
    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, +90.0);
    path.lineTo(x1 + radius, y2);
    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
    path.closeSubpath();
    return path;
}
