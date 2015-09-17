#include "ecs.hpp"
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
	world.add_system<MoveSystem>();

	Entity e = world.create();
	e.add<PositionComponent>(50, 50);

	Entity e2 = world.create();
	e2.add<PositionComponent>(60, 60);
	e2.add<VelocityComponent>(1, 1);

	world.update();
	MoveSystem& ms = world.get_system<MoveSystem>();
	ms.update(1.0f);

	PositionComponent &pc = e.get<PositionComponent>();

	cout << "x: " << pc.x << ", y: " << pc.y << endl;
	cout << e.to_string() << endl;

	cout << "Entities with pos:" << endl;
	world.for_each<PositionComponent>([](Entity e, PositionComponent& pos) {
		cout << e.to_string() << " x: " << pos.x << " y: " << pos.y << endl;
	});

	cout << "Entities with pos and vel:" << endl;
	world.for_each<PositionComponent, VelocityComponent>([](Entity e, PositionComponent& pos, VelocityComponent& vel) {
		cout << e.to_string() << " x: " << pos.x << " y: " << pos.y << endl;
	});

	return 0;
}
