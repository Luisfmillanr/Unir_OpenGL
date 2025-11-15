// --------------------------------------------------------------
// main.c
// Trabajo "Proyecciones 3D" - Paso:
//   Ventana + proyección ortográfica + UN cubo 3D dibujado.
// --------------------------------------------------------------

#include <stdio.h>     // Funciones estándar de C (printf, etc.).
#include <GL/glut.h>   // FreeGLUT/OpenGL clásico en Linux.

// Tamaño actual de la ventana (en píxeles).
int window_width  = 800;
int window_height = 600;

// --------------------------------------------------------------
// Función auxiliar: dibuja un cubo centrado en el origen.
//
// Definimos un cubo de lado 2, que va de -1 a +1 en cada eje.
// Cada cara tendrá un color distinto para que se aprecie mejor
// la orientación del cubo.
// --------------------------------------------------------------
void drawCube(void)
{
    // Comenzamos a dibujar usando QUADS (cuadriláteros).
    glBegin(GL_QUADS);

    // ------- Cara frontal (Z positiva) -------
    glColor3f(1.0f, 0.0f, 0.0f);     // Rojo
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // ------- Cara trasera (Z negativa) -------
    glColor3f(0.0f, 1.0f, 0.0f);     // Verde
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // ------- Cara izquierda (X negativa) -------
    glColor3f(0.0f, 0.0f, 1.0f);     // Azul
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    // ------- Cara derecha (X positiva) -------
    glColor3f(1.0f, 1.0f, 0.0f);     // Amarillo
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    // ------- Cara superior (Y positiva) -------
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    // ------- Cara inferior (Y negativa) -------
    glColor3f(0.0f, 1.0f, 1.0f);     // Cian
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    glEnd();
}

// --------------------------------------------------------------
// Función reshape: ya estaba antes, no cambia.
// Ajusta viewport y proyección ortográfica cuando cambia el tamaño.
// --------------------------------------------------------------
void reshape(int w, int h)
{
    window_width  = (w > 0) ? w : 1;
    window_height = (h > 0) ? h : 1;

    glViewport(0, 0, window_width, window_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-2.0, 2.0,   // left, right
            -2.0, 2.0,   // bottom, top
            -10.0, 10.0  // near, far
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// --------------------------------------------------------------
// Función de dibujo (display).
// Ahora:
//   1) Limpia los buffers.
//   2) Coloca el cubo a una distancia razonable.
//   3) Lo rota un poco para ver varias caras.
//   4) Llama a drawCube().
// --------------------------------------------------------------
void display(void)
{
    // 1) Limpiar buffers de color y profundidad.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2) Asegurarnos de partir de una matriz de modelo-vista "limpia".
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 3) Trasladar el cubo hacia Z negativa.
    //    Recordemos:
    //      - El "ojo" está en (0,0,0) mirando hacia -Z.
    //      - Si el cubo estuviera en Z=0, estaría "encima" del ojo.
    //    Lo movemos, por ejemplo, a Z = -5.
    glTranslatef(0.0f, 0.0f, -5.0f);

    // 4) Rotar un poco el cubo para que se vean varias caras.
    //    Probamos con 25 grados en X y 35 en Y.
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);   // Rotar 25° alrededor del eje X
    glRotatef(35.0f, 0.0f, 1.0f, 0.0f);   // Luego 35° alrededor del eje Y

    // 5) Dibujar el cubo ya transformado.
    drawCube();

    // 6) Enviar comandos a la GPU.
    glFlush();
}

// --------------------------------------------------------------
// Función principal.
// --------------------------------------------------------------
int main(int argc, char **argv)
{
    printf("Iniciando programa Proyecciones 3D...\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Proyecciones 3D - Cubo simple");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}


