extends Button


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass


func _on_pressed() -> void:
	var panel = get_node("Panel")
	panel.visible = !panel.visible


func _on_save_pressed() -> void:
	var file_dialog = get_node("Panel/VBoxContainer/HBoxContainer/Save/FileDialog")
	file_dialog.visible = true


func _on_load_pressed() -> void:
	var file_dialog = get_node("Panel/VBoxContainer/HBoxContainer/Load/FileDialog")
	file_dialog.visible = true


func _on_save_file_dialog_file_selected(path: String) -> void:
	var base_width = float($Panel/VBoxContainer/BaseWidth/LineEdit.text)
	var wheel_circumference = float($Panel/VBoxContainer/WheelCircumference/LineEdit.text)
	var starting_position_x = float($Panel/VBoxContainer/StartingPosition/Values/LineEditX.text)
	var starting_position_y = float($Panel/VBoxContainer/StartingPosition/Values/LineEditY.text)
	var starting_position_angle = float($Panel/VBoxContainer/StartingPosition/Values/LineEditAngle.text)
	var left_motor_inversed = bool($"Panel/VBoxContainer/Motors inversed/StartingPosition/CheckBoxLeftMotor".button_pressed)
	var right_motor_inversed = bool($"Panel/VBoxContainer/Motors inversed/StartingPosition/CheckBoxRightMotor".button_pressed)
	
	var save_file = FileAccess.open(path,FileAccess.WRITE)
	save_file.store_line(str(base_width) + " " + str(wheel_circumference))
	save_file.store_line(str(starting_position_x) + " " + str(starting_position_y) + " " + str(starting_position_angle))
	save_file.store_line(str(int(left_motor_inversed)) + " " + str(int(right_motor_inversed)))


func _on_load_file_dialog_file_selected(path: String) -> void:
	var base_width = $Panel/VBoxContainer/BaseWidth/LineEdit as LineEdit
	var wheel_circumference = $Panel/VBoxContainer/WheelCircumference/LineEdit as LineEdit
	var starting_position_x = $Panel/VBoxContainer/StartingPosition/Values/LineEditX as LineEdit
	var starting_position_y = $Panel/VBoxContainer/StartingPosition/Values/LineEditY as LineEdit
	var starting_position_angle = $Panel/VBoxContainer/StartingPosition/Values/LineEditAngle as LineEdit
	var left_motor_inversed = $"Panel/VBoxContainer/Motors inversed/StartingPosition/CheckBoxLeftMotor" as CheckBox
	var right_motor_inversed = $"Panel/VBoxContainer/Motors inversed/StartingPosition/CheckBoxRightMotor" as CheckBox
	
	var load_file = FileAccess.open(path, FileAccess.READ)
	var parameters = load_file.get_line().split(" ")
	parameters.append_array(load_file.get_line().split(" "))
	parameters.append_array(load_file.get_line().split(" "))
	
	base_width.text = parameters.get(0)
	wheel_circumference.text = parameters.get(1)
	starting_position_x.text = parameters.get(2)
	starting_position_y.text = parameters.get(3)
	starting_position_angle.text = parameters.get(4)
	left_motor_inversed.button_pressed = parameters.get(5) == "1"
	right_motor_inversed.button_pressed = parameters.get(6) == "1"
	
