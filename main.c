// --------------------------------------------------------------
// main.c
// Trabajo "Proyecciones 3D" - Paso 2
//   - Ventana con 4 viewports (cuatro subventanas).
//   - En los 4 se dibuja el MISMO cubo con proyección ortográfica.
//   - Ojo fijo en (0,0,0), sin gluLookAt().
// --------------------------------------------------------------

#include <stdio.h>     // Funciones estándar de C (printf, etc.).
#include <GL/glut.h>   // FreeGLUT/OpenGL clásico en Linux.

// Tamaño actual de la ventana (en píxeles).
int window_width  = 800;
int window_height = 600;

// --------------------------------------------------------------
// Dibuja un cubo centrado en el origen, de lado 2 (-1 a +1).
// Cada cara tiene un color distinto.
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
// Configura PROYECCIÓN ORTOGRÁFICA y dibuja el cubo en el
// viewport ACTUAL. Se llama una vez por viewport.
// --------------------------------------------------------------
void setupOrthoAndDrawCube(void)
{
    // Proyección
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0,   // left, right
            -2.0, 2.0,   // bottom, top
            -10.0, 10.0  // near, far
    );

    // Modelo-vista
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(35.0f, 0.0f, 1.0f, 0.0f);

    drawCube();
}

// --------------------------------------------------------------
// reshape: guarda el tamaño nuevo de ventana.
// La división en 4 viewports se hace en display().
// --------------------------------------------------------------
void reshape(int w, int h)
{
    window_width  = (w > 0) ? w : 1;
    window_height = (h > 0) ? h : 1;
    glViewport(0, 0, window_width, window_height);
}

// --------------------------------------------------------------
// display: limpia, calcula 4 viewports y dibuja el cubo en cada uno.
// --------------------------------------------------------------
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int half_w = window_width  / 2;
    int half_h = window_height / 2;

    // Viewport 1: arriba-izquierda
    glViewport(0, half_h, half_w, half_h);
    setupOrthoAndDrawCube();

    // Viewport 2: arriba-derecha
    glViewport(half_w, half_h, half_w, half_h);
    setupOrthoAndDrawCube();

    // Viewport 3: abajo-izquierda
    glViewport(0, 0, half_w, half_h);
    setupOrthoAndDrawCube();

    // Viewport 4: abajo-derecha
    glViewport(half_w, 0, half_w, half_h);
    setupOrthoAndDrawCube();

    glFlush();
}

/// --------------------------------------------------------------
// main
// --------------------------------------------------------------
int main(int argc, char **argv)
{
    // Mensaje en la consola para saber qué versión estamos ejecutando.
    printf("Iniciando programa Proyecciones 3D - Paso 2 (4 viewports ortográficos)...\n");

    // Inicializar GLUT / FreeGLUT.
    glutInit(&argc, argv);

    // Modo de visualización:
    //  - GLUT_SINGLE: un buffer (no hay animación).
    //  - GLUT_RGB: colores RGB.
    //  - GLUT_DEPTH: activar buffer de profundidad (necesario en 3D).
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    // Tamaño y posición inicial de la ventana.
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);

    // Crear la ventana con un título descriptivo.
    glutCreateWindow("Proyecciones 3D - Paso 2 (4 viewports ortográficos)");

    // Color de fondo (negro opaco).
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Activar test de profundidad para ocultar caras traseras del cubo.
    glEnable(GL_DEPTH_TEST);

    // Registrar funciones de callback.
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Entrar en el bucle principal de eventos de GLUT.
    glutMainLoop();

    // Nunca se llega aquí normalmente, pero por estilo devolvemos 0.
    return 0;
}
