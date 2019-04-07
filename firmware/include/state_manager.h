#if !defined(__STATE_MANAGER_H__)
#define __STATE_MANAGER_H__

#include <string>

class StateManager {
  private:
    static auto parseFloat(const std::string&) -> float;
    static auto printFloat(const float) -> std::string;
  public:
    enum class Mode {
      RAW,
      VEL_PID,
    };
    struct {
      Mode mode = Mode::RAW;
      float motor_power = 0; // %
      float enc_vel = 0; // Hz
      float target_vel = 0; // Hz
      bool debug = false;
    } state = {};
    auto executeTextCommand(const std::string&) -> void;
};

#endif // __STATE_MANAGER_H__
