extends Button

var file_path: String
var ip :String

func _on_pressed() -> void:
	var panel: Panel = get_node("Panel")
	panel.visible = !panel.visible

func _on_file_button_pressed() -> void:
	get_node("Panel/VBoxContainer/File/Load").visible = true

func _on_file_selected(path: String) -> void:
	file_path = path

func _on_send_pressed() -> void:
	ip = $Panel/VBoxContainer/HBoxContainer2/IP.text
	var args = [
		"-p", "maker",
		"scp", file_path, "robot@" + ip + ":/home/robot/libc++/"
	]
	OS.execute("sshpass", args)
