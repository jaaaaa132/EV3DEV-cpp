extends Control


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass

func _on_en_pressed() -> void:
	TranslationServer.set_locale("en")
	get_tree().change_scene_to_file("res://main.tscn")

func _on_sk_pressed() -> void:
	TranslationServer.set_locale("sk")
	get_tree().change_scene_to_file("res://main.tscn")
