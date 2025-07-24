extends Control


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass


func _on_exit_pressed() -> void:
	var X = get_node("HBoxContainer/X")
	var Y = get_node("HBoxContainer/Y")
	var acpect_ratio_map_container = get_node("/root/main/Map/Map/AspectRatioContainer")
	var map_size = get_node("/root/main/Map/Map/AspectRatioContainer/TextureRect/Size")
	acpect_ratio_map_container.ratio = float(X.text)/float(Y.text)
	map_size.x = X.text
	map_size.y = Y.text
	queue_free()
