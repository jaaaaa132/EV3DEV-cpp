#include "sub_blocks.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

// MainBlock
void MainBlock::_bind_methods(){
    ClassDB::bind_method(D_METHOD("master_write", "path"), &MainBlock::master_write);
    ClassDB::bind_method(D_METHOD("load", "path"), &MainBlock::load);
}

MainBlock::MainBlock(): Block(){
    destructable = false;
}

MainBlock::~MainBlock(){
    Block::~Block();
}

void MainBlock::_ready(){
    Block::_ready();

    // conneting save dialog button
	NodePath save_dialog_path("/root/main/Ui/overlay/FileManipulation/SaveDialog");
	FileDialog *save_dialog = get_node<FileDialog>(save_dialog_path);
	save_dialog->connect("file_selected", Callable(this, "master_write"));

    // conneting load dialog button
	NodePath load_dialog_path("/root/main/Ui/overlay/FileManipulation/LoadDialog");
	FileDialog *load_dialog = get_node<FileDialog>(load_dialog_path);
	load_dialog->connect("file_selected", Callable(this, "load"));

}

void MainBlock::master_write(String path){
    Ref<FileAccess> file = FileAccess::open(path, FileAccess::WRITE);
    if (!file.is_valid()) return;
    if(next != nullptr){
        file->store_string(next->write());
    }
    file->close();
}

void MainBlock::load(String path) {
    self_destruct(); // Remove all sub blocks before loading new ones
    UtilityFunctions::print("Loading from: " + path);
    Ref<FileAccess> file = FileAccess::open(path, FileAccess::READ);
    if (!file.is_valid()) return;
    while(!file->eof_reached()) {
        String line = file->get_line(); 
        if (line.is_empty()) continue;
        UtilityFunctions::print("Read line: " + line);
        PackedStringArray words = line.split(" ", false);

        Ref<PackedScene> block = nullptr;
        switch(words[0][0]) {
            case 'K': // Comment block
                block = ResourceLoader::get_singleton()->load("res://blocks/comment_block.tscn");
                break;
            case 'S': // GoStraightToPos block
                block = ResourceLoader::get_singleton()->load("res://blocks/go_straight_to_pos_block.tscn");
                break;
            case 'C': // GoCurvedToPos block
                block = ResourceLoader::get_singleton()->load("res://blocks/go_curved_to_pos_block.tscn");
                break;
            case 'A': // RotateToAngle block
                block = ResourceLoader::get_singleton()->load("res://blocks/rotate_to_angle_block.tscn");
                break;
            case 'P': // RotateToPosition block
                block = ResourceLoader::get_singleton()->load("res://blocks/rotate_to_pos_block.tscn");
                break;
            case 'L':
            case 'R': // Arm blocks
                switch(words[1][0]){
                    case 'S': // ArmStop block
                        block = ResourceLoader::get_singleton()->load("res://blocks/arm_stop_block.tscn");
                        break;
                    case 'R': // ArmRun block
                        block = ResourceLoader::get_singleton()->load("res://blocks/arm_run_block.tscn");
                        break;
                    case 'A': // ArmRunToAbs block
                        block = ResourceLoader::get_singleton()->load("res://blocks/arm_run_to_abs_block.tscn");
                        break;
                    case 'M': // ArmRunToRel block
                        block = ResourceLoader::get_singleton()->load("res://blocks/arm_run_to_rel_block.tscn");
                        break;
                    case 'T': // ArmRunTimed block
                        block = ResourceLoader::get_singleton()->load("res://blocks/arm_run_timed_block.tscn");
                        break;
                    case 'D': // ArmRunDirect block
                        block = ResourceLoader::get_singleton()->load("res://blocks/arm_run_direct_block.tscn");
                        break;
                    case 'W': // ArmWait block
                        block = ResourceLoader::get_singleton()->load("res://blocks/arm_wait_block.tscn");
                }
                words[1] = words[0];
                break;
        }
        words.remove_at(0);  
        if (block != nullptr) spawn_next_block(block, words);
        else UtilityFunctions::push_error("block wasn't loaded properly, block == nullptr");
    }
}

