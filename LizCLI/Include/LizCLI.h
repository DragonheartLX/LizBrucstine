#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace liz
{
    class LizCLI
    {
    public:
        LizCLI();
        ~LizCLI();

        bool isRunning();

        std::function<void(std::string)> onCommand;

    private:
        std::atomic<bool> m_IsRunning;

        std::thread m_InputThread;
        std::thread m_WriteThread;

        std::condition_variable m_WriteCond;

        std::mutex m_QueueMutex;
        std::mutex m_CmdMutex;

        std::queue<std::string> m_WriteQueue;
        std::string m_CmdBuffer;

        void m_InputFunc();
        void m_WriteFunc();

        void m_WriteCmd();
    };
} // namespace liz