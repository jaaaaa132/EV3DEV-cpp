#ifndef DRAGGABLE_CAMERA_H
#define DRAGGABLE_CAMERA_H

#include <godot_cpp/classes/camera2d.hpp>

namespace godot {

class DraggableCamera : public Camera2D {
	GDCLASS(DraggableCamera, Camera2D)

private:

protected:
	static void _bind_methods();
	bool dragging;
	Vector2 offset;

public:
	DraggableCamera();
    ~DraggableCamera();

	void _ready() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;

	void drag_true();
	void drag_false();
};

}

#endif
