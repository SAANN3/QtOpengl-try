#include "glwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL, true);
    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    GLWindow window;
    Q_ASSERT(glGetError() == 0);
    window.setFormat(format);
    window.resize(640,480);
    window.show();

    return a.exec();
}
