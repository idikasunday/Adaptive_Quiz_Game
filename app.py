import random
import requests
from flask import Flask, jsonify, request
from flask_cors import CORS
from urllib.parse import unquote

app = Flask(__name__)
CORS(app)  # Enable CORS for all routes

# OpenTDB Configuration
OPENTDB_API = "https://opentdb.com/api.php"
CATEGORY_MAP = {
    "9": "General Knowledge",
    "18": "Science: Computers",
    "30": "Science: Gadgets",
    "21": "Sports",
    "22": "Geography",
    "23": "History"
}

# Difficulty levels and thresholds
DIFFICULTY_LEVELS = ['easy', 'medium', 'hard']
SCORE_THRESHOLD_LOW = 30   # Below this score, move down a level
SCORE_THRESHOLD_HIGH = 80  # Above this score, move up a level

# In-memory user session management
user_sessions = {}

def get_user_session(user_id):
    """Retrieve or create a user session."""
    if user_id not in user_sessions:
        user_sessions[user_id] = {
            'current_difficulty': 'easy',
            'score_history': []
        }
    return user_sessions[user_id]

def update_user_difficulty(user_id, score):
    """Adjust user's difficulty based on performance."""
    session = get_user_session(user_id)
    session['score_history'].append(score)
    
    recent_scores = session['score_history'][-3:]  # Only last 3 attempts
    avg_score = sum(recent_scores) / len(recent_scores) if recent_scores else score
    
    current_index = DIFFICULTY_LEVELS.index(session['current_difficulty'])

    if avg_score < SCORE_THRESHOLD_LOW and current_index > 0:
        session['current_difficulty'] = DIFFICULTY_LEVELS[current_index - 1]
    elif avg_score > SCORE_THRESHOLD_HIGH and current_index < len(DIFFICULTY_LEVELS) - 1:
        session['current_difficulty'] = DIFFICULTY_LEVELS[current_index + 1]
    
    return session['current_difficulty']

def fetch_questions(category_id, difficulty='easy', count=5):
    """Fetch questions from OpenTDB."""
    params = {
        'amount': count,
        'category': category_id,
        'difficulty': difficulty,
        'type': 'multiple'
    }
    
    try:
        response = requests.get(OPENTDB_API, params=params, timeout=5)
        response.raise_for_status()
        data = response.json()
        
        if data['response_code'] != 0:
            raise ValueError(f"OpenTDB API error: {data['response_code']}")
        
        return data['results']
    except Exception as e:
        print(f"Error fetching questions: {str(e)}")
        return None

# ---------- API Endpoints ----------

@app.route('/api/quiz/generate', methods=['GET'])
def generate_quiz_questions():
    """Legacy endpoint for Unreal Engine compatibility."""
    try:
        category_id = request.args.get('category', '9')
        user_id = request.args.get('user_id', 'default_user')
        
        session = get_user_session(user_id)
        difficulty = session['current_difficulty']

        questions = fetch_questions(category_id, difficulty, 10)
        if not questions:
            return jsonify({"error": "Failed to fetch questions"}), 500

        formatted = []
        for q in questions:
            answers = q['incorrect_answers'] + [q['correct_answer']]
            random.shuffle(answers)
            
            formatted.append({
                "question": unquote(q['question']),
                "answers": [unquote(a) for a in answers],
                "correct_answer": unquote(q['correct_answer']),
                "difficulty": difficulty
            })
        
        return jsonify(formatted)
    
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/api/quiz/questions', methods=['GET'])
def get_quiz_questions():
    """Modern, improved endpoint for fetching questions."""
    try:
        category_id = request.args.get('category_id', '9')
        user_id = request.args.get('user_id', 'default_user')
        count = int(request.args.get('count', 5))
        
        count = max(1, min(count, 50))  # Clamp count between 1 and 50
        
        session = get_user_session(user_id)
        difficulty = session['current_difficulty']

        questions = fetch_questions(category_id, difficulty, count)
        if not questions:
            return jsonify({"error": "Failed to fetch questions"}), 500

        result = []
        for q in questions:
            answers = q['incorrect_answers'] + [q['correct_answer']]
            random.shuffle(answers)

            result.append({
                "id": len(result) + 1,
                "question": unquote(q['question']),
                "answers": [unquote(a) for a in answers],
                "correct_answer": unquote(q['correct_answer']),  # <-- FIX: Include correct answer
                "category": CATEGORY_MAP.get(category_id, "Unknown"),
                "difficulty": difficulty
            })
        
        return jsonify({
            "success": True,
            "questions": result,
            "current_difficulty": difficulty
        })
    
    except Exception as e:
        return jsonify({"error": str(e)}), 400

@app.route('/api/quiz/check', methods=['POST'])
def check_answer():
    """Validate user answer and update difficulty."""
    try:
        data = request.get_json()
        user_answer = data['user_answer']
        correct_answer = data['correct_answer']
        user_id = data.get('user_id', 'default_user')
        score = int(data.get('score', 0))

        is_correct = user_answer.strip().lower() == correct_answer.strip().lower()
        
        new_difficulty = update_user_difficulty(user_id, score)
        
        return jsonify({
            "is_correct": is_correct,
            "correct_answer": correct_answer,
            "new_difficulty": new_difficulty,
            "message": f"Difficulty adjusted to {new_difficulty} based on your performance."
        })
    
    except Exception as e:
        return jsonify({"error": str(e)}), 400

@app.route('/api/quiz/categories', methods=['GET'])
def get_categories():
    """Get list of available quiz categories."""
    return jsonify({
        "categories": CATEGORY_MAP,
        "default": "9"
    })

@app.route('/api/quiz/user_info', methods=['GET'])
def get_user_info():
    """Retrieve user's current difficulty and score history."""
    try:
        user_id = request.args.get('user_id', 'default_user')
        session = get_user_session(user_id)
        
        return jsonify({
            "user_id": user_id,
            "current_difficulty": session['current_difficulty'],
            "score_history": session['score_history']
        })
    
    except Exception as e:
        return jsonify({"error": str(e)}), 400

# ---------- Run Server ----------

if __name__ == '__main__':
    print("Quiz API Server Running")
    print("Endpoints Available:")
    print("- GET /api/quiz/generate")
    print("- GET /api/quiz/questions")
    print("- POST /api/quiz/check")
    print("- GET /api/quiz/categories")
    print("- GET /api/quiz/user_info")
    app.run(host='0.0.0.0', port=5000, debug=True)
