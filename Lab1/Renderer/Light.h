#pragma once
#include <glm/glm.hpp>

constexpr int MaxLights = 32;
constexpr int MaxShadowedLights = 32;

struct Light {
    int id = -1;
    char name[64] = "";
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;
    float range = 20.0f;
    bool castsShadows = true;
    bool enabled = true;
};
