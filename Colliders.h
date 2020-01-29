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
   
    friend class CollisionSystem;
};

class CircleCollider
        : public Collider
{
public:
    CircleCollider();
    CircleCollider(std::string_view name, float radius = 64.0f);
    ~CircleCollider();

    virtual bool CheckCollision(const Collider& other) const override;
    bool CheckRectangleCollision(const Collider& other) const noexcept;

    float Radius = 64.0f;

protected:
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
};

class RectangleCollider
    : public Collider
{
public:
    RectangleCollider();
    RectangleCollider(std::string_view name, const Rectangle& rectangle);
    ~RectangleCollider();

    // Collider ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
    virtual bool CheckCollision(const Collider& other) const override;

    Rectangle RectangleBox = {};
protected:
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
};


#endif //TEAMPROJECT4_CIRCLECOLLIDER_H
