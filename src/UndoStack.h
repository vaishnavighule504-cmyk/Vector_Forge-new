#pragma once
#include <memory>
#include <vector>
#include "Command.h"

class UndoStack {
public:
    // execute = false when the action has already happened (like a drag-move).
    void push(std::unique_ptr<Command> cmd, bool execute = true);
    bool undo();
    bool redo();

        void clear() { m_undo.clear(); m_redo.clear(); }
private:
    std::vector<std::unique_ptr<Command>> m_undo;
    std::vector<std::unique_ptr<Command>> m_redo;
};