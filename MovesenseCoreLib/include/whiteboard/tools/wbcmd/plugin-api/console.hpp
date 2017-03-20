#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#undef DELETE
#endif

class Console
{
protected:

#if WIN32
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO sbi;
#endif

public:
    Console()
    {
#if WIN32
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(hConsole, &sbi);
#endif
    }

    ~Console()
    {
#if WIN32
        // Restore attributes
        SetConsoleTextAttribute(hConsole, sbi.wAttributes);
#endif
    }

    enum class Color
    {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        RED = 4,
        INTENSITY = 8,
        CYAN = GREEN | BLUE,
        YELLOW = RED | GREEN,
        GRAY = RED | BLUE | GREEN,
        WHITE = GRAY | INTENSITY,
        BRIGHT_RED = RED | INTENSITY,
        BRIGHT_YELLOW = YELLOW | INTENSITY,
        BRIGHT_CYAN = GREEN | BLUE | INTENSITY
    };

    size_t getWindowWidth() const
    {
#if WIN32
        return static_cast<size_t>(sbi.srWindow.Right - sbi.srWindow.Left) + 1;
#else
        return 80;
#endif
    }

    size_t getWindowHeight() const
    {
#if WIN32
        return static_cast<size_t>(sbi.srWindow.Bottom - sbi.srWindow.Top) + 1;
#else
        return 25;
#endif
    }

    size_t getBufferWidth() const
    {
#if WIN32
        return static_cast<size_t>(sbi.dwSize.X);
#else
        return 80;
#endif
    }

    size_t getBufferHeight() const
    {
#if WIN32
        return static_cast<size_t>(sbi.dwSize.Y);
#else
        return 25;
#endif
    }

    void setCursorPosition(size_t x, size_t y)
    {
#if WIN32
        CONSOLE_SCREEN_BUFFER_INFO tempSbi;
        GetConsoleScreenBufferInfo(hConsole, &tempSbi);
        COORD cursorPos{ static_cast<SHORT>(x) + tempSbi.srWindow.Left, static_cast<SHORT>(y) + tempSbi.srWindow.Top };
        SetConsoleCursorPosition(hConsole, cursorPos);
#endif
    }

    void setColor(const Color c, const Color bc = Color::BLACK)
    {
#if WIN32
        SetConsoleTextAttribute(hConsole, static_cast<WORD>(c) | (static_cast<WORD>(bc) << 4));
#endif
    }

    void setSize(size_t windowWidth, size_t windowHeight, size_t bufferWidth = 0, size_t bufferHeight = 0)
    {
        // Sanity check input parameters
        if (bufferWidth == 0) bufferWidth = getBufferWidth();
        if (bufferHeight == 0) bufferHeight = getBufferHeight();
        if (bufferWidth < windowWidth) bufferWidth = windowWidth;
        if (bufferHeight < windowHeight) bufferHeight = windowHeight;

#if WIN32
        size_t currentWindowWidth = getWindowWidth();
        size_t currentWindowHeight = getWindowHeight();
        size_t currentBufferWidth = getBufferWidth();
        size_t currentBufferHeight = getBufferHeight();

        // Adjust widths
        if (bufferWidth < currentWindowWidth)
        {
            // Must make window smaller first
            currentWindowWidth = bufferWidth;
            SMALL_RECT rect = { 0, 0, static_cast<SHORT>(currentWindowWidth) - 1, static_cast<SHORT>(currentWindowHeight) - 1 };
            SetConsoleWindowInfo(hConsole, TRUE, &rect);
        }

        if (bufferWidth != currentBufferWidth)
        {
            // Set buffer width
            currentBufferWidth = bufferWidth;
            COORD newSize = { static_cast<SHORT>(bufferWidth), static_cast<SHORT>(currentBufferHeight) };
            SetConsoleScreenBufferSize(hConsole, newSize);
        }

        if (windowWidth != currentWindowWidth)
        {
            // Set window width
            currentWindowWidth = windowWidth;
            SMALL_RECT rect = { 0, 0, static_cast<SHORT>(currentWindowWidth) - 1, static_cast<SHORT>(currentWindowHeight) - 1 };
            SetConsoleWindowInfo(hConsole, TRUE, &rect);
        }

        // Adjust heights
        if (bufferHeight < currentWindowHeight)
        {
            // Must make window smaller first
            currentWindowHeight = bufferHeight;
            SMALL_RECT rect = { 0, 0, static_cast<SHORT>(currentWindowWidth) - 1, static_cast<SHORT>(currentWindowHeight) - 1 };
            SetConsoleWindowInfo(hConsole, TRUE, &rect);
        }

        if (bufferHeight != currentBufferHeight)
        {
            // Set buffer height
            currentBufferHeight = bufferHeight;
            COORD newSize = { static_cast<SHORT>(bufferWidth), static_cast<SHORT>(currentBufferHeight) };
            SetConsoleScreenBufferSize(hConsole, newSize);
        }

        if (windowHeight != currentWindowHeight)
        {
            // Set window height
            currentWindowHeight = windowHeight;
            SMALL_RECT rect = { 0, 0, static_cast<SHORT>(currentWindowWidth) - 1, static_cast<SHORT>(currentWindowHeight) - 1 };
            SetConsoleWindowInfo(hConsole, TRUE, &rect);
        }

        GetConsoleScreenBufferInfo(hConsole, &sbi);
#endif
    }
};


class ConsoleColorScope : private Console
{
public:

    ConsoleColorScope(const Color c, const Color bc = Color::BLACK)
    {
        setColor(c, bc);
    }

    ~ConsoleColorScope()
    {
    }
};
