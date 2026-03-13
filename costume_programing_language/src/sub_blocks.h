#ifndef SUB_BLOCKS_H
#define SUB_BLOCKS_H

#include "block.h"
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/option_button.hpp>

namespace godot {

class MainBlock : public Block {
	GDCLASS(MainBlock, Block)
private:
protected:
    static void _bind_methods();
public:
    MainBlock();
    ~MainBlock();

    void _ready() override;

    void master_write(String path);
    void load(String path);
};

class CommentBlock : public Block {
    GDCLASS(CommentBlock, Block)
private:
protected:
    static void _bind_methods();
    LineEdit *text_edit = nullptr;
public:
    CommentBlock();
    ~CommentBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class GoStraightToPosBlock : public Block {
    GDCLASS(GoStraightToPosBlock, Block)
private:
protected:
    static void _bind_methods();
    LineEdit *pos_x = nullptr;
    LineEdit *pos_y = nullptr;
    LineEdit *angle_precision = nullptr;
    LineEdit *max_speed = nullptr;
    CheckBox *forward_only = nullptr;

public:
    GoStraightToPosBlock();
    ~GoStraightToPosBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class GoCurvedToPosBlock : public Block {
    GDCLASS(GoCurvedToPosBlock, Block)
private:
protected:
    static void _bind_methods();
    LineEdit *pos_x = nullptr;
    LineEdit *pos_y = nullptr;
    LineEdit *precision = nullptr;
    LineEdit *max_speed = nullptr;
    CheckBox *forward_only = nullptr;

public:
    GoCurvedToPosBlock();
    ~GoCurvedToPosBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class MoveTankDirectTimedBlock : public Block {
    GDCLASS(MoveTankDirectTimedBlock, Block)
private:
protected:
    static void _bind_methods();
    LineEdit *left_motor_speed = nullptr;
    LineEdit *right_motor_speed = nullptr;
    LineEdit *time_ms = nullptr;
    OptionButton *stop_action = nullptr;		

public:
    MoveTankDirectTimedBlock();
    ~MoveTankDirectTimedBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class RotateToAngleBlock : public Block {
    GDCLASS(RotateToAngleBlock, Block)
private:
protected:
    static void _bind_methods();
    LineEdit *angle = nullptr;
    LineEdit *angle_precision = nullptr;
    LineEdit *max_speed = nullptr;

public:
    RotateToAngleBlock();
    ~RotateToAngleBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class RotateToPositionBlock : public Block {
    GDCLASS(RotateToPositionBlock, Block)
private:
protected:
    static void _bind_methods();
    LineEdit *pos_x = nullptr;
    LineEdit *pos_y = nullptr;
    LineEdit *angle_precision = nullptr;
    LineEdit *max_speed = nullptr;
    LineEdit *angle_offset = nullptr;

public:
    RotateToPositionBlock();
    ~RotateToPositionBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class ResetPositionBlock : public Block {
    GDCLASS(ResetPositionBlock, Block)
private:
protected:
    static void _bind_methods();
    LineEdit *x = nullptr;
    LineEdit *y = nullptr;
    LineEdit *angle = nullptr; 

public:
    ResetPositionBlock();
    ~ResetPositionBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class GoUntilReflectionBlock : public Block {
    GDCLASS(GoUntilReflectionBlock, Block)
private:
protected:
    static void _bind_methods();
    LineEdit *reflection = nullptr;
    CheckBox *darker = nullptr;
    LineEdit *speed = nullptr; 

public:
    GoUntilReflectionBlock();
    ~GoUntilReflectionBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class ArmStopBlock : public Block {
    GDCLASS(ArmStopBlock, Block)
private:
protected:
    static void _bind_methods();
    OptionButton *arm = nullptr;
    OptionButton *stop_action = nullptr;

public:
    ArmStopBlock();
    ~ArmStopBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class ArmRunBlock : public Block {
    GDCLASS(ArmRunBlock, Block)
private:
protected:
    static void _bind_methods();
    OptionButton *arm = nullptr;
    LineEdit *speed = nullptr;
    LineEdit *acceleration = nullptr;

public:
    ArmRunBlock();
    ~ArmRunBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class ArmRunToAbsBlock : public Block {
    GDCLASS(ArmRunToAbsBlock, Block)
private:
protected:
    static void _bind_methods();
    OptionButton *arm = nullptr;
    LineEdit *position = nullptr;
    LineEdit *speed = nullptr;
    OptionButton *stop_action = nullptr;

public:
    ArmRunToAbsBlock();
    ~ArmRunToAbsBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class ArmRunToRelBlock : public Block {
    GDCLASS(ArmRunToRelBlock, Block)
private:
protected:
    static void _bind_methods();
    OptionButton *arm = nullptr;
    LineEdit *position = nullptr;
    LineEdit *speed = nullptr;
    OptionButton *stop_action = nullptr;

public:
    ArmRunToRelBlock();
    ~ArmRunToRelBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class ArmRunTimedBlock : public Block {
    GDCLASS(ArmRunTimedBlock, Block)
private:
protected:
    static void _bind_methods();
    OptionButton *arm = nullptr;
    LineEdit *time = nullptr;
    LineEdit *speed = nullptr;
    OptionButton *stop_action = nullptr;

public:
    ArmRunTimedBlock();
    ~ArmRunTimedBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class ArmRunDirectBlock : public Block {
    GDCLASS(ArmRunDirectBlock, Block)
private:
protected:
    static void _bind_methods();
    OptionButton *arm = nullptr;
    LineEdit *speed = nullptr;
    CheckBox *reversed = nullptr; 

public:
    ArmRunDirectBlock();
    ~ArmRunDirectBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

class ArmWaitBlock : public Block {
    GDCLASS(ArmWaitBlock, Block)
private:
protected:
    static void _bind_methods();
    OptionButton *arm = nullptr;

public:
    ArmWaitBlock();
    ~ArmWaitBlock();

    void _ready() override;

    String write() override;
    void set_parameters(PackedStringArray parameters) override;
};

}
#endif