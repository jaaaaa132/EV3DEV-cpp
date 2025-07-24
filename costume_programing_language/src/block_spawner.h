#ifndef BLOCK_SPAWNER_H
#define BLOCK_SPAWNER_H

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

namespace godot {

class BlockSpawner : public Button {
	GDCLASS(BlockSpawner, Button)

private:

protected:
	static void _bind_methods();

    Ref<godot::PackedScene> scene_ref;
public:
	BlockSpawner();
    ~BlockSpawner();

    void _ready() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;

    void on_pressed();

    void set_scene(const Ref<PackedScene> &p_scene);
    Ref<PackedScene> get_scene();
};

}

#endif
