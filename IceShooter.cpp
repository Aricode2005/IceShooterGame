#include "ConsoleGameEngine.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <mmsystem.h> 

using namespace std;

class IceShooter : public ConsoleGameEngine {
public:
    IceShooter() { m_sAppName = L"Ice Shooter"; }

private:
    struct Entity {
        float x, y, dx, dy;
        int size;
        float angle;
    };
    
    bool isDead;
    bool isPaused;
    bool hasStarted;
    int score = 0;
    
    vector<Entity> ice;
    vector<Entity> bul;
    Entity plr;
    
    vector<pair<float, float>> modPlr;
    vector<pair<float, float>> modIce;

protected:
    virtual bool OnUserCreate() {
        modPlr = {
            { 0.0f, -4.0f }, { 2.0f, -1.0f }, { 2.0f, 3.0f }, 
            { 4.0f, 4.0f }, { -4.0f, 4.0f }, { -2.0f, 3.0f }, { -2.0f, -1.0f }
        }; 

        for (int i = 0; i < 10; i++) {
            float r = ((float)rand() / (float)RAND_MAX * 0.4f + 0.6f);
            float a = ((float)i / 10.0f) * 6.28318f;
            modIce.push_back({ r * sinf(a), r * cosf(a) });
        }
        
        ResetGame();
        return true;
    }

