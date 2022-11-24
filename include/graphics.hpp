#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

extern int LastThingDrawn;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum EDRAW_TYPE {
    ESPRITE,
    EMODEL,
    EEMPTY = -1
};

enum EDIR_NUM {
    EDIR_UP,
    EDIR_UP_RIGHT,
    EDIR_RIGHT,
    EDIR_DOWN_RIGHT,
    EDIR_DOWN,
    EDIR_DOWN_LEFT,
    EDIR_LEFT,
    EDIR_UP_LEFT,
    EDIR_NONE = -1
};


// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() const
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

extern Shader  *MODELSHADER;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    float Roll;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == DOWN)
            Position -= WorldUp * velocity;
        if (direction == UP)
            Position += WorldUp * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 90.0f)
            Zoom = 90.0f; 
    }

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};


extern Camera camera;
extern float baseheight;

typedef struct vert {
    float x,y,z;
} vert;

typedef struct txc {
    float x,y;
} txc;


typedef struct mat {
    float r,g,b;
    bool tex;
} mat;

typedef struct trindex {
    	int matid;
		int vt1, vt2, vt3, uv1, uv2, uv3, vn1, vn2, vn3;
} trindex;

class StaticMesh {
    public :
        int pntnum, plycnt;
        unsigned int VBO, VAO;
        Shader *shader;
        std::vector<float> vertices;

