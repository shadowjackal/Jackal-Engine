#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#ifdef _WIN32 
#include "mingw.thread.h"
#endif

#ifdef __linux__
#include <thread>
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

void jklstart(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);
extern GLFWwindow* window;

extern int countywounty;
extern int inputdir;
extern int buttontec;

struct JklScene {
    virtual void codeInit(void) { std::cout << "BEHAVIOUR UNDEFINED ! : BEGIN " << std::endl;};
    virtual void codeLoop(void) { std::cout << "BEHAVIOUR UNDEFINED ! : LOOP " << std::endl;};
};

void jklsetScene(JklScene* nscene);
void jklrun(void);
#endif /* _ENGINE_H_*/