extends Button


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass


func _on_pressed() -> void:
	get_node("/root/main/Map/Map").visible = true
	var map_size = get_node("/root/main/Map/Map/AspectRatioContainer/TextureRect/Size")
	var point = get_node("/root/main/Map/Map/AspectRatioContainer/TextureRect/Point")
	var x = float(get_node("X").text)
	var y = float(get_node("Y").text)
	point.anchor_left =  x / float(map_size.x)
	point.anchor_right = point.anchor_left
	point.anchor_top = 1 - y / float(map_size.y)
	point.anchor_bottom = point.anchor_top
	
