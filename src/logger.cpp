#include "logger.h"

#undef LOG_RESET_COLOR
#undef LOG_COLOR_E
#undef LOG_COLOR_W
#undef LOG_COLOR_I
#undef LOG_COLOR_D
#undef LOG_COLOR_V

#define LOG_COLOR_BLACK  "30"
#define LOG_COLOR_RED    "31"
#define LOG_COLOR_GREEN  "32"
#define LOG_COLOR_BROWN  "33"
#define LOG_COLOR_BLUE   "34"
#define LOG_COLOR_PURPLE "35"
#define LOG_COLOR_CYAN   "36"
#define LOG_COLOR(COLOR) "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)  "\033[1;" COLOR "m"
#define LOG_RESET_COLOR  "\033[0m"
#define LOG_COLOR_E      LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W      LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I      LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D      LOG_COLOR(LOG_COLOR_BLUE)
#define LOG_COLOR_V      LOG_COLOR(LOG_COLOR_CYAN)

Logger::Logger() :
#ifdef NO_GLOBAL_INSTANCES
m_serial(0),
#else
m_serial(&Serial),
#endif
m_level(DEBUG_LEVEL_DEBUG),
m_printIsNewline(true),
m_noColor(false)
{
}

// cppcheck-suppress unusedFunction
void Logger::setSerial(Stream *serial)
{
    m_serial = serial;
}

// cppcheck-suppress unusedFunction
void Logger::setDebugLevel(debug_level_t level)
{
    m_level = level;
}

void Logger::enableColor(bool enable)
{
    m_noColor = !enable;
}

// cppcheck-suppress unusedFunction
void Logger::printA(const String &text, const char *file, uint32_t line)
{
    printStartColor(DEBUG_LEVEL_NONE);
    printHeader(DEBUG_LEVEL_NONE, file, line, false);
    m_serial->print(text);
    printEndColor(DEBUG_LEVEL_NONE);
}

// cppcheck-suppress unusedFunction
void Logger::printE(const String &text, const char *file, uint32_t line)
{
    printStartColor(DEBUG_LEVEL_ERROR);
    printHeader(DEBUG_LEVEL_ERROR, file, line, false);
    m_serial->print(text);
    printEndColor(DEBUG_LEVEL_ERROR);
}

// cppcheck-suppress unusedFunction
void Logger::printlnA(const String &text, const char *file, uint32_t line)
{
    printStartColor(DEBUG_LEVEL_NONE);
    printHeader(DEBUG_LEVEL_NONE, file, line, true);
    m_serial->println(text);
    printEndColor(DEBUG_LEVEL_NONE);
}

// cppcheck-suppress unusedFunction
void Logger::printlnE(const String &text, const char *file, uint32_t line)
{
    printStartColor(DEBUG_LEVEL_ERROR);
    printHeader(DEBUG_LEVEL_ERROR, file, line, true);
    m_serial->println(text);
    printEndColor(DEBUG_LEVEL_ERROR);
}

// cppcheck-suppress unusedFunction
void Logger::printV(const String &text, const char *file, uint32_t line)
{
    if (m_level >= DEBUG_LEVEL_VERBOSE)
    {
        printStartColor(DEBUG_LEVEL_VERBOSE);
        printHeader(DEBUG_LEVEL_VERBOSE, file, line, false);
        m_serial->print(text);
        printEndColor(DEBUG_LEVEL_VERBOSE);
    }
}

// cppcheck-suppress unusedFunction
void Logger::printD(const String &text, const char *file, uint32_t line)
{
    if (m_level >= DEBUG_LEVEL_DEBUG)
    {
        printStartColor(DEBUG_LEVEL_DEBUG);
        printHeader(DEBUG_LEVEL_DEBUG, file, line, false);
        m_serial->print(text);
        printEndColor(DEBUG_LEVEL_DEBUG);
    }
}

// cppcheck-suppress unusedFunction
void Logger::printI(const String &text, const char *file, uint32_t line)
{
    if (m_level >= DEBUG_LEVEL_INFO) {
        printStartColor(DEBUG_LEVEL_INFO);
        printHeader(DEBUG_LEVEL_INFO, file, line, false);
        m_serial->print(text);
        printEndColor(DEBUG_LEVEL_INFO);
    }
}

