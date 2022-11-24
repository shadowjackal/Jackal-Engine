#ifndef _ANIMATIONS_HPP_
#define _ANIMATIONS_HPP_

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct JklVectorKey {
  double mTime;
  glm::vec3 mValue;
};

typedef struct JklQuatKey {
  double mTime;
  glm::quat mValue;
};

struct JklNode {
  std::string mName;
  uint32_t mNumChildren;
  JklNode **mChildren;
};

typedef struct JklAnimNode {
  std::string mNodeName; // The name of the node (bone) affected by this animation
  uint32_t mNumPositionKeys;
  uint32_t mNumRotationKeys;
  uint32_t mNumScalingKeys;
  std::vector<JklVectorKey> mPositionKeys; // The translation for each keyframe
  std::vector<JklQuatKey> mRotationKeys; // The rotation for each keyframe
  std::vector<JklVectorKey> mScalingKeys; // The scaling for each keyframe
};

struct JklAnimation {
  JklAnimNode **mChannels;
  double mDuration; // Duration of the animation in ticks
  double mTicksPerSecond;
};

struct JklVtxWeight {
  uint32_t mVertexId; // The index of the vertex influenced by this bone.
  float mWeight; // The weight of this bone.
};

struct JklBone {
  std::string mName;
  uint32_t mNumWeights;
  glm::mat4x4 mOffsetMatrix;
  JklVtxWeight* mWeights;
};

#define MAX_BONE_INFLUENCE 4

struct JklAnimVtx {
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

#endif