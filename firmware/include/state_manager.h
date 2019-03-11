#if !defined(__STATE_MANAGER_H__)
#define __STATE_MANAGER_H__

#include <any>
#include <string>

class StateManager {
  private:
    static auto parseFloat(const char*) -> float;
  public:
    enum class Mode {
      RAW,
    };
    struct {
      Mode mode = Mode::RAW;
      float motor_power = 0; // -1 ~ 1
      float enc_speed = 0; // Hz
    } state = {};
    auto executeTextCommand(const std::string) -> void;
};

#endif // __STATE_MANAGER_H__