// cppcheck-suppress unusedFunction
void Logger::printW(const String &text, const char *file, uint32_t line)
{
    if (m_level >= DEBUG_LEVEL_WARN) {
        printStartColor(DEBUG_LEVEL_WARN);
        printHeader(DEBUG_LEVEL_WARN, file, line, false);
        m_serial->print(text);
        printEndColor(DEBUG_LEVEL_WARN);
    }
}

// cppcheck-suppress unusedFunction
void Logger::printlnV(const String &text, const char *file, uint32_t line)
{
    if (m_level >= DEBUG_LEVEL_VERBOSE)
    {
        printStartColor(DEBUG_LEVEL_VERBOSE);
        printHeader(DEBUG_LEVEL_VERBOSE, file, line, true);
        m_serial->println(text);
        printEndColor(DEBUG_LEVEL_VERBOSE);
    }
}

// cppcheck-suppress unusedFunction
void Logger::printlnD(const String &text, const char *file, uint32_t line)
{
    if (m_level >= DEBUG_LEVEL_DEBUG) {
        printStartColor(DEBUG_LEVEL_DEBUG);
        printHeader(DEBUG_LEVEL_DEBUG, file, line, true);
        m_serial->println(text);
        printEndColor(DEBUG_LEVEL_DEBUG);
    }
}

// cppcheck-suppress unusedFunction
void Logger::printlnI(const String &text, const char *file, uint32_t line)
{
    if (m_level >= DEBUG_LEVEL_INFO)
    {
        printStartColor(DEBUG_LEVEL_INFO);
        printHeader(DEBUG_LEVEL_INFO, file, line, true);
        m_serial->println(text);
        printEndColor(DEBUG_LEVEL_INFO);
    }
}

// cppcheck-suppress unusedFunction
void Logger::printlnW(const String &text, const char *file, uint32_t line)
{
    if (m_level >= DEBUG_LEVEL_WARN)
    {
        printStartColor(DEBUG_LEVEL_WARN);
        printHeader(DEBUG_LEVEL_WARN, file, line, true);
        m_serial->println(text);
        printEndColor(DEBUG_LEVEL_WARN);
    }
}

void Logger::printStartColor(debug_level_t level)
{
    if (m_noColor)
    {
        return;
    }

    switch (level)
    {
        case DEBUG_LEVEL_ERROR:
            m_serial->print(LOG_COLOR_E);
            break;
        case DEBUG_LEVEL_WARN:
            m_serial->print(LOG_COLOR_W);
            break;
        case DEBUG_LEVEL_INFO:
            m_serial->print(LOG_COLOR_I);
            break;
        case DEBUG_LEVEL_DEBUG:
            m_serial->print(LOG_COLOR_D);
            break;
        case DEBUG_LEVEL_VERBOSE:
            m_serial->print(LOG_COLOR_V);
            break;
        default:
            break;
    }
}

void Logger::printHeader(debug_level_t level, const char *file, uint32_t line, bool isln)
{
    if (m_printIsNewline)
    {
        m_serial->printf("%c %25s %4d : ", levelToChar(level), file, line);
        if (!isln)
        {
            m_printIsNewline = false;
        }
    }
    else
    {
        m_printIsNewline = isln;
    }
}

void Logger::printEndColor(debug_level_t level) {
    if (m_noColor)
    {
        return;
    }

    switch (level)
    {
        case DEBUG_LEVEL_ERROR:
        case DEBUG_LEVEL_WARN:
        case DEBUG_LEVEL_INFO:
        case DEBUG_LEVEL_DEBUG:
        case DEBUG_LEVEL_VERBOSE:
            m_serial->print(LOG_RESET_COLOR);
            break;
        default:
            break;
    }
}

char Logger::levelToChar(debug_level_t level)
{
    switch (level)
    {
        case DEBUG_LEVEL_ERROR:
            return 'E';
        case DEBUG_LEVEL_WARN:
            return 'W';
        case DEBUG_LEVEL_INFO:
            return 'I';
        case DEBUG_LEVEL_DEBUG:
            return 'D';
        case DEBUG_LEVEL_VERBOSE:
            return 'V';
        default:
            return ' ';
    }
}
