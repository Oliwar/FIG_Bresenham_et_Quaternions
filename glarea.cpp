// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include "quaternion.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>

GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);
    setFormat(sf);

    setEnabled(true);                   // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus);    // accepte focus
    setFocus();                         // donne le focus

    timer = new QTimer(this);
    timer->setInterval(50);           // msec
    connect (timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start();
    elapsedTimer.start();

    grille = Grille(100, 100);
}


GLArea::~GLArea()
{
    delete timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void GLArea::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.5f,0.5f,1.0f,1.0f);
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();

    // shader du sol
    program_simple = new QOpenGLShaderProgram(this);
    program_simple->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vsh");
    program_simple->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fsh");
    if (! program_simple->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_simple->log();
    }
}


void GLArea::makeGLObjects()
{
    //Création d'un pixel
    QVector<GLfloat> vertData_pixel;
    float x, y, r, v, b;
    float pas = 0.5f;
    for(Pixel p : grille.pixels){
        x = p.x;
        y = p.y;
        GLfloat vertices_pixel[] = {
            x-pas, y-pas, 0,
            x+pas, y-pas, 0,
            x+pas, y+pas, 0,
            x-pas, y-pas, 0,
            x-pas, y+pas, 0,
            x+pas, y+pas, 0
        };

        r = p.r;
        v = p.v;
        b = p.b;
        GLfloat colors_pixel[] = {
            r, v, b,
            r, v, b,
            r, v, b,
            r, v, b,
            r, v, b,
            r, v, b
        };

        for (int i = 0; i < 6; ++i) {
            // coordonnées sommets
            for (int j = 0; j < 3; j++)
                vertData_pixel.append(vertices_pixel[i*3+j]);
            for (int j = 0; j < 3; j++)
                vertData_pixel.append(colors_pixel[i*3+j]);
        }
    }

    vbo_pixel.create();
    vbo_pixel.bind();
    vbo_pixel.allocate(vertData_pixel.constData(), vertData_pixel.count() * int(sizeof(GLfloat)));
}


void GLArea::tearGLObjects()
{
    vbo_pixel.destroy();
}


void GLArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    windowRatio = float(w) / h;
}


void GLArea::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Matrice de projection
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, windowRatio, 1.0f, 1000.0f);

    // Matrice de vue (caméra)
    QMatrix4x4 viewMatrix;
    if(!quaternion){
        viewMatrix.translate(xPos, yPos, zPos);
        viewMatrix.rotate(xRot, 1, 0, 0);
        viewMatrix.rotate(yRot, 0, 1, 0);
        viewMatrix.rotate(zRot, 0, 0, 1);
    } else {
        Quaternion viewMatrixQuaternion;
        viewMatrix.translate(xPos, yPos, zPos);
        viewMatrixQuaternion.rotate(viewMatrix, xRot, QVector3D(1, 0, 0));
        viewMatrixQuaternion.rotate(viewMatrix, yRot, QVector3D(0, 1, 0));
        viewMatrixQuaternion.rotate(viewMatrix, zRot, QVector3D(0, 0, 1));
    }

    // Affichage du sol
    vbo_pixel.bind();
    program_simple->bind();

    program_simple->setUniformValue("projectionMatrix", projectionMatrix);
    program_simple->setUniformValue("viewMatrix", viewMatrix);

    program_simple->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
    program_simple->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program_simple->enableAttributeArray("in_position");
    program_simple->enableAttributeArray("colAttr");

    grille.affiche(program_simple);

    program_simple->disableAttributeArray("in_position");
    program_simple->disableAttributeArray("colAttr");
    program_simple->release();
}

void GLArea::mousePressEvent(QMouseEvent *ev)
{
    lastPos = ev->pos();
}


void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}


void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    int dx = ev->x() - lastPos.x();
    int dy = ev->y() - lastPos.y();

    if (ev->buttons() & Qt::LeftButton) {
        xRot += dy;
        yRot += dx;
        update();
    } else if (ev->buttons() & Qt::RightButton) {
        xPos += dx/10.0f;
        yPos -= dy/10.0f;
        update();
    } else if (ev->buttons() & Qt::MidButton) {
        xPos += dx/10.0f;
        zPos += dy;
        update();
    }

    lastPos = ev->pos();
}

float GLArea::rand_float(float min, float max){
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

void GLArea::onTimeout()
{
    static qint64 old_chrono = elapsedTimer.elapsed(); // static : initialisation la première fois et conserve la dernière valeur
    qint64 chrono = elapsedTimer.elapsed();
    dt = (chrono - old_chrono) / 1000.0f;
    old_chrono = chrono;

    update();
}
