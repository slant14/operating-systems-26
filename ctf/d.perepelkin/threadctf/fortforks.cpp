#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include <cerrno>
#include <csignal>
#include <cstdlib>

import std;

namespace
{
    namespace Constants
    {
        inline constexpr std::string_view pipePath{"/home/ctf/secrets"};
        inline constexpr std::string_view outputPath{"/tmp/flag.txt"};
        inline constexpr std::string_view statusPath{"/tmp/status.txt"};

        inline constexpr std::size_t kilobyte{1u << 10u};
        inline constexpr std::size_t megaByte{(1u << 10u) * kilobyte};

        inline constexpr std::size_t readBufferSize{256u * kilobyte};
        inline constexpr std::size_t tailBufferSize{64u * kilobyte};

        inline constexpr std::size_t pipeKernelBufferSize{megaByte};

        using namespace std::chrono_literals;
        constexpr auto relayInterval{1500ms};
    }

    volatile std::sig_atomic_t mustRelay{0};

    constexpr void alarmHandler(int) noexcept
    {
        mustRelay = 1;
    }

    class UniqueFd
    {
    public:
        UniqueFd() noexcept : fd{-1}
        {
        }

        explicit UniqueFd(const int fd) noexcept : fd{fd}
        {
        }

        ~UniqueFd() noexcept
        {
            reset();
        }

        UniqueFd(const UniqueFd&) = delete;
        UniqueFd& operator=(const UniqueFd&) = delete;

        UniqueFd(UniqueFd&& other) noexcept : fd{std::exchange(other.fd, -1)}
        {
        }

        UniqueFd& operator=(UniqueFd&& other) noexcept
        {
            if (this != &other) reset(std::exchange(other.fd, -1));
            return *this;
        }

        [[nodiscard]] static std::expected<UniqueFd, std::error_code> open(
            const std::filesystem::path& path, const int flags, const mode_t mode = 0) noexcept
        {
            const int fd = ::open(path.c_str(), flags, mode);
            if (fd < 0)
            {
                return std::unexpected(std::error_code(errno, std::generic_category()));
            }
            return UniqueFd{fd};
        }

        void reset(const int newFd = -1) noexcept
        {
            if (fd >= 0)
            {
                ::close(fd);
            }
            fd = newFd;
        }

        [[nodiscard]] int get() const noexcept
        {
            return fd;
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return fd >= 0;
        }

    private:
        int fd;
    };

    class TailBuffer
    {
    public:
        explicit TailBuffer(const std::size_t capacity) noexcept
        {
            buffer.reserve(capacity);
        }

        void append(std::span<const std::byte> data) noexcept
        {
            const std::size_t maxCapacity{buffer.capacity()};
            if (!maxCapacity) return;
            if (data.size() >= maxCapacity)
            {
                buffer.assign_range(data.last(maxCapacity));
                return;
            }

            if (const std::size_t currentSize{buffer.size()};
                currentSize + data.size() > maxCapacity)
            {
                const auto overflow{static_cast<std::ptrdiff_t>((currentSize + data.size()) - maxCapacity)};
                buffer.erase(buffer.begin(), buffer.begin() + overflow);
            }

            buffer.append_range(data);
        }

        [[nodiscard]] std::span<const std::byte> view() const noexcept
        {
            return buffer;
        }

    private:
        std::vector<std::byte> buffer;
    };

    void writeAll(const int fd, const std::span<const std::byte> data) noexcept
    {
        std::size_t offset{0u};
        while (offset < data.size())
        {
            const ssize_t bytesWritten{::write(fd, data.data() + offset, data.size() - offset)};
            if (bytesWritten < 0)
            {
                if (errno == EINTR) continue;
                break;
            }
            if (!bytesWritten) break;
            offset += static_cast<std::size_t>(bytesWritten);
        }
    }

