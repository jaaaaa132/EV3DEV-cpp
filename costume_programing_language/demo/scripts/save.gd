extends Button

@onready var save_dialog = $"../SaveDialog"

func _on_pressed() -> void:
	save_dialog.visible = true