        void LoadModel(char* path, Shader* shader) {
            std::ifstream infile(path, std::ios::binary);
            int vcnt, uvcnt, ncnt, tricnt, mtlcnt;

            std::vector<vert> pointtable;
            std::vector<txc> uvtable;
            std::vector<vert> normaltable;
            std::vector<trindex> trilist;
            std::vector<mat> matlist;

            infile.read(reinterpret_cast<char*>(&vcnt),sizeof(int));
            infile.read(reinterpret_cast<char*>(&uvcnt),sizeof(int));
            infile.read(reinterpret_cast<char*>(&ncnt),sizeof(int));
            infile.read(reinterpret_cast<char*>(&tricnt),sizeof(int));
            infile.read(reinterpret_cast<char*>(&mtlcnt),sizeof(int));

            for(int i = 0; i < vcnt; i++) { // loads point table
                float xx,yy,zz;
                infile.read(reinterpret_cast<char*>(&xx),sizeof(float));
                infile.read(reinterpret_cast<char*>(&yy),sizeof(float));
                infile.read(reinterpret_cast<char*>(&zz),sizeof(float));
                pointtable.push_back((vert){xx,yy,zz});
            }

            for(int i = 0; i < uvcnt; i++) { // loads point table
                float xx,yy;
                infile.read(reinterpret_cast<char*>(&xx),sizeof(float));
                infile.read(reinterpret_cast<char*>(&yy),sizeof(float));
                uvtable.push_back((txc){xx,yy});
            }

            for(int i = 0; i < ncnt; i++) { // loads point table
                float xx,yy,zz;
                infile.read(reinterpret_cast<char*>(&xx),sizeof(float));
                infile.read(reinterpret_cast<char*>(&yy),sizeof(float));
                infile.read(reinterpret_cast<char*>(&zz),sizeof(float));
                normaltable.push_back((vert){xx,yy,zz});
            }


            for(int i = 0; i < tricnt; i++) {
                uint8_t matid;
		        int vt1, vt2, vt3, uv1, uv2, uv3, vn1, vn2, vn3;

                infile.read(reinterpret_cast<char*>(&matid),sizeof(uint8_t));

                infile.read(reinterpret_cast<char*>(&vt1),sizeof(int));
                infile.read(reinterpret_cast<char*>(&uv1),sizeof(int));
                infile.read(reinterpret_cast<char*>(&vn1),sizeof(int));                
                
                infile.read(reinterpret_cast<char*>(&vt2),sizeof(int));
                infile.read(reinterpret_cast<char*>(&uv2),sizeof(int));
                infile.read(reinterpret_cast<char*>(&vn2),sizeof(int));                
                
                infile.read(reinterpret_cast<char*>(&vt3),sizeof(int));
                infile.read(reinterpret_cast<char*>(&uv3),sizeof(int));
                infile.read(reinterpret_cast<char*>(&vn3),sizeof(int));

                trilist.push_back((trindex){(int)matid,vt1,vt2,vt3,uv1,uv2,uv3,vn1,vn2,vn3});
            }

            for(int i = 0; i < mtlcnt; i++) {
                float rr,gg,bb;
                uint8_t texuse;
                bool istex = false;
			    infile.read(reinterpret_cast<char*>(&texuse),sizeof(uint8_t));
			    infile.read(reinterpret_cast<char*>(&rr),sizeof(float)); 
			    infile.read(reinterpret_cast<char*>(&gg),sizeof(float)); 	
			    infile.read(reinterpret_cast<char*>(&bb),sizeof(float)); 	

                if(texuse != 0) istex = true;
                matlist.push_back((mat){rr,gg,bb,istex});
            }


            for(int i = 0; i < tricnt; i++) {
                this->vertices.push_back(pointtable[trilist[i].vt1].x);
                this->vertices.push_back(pointtable[trilist[i].vt1].y);
                this->vertices.push_back(pointtable[trilist[i].vt1].z);

                this->vertices.push_back(matlist[trilist[i].matid].r);
                this->vertices.push_back(matlist[trilist[i].matid].g);
                this->vertices.push_back(matlist[trilist[i].matid].b);

                this->vertices.push_back(normaltable[trilist[i].vn1].x);
                this->vertices.push_back(normaltable[trilist[i].vn1].y);
                this->vertices.push_back(normaltable[trilist[i].vn1].z);

                this->vertices.push_back(uvtable[trilist[i].uv1].x);
                this->vertices.push_back(uvtable[trilist[i].uv1].y);


                this->vertices.push_back(pointtable[trilist[i].vt2].x);
                this->vertices.push_back(pointtable[trilist[i].vt2].y);
                this->vertices.push_back(pointtable[trilist[i].vt2].z);

                this->vertices.push_back(matlist[trilist[i].matid].r);
                this->vertices.push_back(matlist[trilist[i].matid].g);
                this->vertices.push_back(matlist[trilist[i].matid].b);

                this->vertices.push_back(normaltable[trilist[i].vn2].x);
                this->vertices.push_back(normaltable[trilist[i].vn2].y);
                this->vertices.push_back(normaltable[trilist[i].vn2].z);

                this->vertices.push_back(uvtable[trilist[i].uv2].x);
                this->vertices.push_back(uvtable[trilist[i].uv2].y);


                this->vertices.push_back(pointtable[trilist[i].vt3].x);
                this->vertices.push_back(pointtable[trilist[i].vt3].y);
                this->vertices.push_back(pointtable[trilist[i].vt3].z);

                this->vertices.push_back(matlist[trilist[i].matid].r);
                this->vertices.push_back(matlist[trilist[i].matid].g);
                this->vertices.push_back(matlist[trilist[i].matid].b);

                this->vertices.push_back(normaltable[trilist[i].vn3].x);
                this->vertices.push_back(normaltable[trilist[i].vn3].y);
                this->vertices.push_back(normaltable[trilist[i].vn3].z);

                this->vertices.push_back(uvtable[trilist[i].uv3].x);
                this->vertices.push_back(uvtable[trilist[i].uv3].y);
            }

            glGenVertexArrays(1, &this->VAO);
            glGenBuffers(1, &this->VBO);
        
            glBindVertexArray(this->VAO);
        
            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);
        
            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
            glEnableVertexAttribArray(3);

            this->plycnt = tricnt;

            this->shader = shader;

            infile.close();
        }


        void Draw(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) {

            glBindVertexArray(this->VAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::scale(model,scale);
            model = glm::rotate(model, (rotation.x * ( 3.14159265358979323846f / 180.0f )), glm::vec3(1.f,0.f,0.f));
            model = glm::rotate(model, (rotation.y * ( 3.14159265358979323846f / 180.0f )), glm::vec3(0.f,1.f,0.f));
            model = glm::rotate(model, (rotation.z * ( 3.14159265358979323846f / 180.0f )), glm::vec3(0.f,0.f,1.f));

            this->shader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, this->plycnt*3);
        }
};


#define MAX_BONE_INFLUENCE 4

struct AnimVertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh {
    public :
    std::vector<AnimVertex>       vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;


