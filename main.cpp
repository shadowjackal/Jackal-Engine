#include "include/engineinit.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/graphics.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <glm/gtx/string_cast.hpp>

int LastThingDrawn;

// settings
const unsigned int SCR_WIDTH = 800+400;
const unsigned int SCR_HEIGHT = 600+300;


// camera
Camera camera(glm::vec3(0.0f, 1.0f, 20.0f));

Shader* CURRENT_SHADER;



unsigned int LoadTexture(const char *filename) {
    unsigned int texture;
    int width, height, nrChannels;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    return texture;
};


typedef struct TestScene : JklScene {

    unsigned int texture1; 
    Model ourModel;
	Animation danceAnimation;
	Animator animator;
    int hasPrinted = 0;

    TestScene(void) {

    };

    void codeInit(void) override {
        
        texture1 = LoadTexture("resources/grid.png");
        	ourModel = Model("resources/SONCANIM.fbx");
	        danceAnimation = Animation("resources/SONCANIM.fbx",&ourModel);
	        animator = Animator(&danceAnimation);
            animator.PlayAnimation(&danceAnimation);
    };

    void codeLoop(void) override {
        CURRENT_SHADER->use();
        CURRENT_SHADER->setInt("texture_diffuse1",0);
		animator.UpdateAnimation(deltaTime);		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 20000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		CURRENT_SHADER->setMat4("projection", projection);
		CURRENT_SHADER->setMat4("view", view);
        if(inputdir == 0) camera.Position.z -= 1.f;
        if(inputdir == 4) camera.Position.z += 1.f;

        auto transforms = animator.GetFinalBoneMatrices();

        if(hasPrinted == 0) {
          for(int i = 0; i < transforms.size(); i++)  {
            std::cout << glm::to_string(transforms[i]) << std::endl;
          }
          hasPrinted = 1;
        };

        glm::mat4 testtttt(1.f);
		for (int i = 0; i < transforms.size(); ++i)
			CURRENT_SHADER->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", testtttt);


		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
        model *= glm::eulerAngleXYZ(0.f,90.f,0.f);
		model = glm::translate(model, glm::vec3(0.0f, -0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));	// it's a bit too big for our scene, so scale it down
		CURRENT_SHADER->setMat4("model", model); 
		ourModel.Draw(*CURRENT_SHADER);
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


