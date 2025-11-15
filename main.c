// --------------------------------------------------------------
// main.c
// Trabajo "Proyecciones 3D"
//   Ventana con 4 viewports mostrando el MISMO cubo con:
//   1) Proyección ortogonal.
//   2) Proyección gabinete (oblicua paralela).
//   3) Proyección perspectiva simétrica.
//   4) Proyección perspectiva oblicua.
// Restricción: el "ojo" permanece en (0,0,0), sin gluLookAt().
// --------------------------------------------------------------

#include <stdio.h>     // Funciones estándar de C (printf, etc.).
#include <GL/glut.h>   // FreeGLUT/OpenGL clásico en Linux.

// Tamaño actual de la ventana (en píxeles).
int window_width  = 800;
int window_height = 600;

// --------------------------------------------------------------
// Dibuja un cubo centrado en el origen, de lado 2 (-1 a +1).
// Cada cara tiene un color distinto para apreciar la orientación.
// --------------------------------------------------------------
void drawCube(void)
{
    glBegin(GL_QUADS);

    // Cara frontal (Z positiva)
    glColor3f(1.0f, 0.0f, 0.0f);     // Rojo
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Cara trasera (Z negativa)
    glColor3f(0.0f, 1.0f, 0.0f);     // Verde
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Cara izquierda (X negativa)
    glColor3f(0.0f, 0.0f, 1.0f);     // Azul
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    // Cara derecha (X positiva)
    glColor3f(1.0f, 1.0f, 0.0f);     // Amarillo
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    // Cara superior (Y positiva)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    // Cara inferior (Y negativa)
    glColor3f(0.0f, 1.0f, 1.0f);     // Cian
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    glEnd();
}

// --------------------------------------------------------------
// Configura la MATRIZ MODELO-VISTA común para TODAS las vistas.
// Aquí "movemos el objeto", NO la cámara:
//   - Ponemos el cubo a Z = -5.
//   - Lo rotamos un poco para ver varias caras.
// --------------------------------------------------------------
void setupModelViewCommon(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Trasladar el cubo hacia Z negativa.
    glTranslatef(0.0f, 0.0f, -5.0f);

    // Rotar un poco el cubo.
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);   // 25° en X
    glRotatef(35.0f, 0.0f, 1.0f, 0.0f);   // 35° en Y
}

// --------------------------------------------------------------
// 1) Proyección ORTOGRÁFICA (viewport 1).
//    Las líneas paralelas siguen paralelas, sin perspectiva.
// --------------------------------------------------------------
void renderOrtho(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-2.0, 2.0,   // left, right
            -2.0, 2.0,   // bottom, top
            -10.0, 10.0  // near, far
    );

    setupModelViewCommon();
    drawCube();
}

// --------------------------------------------------------------
// 2) Proyección GABINETE (viewport 2).
//    Es una proyección oblicua paralela:
//      - Partimos de una ortográfica.
//      - Aplicamos una matriz de "shear" (cizalla) en X según Z.
//    Aproximación sencilla: x' = x + 0.5 * z
// --------------------------------------------------------------
void renderCabinet(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Base ortográfica.
    glOrtho(-2.0, 2.0,   // left, right
            -2.0, 2.0,   // bottom, top
            -10.0, 10.0  // near, far
    );

    // Matriz de shear en X respecto a Z:
    //   | 1   0   k   0 |
    //   | 0   1   0   0 |
    //   | 0   0   1   0 |
    //   | 0   0   0   1 |
    // En OpenGL (column-major) se define así:
    GLfloat k = 0.5f;  // factor de "cabinet" (mitad de la profundidad)
    GLfloat shearMat[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,   // Columna 0
        0.0f, 1.0f, 0.0f, 0.0f,   // Columna 1
        k,    0.0f, 1.0f, 0.0f,   // Columna 2 (afecta X con Z)
        0.0f, 0.0f, 0.0f, 1.0f    // Columna 3
    };

    glMultMatrixf(shearMat);

    setupModelViewCommon();
    drawCube();
}

// --------------------------------------------------------------
// 3) Proyección PERSPECTIVA SIMÉTRICA (viewport 3).
//    Es la perspectiva "normal": frustum centrado, un punto de
//    fuga en el centro, tamaño depende de la distancia.
//    Usamos gluPerspective(fovY, aspect, near, far).
// --------------------------------------------------------------
void renderPerspectiveSymmetric(float aspect)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Campo de visión vertical de 60 grados,
    // aspecto = ancho/alto del viewport, near/far típicos.
    gluPerspective(60.0, aspect, 1.0, 20.0);

    setupModelViewCommon();
    drawCube();
}

// --------------------------------------------------------------
// 4) Proyección PERSPECTIVA OBLICUA (viewport 4).
//    Partimos de una perspectiva simétrica y le aplicamos un
//    "shear" suave en X para inclinar la dirección de visión.
//    Esto produce una perspectiva con punto de fuga "descentrado".
// --------------------------------------------------------------
void renderPerspectiveOblique(float aspect)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Perspectiva base.
    gluPerspective(60.0, aspect, 1.0, 20.0);

    // Shear suave en X según Z (similar idea a gabinete, pero
    // aplicado encima de la perspectiva).
    GLfloat k = 0.3f;  // menor que 0.5 para no distorsionar tanto
    GLfloat shearMat[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,   // Columna 0
        0.0f, 1.0f, 0.0f, 0.0f,   // Columna 1
        k,    0.0f, 1.0f, 0.0f,   // Columna 2
        0.0f, 0.0f, 0.0f, 1.0f    // Columna 3
    };

    glMultMatrixf(shearMat);

    setupModelViewCommon();
    drawCube();
}

// --------------------------------------------------------------
// reshape: guarda el nuevo tamaño de la ventana.
// La división en 4 viewports se hace en display().
// --------------------------------------------------------------
void reshape(int w, int h)
{
    window_width  = (w > 0) ? w : 1;
    window_height = (h > 0) ? h : 1;

    glViewport(0, 0, window_width, window_height);
}

// --------------------------------------------------------------
// display:
//   - Limpia buffers.
//   - Calcula 4 viewports (2x2).
//   - En cada uno aplica una proyección distinta.
// --------------------------------------------------------------
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int half_w = window_width  / 2;
    int half_h = window_height / 2;

    // Todos los viewports tienen el mismo aspecto:
    float aspect = (half_h > 0) ? ( (float)half_w / (float)half_h ) : 1.0f;

    // 1) Arriba-izquierda: ORTOGRÁFICA
    glViewport(0, half_h, half_w, half_h);
    renderOrtho();

    // 2) Arriba-derecha: GABINETE
    glViewport(half_w, half_h, half_w, half_h);
    renderCabinet();

    // 3) Abajo-izquierda: PERSPECTIVA SIMÉTRICA
    glViewport(0, 0, half_w, half_h);
    renderPerspectiveSymmetric(aspect);

    // 4) Abajo-derecha: PERSPECTIVA OBLICUA
    glViewport(half_w, 0, half_w, half_h);
    renderPerspectiveOblique(aspect);

    glFlush();
}

// --------------------------------------------------------------
// main
// --------------------------------------------------------------
int main(int argc, char **argv)
{
    printf("Iniciando programa Proyecciones 3D - 4 tipos de proyección...\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Proyecciones 3D - Orto, Gabinete, Persp. Simétrica y Oblicua");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
