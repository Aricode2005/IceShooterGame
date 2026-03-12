# Ice Shooter 🧊🚀

A fast-paced, retro arcade shooter built in **C++** using the Windows Console API. This project demonstrates 2D vector mathematics, frame-independent physics, and procedural object fragmentation.

## 📝 The Concept
You pilot a high-speed hovercraft across a vast, frozen lake. The core challenge lies in the **drifting physics**—since you are on ice, your craft has significant momentum. You must use thermal blasts to shatter giant ice boulders and collect mineral cores while counter-steering to avoid a collision.



## 🎮 How to Play
1. **Start:** Press `S` on the menu to deploy.
2. **Move:** Use the **Arrow Keys** to rotate and apply thrust.
3. **Shoot:** Press **Spacebar** to fire thermal blasts. 
4. **Shatter:** Large ice boulders split into smaller, faster fragments when hit.
5. **Survive:** One collision destroys your craft. Press **Enter** to restart.

### Controls Reference
| Key | Action |
| :--- | :--- |
| **S** | Start Game |
| **Up Arrow** | Thrust (Accelerate) |
| **Left/Right** | Rotate Craft |
| **Space** | Fire Thermal Blast |
| **P** | Pause / Resume |
| **Enter** | Restart after death |

## 🚀 Installation & Quick Start
Follow these steps to compile and play the game on Windows.

### 1. Prerequisites
You need a C++ compiler (like **MinGW / g++**) installed on your system. You can verify this by running `g++ --version` in your terminal.

### 2. Setup
Open your Command Prompt (CMD) or PowerShell and run:

```bash
# Clone the repository
git clone [https://github.com/Aricode2005/Ice-Shooter.git](https://github.com/Aricode2005/Ice-Shooter.git)

# Enter the project folder
cd Ice-Shooter

# Compile the source code
g++ Asteroids.cpp -o IceShooter.exe -luser32 -lgdi32 -lwinmm

# Run the game
./IceShooter.exe
