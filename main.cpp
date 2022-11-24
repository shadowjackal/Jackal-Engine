#include "include/engineinit.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/graphics.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

// settings
const unsigned int SCR_WIDTH = 800+400;
const unsigned int SCR_HEIGHT = 600+300;


// camera
Camera camera(glm::vec3(0.0f, 5.f, 15.0f));

Shader* CURRENT_SHADER;


struct TestScene : JklScene {
    Model   test = Model("resources/SecretWolfZone.jkl");
    unsigned int texture1; 
    void codeInit(void) override {
        
        texture1 = LoadTexture("resources/grid.png");
        test.AnimationInternal.PlayAnimation();
    }
    void codeLoop(void) override {
        CURRENT_SHADER->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);

        glm::mat4 projection = glm::perspective(glm::radians(60.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 5000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        projection = glm::rotate(projection, camera.Roll * ( 3.14159265358979323846f / 180.0f ), glm::vec3(0.f,0.f,01.f));
        CURRENT_SHADER->setMat4("projection", projection);
        CURRENT_SHADER->setMat4("view", view);

        CURRENT_SHADER->setVec3("viewPos", camera.Position);

        CURRENT_SHADER->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        CURRENT_SHADER->setVec3("dirLight.ambient", glm::vec3(0.8f));
        CURRENT_SHADER->setVec3("dirLight.diffuse", 1.f, 1.f, 1.f);
        CURRENT_SHADER->setVec3("dirLight.specular", 1.f, 1.f, 1.f);
        CURRENT_SHADER->setInt("ourTexture",0);
        test.AnimationInternal.UpdateAnimation(1.0f);
        std::vector<glm::mat4> transforms = test.AnimationInternal.FinalTrans;
        glm::mat4 testttt = glm::mat4(1.0f);
        testttt = glm::scale(testttt,glm::vec3(-1.f));
		for (int i = 0; i < transforms.size(); ++i)
			CURRENT_SHADER->setMat4("gBones[" + std::to_string(i) + "]", testttt);

        test.Draw(*CURRENT_SHADER);
    };
};

int main()
{
    jklstart(SCR_WIDTH,SCR_HEIGHT);
TestScene scenstance;

    Shader ourShader("resources/texflat.vs", "resources/texflat.fs");
    CURRENT_SHADER = &ourShader;



    jklsetScene(&scenstance);


    jklrun();               


    glfwTerminate();
    return 0;
}


