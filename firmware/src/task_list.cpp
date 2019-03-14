#include "task_list.h"

#include <memory>
#include <string>
#include <vector>

#include "task.h"

auto TaskList::at(const std::string key) -> std::unique_ptr<Task>& {
  return list.at(map.at(key));
}

auto TaskList::begin() const -> std::vector<std::unique_ptr<Task>>::const_iterator {
  return list.begin();
}

auto TaskList::contains(const std::string key) const -> bool {
  return map.find(key) == map.end();
}

auto TaskList::end() const -> std::vector<std::unique_ptr<Task>>::const_iterator {
  return list.end();
}

auto TaskList::emplace_back(const std::string key, Task* const task) -> void {
  auto index = list.size();
  map.emplace(key, index);
  list.emplace_back(task);
}

auto TaskList::erase(const std::string key) -> void {
  auto index = map.at(key);
  map.erase(key);
  list.erase(list.begin() + index);
}
