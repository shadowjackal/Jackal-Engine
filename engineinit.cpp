#include "include/engineinit.hpp"
#include "include/graphics.hpp"

int countywounty = 0;
int inputdir = -1;
int buttontec;

extern int LastThingDrawn;
float deltaTime = 0.0f;

GLFWwindow* window;
JklScene* CurrentScene;
Shader  *MODELSHADER;


void jklstart(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT) {
        // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "jackal_engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    MODELSHADER = new Shader("resources/texflat.vs","resources/texflat.fs");
}
 
void jklsetScene(JklScene* nscene) {
    CurrentScene = nscene;
    CurrentScene->codeInit();
}

void jklrun(void) {
    int frames = 0;
    deltaTime = 0.0f;
    float lastFrame = 0.0f;
    auto start = std::chrono::steady_clock::now();
    while (!glfwWindowShouldClose(window))
    {
        LastThingDrawn = -1;
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        ++frames;
        auto now = std::chrono::steady_clock::now();
        auto diff = now - start;
        auto end = now + std::chrono::milliseconds(16);
        if(diff >= std::chrono::seconds(1))
        {
            start = now;
            std::cout << "FPS: " << frames << std::endl;
            frames = 0;
        }
        processInput(window);


        CurrentScene->codeLoop();

        glfwSwapBuffers(window);

        glfwPollEvents();


    };
};


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    bool inputbools[5];
    for(int i = 0; i < 5; i++) {
        inputbools[i] = false;
    };

    inputdir = -1;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) inputbools[0] = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) inputbools[1] = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) inputbools[2] = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) inputbools[3] = true;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) inputbools[4] = true;

    if(inputbools[0] == true) inputdir = 0; // up
    if(inputbools[1] == true) inputdir = 2; // right
    if(inputbools[2] == true) inputdir = 4; // down
    if(inputbools[3] == true) inputdir = 6; // left

    if(inputbools[0] == true && inputbools[1] == true) inputdir = 1;
    if(inputbools[1] == true && inputbools[2] == true) inputdir = 3;
    if(inputbools[2] == true && inputbools[3] == true) inputdir = 5;
    if(inputbools[3] == true && inputbools[0] == true) inputdir = 7;
    if(inputbools[4] == true && buttontec != 2) {
        buttontec = 1;
    };

    if(buttontec == 2) {
        countywounty += 1;
    }
    
    if(countywounty >= 10) {
        buttontec = 0;
        countywounty = 0;
    }
    std::cout << "counter : " << countywounty << std::endl;
 }

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
   // camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
   // camera.ProcessMouseScroll(static_cast<float>(yoffset));
}