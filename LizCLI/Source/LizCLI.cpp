#include "LizCLI.h"

#include <Macros.h>

#include <iostream>
#include <thread>

#ifdef LIZ_OS_WIN
    #include <Windows.h>
    #include <conio.h>
#endif

#ifdef LIZ_OS_LINUX
    #include <fcntl.h>
    #include <termios.h>

static struct termios s_old_termios, s_current_termios;
#endif

void initTerminal()
{
#ifdef LIZ_OS_WIN
    // Windows
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT));

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

#ifdef LIZ_OS_LINUX
    tcgetattr(0, &s_old_termios);
    s_current_termios = s_old_termios;
    s_current_termios.c_lflag &= ~ICANON;
    s_current_termios.c_lflag &= ~ECHO;
    s_current_termios.c_cc[VMIN]  = 0;
    s_current_termios.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &s_current_termios);

    setbuf(stdout, NULL);

    std::setlocale(LC_ALL, "en_US.UTF-8");
#endif
}

void resetTerminal()
{
#ifdef LIZ_OS_LINUX
    tcsetattr(STDIN_FILENO, TCSANOW, &s_old_termios);
#endif
}

char getchar_no_echo()
{
    char inputChar = -1;
#ifdef LIZ_OS_WIN
    if (_kbhit())
    {
        inputChar = _getch();
    }
#endif

#ifdef LIZ_OS_LINUX
    struct termios newt, oldt;

    int tty = open("/dev/tty", O_RDONLY);
    tcgetattr(tty, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(tty, TCSANOW, &newt);
    read(tty, &inputChar, 1);
    tcsetattr(tty, TCSANOW, &oldt);
#endif

    return inputChar;
}

namespace liz
{
    LizCLI::LizCLI()
    {
        m_IsRunning = true;

        initTerminal();
        m_WriteThread = std::thread(&LizCLI::m_WriteFunc, this);
        m_InputThread = std::thread(&LizCLI::m_InputFunc, this);

        std::lock_guard<std::mutex> lock(m_QueueMutex);
    }

    LizCLI::~LizCLI()
    {
        m_IsRunning = false;
        m_WriteCond.notify_one();

        m_InputThread.join();
        m_WriteThread.join();

        resetTerminal();
    }

    bool LizCLI::isRunning() { return m_IsRunning; }

    void LizCLI::m_InputFunc()
    {
        while (isRunning())
        {
            char inputChar = getchar_no_echo();

            std::lock_guard<std::mutex> lock(m_CmdMutex);

            switch (inputChar)
            {
                case -1: break;
                case '\r':
                case '\n':
                    m_CmdBuffer.clear();
                    m_WriteCmd();
                    break;
                case 127: // DEL
                case '\b':
                    if (m_CmdBuffer.size() > 0) m_CmdBuffer.pop_back();
                    m_WriteCmd();
                    break;
                case -32: // arrow
                    inputChar = getchar_no_echo();
                    // 72 up
                    // 80 down
                    // 75 left
                    // 77 right
                    break;
                default:
                    // receive num character symbol
                    if (inputChar < 32 || inputChar > 126) break;
                    m_CmdBuffer.push_back(inputChar);
                    m_WriteCmd();
                    break;
            }
        }
    }

    void LizCLI::m_WriteFunc()
    {
        m_WriteCmd();

        while (isRunning())
        {
            std::lock_guard<std::mutex> lock(m_QueueMutex);
            if (!m_WriteQueue.empty())
            {
                std::cout << m_WriteQueue.front() << std::flush;
                m_WriteQueue.pop();
            }
        }
    }

    void LizCLI::m_WriteCmd()
    {
        std::string logFormat;

        logFormat.clear();
        logFormat += "x\x1b[2K\x1b[0G";
        logFormat = logFormat + "\x1b[0m" + "LizCLI> ";

        logFormat += m_CmdBuffer;
        std::lock_guard<std::mutex> lock(m_QueueMutex);
        m_WriteQueue.push(logFormat);
    }
} // namespace liz