    void ResetGame() {
        plr.x = ScreenWidth() / 2.0f;
        plr.y = ScreenHeight() / 2.0f;
        plr.dx = 0.0f; plr.dy = 0.0f; plr.angle = 0.0f;

        bul.clear();
        ice.clear();
        ice.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, 16, 0.0f });
        ice.push_back({ 100.0f, 20.0f, -5.0f, 3.0f, 16, 0.0f });

        isDead = false;
        isPaused = false;
        hasStarted = false; 
        score = 0;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {
        static bool wasInFocus = true;
        bool hasFocus = m_bConsoleInFocus; 
        
        if (wasInFocus && !hasFocus) isPaused = true;
        wasInFocus = hasFocus; 
        
        if (!hasStarted && m_keys['S'].bReleased) hasStarted = true;
        if (hasStarted && !isDead && m_keys['P'].bReleased) isPaused = !isPaused;
        if (isDead && m_keys[VK_RETURN].bReleased) ResetGame();

        if (hasStarted && !isPaused && !isDead) {
            
            if (m_keys[VK_LEFT].bHeld)  plr.angle -= 5.0f * fElapsedTime;
            if (m_keys[VK_RIGHT].bHeld) plr.angle += 5.0f * fElapsedTime;

            if (m_keys[VK_UP].bHeld) {
                plr.dx += sin(plr.angle) * 50.0f * fElapsedTime;
                plr.dy += -cos(plr.angle) * 50.0f * fElapsedTime;
            }
            
            plr.dx -= plr.dx * 1.5f * fElapsedTime;
            plr.dy -= plr.dy * 1.5f * fElapsedTime;
            
            plr.x += plr.dx * fElapsedTime;
            plr.y += plr.dy * fElapsedTime;
            WrapCoordinates(plr.x, plr.y, plr.x, plr.y);
            
            if (m_keys[VK_SPACE].bReleased) {
                bul.push_back({ plr.x, plr.y, 50.0f * sinf(plr.angle), -50.0f * cosf(plr.angle), 0, 0.0f });
                PlaySound(L"SystemDefault", NULL, SND_ALIAS | SND_ASYNC);
            }

            for (auto &i : ice) {
                if (IspointInsideCircle(i.x, i.y, i.size, plr.x, plr.y)) {
                    isDead = true; 
                    PlaySound(L"SystemHand", NULL, SND_ALIAS | SND_ASYNC);
                }
            }
                
            for (auto &i : ice) {
                i.x += i.dx * fElapsedTime;
                i.y += i.dy * fElapsedTime;
                i.angle += 0.5f * fElapsedTime;
                WrapCoordinates(i.x, i.y, i.x, i.y);
            }
            
            vector<Entity> newIce;
            for (auto &b : bul) {
                b.x += b.dx * fElapsedTime;
                b.y += b.dy * fElapsedTime;
                for (auto &i : ice) {
                    if (IspointInsideCircle(i.x, i.y, i.size, b.x, b.y)) {
                        b.x = -100; 
                        PlaySound(L"SystemExclamation", NULL, SND_ALIAS | SND_ASYNC);
                        if (i.size > 4) {
                            float a1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                            float a2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                            newIce.push_back({ i.x, i.y, 10.0f * sinf(a1), 10.0f * cosf(a1), i.size >> 1, 0.0f });
                            newIce.push_back({ i.x, i.y, 10.0f * sinf(a2), 10.0f * cosf(a2), i.size >> 1, 0.0f });
                        }
                        i.x = -100;
                        score += 100;
                    }
                }
            }
            
            for (auto i : newIce) ice.push_back(i);
            
            if (bul.size() > 0) {
                auto it = remove_if(bul.begin(), bul.end(), [&](Entity o) { return o.x < 1 || o.y < 1 || o.x >= ScreenWidth() || o.y >= ScreenHeight(); });
                if (it != bul.end()) bul.erase(it, bul.end());
            }
            if (ice.size() > 0) {
                auto it = remove_if(ice.begin(), ice.end(), [&](Entity o) { return (o.x < 0); });
                if (it != ice.end()) ice.erase(it, ice.end());
            }
            
            if (ice.empty()) {
                score += 1000;
                ice.clear(); bul.clear();
                ice.push_back({ 30.0f * sinf(plr.angle - 3.14159f/2.0f) + plr.x, 30.0f * cosf(plr.angle - 3.14159f/2.0f) + plr.y, 10.0f * sinf(plr.angle), 10.0f * cosf(plr.angle), 16, 0.0f });
                ice.push_back({ 30.0f * sinf(plr.angle + 3.14159f/2.0f) + plr.x, 30.0f * cosf(plr.angle + 3.14159f/2.0f) + plr.y, 10.0f * sinf(-plr.angle), 10.0f * cosf(-plr.angle), 16, 0.0f });
            }
        }

        if (!hasStarted) {
            Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
            DrawString(10, 10, std::wstring(L"ICE SHOOTER"), FG_CYAN);
            DrawString(10, 16, std::wstring(L"PRESS 'S' TO START"), FG_YELLOW);
        } 
        else if (isDead) {
            Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_DARK_RED);
            for (auto &b : bul) Draw(b.x, b.y, PIXEL_SOLID, FG_YELLOW); 
            for (auto &i : ice) DrawWireFrameModel(modIce, i.x, i.y, i.angle, i.size, FG_BLACK);
            DrawWireFrameModel(modPlr, plr.x, plr.y, plr.angle, 1.0f, FG_WHITE);
            DrawString(4, 10, L"YOU DIED", FG_WHITE);
            DrawString(4, 16, L"PRESS ENTER TO RESTART...", FG_YELLOW);
            DrawString(4, 25, L"SCORE: " + to_wstring(score), FG_WHITE);
        } 
        else if (isPaused) {
            Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
            for (auto &b : bul) Draw(b.x, b.y, PIXEL_SOLID, FG_DARK_RED); 
            for (auto &i : ice) DrawWireFrameModel(modIce, i.x, i.y, i.angle, i.size, FG_DARK_CYAN);
            DrawWireFrameModel(modPlr, plr.x, plr.y, plr.angle, 1.0f, FG_GREY);
            DrawString(4, 10, L"PAUSED", FG_YELLOW);
            DrawString(4, 16, L"PRESS 'P' TO RESUME", FG_WHITE);
        } 
        else {
            Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
            for (auto &b : bul) Draw(b.x, b.y, PIXEL_SOLID, FG_RED); 
            for (auto &i : ice) DrawWireFrameModel(modIce, i.x, i.y, i.angle, i.size, FG_CYAN);
            DrawWireFrameModel(modPlr, plr.x, plr.y, plr.angle, 1.0f, FG_WHITE);
            DrawString(2, 2, L"SCORE: " + to_wstring(score), FG_WHITE);
        }

        return true;
    }

    bool IspointInsideCircle(float cx, float cy, float radius, float x, float y) {
        return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius;
    }

    void WrapCoordinates(float ix, float iy, float &ox, float &oy) {
        ox = ix; oy = iy;
        if (ix < 0.0f) ox = ix + (float)ScreenWidth();
        if (ix >= (float)ScreenWidth()) ox = ix - (float)ScreenWidth();
        if (iy < 0.0f) oy = iy + (float)ScreenHeight();
        if (iy >= (float)ScreenHeight()) oy = iy - (float)ScreenHeight();
    }

    virtual void Draw(int x, int y, short c = 0x2588, short col = 0x000F) {
        float fx, fy; WrapCoordinates(x, y, fx, fy);
        ConsoleGameEngine::Draw(fx, fy, c, col);
    }

    void DrawWireFrameModel(const vector<pair<float, float>> &vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short col = FG_WHITE) {
        vector<pair<float, float>> vecTransformedCoordinates;
        int verts = vecModelCoordinates.size();
        vecTransformedCoordinates.resize(verts);
        for (int i = 0; i < verts; i++) {
            vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
            vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
        }
        for (int i = 0; i < verts; i++) {
            vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s + x;
            vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s + y;
        }
        for (int i = 0; i < verts + 1; i++) {
            int j = (i + 1);
            DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, 
                vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, L'#', col);
        }
    }
};

int main() {
    IceShooter game;
    if (game.ConstructConsole(160, 100, 8, 8)) game.Start();
    return 0;
}