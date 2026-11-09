#include <cerrno>
#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <spawn.h>

import std;

struct Shard
{
    std::uint32_t id;
    std::filesystem::path path;
};

extern char** environ;

namespace Constants
{
    inline constexpr std::string_view decryptorPath{"./Decrapinator/decrapinator-x86_64-linux"};
    inline constexpr std::string_view shardsPath{"./shards"};
    inline constexpr std::size_t BufferSize{256u};
    inline constexpr std::size_t ThreadsPerShard{5u};
}

void bumpFdLimit()
{
    rlimit rl{};
    if (getrlimit(RLIMIT_NOFILE, &rl) == 0)
    {
        rl.rlim_cur = std::min<rlim_t>(rl.rlim_max, 65536u);
        setrlimit(RLIMIT_NOFILE, &rl);
    }
}

std::optional<std::string> decryptShard(
    const std::filesystem::path& decryptorPath,
    const std::filesystem::path& shardPath,
    int devNullFd,
    std::stop_token stopToken)
{
    std::string decryptorPathString{decryptorPath.string()};
    std::string decryptorOptionString{"decrypt"};
    std::string shardPathString{shardPath.string()};

    char* args[]{
        decryptorPathString.data(),
        decryptorOptionString.data(),
        shardPathString.data(),
        nullptr
    };

    using namespace std::chrono_literals;
    constexpr auto sleepTime{5ms};

    while (!stopToken.stop_requested())
    {
        int pipeFds[2];
        if (pipe2(pipeFds, O_CLOEXEC) == -1)
        {
            std::this_thread::sleep_for(sleepTime);
            continue;
        }

        posix_spawn_file_actions_t actions;
        posix_spawn_file_actions_init(&actions);
        posix_spawn_file_actions_adddup2(&actions, pipeFds[1], STDOUT_FILENO);
        if (devNullFd != -1) posix_spawn_file_actions_adddup2(&actions, devNullFd, STDERR_FILENO);

        pid_t pid{};
        int spawnError{
            posix_spawn(
                &pid,
                decryptorPathString.c_str(),
                &actions,
                nullptr,
                args,
                environ
            )
        };
        posix_spawn_file_actions_destroy(&actions);

        //Parent
        close(pipeFds[1]);

        if (spawnError)
        {
            close(pipeFds[0]);
            std::this_thread::sleep_for(sleepTime);
            continue;
        }

        std::string output;
        char buffer[Constants::BufferSize];

        {
            std::stop_callback cancelCallback{
                stopToken, [pid]() noexcept
                {
                    kill(pid, SIGKILL);
                }
            };

            for (ssize_t bytesRead{read(pipeFds[0], buffer, sizeof(buffer))};
                 bytesRead > 0;
                 bytesRead = read(pipeFds[0], buffer, sizeof(buffer)))
            {
                output.append(buffer, static_cast<std::size_t>(bytesRead));
            }
        }

        close(pipeFds[0]);

        int status{0};
        while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
        {
        }

        if (!stopToken.stop_requested() && WIFEXITED(status) && !WEXITSTATUS(status) && !output.empty())
        {
            while (!output.empty() && (output.back() == '\n' || output.back() == '\r'))
            {
                output.pop_back();
            }
            return output;
        }
    }
    return std::nullopt;
}

int main()
{
    bumpFdLimit();

    std::system("tar -xf '/home/easyg/Downloads/AyuGram Desktop/shards.tar.gz'");

    std::vector<Shard> shards;
    for (auto&& entry : std::filesystem::directory_iterator(Constants::shardsPath))
    {
        std::uint32_t id{};
        auto stringId{entry.path().stem().string()};
        if (auto [pointer, errorCode]
        {
            std::from_chars(stringId.data(), stringId.data() + stringId.size(), id)
        }; errorCode == std::errc{})
        {
            shards.emplace_back(id, entry.path());
        }
    }

    if (shards.empty())
    {
        std::println(stderr, "No shards found");
        return EXIT_FAILURE;
    }

    std::ranges::sort(shards, {}, &Shard::id);

    const int devNullFd{open("/dev/null", O_WRONLY | O_CLOEXEC)};

    std::vector<std::string> results{shards.size()};
    std::vector<std::stop_source> stopSources{shards.size()};
    std::atomic remainingShards{shards.size()};

    {
        const std::size_t totalWorkers{shards.size() * Constants::ThreadsPerShard};

        std::vector<std::jthread> workers;
        workers.reserve(totalWorkers);

        for (std::size_t i = 0; i < totalWorkers; ++i)
        {
            workers.emplace_back([&remainingShards, &shards, &stopSources, &results, devNullFd, i ]
                {
                    for (std::size_t index{i % shards.size()};
                         remainingShards.load(std::memory_order::acquire) > 0;
                         index = (index + 1) % shards.size())
                    {
                        if (stopSources[index].stop_requested()) continue;

                        if (auto result{
                            decryptShard(
                                Constants::decryptorPath,
                                shards[index].path,
                                devNullFd,
                                stopSources[index].get_token())
                        }; result && stopSources[index].request_stop())
                        {
                            results[index] = std::move(*result);
                            if (remainingShards.fetch_sub(1, std::memory_order::acq_rel) == 1) break;
                        }
                    }
                }
            );
        }
    }

    if (devNullFd != -1) close(devNullFd);

    std::println("Flag:\t{}", results | std::views::join | std::ranges::to<std::string>());

    return EXIT_SUCCESS;
}
