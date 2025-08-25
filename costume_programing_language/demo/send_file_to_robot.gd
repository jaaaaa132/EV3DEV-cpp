extends Button

var file_path: String

func _on_pressed() -> void:
	var panel: Panel = get_node("Panel")
	panel.visible = !panel.visible

func _on_file_button_pressed() -> void:
	$Panel/Options/VBoxContainer/File/Load.visible = true

func _on_file_selected(path: String) -> void:
	file_path = path

func _on_send_pressed() -> void:
	pass # finish next time
