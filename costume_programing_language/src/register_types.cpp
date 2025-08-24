#include "register_types.h"

#include "block.h"
#include "sub_blocks.h"
#include "block_spawner.h"
#include "draggable_camera.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_RUNTIME_CLASS(Block);
	GDREGISTER_RUNTIME_CLASS(MainBlock);
	GDREGISTER_RUNTIME_CLASS(CommentBlock);
	GDREGISTER_RUNTIME_CLASS(GoStraightToPosBlock);
	GDREGISTER_RUNTIME_CLASS(GoCurvedToPosBlock);
	GDREGISTER_RUNTIME_CLASS(RotateToAngleBlock);
	GDREGISTER_RUNTIME_CLASS(RotateToPositionBlock);
	GDREGISTER_RUNTIME_CLASS(ArmStopBlock);
	GDREGISTER_RUNTIME_CLASS(ArmRunBlock);
	GDREGISTER_RUNTIME_CLASS(ArmRunToAbsBlock);
	GDREGISTER_RUNTIME_CLASS(ArmRunToRelBlock);
	GDREGISTER_RUNTIME_CLASS(ArmRunTimedBlock);
	GDREGISTER_RUNTIME_CLASS(ArmRunDirectBlock);
	GDREGISTER_RUNTIME_CLASS(ArmWaitBlock);
	GDREGISTER_RUNTIME_CLASS(BlockSpawner);
	GDREGISTER_RUNTIME_CLASS(DraggableCamera);
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_example_module);
	init_obj.register_terminator(uninitialize_example_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
