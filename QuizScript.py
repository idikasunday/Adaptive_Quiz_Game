import unreal
import random
import requests

# Function to fetch questions
def fetch_questions():
    url = "https://opentdb.com/api.php?amount=1&type=multiple"
    response = requests.get(url)
    
    if response.status_code == 200:
        data = response.json()
        question_data = data['results'][0]

        # Get question and answers
        question = question_data['question']
        choices = question_data['incorrect_answers'] + [question_data['correct_answer']]
        random.shuffle(choices)

        # Send to Unreal
        quiz_ui = unreal.GameplayStatics.get_all_widgets_of_class(unreal.EditorLevelLibrary.get_editor_world(), unreal.QuizUI)[0]
        quiz_ui.set_question(question, choices, question_data['correct_answer'])

fetch_questions()
