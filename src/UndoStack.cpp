#include "UndoStack.h"

void UndoStack::push(std::unique_ptr<Command> cmd, bool execute) {
    if (execute) cmd->redo();
    m_undo.push_back(std::move(cmd));
    m_redo.clear();   // a new action makes the old "future" impossible
}

bool UndoStack::undo() {
    if (m_undo.empty()) return false;
    std::unique_ptr<Command> cmd = std::move(m_undo.back());
    m_undo.pop_back();
    cmd->undo();
    m_redo.push_back(std::move(cmd));
    return true;
}

bool UndoStack::redo() {
    if (m_redo.empty()) return false;
    std::unique_ptr<Command> cmd = std::move(m_redo.back());
    m_redo.pop_back();
    cmd->redo();
    m_undo.push_back(std::move(cmd));
    return true;
}