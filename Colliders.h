//
// Created by 周哲輝 on 2020/01/16.
//

#ifndef TEAMPROJECT4_CIRCLECOLLIDER_H
#define TEAMPROJECT4_CIRCLECOLLIDER_H
#include "ECS.h"

class Collider
    : public Component
{
public:
    Collider();
    Collider(std::string_view name);
    virtual ~Collider() = default;

    virtual bool CheckCollision(const Collider& other) const = 0;

protected:
    virtual void Initialize() override = 0;
    virtual void Update(float deltaTime) override = 0;
    virtual void Render() override = 0;
   
};

class CircleCollider
        : public Collider
{
public:
    CircleCollider();
    CircleCollider(std::string_view name, float radius = 50.0f);
    ~CircleCollider();

    virtual bool CheckCollision(const Collider& other) const override;

    float Radius = 50.0f;

protected:
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;

};


#endif //TEAMPROJECT4_CIRCLECOLLIDER_H