    void writeStatus(std::uint64_t totalBytes, std::string_view state) noexcept
    {
        const auto statusFd{
            UniqueFd::open(Constants::statusPath.data(), O_WRONLY | O_CREAT | O_TRUNC, 0644)
        };
        if (!statusFd) return;

        std::string output{
            std::format(
                "PID: {} | Read: {} MiB ({} B) | State: {}\n",
                getpid(),
                totalBytes / Constants::megaByte,
                totalBytes,
                state
            )
        };

        writeAll(statusFd->get(), std::as_bytes(std::span{output}));
    }

    void armTimer() noexcept
    {
        const auto secs{std::chrono::duration_cast<std::chrono::seconds>(Constants::relayInterval)};
        const auto usecs{std::chrono::duration_cast<std::chrono::microseconds>(Constants::relayInterval - secs)};

        const itimerval timer{
            .it_value = {
                .tv_sec = secs.count(),
                .tv_usec = usecs.count()
            }
        };
        setitimer(ITIMER_REAL, &timer, nullptr);
    }

    void configureSignals() noexcept
    {
        std::signal(SIGHUP, SIG_IGN);
        std::signal(SIGPIPE, SIG_IGN);

        struct sigaction action{};
        action.sa_handler = alarmHandler;
        action.sa_flags = 0;

        sigemptyset(&action.sa_mask);
        sigaction(SIGALRM, &action, nullptr);
    }

    void detachFromTerminal() noexcept
    {
        const pid_t pid{fork()};
        if (pid == -1) _exit(1);
        if (pid > 0) _exit(0);

        setsid();

        if (const auto devNull = UniqueFd::open("/dev/null", O_RDWR))
        {
            dup2(devNull->get(), STDIN_FILENO);
            dup2(devNull->get(), STDOUT_FILENO);
            dup2(devNull->get(), STDERR_FILENO);
        }
    }

    void relayProcess(const std::uint64_t totalBytes, const TailBuffer& tailBuffer) noexcept
    {
        if (const auto outFd{UniqueFd::open(Constants::outputPath.data(), O_WRONLY | O_CREAT | O_TRUNC, 0644)})
        {
            writeAll(outFd->get(), tailBuffer.view());
        }

        const pid_t pid{fork()};
        if (pid < 0) return;
        if (pid > 0) _exit(0);

        setsid();
        mustRelay = 0;
        writeStatus(totalBytes, "RUNNING");
        armTimer();
    }
}

int main()
{
    detachFromTerminal();
    configureSignals();

    const auto pipeFd{UniqueFd::open(Constants::pipePath.data(), O_RDONLY)};
    if (!pipeFd)
    {
        writeStatus(0, "ERROR_OPEN_FILE");
        return EXIT_FAILURE;
    }

    fcntl(pipeFd->get(), F_SETPIPE_SZ, Constants::pipeKernelBufferSize);

    std::vector<std::byte> readBuffer(Constants::readBufferSize);
    TailBuffer tailBuffer{Constants::tailBufferSize};

    std::uint64_t totalBytes{0};
    auto lastRelayTime{std::chrono::high_resolution_clock::now()};

    writeStatus(0, "STARTED");
    armTimer();

    while (true)
    {
        const auto now{std::chrono::high_resolution_clock::now()};
        if (mustRelay || (now - lastRelayTime) >= Constants::relayInterval)
        {
            relayProcess(totalBytes, tailBuffer);
            lastRelayTime = std::chrono::high_resolution_clock::now();
        }

        const ssize_t bytesRead{read(pipeFd->get(), readBuffer.data(), readBuffer.size())};
        if (bytesRead < 0)
        {
            if (errno == EINTR) continue;
            writeStatus(totalBytes, "ERROR_READ");
            return EXIT_FAILURE;
        }

        if (bytesRead == 0)
        {
            writeStatus(totalBytes, "COMPLETED");
            break;
        }

        const auto count{static_cast<std::size_t>(bytesRead)};
        totalBytes += count;
        tailBuffer.append(std::span{readBuffer}.first(count));
    }

    if (const auto outFd{UniqueFd::open(Constants::outputPath.data(), O_WRONLY | O_CREAT | O_TRUNC, 0644)})
    {
        writeAll(outFd->get(), tailBuffer.view());
    }


    return EXIT_SUCCESS;
}
