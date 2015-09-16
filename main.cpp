#include "entity/ecs.hpp"
#include <iostream>
using namespace entity;
using namespace std;

struct PositionComponent
{
	PositionComponent(int x = 0, int y = 0) : x(x), y(y) {}
	int x, y;
};

struct VelocityComponent
{
	VelocityComponent(int dx = 0, int dy = 0) : dx(dx), dy(dy) {}
	int dx, dy;
};

class MoveSystem : public System
{
public:
	MoveSystem()
	{
		require_component<PositionComponent>();
	}

	virtual void update(float delta)
	{
		cout << "i moved!" << endl;
	}
};

int main()
{
	Entities world;
	Entity e = world.create();
	e.add<PositionComponent>(50, 50);
	world.add_system<MoveSystem>();

	world.update();
	MoveSystem& ms = world.get_system<MoveSystem>();
	ms.update(1.0f);

	PositionComponent &pc = e.get<PositionComponent>();

	cout << "x: " << pc.x << ", y: " << pc.y << endl;

	return 0;
}
