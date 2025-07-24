#ifndef BLOCK_H
#define BLOCK_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

namespace godot {

class Block : public Node2D {
	GDCLASS(Block, Node2D)

private:

protected:
	static void _bind_methods();
	
	bool dragging;
	Vector2 offset;
	
	Block* next;
	Block* previous;
	void snap_to_previous_block(double delta);

	bool destructable;

public:
	Block();
    ~Block();

	void _ready() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;

	void drag_true();
	void drag_false();

	void previous_block_entered(Node2D* body);
	void previous_block_exited(Node2D *body);

	void self_destruct();

	virtual String write();

	void spawn_next_block(Ref<PackedScene> scene, PackedStringArray parameters);
	virtual void set_parameters(PackedStringArray parameters);
	void connect_next_block(Block* next_block);
};

}

#endif
