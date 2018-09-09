#include "application.h"

#include <iostream>
#include <cassert>
#include <unistd.h>

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

using namespace std;
bool draw_solid=true;
bool paused=true;
float time_offset=0;
void draw_grid();

application::application()
{
}

application::~application()
{
}

// triggered once after the OpenGL context is initialized
void application::init_event()
{

    cout << "CAMERA CONTROLS: \n  LMB: Rotate \n  MMB: Move \n  RMB: Zoom" << endl;
    cout << "KEYBOARD CONTROLS: \n";
    cout<<"  '=': Toggle wireframe mode" << endl;
    cout<<"  'p': play/pause" << endl;
    cout<<"  'r': reset" << endl;

    const GLfloat ambient[] = { 0.15, 0.15, 0.15, 1.0 };
    const GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    const GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

    // enable a light
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glEnable(GL_LIGHT1);

    // enable depth-testing, colored materials, and lighting
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    // normalize normals so lighting calculations are correct
    // when using GLUT primitives
    glEnable(GL_NORMALIZE);

    // enable smooth shading
    glShadeModel(GL_SMOOTH);

    set_camera_for_box(vec3(-10,-10,-10),vec3(10,10,10));

    t.reset();
}
void draw_torus(float inner_radius,float outer_radius)
{
    draw_solid ? glutSolidTorus(inner_radius, outer_radius, 20,20) : glutWireTorus(inner_radius,outer_radius, 20,20);
}
void draw_sphere(float radius)
{
    draw_solid ? glutSolidSphere(radius, 20, 20) : glutWireSphere(radius, 20, 20);
}

void draw_unit_cube()
{draw_solid ? glutSolidCube(1) : glutWireCube(1); }

void draw_dodecadron()
{draw_solid ? glutSolidDodecahedron() : glutWireDodecahedron();}

//
// create some various objects in the world
//
void application::draw_scene(float time_elapsed) 
{

    // TODO: Fix this function so that it matches the described animation

    // Blue Sphere
    glPushMatrix();
    glTranslatef(5, 1, 0);
    glColor3f(0, 0, 1); 
    draw_sphere(1);
    //glPopMatrix();
    
    // Yellow Sphere 
    float angular_speed=-150;
    glPushMatrix();
    glRotatef(time_elapsed*angular_speed,0,1,0); 
    glTranslatef(2,0,0);
    glColor3f(1, 1, 0);
    draw_sphere(0.5);
    glPopMatrix();

    // Cyan Dodecahedron
    //angular_speed=20;
    glPushMatrix();
    glRotatef(time_elapsed*20,0,1,0); 
    glTranslatef(8,0,0);
    glPushMatrix();
    glScalef(0.5, 0.5, 0.5);
    glColor3f(0, 1, 1);
    draw_dodecadron();
    glPopMatrix();
    
    // Green Sphere
    //angular_speed=300;
    glPushMatrix();
    glRotatef(time_elapsed*300,1,0,0);
    glTranslatef(0, 2, 0);
    glColor3f(0, 1, 0);
    draw_sphere(0.2);
    glPopMatrix();
        
    // Purple Box
    //angular_speed=100;
    glPushMatrix();
    glRotatef(time_elapsed*100,0,1,0);
    glTranslatef(4, 0, 0);
    glPushMatrix();
    glScalef(0.5, 2, 0.5);
    glColor3f(1, 0, 1);
    draw_unit_cube();
    glPopMatrix();

    // Red Dodecahedron
    glPushMatrix();
    glRotatef(time_elapsed*350,0,1,0);
    glTranslatef(1, 0, 0);
    glColor3f(1, 0, 0);
    glPushMatrix();	
    glScalef(0.2, 0.2, 0.2);
    draw_dodecadron();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

// triggered each time the application needs to redraw
void application::draw_event()
{
    float elap = time_offset;
    if(!paused) elap+=t.elapsed();
    const GLfloat light_pos1[] = { 0.0, 10.0, 0.0, 1 };
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
    // apply our camera transformation
    apply_gl_transform();
    // draws the grid and frame at the origin
    draw_grid();
    
    //draw the scene
    glPushMatrix();
    draw_scene(elap);
    glPopMatrix();
}

// triggered when mouse is clicked
void application::mouse_click_event(int button, int button_state, int x, int y)
{
}

// triggered when mouse button is held down and the mouse is
// moved
void application::mouse_move_event(
    int x, int y
    )
{
}

// triggered when a key is pressed on the keyboard
void application::keyboard_event(unsigned char key, int x, int y)
{
    if (key == '=') 
        draw_solid=!draw_solid;
    if (key == 'p'){
        if(paused) t.reset();
        else time_offset=t.elapsed();
        paused=!paused;
    }
    if (key == 'r'){ 
        time_offset=0;
        t.reset();}
}

void draw_grid()
{
    glDisable(GL_LIGHTING);
    glLineWidth(2.0);

    //
    // Draws the coordinate frame at origin
    //
    glPushMatrix();
    glScalef(0.5, 0.5, 0.5);
    glBegin(GL_LINES);

    // x-axis
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    // y-axis
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);

    // z-axis
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();
    glPopMatrix();

    //
    // Draws a grid along the x-z plane
    //
    glLineWidth(1.0);
    glColor3f(.20, .20, .20);
    glBegin(GL_LINES);

    int ncells = 20;
    int ncells2 = ncells/2;

    for (int i= 0; i <= ncells; i++)
    {
        int k = -ncells2;
        k +=i;
        glVertex3f(ncells2,0,k);
        glVertex3f(-ncells2,0,k);
        glVertex3f(k,0,ncells2);
        glVertex3f(k,0,-ncells2);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}
