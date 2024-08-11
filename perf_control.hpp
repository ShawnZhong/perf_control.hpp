#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdexcept>
#include <string>
#include <vector>

class PerfControl {
 public:
  PerfControl(const std::vector<std::string> &args) {
    if (pipe2(ctl_fds, 0) < 0 || pipe2(ack_fds, 0) < 0)
      throw std::runtime_error("Failed to create pipe");

    std::vector<std::string> perf_args = args;
    perf_args.insert(
        perf_args.end(),
        {"--control",
         "fd:" + std::to_string(ctl_fds[0]) + "," + std::to_string(ack_fds[1]),
         "--delay=-1", "-p", std::to_string(getpid())});
    perf_pid = exec(perf_args);
    control("ping");
  }

  ~PerfControl() {
    control("stop");
    wait(perf_pid);
  }

  void enable() const { control("enable"); }
  void disable() const { control("disable"); }

 private:
  int ctl_fds[2];
  int ack_fds[2];
  pid_t perf_pid;

  void control(const std::string &cmd) const {
    ssize_t ret = write(ctl_fds[1], cmd.c_str(), cmd.size());
    if (ret != cmd.size())
      throw std::runtime_error("Failed to write cmd: " + cmd);

    char buf[16] = {};
    ret = read(ack_fds[0], buf, sizeof(buf));
    if (ret < 0) throw std::runtime_error("Failed to read ack");
    if (ret != 5 || buf[0] != 'a' || buf[1] != 'c' || buf[2] != 'k')
      throw std::runtime_error("Unexpected ack: `" + std::string(buf) + "`");
  }

  static pid_t exec(const std::vector<std::string> &args) {
    pid_t pid = fork();
    if (pid < 0) throw std::runtime_error("Failed to fork");
    if (pid == 0) {  // child
      std::vector<char *> c_args;
      c_args.reserve(args.size() + 1);
      for (const auto &arg : args)
        c_args.emplace_back(const_cast<char *>(arg.c_str()));
      c_args.emplace_back(nullptr);
      execvp(c_args[0], c_args.data());
      throw std::runtime_error("Exec failed");
    }
    return pid;
  }

  static void wait(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status))
      throw std::runtime_error("Child proc exited abnormally");
    if (WEXITSTATUS(status) != 0)
      throw std::runtime_error("Child proc exited with code " +
                               std::to_string(WEXITSTATUS(status)));
  }
};