    // constructor
    Mesh(std::vector<AnimVertex> vertices, std::vector<unsigned int> indices)
    {
        this->vertices = vertices;
        this->indices = indices;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    void Draw(Shader &shader) {
            glBindVertexArray(this->VAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.f,0.f,0.f));
            shader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); 
        }


    private :
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(AnimVertex), &vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);	
            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(9 * sizeof(float)));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(12 * sizeof(float)));
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, 22 * sizeof(float), (void*)(15 * sizeof(float)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(18 * sizeof(float)));
            glEnableVertexAttribArray(6);
 
        glBindVertexArray(0);
    }
};


struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

struct AnimNodeFile {
	glm::mat4 transformation;
	int BoneID;
	int childrenCount;
	std::vector<int> children;
}; 

class Bone {
    public :
    Bone() {};
    
    Bone(int boneiid, std::vector<KeyPosition> keyPos, std::vector<KeyRotation> keyRot, std::vector<KeyScale> keyScal, glm::mat4 offedSet) {
        this->BoneID = boneiid;
        this->KeyFramePos = keyPos;
        this->KeyFrameRot = keyRot;
        this->KeyFrameScal = keyScal;
        this->OoFset = offedSet;
    };

    int BoneID;
    glm::mat4 LocalTrans;
    std::vector<KeyPosition> KeyFramePos;
    std::vector<KeyRotation> KeyFrameRot;
    std::vector<KeyScale> KeyFrameScal;
    glm::mat4 OoFset;
    /* Gets the current index on mKeyPositions to interpolate to based on 
    the current animation time*/
    int GetPositionIndex(float animationTime)
    {
        for (int index = 0; index < KeyFramePos.size() - 1; ++index)
        {
            if (animationTime < KeyFramePos[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

    /* Gets the current index on mKeyRotations to interpolate to based on the 
    current animation time*/
    int GetRotationIndex(float animationTime)
    {
        for (int index = 0; index < KeyFrameRot.size() - 1; ++index)
        {
            if (animationTime < KeyFrameRot[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

    /* Gets the current index on mKeyScalings to interpolate to based on the 
    current animation time */
    int GetScaleIndex(float animationTime)
    {
        for (int index = 0; index < KeyFrameScal.size() - 1; ++index)
        {
            if (animationTime < KeyFrameScal[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }


    /* Gets normalized value for Lerp & Slerp*/
    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff = nextTimeStamp - lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    /*figures out which position keys to interpolate b/w and performs the interpolation 
    and returns the translation matrix*/
    glm::mat4 InterpolatePosition(float animationTime)
    {
        if (1 == KeyFramePos.size())
            return glm::translate(glm::mat4(1.0f), KeyFramePos[0].position);

        int p0Index = GetPositionIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(KeyFramePos[p0Index].timeStamp,
            KeyFramePos[p1Index].timeStamp, animationTime);
        glm::vec3 finalPosition = glm::mix(KeyFramePos[p0Index].position,
            KeyFramePos[p1Index].position, scaleFactor);
        return glm::translate(glm::mat4(1.0f), finalPosition);
    }

    /*figures out which rotations keys to interpolate b/w and performs the interpolation 
    and returns the rotation matrix*/
    glm::mat4 InterpolateRotation(float animationTime)
    {
        if (1 == KeyFrameRot.size())
        {
            auto rotation = glm::normalize(KeyFrameRot[0].orientation);
            return glm::toMat4(rotation);
        }

        int p0Index = GetRotationIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(KeyFrameRot[p0Index].timeStamp,
            KeyFrameRot[p1Index].timeStamp, animationTime);
        glm::quat finalRotation = glm::slerp(KeyFrameRot[p0Index].orientation,
            KeyFrameRot[p1Index].orientation, scaleFactor);
        finalRotation = glm::normalize(finalRotation);
        return glm::toMat4(finalRotation);
    }

    /*figures out which scaling keys to interpolate b/w and performs the interpolation 
    and returns the scale matrix*/
    glm::mat4 InterpolateScaling(float animationTime)
    {
        if (1 == KeyFrameScal.size())
            return glm::scale(glm::mat4(1.0f), KeyFrameScal[0].scale);

        int p0Index = GetScaleIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(KeyFrameScal[p0Index].timeStamp,
            KeyFrameScal[p1Index].timeStamp, animationTime);
        glm::vec3 finalScale = glm::mix(KeyFrameScal[p0Index].scale, KeyFrameScal[p1Index].scale
            , scaleFactor);
        return glm::scale(glm::mat4(1.0f), finalScale);
    };
	
    /*interpolates  b/w positions,rotations & scaling keys based on the curren time of 
    the animation and prepares the local transformation matrix by combining all keys 
    tranformations*/
    void Update(float animationTime)
    {
        glm::mat4 translation = InterpolatePosition(animationTime);
        glm::mat4 rotation = InterpolateRotation(animationTime);
        glm::mat4 scale = InterpolateScaling(animationTime);
        LocalTrans = translation * rotation * scale;
    };


};

class Animation {
    public :
    float frameloc;
    float Duration;
    float CurrentTime;
    int TPS;
    float Delta;
    std::vector<AnimNodeFile> hierarch;
    std::vector<Bone> bones;
    std::vector<glm::mat4> FinalTrans;

    Animation() {

    };

    Animation(float Dura, int Ticks, std::vector<AnimNodeFile> hrar, std::vector<Bone> bonelist) {
        this->Duration = Dura;
        this->TPS = Ticks;
        this->hierarch = hrar;
        this->bones = bonelist;
        this->FinalTrans.resize(bonelist.size());
    }


    void PlayAnimation()
    {
        CurrentTime = 0.0f;
    }

        Bone* FindBoneID(int id) {
            for(int i = 0; i < bones.size(); i++) {
                if(bones[i].BoneID == id) {
                    return &bones[i];
                };
            };
        }

    void CalculateBoneTransform(const AnimNodeFile* node, glm::mat4 parentTransform)
    {
        int nodeName = node->BoneID;
        glm::mat4 nodeTransform = node->transformation;
	    Bone* Bone;
	
        if (nodeName != -1)
        {
            Bone = FindBoneID(nodeName);
            Bone->Update(CurrentTime);
            nodeTransform = Bone->LocalTrans;
        }
	
        glm::mat4 globalTransformation = parentTransform * nodeTransform;
	
        if (node->childrenCount > 0 && node->BoneID != -1)
        {
            FinalTrans[node->BoneID] = globalTransformation * Bone->OoFset;
        }
	
        for (int i = 0; i < node->childrenCount; i++)
            CalculateBoneTransform(&hierarch[node->children[i]], globalTransformation);
    }

void UpdateAnimation(float dt)
    {
        Delta = dt;
        
        CurrentTime += TPS * dt;
        CurrentTime = fmod(CurrentTime, Duration);
        CalculateBoneTransform(&hierarch[0], glm::mat4(1.0f));
        
    }

};

class Model {
    public :
    std::vector<Mesh>    meshes;
    Animation AnimationInternal;

        // constructor, expects a filepath to a 3D model.
    Model(std::string const &path) 
    {
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    void Draw(Shader &shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

    private :

    void    loadModel(std::string const &path) {
        std::ifstream infile(path, std::ios::binary);
        int MeshCnt, indCnt, vtxCnt;

        infile.read(reinterpret_cast<char*>(&MeshCnt),sizeof(int));

        for(int i = 0; i < MeshCnt; i++) {
            
            std::vector<AnimVertex> vtxtable;
            std::vector<unsigned int> indtable; 

            infile.read(reinterpret_cast<char*>(&vtxCnt),sizeof(int));
            infile.read(reinterpret_cast<char*>(&indCnt),sizeof(int));

            for(int j = 0; j < vtxCnt; j++) {
                AnimVertex TemptempVtx;
                infile.read(reinterpret_cast<char*>(&TemptempVtx.Position),sizeof(glm::vec3));
                infile.read(reinterpret_cast<char*>(&TemptempVtx.Normal),sizeof(glm::vec3));
                infile.read(reinterpret_cast<char*>(&TemptempVtx.TexCoords),sizeof(glm::vec2));
                infile.read(reinterpret_cast<char*>(&TemptempVtx.Tangent),sizeof(glm::vec3));
                infile.read(reinterpret_cast<char*>(&TemptempVtx.Bitangent),sizeof(glm::vec3));

                infile.read(reinterpret_cast<char*>(&TemptempVtx.m_BoneIDs[0]),sizeof(int));
                infile.read(reinterpret_cast<char*>(&TemptempVtx.m_BoneIDs[1]),sizeof(int));
                infile.read(reinterpret_cast<char*>(&TemptempVtx.m_BoneIDs[2]),sizeof(int));
                infile.read(reinterpret_cast<char*>(&TemptempVtx.m_BoneIDs[3]),sizeof(int));

                infile.read(reinterpret_cast<char*>(&TemptempVtx.m_Weights[0]),sizeof(float));
                infile.read(reinterpret_cast<char*>(&TemptempVtx.m_Weights[1]),sizeof(float));
                infile.read(reinterpret_cast<char*>(&TemptempVtx.m_Weights[2]),sizeof(float));
                infile.read(reinterpret_cast<char*>(&TemptempVtx.m_Weights[3]),sizeof(float));

                vtxtable.push_back(TemptempVtx); 
            };

            for(int j = 0; j < indCnt; j++) {
                unsigned int temp;
                infile.read(reinterpret_cast<char*>(&temp),sizeof(unsigned int));
                indtable.push_back(temp);
            };

            meshes.push_back(Mesh(vtxtable,indtable));
        };
        
        float AnimDuration;
        int TpsCount;
        int BoneCount;
        int NodeCount;
        std::vector<Bone> tempBoneList;
        std::vector<AnimNodeFile> tempNodeList;

        infile.read(reinterpret_cast<char*>(&AnimDuration),sizeof(float));
        infile.read(reinterpret_cast<char*>(&TpsCount),sizeof(int));
        infile.read(reinterpret_cast<char*>(&BoneCount),sizeof(int));
        infile.read(reinterpret_cast<char*>(&NodeCount),sizeof(int));

        for(int i = 0; i < BoneCount; i++) {
            int BoneID;
            glm::mat4 LocalTrans;
            int numKeyPos;
            int numKeyRot;
            int numKeySca;
            infile.read(reinterpret_cast<char*>(&BoneID),sizeof(int));
            infile.read(reinterpret_cast<char*>(&LocalTrans),sizeof(glm::mat4));
            infile.read(reinterpret_cast<char*>(&numKeyPos),sizeof(int));
            infile.read(reinterpret_cast<char*>(&numKeyRot),sizeof(int));
            infile.read(reinterpret_cast<char*>(&numKeySca),sizeof(int));
            
            std::vector<KeyPosition> tempPos;
            std::vector<KeyRotation> tempRot;
            std::vector<KeyScale> tempScal;

            for(int j = 0; j < numKeyPos; j++) {
                tempPos.push_back({});
                infile.read(reinterpret_cast<char*>(&tempPos[j].position), sizeof(glm::vec3));
                infile.read(reinterpret_cast<char*>(&tempPos[j].timeStamp), sizeof(float));
            };
                        
            for(int j = 0; j < numKeyRot; j++) {
                tempRot.push_back({});
                infile.read(reinterpret_cast<char*>(&tempRot[j].orientation), sizeof(glm::quat));
                infile.read(reinterpret_cast<char*>(&tempRot[j].timeStamp), sizeof(float));
            };        
            
            for(int j = 0; j < numKeySca; j++) {
                tempScal.push_back({});
                infile.read(reinterpret_cast<char*>(&tempScal[j].scale), sizeof(glm::vec3));
                infile.read(reinterpret_cast<char*>(&tempScal[j].timeStamp), sizeof(float));
            };

            glm::mat4 tempOffset;
            infile.read(reinterpret_cast<char*>(&tempOffset),sizeof(glm::mat4));
            tempBoneList.push_back(Bone(BoneID,tempPos,tempRot,tempScal,tempOffset));
        };
        
        for(int i = 0; i < NodeCount; i++) {
            int BoneID;
            glm::mat4 OrientR;
            int ChildCount;
            std::vector<int> children;

            infile.read(reinterpret_cast<char*>(&BoneID),sizeof(int));
            infile.read(reinterpret_cast<char*>(&OrientR),sizeof(glm::mat4));
            infile.read(reinterpret_cast<char*>(&ChildCount),sizeof(int));
            for(int j = 0; j < ChildCount; j++) {
                int ChildID;
                infile.read(reinterpret_cast<char*>(&ChildID),sizeof(int));
                children.push_back(ChildID);
            };

            tempNodeList.push_back((AnimNodeFile){OrientR,BoneID,ChildCount,children});
        };

        AnimationInternal = Animation(AnimDuration, TpsCount, tempNodeList, tempBoneList);
    };
};

unsigned int LoadTexture(const char *filename);
#endif
