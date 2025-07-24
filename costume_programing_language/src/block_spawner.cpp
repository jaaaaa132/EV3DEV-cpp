#include "block_spawner.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void BlockSpawner::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_pressed"), &BlockSpawner::on_pressed);

    ClassDB::bind_method(D_METHOD("set_scene", "scene"), &BlockSpawner::set_scene);
    ClassDB::bind_method(D_METHOD("get_scene"), &BlockSpawner::get_scene);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");
}

BlockSpawner::BlockSpawner(){
    connect("pressed", Callable(this, "on_pressed"));
}

BlockSpawner::~BlockSpawner(){

}

void BlockSpawner::_ready(){
	
}

void BlockSpawner::_process(double delta){
	
}

void BlockSpawner::_physics_process(double delta){

}

void BlockSpawner::set_scene(const Ref<PackedScene> &p_scene){
    scene_ref = p_scene;
}

Ref<PackedScene> BlockSpawner::get_scene(){
    return scene_ref;
}

void BlockSpawner::on_pressed(){
    Node* Block = scene_ref->instantiate();
    get_node<Node>("/root/main")->add_child(Block);
}