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
	send_file()

func send_file():
	var os_name = OS.get_name()
	var user = "robot"
	var password = "maker"
	ip = $Panel/VBoxContainer/HBoxContainer2/IP.text
	var target = "%s@%s:/home/robot/libc++/" % [user, ip]

	if os_name == "Windows":
		print("windows")
		# Use PuTTY's pscp.exe
		var args = [
			"-pw", password,
			file_path,
			target
			]
		OS.execute("pscp.exe", args)

	elif os_name == "X11" or os_name == "Linux":
		print("linux")
		# Use sshpass + scp
		var args = [
			"-p", password,
			"scp", file_path, target
			]
		OS.execute("sshpass", args)
	else:
		print("unsupported OS")
