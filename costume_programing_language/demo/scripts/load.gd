extends Button

@onready var load_dialog = $"../LoadDialog"

func _on_pressed() -> void:
	load_dialog.visible = true
