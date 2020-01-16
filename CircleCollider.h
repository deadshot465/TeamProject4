//
// Created by 周哲輝 on 2020/01/16.
//

#ifndef TEAMPROJECT4_CIRCLECOLLIDER_H
#define TEAMPROJECT4_CIRCLECOLLIDER_H
#include "ECS.h"

class CircleCollider
        : public Component
{
public:
    CircleCollider();
    CircleCollider(std::string_view name, float radius = 50.0f);
    ~CircleCollider();

    bool CheckCollision(const glm::vec2& otherCenter, float otherRadius);

    float Radius = 50.0f;

protected:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render() override;

};


#endif //TEAMPROJECT4_CIRCLECOLLIDER_H
