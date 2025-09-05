extends Button


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass


func _on_pressed() -> void:
	var file_dialog = get_node("FileDialog")
	file_dialog.visible = true


func _on_file_dialog_file_selected(path: String) -> void:	
	#var map_texture = get_node("/root/main/Map/Map/AspectRatioContainer/TextureRect")
	#map_texture.texture = load(path)
	pass
	
