 # 🎮 Adaptive Quiz Platform

An AI-driven, gamified quiz system that adapts to user performance in real time. Built using Unreal Engine 5.4 for the frontend and Flask (Python) for the backend, this project integrates open quiz data from the OpenTDB API with a responsive, educational UI.

---

## 📦 Features

- Real-time adaptive difficulty using a simple, interpretable algorithm
- Engaging 2D UI developed in Unreal Engine 5.4
- Dynamic quiz content via OpenTDB API
- Lightweight Python backend using Flask
- Gamified scoring and visual feedback

---

## 🖥️ System Requirements

- Python 3.8 or newer
- Unreal Engine 5.4
- VaRest Plugin for UE5 (enabled from Marketplace or Plugins menu)
- OpenTDB API

---

## 🔧 Installation

### 1. Download R_QuizGame from the school blackboard


### 2.Set Up Backend 
cd backend
pip install -r requirements.txt
python app.py

Server starts at: http://127.0.0.1:5000


## 3. Set Up the Frontend (Unreal Engine)
Open the frontend/UnrealProject.uproject file using Unreal Engine 5.4.

Enable the VaRest plugin via Edit > Plugins.

Locate the Quiz UI Blueprint and check the VaRest GET/POST request nodes.

Update the endpoint URLs to match your local Flask address if needed.

Press Play to begin the game.

###Legal and Ethical Considerations
No user data is stored; all data is session-based.

Fully GDPR-compliant.

Uses OpenTDB for open-licensed trivia content.

Designed to support transparent, ethical adaptive learning.

🚀## Future Improvements
Persistent user profiles and progress tracking

Richer gamification (achievements, avatars)

Enhanced accessibility features

Teacher/admin dashboard for custom question input

🙌## Credits
OpenTDB – Trivia content

Unreal Engine – Game frontend

Flask – Backend framework

VaRest Plugin – Unreal REST API integration

