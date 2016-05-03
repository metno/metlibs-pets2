/*
  libpets2 - presentation and editing of time series

  Copyright (C) 2006-2016 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "ptQPainter.h"
#include "ptColor.h"

#include <QApplication>
#include <QImage>
#include <QPrinter>

#include <iostream>

static void drawImageWithBounds(pets2::ptPainter& painter, float x, float y,
    const QImage& image, float scale)
{
  painter.drawRect(x, y,
      x+scale*image.width()  * painter.pixWidth(),
      y+scale*image.height() * painter.pixHeight());
  painter.drawImage(x, y, image, scale);
}

static void paint(QPaintDevice* device)
{
  pets2::ptQCanvas pcanvas(device);
  pets2::ptQPainter ppainter(&pcanvas);

  const float GLW=1500, GLH=1000, D=10;
  ppainter.setLine(pets2::ptColor("RED"), 2, pets2::DASHED);
  ppainter.drawLine(D, D, GLW-D, GLH-D);

  ppainter.setFill(pets2::ptColor("GREEN"), pets2::DIAGCROSS);
  ppainter.drawRect(D, D, 100+D, 100+D);

  const QImage weather01 = QImage(TEST_SRCDIR "weather_01.png");
  {
    const float x=D+100, y=D+200, w=40, h=2*w, s=2.5;
    ppainter.setLine(pets2::ptColor("BLUE"), 4, pets2::DOTTED);
    drawImageWithBounds(ppainter, x, y, weather01, s);
  }
  {
    const float w=80, h=145, x=GLW-D-w, y=GLH-D-h, s=1;
    ppainter.setLine(pets2::ptColor("BLACK"), 2, pets2::DASHDASHDOTTED);
    drawImageWithBounds(ppainter, x, y, weather01, s);
  }
  {
    const float dy = 10;
    ppainter.setFill(pets2::ptColor("WHITE"));
    ppainter.drawRect(0, GLH-4*((4+2)*dy), 600, GLH);

    std::cout << "==================== linestyle tests ====================" << std::endl;
    const pets2::ptLineStyle ls[4] = {
      pets2::DASHED, pets2::DASHDOTTED, pets2::DASHDASHDOTTED, pets2::DOTTED
    };
    const char* lsn[4] = {
      "DASHED", "DASHDOTTED", "DASHDASHDOTTED", "DOTTED"
    };
    const char* col[4] = {
      "BLUE", "RED", "CYAN", "MAGENTA"
    };
    float y = GLH-D;
    for (int i=0; i<4; ++i) {
      ppainter.setColor(pets2::ptColor(col[i]));
      ppainter.setLineStyle(ls[i]);
      const float x0 = D, x1 = x0+200;
      ppainter.drawText(lsn[i], x1+20, y-15);
      for (int w=1; w<=4; ++w) {
        ppainter.setLineWidth(w);
        ppainter.drawLine(x0, y, x1, y);
        y -= dy;
      }
      y -= 2*dy;
    }
  }
}

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  {
    QImage img(750, 500, QImage::Format_ARGB32_Premultiplied);
    paint(&img);
    img.save("TestQPainter1.png", "PNG");
  }

  {
    QPrinter* printer = new QPrinter(QPrinter::ScreenResolution);
    printer->setOutputFormat(QPrinter::PostScriptFormat);
    printer->setOutputFileName("TestQPainter1.ps");
    printer->setFullPage(true);
    printer->setPaperSize(QSizeF(750, 500), QPrinter::DevicePixel);

    paint(printer);
    delete printer;
  }

  return 0;
}
