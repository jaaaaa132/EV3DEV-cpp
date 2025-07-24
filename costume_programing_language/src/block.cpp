#include "block.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/camera2d.hpp>

using namespace godot;

void Block::_bind_methods() {
	ClassDB::bind_method(D_METHOD("drag_true"), &Block::drag_true);
	ClassDB::bind_method(D_METHOD("drag_false"), &Block::drag_false);
	ClassDB::bind_method(D_METHOD("previous_block_entered", "body"), &Block::previous_block_entered);
	ClassDB::bind_method(D_METHOD("previous_block_exited", "body"), &Block::previous_block_exited);
}

Block::Block(): dragging(false), previous(nullptr), next(nullptr), destructable(true) {

}

Block::~Block() {

}

void Block::_ready(){
	// conneting button for dragging
	NodePath button_path("Button");
	Button *button = get_node<Button>(button_path);
	button->connect("button_down", Callable(this, "drag_true"));
	button->connect("button_up", Callable(this, "drag_false"));

	// connecting collision area for snaping
	NodePath area_2d_path("Area2D");
	Area2D *area_2d = get_node<Area2D>(area_2d_path);
	area_2d->connect("body_entered", Callable(this, "previous_block_entered"));
	area_2d->connect("body_exited", Callable(this, "previous_block_exited"));
	
	dragging = false;
	previous = nullptr;
	next = nullptr;

	if(get_node<Block>("/root/main/MainBlock") == this){
		set_position(get_viewport()->get_camera_2d()->get_position());
	}
	else {
		get_node<Block>("/root/main/MainBlock")->connect_next_block(this);
	}
	
}

void Block::_process(double delta) {
	if(dragging) set_position(get_viewport()->get_mouse_position() + offset);

	// moving attached block
	snap_to_previous_block(delta);
}

void Block::_physics_process(double delta){

}

void Block::drag_true(){
	dragging = true;
	offset = get_position() - get_viewport()->get_mouse_position();
}

void Block::drag_false(){
	dragging = false;
	constexpr float safety_anchor_offset = 0.02f; // to prevent acidental block deletion
	constexpr float ui_anchor = 0.2f;
	constexpr float ui_minimum_size = 350.0f;
	constexpr float safety_offset = 50.0f; // to prevent acidental block deletion when ui in minimum size

	if((get_viewport()->get_camera_2d()->get_position() - get_position()).x > get_viewport()->get_visible_rect().get_size().x * (0.5 - ui_anchor + safety_anchor_offset) || (get_viewport()->get_camera_2d()->get_position() - get_position()).x > get_viewport()->get_visible_rect().get_size().x / 2 - ui_minimum_size + safety_offset ) self_destruct(); // delete node if placed above ui spawner buttons
}

void Block::previous_block_entered(Node2D* body){
	if(previous != nullptr) return;
	Node* previous_block_node = body->get_parent();
	//UtilityFunctions::print(previous_block_node->get_class());
	Block* previous_block = dynamic_cast<Block*>(previous_block_node);
	if(previous_block->next != nullptr) return;
	previous = previous_block;
	previous->next = this;
}

void Block::previous_block_exited(Node2D *body){
	if(body != previous) return;
	Node* previous_block_node = body->get_parent();
	UtilityFunctions::print("body exited of type:" + previous_block_node->get_class());
	Block* previous_block = dynamic_cast<Block*>(previous_block_node);
	previous_block->next = nullptr;
	previous = nullptr;
}

void Block::snap_to_previous_block(double delta){
	if(previous == nullptr) return;
	if(get_position().distance_to(previous->get_position()) > 200 && dragging){
		previous->next = nullptr;
		previous = nullptr;
		return;
	}
	if(dragging) return;
	set_position(get_position().lerp(previous->get_position() + Vector2(0, 50), std::min(1.0f, float(delta * 100))));
}

void Block::self_destruct(){
	if(next != nullptr) next->self_destruct();
	if(destructable) queue_free();
	next = nullptr;
}

String Block::write(){
    String command = "";
    command += next->write();
    return command;
}

void Block::spawn_next_block(Ref<PackedScene> scene, PackedStringArray parameters) {
	UtilityFunctions::print("spawning next block");
	if (next != nullptr) {
		next->spawn_next_block(scene, parameters);
		return;
	}
	if (scene == nullptr) return;
	UtilityFunctions::print("spawning next block scene != nullptr");
	Node* new_block = scene->instantiate();
	add_sibling(new_block);
	if (new_block == nullptr) return;
	Block* block = dynamic_cast<Block*>(new_block);	
	if (block == nullptr) return;
	UtilityFunctions::print("spawning next block scene instantiated and casted");
	block->set_position(get_position() + Vector2(0, 50));
	next = block;
	block->previous = this;
	block->set_parameters(parameters);
}

void Block::set_parameters(PackedStringArray parameters) {
	// Default implementation does nothing, can be overridden by subclasses
	UtilityFunctions::print("No parameters to set for this block.");
}

void Block::connect_next_block(Block* next_block) {
	if (next != nullptr) {
		next->connect_next_block(next_block);
		return;
	}
	next = next_block;
	next->previous = this;

}