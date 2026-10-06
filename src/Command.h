#pragma once

// Abstract command: one user action that can be redone and undone.
class Command {
public:
    virtual ~Command() = default;
    virtual void redo() = 0;   // do (or re-do) the action
    virtual void undo() = 0;   // reverse it
};