// CommentBlock
void CommentBlock::_bind_methods(){

}

CommentBlock::CommentBlock(): Block() {
    
}

CommentBlock::~CommentBlock() {
    Block::~Block();
}

void CommentBlock::_ready() {
    Block::_ready();

    // getting textfield
    text_edit = get_node<LineEdit>("TextEdit");
}

String CommentBlock::write() {
    if(text_edit == nullptr) {
        UtilityFunctions::push_error("CommentBlock text_edit not set correctly!");
        return "error\n";
    }
    String command = "K " + text_edit->get_text() + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void CommentBlock::set_parameters(PackedStringArray parameters) {
    UtilityFunctions::print("Setting parameters for CommentBlock");
    if(text_edit == nullptr){
        UtilityFunctions::push_error("CommentBlock text_edit not set correctly!");
        return;
    }
    String text = "";
    for (const String &param : parameters) {
        text += param;
        text += " ";
    }
    text_edit->set_text(text);
}

// GoStraightToPosBlock
void GoStraightToPosBlock::_bind_methods(){

}

GoStraightToPosBlock::GoStraightToPosBlock(): Block() {
    
}

GoStraightToPosBlock::~GoStraightToPosBlock() {
    Block::~Block();
}

void GoStraightToPosBlock::_ready() {
    Block::_ready();

    // getting block parameters
    pos_x = get_node<LineEdit>("Button/Position/X");
    pos_y = get_node<LineEdit>("Button/Position/Y");
    angle_precision = get_node<LineEdit>("Button/AnglePrecision");
    max_speed = get_node<LineEdit>("Button/MaxSpeed");
    forward_only = get_node<CheckBox>("Button/ForwardOnly");
}

String GoStraightToPosBlock::write() {
    if(pos_x == nullptr || pos_y == nullptr || angle_precision == nullptr || max_speed == nullptr || forward_only == nullptr) {
        UtilityFunctions::push_error("GoStraightToPosBlock parameters not set correctly!");
        return "error\n";
    }
    String command = "S " + pos_x->get_text() + " " + pos_y->get_text() + " " + angle_precision->get_text() + " " + max_speed->get_text() + " " + (forward_only->is_pressed()? "1" : "0") + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void GoStraightToPosBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for GoStraightToPosBlock");
    if(pos_x == nullptr || pos_y == nullptr || angle_precision == nullptr || max_speed == nullptr || forward_only == nullptr){
        UtilityFunctions::push_error("GoStraightToPosBlock parameters not set correctly!");
        return;
    }
    pos_x->set_text(parameters[0]);
    pos_y->set_text(parameters[1]);
    angle_precision->set_text(parameters[2]);
    max_speed->set_text(parameters[3]);
    forward_only->set_pressed(bool(parameters[4] == "1"));
}

// GoCurvedToPosBlock
void GoCurvedToPosBlock::_bind_methods(){

}

GoCurvedToPosBlock::GoCurvedToPosBlock(): Block() {
    
}

GoCurvedToPosBlock::~GoCurvedToPosBlock() {
    Block::~Block();
}

void GoCurvedToPosBlock::_ready() {
    Block::_ready();

    // getting block parameters
    pos_x = get_node<LineEdit>("Button/Position/X");
    pos_y = get_node<LineEdit>("Button/Position/Y");
    precision = get_node<LineEdit>("Button/Precision");
    max_speed = get_node<LineEdit>("Button/MaxSpeed");
    forward_only = get_node<CheckBox>("Button/ForwardOnly");
}

String GoCurvedToPosBlock::write() {
    if(pos_x == nullptr || pos_y == nullptr || precision == nullptr || max_speed == nullptr || forward_only == nullptr) {
        UtilityFunctions::push_error("GoCurvedToPosBlock parameters not set correctly!");
        return "error\n";
    }
    String command = "C " + pos_x->get_text() + " " + pos_y->get_text() + " " + precision->get_text() + " " + max_speed->get_text() + " " + (forward_only->is_pressed()? "1" : "0") + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void GoCurvedToPosBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for GoCurvedToPosBlock");
    if(pos_x == nullptr || pos_y == nullptr || precision == nullptr || max_speed == nullptr || forward_only == nullptr){
        UtilityFunctions::push_error("GoCurvedToPosBlock parameters not set correctly!");
        return;
    }
    pos_x->set_text(parameters[0]);
    pos_y->set_text(parameters[1]);
    precision->set_text(parameters[2]);
    max_speed->set_text(parameters[3]);
    forward_only->set_pressed(bool(parameters[4] == "1"));
}

// RotateToAngleBlock
void RotateToAngleBlock::_bind_methods(){

}

RotateToAngleBlock::RotateToAngleBlock(): Block() {
    
}

RotateToAngleBlock::~RotateToAngleBlock() {
    Block::~Block();
}

void RotateToAngleBlock::_ready() {
    Block::_ready();

    // getting block parameters
    angle = get_node<LineEdit>("Button/HBoxContainer/Angle");
    angle_precision = get_node<LineEdit>("Button/HBoxContainer/AnglePrecision");
    max_speed = get_node<LineEdit>("Button/HBoxContainer/MaxSpeed");
}

String RotateToAngleBlock::write() {
    if(angle == nullptr || angle_precision == nullptr || max_speed == nullptr) {
        UtilityFunctions::push_error("RotateToAngleBlock parameters not set correctly!");
        return "error\n";
    }
    String command = "A " + angle->get_text() + " " + angle_precision->get_text() + " " + max_speed->get_text() + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void RotateToAngleBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for RotateToAngleBlock");
    if(angle == nullptr || angle_precision == nullptr || max_speed == nullptr){
        UtilityFunctions::push_error("RotateToAngleBlock parameters not set correctly!");
        return;
    }
    angle->set_text(parameters[0]);
    angle_precision->set_text(parameters[1]);
    max_speed->set_text(parameters[2]);
}

// RotateToPositionBlock
void RotateToPositionBlock::_bind_methods(){

}

RotateToPositionBlock::RotateToPositionBlock(): Block() {
    
}

RotateToPositionBlock::~RotateToPositionBlock() {
    Block::~Block();
}

void RotateToPositionBlock::_ready() {
    Block::_ready();

    // getting block parameters
    pos_x = get_node<LineEdit>("Button/HBoxContainer/Position/X");
    pos_y = get_node<LineEdit>("Button/HBoxContainer/Position/Y");
    angle_precision = get_node<LineEdit>("Button/HBoxContainer/AnglePrecision");
    max_speed = get_node<LineEdit>("Button/HBoxContainer/MaxSpeed");
    angle_offset = get_node<LineEdit>("Button/HBoxContainer/AngleOffset");
}

String RotateToPositionBlock::write() {
    if(pos_x == nullptr || pos_y == nullptr || angle_precision == nullptr || max_speed == nullptr || angle_offset == nullptr) {
        UtilityFunctions::push_error("RotateToPositionBlock parameters not set correctly!");
        return "error\n";
    }
    String command = "P " + pos_x->get_text() + " " + pos_y->get_text() + " " + angle_precision->get_text() + " " + max_speed->get_text() + " " + angle_offset->get_text() + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void RotateToPositionBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for RotateToPositionBlock");
    if(pos_x == nullptr || pos_y == nullptr || angle_precision == nullptr || max_speed == nullptr || angle_offset == nullptr){
        UtilityFunctions::push_error("RotateToPositionBlock parameters not set correctly!");
        return;
    }
    pos_x->set_text(parameters[0]);
    pos_y->set_text(parameters[1]);
    angle_precision->set_text(parameters[2]);
    max_speed->set_text(parameters[3]);
    angle_offset->set_text(parameters[4]);
}

// ArmStopBlock
void ArmStopBlock::_bind_methods(){

}

ArmStopBlock::ArmStopBlock(): Block() {
    
}

ArmStopBlock::~ArmStopBlock() {
    Block::~Block();
}

void ArmStopBlock::_ready() {
    Block::_ready();

    // getting block parameters
    arm = get_node<OptionButton>("Button/HBoxContainer/Arm");
    stop_action = get_node<OptionButton>("Button/HBoxContainer/StopAction");

}

String ArmStopBlock::write() {
    if(arm == nullptr || stop_action == nullptr) {
        UtilityFunctions::push_error("ArmStopBlock parameters not set correctly!");
        return "error\n";
    }
    String stop_action_text;
    switch(stop_action->get_selected_id()) {
        case 0: stop_action_text = "hold"; break;
        case 1: stop_action_text = "brake"; break;
        case 2: stop_action_text = "coast"; break;    
    }
    String command = String(arm->get_selected_id() == 0 ? "L " : "R ") + "S " + stop_action_text + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void ArmStopBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for ArmStopBlock");
    if(arm == nullptr || stop_action == nullptr){
        UtilityFunctions::push_error("ArmStopBlock parameters not set correctly!");
        return;
    }
    arm->select(parameters[0] == "L" ? 0 : 1);
    switch(parameters[1][0]) {
        case 'h': stop_action->select(0); break; // hold
        case 'b': stop_action->select(1); break; // brake
        case 'c': stop_action->select(2); break; // coast
        default: UtilityFunctions::push_error("Unknown stop action: " + parameters[1]);
    }
}

// ArmRunBlock
void ArmRunBlock::_bind_methods(){

}

ArmRunBlock::ArmRunBlock(): Block() {
    
}

ArmRunBlock::~ArmRunBlock() {
    Block::~Block();
}

void ArmRunBlock::_ready() {
    Block::_ready();

    // getting block parameters
    arm = get_node<OptionButton>("Button/HBoxContainer/Arm");
    speed = get_node<LineEdit>("Button/HBoxContainer/Speed");
    acceleration = get_node<LineEdit>("Button/HBoxContainer/Acceleration");

}

String ArmRunBlock::write() {
    if(arm == nullptr || speed == nullptr || acceleration == nullptr) {
        UtilityFunctions::push_error("ArmRunBlock parameters not set correctly!");
        return "error\n";
    }

    String command = String(arm->get_selected_id() == 0 ? "L " : "R ") + "R " + speed->get_text() + " " + acceleration->get_text() + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void ArmRunBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for ArmRunBlock");
    if(arm == nullptr || speed == nullptr || acceleration == nullptr){
        UtilityFunctions::push_error("ArmRunBlock parameters not set correctly!");
        return;
    }
    arm->select(parameters[0] == "L" ? 0 : 1);
    speed->set_text(parameters[1]);
    acceleration->set_text(parameters[2]);
}

// ArmRunToAbsBlock
void ArmRunToAbsBlock::_bind_methods(){

}

ArmRunToAbsBlock::ArmRunToAbsBlock(): Block() {
    
}

ArmRunToAbsBlock::~ArmRunToAbsBlock() {
    Block::~Block();
}

void ArmRunToAbsBlock::_ready() {
    Block::_ready();

    // getting block parameters
    arm = get_node<OptionButton>("Button/HBoxContainer/Arm");
    position = get_node<LineEdit>("Button/HBoxContainer/Position");
    speed = get_node<LineEdit>("Button/HBoxContainer/Speed");
    stop_action = get_node<OptionButton>("Button/HBoxContainer/StopAction");

}

String ArmRunToAbsBlock::write() {
    if(arm == nullptr || stop_action == nullptr || position == nullptr || speed == nullptr) {
        UtilityFunctions::push_error("ArmRunToAbsBlock parameters not set correctly!");
        return "error\n";
    }
    String stop_action_text;
    switch(stop_action->get_selected_id()) {
        case 0: stop_action_text = "hold"; break;
        case 1: stop_action_text = "brake"; break;
        case 2: stop_action_text = "coast"; break;    
    }
    String command = String(arm->get_selected_id() == 0 ? "L " : "R ") + "A " + position->get_text() + " " + speed->get_text() + " " + stop_action_text + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void ArmRunToAbsBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for ArmRunToAbsBlock");
    if(arm == nullptr || stop_action == nullptr || position == nullptr || speed == nullptr){
        UtilityFunctions::push_error("ArmRunToAbsBlock parameters not set correctly!");
        return;
    }
    arm->select(parameters[0] == "L" ? 0 : 1);
    position->set_text(parameters[1]);
    speed->set_text(parameters[2]);
    switch(parameters[3][0]) {
        case 'h': stop_action->select(0); break; // hold
        case 'b': stop_action->select(1); break; // brake
        case 'c': stop_action->select(2); break; // coast
        default: UtilityFunctions::push_error("Unknown stop action: " + parameters[1]);
    }
}

// ArmRunToRelBlock
void ArmRunToRelBlock::_bind_methods(){

}

ArmRunToRelBlock::ArmRunToRelBlock(): Block() {
    
}

ArmRunToRelBlock::~ArmRunToRelBlock() {
    Block::~Block();
}

void ArmRunToRelBlock::_ready() {
    Block::_ready();

    // getting block parameters
    arm = get_node<OptionButton>("Button/HBoxContainer/Arm");
    position = get_node<LineEdit>("Button/HBoxContainer/Position");
    speed = get_node<LineEdit>("Button/HBoxContainer/Speed");
    stop_action = get_node<OptionButton>("Button/HBoxContainer/StopAction");

}

String ArmRunToRelBlock::write() {
    if(arm == nullptr || stop_action == nullptr || position == nullptr || speed == nullptr) {
        UtilityFunctions::push_error("ArmRunToRelBlock parameters not set correctly!");
        return "error\n";
    }
    String stop_action_text;
    switch(stop_action->get_selected_id()) {
        case 0: stop_action_text = "hold"; break;
        case 1: stop_action_text = "brake"; break;
        case 2: stop_action_text = "coast"; break;    
    }
    String command = String(arm->get_selected_id() == 0 ? "L " : "R ") + "M " + position->get_text() + " " + speed->get_text() + " " + stop_action_text + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void ArmRunToRelBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for ArmRunToRelBlock");
    if(arm == nullptr || stop_action == nullptr || position == nullptr || speed == nullptr){
        UtilityFunctions::push_error("ArmRunToRelBlock parameters not set correctly!");
        return;
    }
    arm->select(parameters[0] == "L" ? 0 : 1);
    position->set_text(parameters[1]);
    speed->set_text(parameters[2]);
    switch(parameters[3][0]) {
        case 'h': stop_action->select(0); break; // hold
        case 'b': stop_action->select(1); break; // brake
        case 'c': stop_action->select(2); break; // coast
        default: UtilityFunctions::push_error("Unknown stop action: " + parameters[1]);
    }
}

// ArmRunTimedBlock
void ArmRunTimedBlock::_bind_methods(){

}

ArmRunTimedBlock::ArmRunTimedBlock(): Block() {
    
}

ArmRunTimedBlock::~ArmRunTimedBlock() {
    Block::~Block();
}

void ArmRunTimedBlock::_ready() {
    Block::_ready();

    // getting block parameters
    arm = get_node<OptionButton>("Button/HBoxContainer/Arm");
    time = get_node<LineEdit>("Button/HBoxContainer/Time");
    speed = get_node<LineEdit>("Button/HBoxContainer/Speed");
    stop_action = get_node<OptionButton>("Button/HBoxContainer/StopAction");

}

String ArmRunTimedBlock::write() {
    if(arm == nullptr || stop_action == nullptr || time == nullptr || speed == nullptr) {
        UtilityFunctions::push_error("ArmRunTimedBlock parameters not set correctly!");
        return "error\n";
    }
    String stop_action_text;
    switch(stop_action->get_selected_id()) {
        case 0: stop_action_text = "hold"; break;
        case 1: stop_action_text = "brake"; break;
        case 2: stop_action_text = "coast"; break;    
    }
    String command = String(arm->get_selected_id() == 0 ? "L " : "R ") + "T " + time->get_text() + " " + speed->get_text() + " " + stop_action_text + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void ArmRunTimedBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for ArmRunTimedBlock");
    if(arm == nullptr || stop_action == nullptr || time == nullptr || speed == nullptr){
        UtilityFunctions::push_error("ArmRunTimedBlock parameters not set correctly!");
        return;
    }
    arm->select(parameters[0] == "L" ? 0 : 1);
    time->set_text(parameters[1]);
    speed->set_text(parameters[2]);
    switch(parameters[3][0]) {
        case 'h': stop_action->select(0); break; // hold
        case 'b': stop_action->select(1); break; // brake
        case 'c': stop_action->select(2); break; // coast
        default: UtilityFunctions::push_error("Unknown stop action: " + parameters[1]);
    }
}

// ArmRunDirectBlock
void ArmRunDirectBlock::_bind_methods(){

}

ArmRunDirectBlock::ArmRunDirectBlock(): Block() {
    
}

ArmRunDirectBlock::~ArmRunDirectBlock() {
    Block::~Block();
}

void ArmRunDirectBlock::_ready() {
    Block::_ready();

    // getting block parameters
    arm = get_node<OptionButton>("Button/HBoxContainer/Arm");
    speed = get_node<LineEdit>("Button/HBoxContainer/Speed");
    reversed = get_node<CheckBox>("Button/HBoxContainer/FHELP/Reversed");

}

String ArmRunDirectBlock::write() {
    if(arm == nullptr || speed == nullptr || reversed == nullptr) {
        UtilityFunctions::push_error("ArmRunDirectBlock parameters not set correctly!");
        return "error\n";
    }
    String command = String(arm->get_selected_id() == 0 ? "L " : "R ") + "D " + speed->get_text() + " " + String(reversed->is_pressed() ? "1" : "0") + "\n";
    if (next != nullptr) command += next->write();
    return command;
}

void ArmRunDirectBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for ArmRunDirectBlock");
    if(arm == nullptr || speed == nullptr || reversed == nullptr){
        UtilityFunctions::push_error("ArmRunDirectBlock parameters not set correctly!");
        return;
    }
    arm->select(parameters[0] == "L" ? 0 : 1);
    speed->set_text(parameters[1]);
    reversed->set_pressed(parameters[2] == "1");
}

// ArmWaitBlock
void ArmWaitBlock::_bind_methods(){

}

ArmWaitBlock::ArmWaitBlock(): Block() {
    
}

ArmWaitBlock::~ArmWaitBlock() {
    Block::~Block();
}

void ArmWaitBlock::_ready() {
    Block::_ready();

    // getting block parameters
    arm = get_node<OptionButton>("Button/HBoxContainer/Arm");

}

String ArmWaitBlock::write() {
    if(arm == nullptr) {
        UtilityFunctions::push_error("ArmWaitBlock parameters not set correctly!");
        return "error\n";
    }
    String command = String(arm->get_selected_id() == 0 ? "L " : "R ") + "W\n";
    if (next != nullptr) command += next->write();
    return command;
}

void ArmWaitBlock::set_parameters(PackedStringArray parameters) {
    if(parameters.size() == 0) return;
    UtilityFunctions::print("Setting parameters for ArmWaitBlock");
    if(arm == nullptr){
        UtilityFunctions::push_error("ArmWaitBlock parameters not set correctly!");
        return;
    }
    arm->select(parameters[0] == "L" ? 0 : 1);
}