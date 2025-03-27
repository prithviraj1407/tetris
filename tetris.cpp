#include <iostream>
#include <thread>
#include <vector>
#include <ncurses.h>
#include <chrono>

using namespace std;

int nScreenWidth = 80;     // Console Screen Size X (columns)
int nScreenHeight = 30;    // Console Screen Size Y (rows)
wstring tetromino[7];
int nFieldWidth = 10;
int nFieldHeight = 20;
unsigned char *pField = nullptr;

int Rotate(int px, int py, int r)
{
    int pi = 0;
    switch (r % 4)
    {
    case 0: pi = py * 4 + px; break;
    case 1: pi = 12 + py - (px * 4); break;
    case 2: pi = 15 - (py * 4) - px; break;
    case 3: pi = 3 - py + (px * 4); break;
    }
    return pi;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    for (int px = 0; px < 4; px++)
        for (int py = 0; py < 4; py++)
        {
            int pi = Rotate(px, py, nRotation);
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

            if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
                if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                    if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
                        return false;
        }

    return true;
}

int main()
{
    // Initialize ncurses
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE); // Non-blocking input
    keypad(stdscr, TRUE);

    // Tetromino definitions
    tetromino[0].append(L"..X...X...X...X.");
    tetromino[1].append(L"..X..XX...X.....");
    tetromino[2].append(L".....XX..XX.....");
    tetromino[3].append(L"..X..XX..X......");
    tetromino[4].append(L".X...XX...X.....");
    tetromino[5].append(L".X...X...XX.....");
    tetromino[6].append(L"..X...X..XX.....");

    pField = new unsigned char[nFieldWidth * nFieldHeight];
    for (int x = 0; x < nFieldWidth; x++)
        for (int y = 0; y < nFieldHeight; y++)
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

    bool bKey[4];
    int nCurrentPiece = 0, nCurrentRotation = 0, nCurrentX = nFieldWidth / 2, nCurrentY = 0;
    int nSpeed = 20, nSpeedCount = 0, nPieceCount = 0, nScore = 0, nLevel = 1;
    bool bForceDown = false, bRotateHold = true, bGameOver = false;
    vector<int> vLines;

    while (!bGameOver)
    {
        this_thread::sleep_for(40ms);
        nSpeedCount++;
        bForceDown = (nSpeedCount == nSpeed);

        // Input
        int ch = getch();
        bKey[0] = (ch == KEY_RIGHT);
        bKey[1] = (ch == KEY_LEFT);
        bKey[2] = (ch == KEY_DOWN);
        bKey[3] = (ch == KEY_UP);

        // Movement
        nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
        nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
        nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

        if (bKey[3])
        {
            nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
            bRotateHold = false;
        }
        else
            bRotateHold = true;

        if (bForceDown)
        {
            nSpeedCount = 0;
            nPieceCount++;
            if (nPieceCount % 10 == 0 && nSpeed >= 10) nSpeed--; // Increase speed every 10 pieces

            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
                nCurrentY++;
            else
            {
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

                // Check for completed lines
                for (int py = 0; py < 4; py++)
                    if (nCurrentY + py < nFieldHeight - 1)
                    {
                        bool bLine = true;
                        for (int px = 1; px < nFieldWidth - 1; px++)
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

                        if (bLine)
                        {
                            // Mark the line for clearing
                            for (int px = 1; px < nFieldWidth - 1; px++)
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                            vLines.push_back(nCurrentY + py);
                        }
                    }

                // Calculate score based on the number of lines cleared
                if (!vLines.empty())
                {
                    // Tetris scoring system
                    switch (vLines.size())
                    {
                    case 1: nScore += 100 * nLevel; break;
                    case 2: nScore += 300 * nLevel; break;
                    case 3: nScore += 500 * nLevel; break;
                    case 4: nScore += 800 * nLevel; break;
                    }

                    // Increase level every 10 lines cleared
                    nLevel += vLines.size() / 10;

                    // Clear completed lines
                    for (auto &v : vLines)
                    {
                        // Shift lines down
                        for (int py = v; py > 0; py--)
                            for (int px = 1; px < nFieldWidth - 1; px++)
                                pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];

                        // Clear the top line
                        for (int px = 1; px < nFieldWidth - 1; px++)
                            pField[px] = 0;
                    }
                    vLines.clear();
                }

                nCurrentX = nFieldWidth / 2;
                nCurrentY = 0;
                nCurrentRotation = 0;
                nCurrentPiece = rand() % 7;

                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }
        }

        clear();
        for (int x = 0; x < nFieldWidth; x++)
            for (int y = 0; y < nFieldHeight; y++)
                mvprintw(y + 2, x + 2, "%c", " ABCDEFG=#"[pField[y * nFieldWidth + x]]);

        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                    mvprintw(nCurrentY + py + 2, nCurrentX + px + 2, "%c", nCurrentPiece + 65);

        mvprintw(1, nFieldWidth + 6, "SCORE: %d", nScore);
        mvprintw(2, nFieldWidth + 6, "LEVEL: %d", nLevel);
        refresh();
    }

    endwin();
    cout << "Game Over!! Score: " << nScore << endl;
    return 0;
}
