#if !defined(__TASK_LIST_H__)
#define __TASK_LIST_H__

#include <memory>
#include <stddef.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "task.h"

class TaskList {
  private:
    std::vector<std::unique_ptr<Task>> list;
    std::unordered_map<std::string, size_t> map;
  public:
    auto at(const std::string) -> std::unique_ptr<Task>&;
    auto begin() const -> std::vector<std::unique_ptr<Task>>::const_iterator;
    auto contains(const std::string) const -> bool;
    auto end() const -> std::vector<std::unique_ptr<Task>>::const_iterator;
    auto emplace_back(const std::string, Task* const) -> void;
    auto erase(const std::string) -> void;
};

#endif // __TASK_LIST_H__
