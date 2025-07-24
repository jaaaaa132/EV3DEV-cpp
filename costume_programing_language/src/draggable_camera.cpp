#include "draggable_camera.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/viewport.hpp>

using namespace godot;

void DraggableCamera::_bind_methods() {
	ClassDB::bind_method(D_METHOD("drag_true"), &DraggableCamera::drag_true);
	ClassDB::bind_method(D_METHOD("drag_false"), &DraggableCamera::drag_false);
}

DraggableCamera::DraggableCamera(): dragging(false) {


}

DraggableCamera::~DraggableCamera() {

}

void DraggableCamera::_ready(){
	// conneting button for dragging
	NodePath button_path("CanvasLayer/Button");
	Button *button = get_node<Button>(button_path);
	button->connect("button_down", Callable(this, "drag_true"));
	button->connect("button_up", Callable(this, "drag_false"));
}

void DraggableCamera::_process(double delta) {
	if(dragging) set_position(offset - get_viewport()->get_mouse_position() + get_position());
	offset = get_viewport()->get_mouse_position();
}

void DraggableCamera::_physics_process(double delta){

}

void DraggableCamera::drag_true(){
	dragging = true;
	offset = get_viewport()->get_mouse_position();
}

void DraggableCamera::drag_false(){
	dragging = false;
}