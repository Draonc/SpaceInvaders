// SpaceInvaders.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SOIL.h"
#include <cstdint>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GL\glut.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#define NUM_BULLETS 6
#define MAX_NUM_ENEMIES 40

GLsizei winWidth = 0;
GLsizei winHeight = 0;
GLint lives = 3;
GLfloat playerX = 0;
GLfloat playerY = 0;
GLint numbullets = 0;
GLint fired = 0;
GLint numEnemies = 40;
GLint killed = 0;
GLfloat speed = .1;
GLint level = 1;
GLint score = 0;
GLfloat enemySpeed = .1;
GLint background;

struct Bullet {
	GLfloat bulletX;
	GLfloat bulletY;
	bool active;
};

struct Enemy {
	GLfloat enemyX;
	GLfloat enemyY;
	bool active;
};

Bullet bullets[NUM_BULLETS];
Enemy enemies[MAX_NUM_ENEMIES];
void checkCollision();
void drawPlayer();
void drawBullets(int i);
void createEnemies();
void drawEnemy(int m);
void drawInfo();
void drawGameOver();
void draw();
void display(void);
void reshape(int w, int h);
void loadImage();
void SpecialInput(int key, int x, int y);


int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	createEnemies();
	glutInitWindowSize(500, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Space Invaders");
	loadImage();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(SpecialInput);
	winWidth = glutGet(GLUT_WINDOW_WIDTH);
	winHeight = glutGet(GLUT_WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}


void checkCollision() {

	for (int n = 0; n < NUM_BULLETS; n++) {
		if (bullets[n].active) {
			for(int a = 0; a < 40; a++){
				if (bullets[n].bulletX + 25 >= enemies[a].enemyX - 25
					&& bullets[n].bulletX + 25 <= enemies[a].enemyX + 25 && enemies[a].active) {
					if ((winHeight - bullets[n].bulletY - 20) <= enemies[a].enemyY + 25) {
						enemies[a].active = false;
						killed++;
						bullets[n].active = false;
						fired--;
						score++;
					}
				}
			}
		}
	}
}

void drawPlayer() {
	playerY = winHeight - 10;
	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex3f(playerX, playerY, 0);
	glVertex3f(playerX + 50, playerY, 0);
	glVertex3f(playerX + 25, playerY - 50, 0);
	glEnd();
}

void drawBullets(int i) {
	glColor3f(1, .5, 0);
	glBegin(GL_POLYGON);
	glVertex3f(bullets[i].bulletX + 25, winHeight - bullets[i].bulletY, 0);
	glVertex3f(bullets[i].bulletX + 35, winHeight - bullets[i].bulletY - 10, 0);
	glVertex3f(bullets[i].bulletX + 25, winHeight - bullets[i].bulletY - 20, 0);
	glVertex3f(bullets[i].bulletX + 15, winHeight - bullets[i].bulletY - 10, 0);
	glEnd();

	bullets[i].bulletY += .3;

	if ((winHeight - bullets[i].bulletY - 20) <= 0) {
		bullets[i].active = false;
		fired--;
		score--;
	}
	glutPostRedisplay();
}

void createEnemies() {
	GLfloat setX = 35;
	GLfloat setY = 35;

	for (int j = 0; j < MAX_NUM_ENEMIES; j++) {
		enemies[j].enemyX = setX;
		enemies[j].enemyY = setY;
		enemies[j].active = true;
		setX += 35;
		if (setX > 350) {
			setY += 35;
			setX = 35;

		}
	}
}

void drawEnemy(int m) {
	GLfloat radius = 15.0;
	GLfloat twicePi = 2.0f * 3.14;

	if (enemies[m].active) {
		glColor3f(0, 1, 0);
		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i <= 50; i++) {
			glVertex2f(
				enemies[m].enemyX + (radius * cos(i *  twicePi / 50)),
				enemies[m].enemyY + enemySpeed + (radius * sin(i * twicePi / 50))
			);
		}
		glEnd();
	}


	int window = winWidth;
	if (enemies[0].enemyX <= 0) {
		enemySpeed = speed;
		enemies[m].enemyY += (300 * enemySpeed);
	}else if( enemies[9].enemyX >= window) {
		enemySpeed = -speed;

		enemies[m].enemyY -= (300 * enemySpeed);
	}

	enemies[m].enemyX += enemySpeed;

	checkCollision();

	if (enemies[39].enemyY >= playerY) {
		lives--;
		killed = 0;
		createEnemies();
	}

	glutPostRedisplay();

}

void drawInfo() {
	char buf[20];
	snprintf(buf, 20, "Score: %d", score);

	glColor3f(1, 0, 0);
	glRasterPos2f(5, 25);

	for (int i = 0; i < 20; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buf[i]);
	}

	snprintf(buf, 20, "Level: %d", level);

	glColor3f(0, 1, 0);
	glRasterPos2f(400, 25);

	for (int i = 0; i < 20; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buf[i]);
	}

	snprintf(buf, 20, "Lives: %d", lives);

	glColor3f(0, 0, 1);
	glRasterPos2f(200, 25);

	for (int i = 0; i < 20; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buf[i]);
	}
}

void drawGameOver() {
	char buf[20];
	snprintf(buf, 20, "GAME OVER");

	glColor3f(1, 0, 0);
	glRasterPos2f(175, 350);

	for (int i = 0; i < 20; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buf[i]);
	}
}

void draw() {
	if (lives > 0) {
		drawInfo();
		drawPlayer();

		for (int i = 0; i < NUM_BULLETS; i++) {
			if (bullets[i].active) {
				drawBullets(i);
			}
		}
		for (int i = 0; i < 40; i++) {
			drawEnemy(i);
		}
		if (numEnemies == killed) {
			killed = 0;
			level++;
			speed += .05;
			createEnemies();
		}

		glutPostRedisplay();
	}
	else {
		drawInfo();
		drawGameOver();
	}
}


void display(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 500, 700, 0.0, -1000.0, 1000.0);
	glDepthMask(false);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, background);

	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex3d(0.0, 0.0, 1);
	glTexCoord2d(1.0, 0.0); glVertex3d(500.0, 0.0, 1);
	glTexCoord2d(1.0, 1.0); glVertex3d(500.0, 700.0, 1);
	glTexCoord2d(0.0, 1.0); glVertex3d(0.0, 700.0, 1);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDepthMask(true);

	glLoadIdentity();
	glOrtho(0.0, winWidth, winHeight, 0.0, -1000.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	draw();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SpecialInput(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:

		if (fired < NUM_BULLETS) {
			bullets[numbullets].active = true;
			bullets[numbullets].bulletX = playerX;
			bullets[numbullets].bulletY = 50;

			if (numbullets++ >= NUM_BULLETS - 1) {
				numbullets = 0;
			}
			fired++;
		}
		break;
	case GLUT_KEY_LEFT:
		playerX -= 10;
		break;
	case GLUT_KEY_RIGHT:
		playerX += 10;
		break;
	}
	glutPostRedisplay();
}
void loadImage() {

	background = SOIL_load_OGL_texture("Background.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	if (background == 0) {
		printf("IMAGE FAILED");
	}
}

