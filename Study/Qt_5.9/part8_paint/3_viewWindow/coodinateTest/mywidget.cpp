#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent)
    : QWidget{parent}
{

}

void MyWidget::paintEvent( QPaintEvent *event )
{

//    paint1();
    paint2();
}

void MyWidget::paint1()
{
    QPainter painter( this );
    QPen pen;
    pen.setStyle( Qt::SolidLine );
    painter.setPen( pen );
    painter.setRenderHint( QPainter::Antialiasing );
    QBrush brush ;
    brush.setColor( Qt::darkBlue );
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush( brush );

    painter.translate( 150, 250 );
//    painter.drawPolygon(points, 5);

    painter.drawPath( getPainterPath( 0 ) );
    painter.save();
    painter.setPen( QColor( Qt::red ) );
    painter.drawPath( getPainterPath( 1 ) );
    painter.drawText( -4, -4, "S1" );

    painter.restore(); //将stack中的状态
    painter.resetTransform();
    painter.translate( 450, 250 );
    painter.rotate( -18 );
    painter.scale( 1.4,  1.4);
    painter.drawPath( getPainterPath( 0 ) );
    painter.setPen( QColor( Qt::red ) );
    painter.drawPath( getPainterPath( 1 ) );
    painter.drawText( 0, 0, "S2" );
}

void MyWidget::paint2()
{
    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing );
    int w = width();
    int h = height();
    int side = qMin(w, h);

    QRect rect((w - side) / 2, (h - side) / 2, side, side);
    painter.drawRect(rect);
    painter.setViewport(rect);
    painter.setWindow(-100, -100, 200, 200);

    QLinearGradient linearGrad( 0, 0, 100, 0 );//线性渐变
    linearGrad.setColorAt(0, Qt::yellow);
    linearGrad.setColorAt(1, Qt::blue);
    linearGrad.setSpread(QGradient::PadSpread); //展布模式
    painter.setBrush(linearGrad);
    painter.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);
    painter.setCompositionMode(QPainter::CompositionMode_Difference);
//    painter.setCompositionMode(QPainter::CompositionMode_Exclusion);


    for(int i = 0; i < 36; i++)
    {
       painter.drawEllipse(QPoint(50, 0), 50, 50);
       painter.rotate(10);
    }

}

QPainterPath MyWidget::getPainterPath( int state, qreal deg, qreal R)
{
    qreal rotateDeg = 2 * 3.1415926 / 5;
    QPoint points[ 5 ] =
    {
        QPoint( R, 0 ),
        QPoint( R * std::cos( rotateDeg ), - R * std::sin( rotateDeg )),
        QPoint( R * std::cos( 2 * rotateDeg ), - R * std::sin( 2 * rotateDeg )),
        QPoint( R * std::cos( 3 * rotateDeg ), - R * std::sin( 3 * rotateDeg )),
        QPoint( R * std::cos( 4 * rotateDeg ), - R * std::sin( 4 * rotateDeg ))
    };

   QPainterPath path;
   //获取直线的path
   if( state == 0 )
   {
       path.moveTo( points[0] );
       path.lineTo( points[2] );
       path.lineTo( points[4] );
       path.lineTo( points[1] );
       path.lineTo( points[3] );
       path.closeSubpath();
   }
   else if( state == 1 )//获取文本的path
   {
       QFont font;
       font.setPointSize(12);
       path.addText( points[0], font, "0" );
       path.addText( points[1], font, "1" );
       path.addText( points[2] - QPoint(4, 4), font, "2" );
       path.addText( points[3] - QPoint(12, 0), font, "3" );
       path.addText( points[4] - QPoint(-4, 0), font, "4" );
   }
   return path;
}


