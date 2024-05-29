#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <stdexcept>

using namespace std;

// Клас DFA (детермінований скінченний автомат)
class DFA {
public:
    set<char> alphabet;  // Алфавіт
    set<int> states;  // Множина станів
    int startState;  // Початковий стан
    set<int> finalStates;  // Множина фінальних станів
    map<int, map<char, int>> transitionFunction;  // Функція переходів

    // Конструктор за замовчуванням
    DFA() {
        startState = 0;
    }

    // Параметризований конструктор
    DFA(const set<char>& alphabet_, const set<int>& states_, int startState_, const set<int>& finalStates_,
        const map<int, map<char, int>>& transitionFunction_)
            : alphabet(alphabet_),
              states(states_),
              startState(startState_),
              finalStates(finalStates_),
              transitionFunction(transitionFunction_) {}

    // Конструктор копіювання
    DFA(const DFA& dfa)
            : DFA(dfa.alphabet,
                  dfa.states,
                  dfa.startState,
                  dfa.finalStates,
                  dfa.transitionFunction) {}

    // Конструктор, який зчитує автомат з файлу
    DFA(const string& pathname) {
        ifstream file(pathname);  // Відкриття файлу
        if (!file.is_open()) {
            throw runtime_error("File not found.");  // Викидання винятку, якщо файл не знайдено
        }

        char ch;
        int num;
        int alphabetSize;
        file >> alphabetSize;  // Зчитування розміру алфавіту
        for (int i = 0; i < alphabetSize; ++i) {
            file >> ch;
            alphabet.insert(ch);  // Зчитування символів алфавіту
        }

        int numberOfStates;
        file >> numberOfStates;  // Зчитування кількості станів
        for (int i = 0; i < numberOfStates; ++i) {
            states.insert(i);  // Додавання станів у множину
        }

        file >> startState;  // Зчитування початкового стану

        int numberOfFinalStates;
        file >> numberOfFinalStates;  // Зчитування кількості фінальних станів
        for (int i = 0; i < numberOfFinalStates; ++i) {
            file >> num;
            finalStates.insert(num);  // Зчитування фінальних станів
        }

        int fromState, toState;
        char viaLetter;
        while (file >> fromState >> viaLetter >> toState) {
            transitionFunction[fromState][viaLetter] = toState;  // Зчитування функції переходів
        }
    }

    // Метод для отримання прийнятних літер
    set<char> getAcceptableLetters() {
        set<char> acceptableLetters;
        for (char viaLetter : alphabet) {  // Перебір всіх символів алфавіту
            auto it = transitionFunction[startState].find(viaLetter);
            if (it != transitionFunction[startState].end() && finalStates.count(it->second)) {
                acceptableLetters.insert(viaLetter);  // Додавання символу до прийнятних, якщо перехід веде до фінального стану
            }
        }
        return acceptableLetters;
    }

    // Метод для отримання неприйнятних літер
    set<char> getNotAcceptableLetters() {
        set<char> notAcceptableLetters(alphabet);  // Спочатку всі літери вважаються неприйнятними
        set<char> acceptableLetters = getAcceptableLetters();
        for (char letter : acceptableLetters) {
            notAcceptableLetters.erase(letter);  // Видалення прийнятних літер з множини неприйнятних
        }
        return notAcceptableLetters;
    }
};

int main() {
    string file_path;
    cout << "Enter the file path: ";
    getline(std::cin, file_path);  // Шлях файлу з автоматом
    try {
        DFA dfa(file_path);  // Створення об'єкту DFA та зчитування даних з файлу
        set<char> notAcceptableLetters = dfa.getNotAcceptableLetters();  // Отримання неприйнятних літер
        cout << "Not acceptable letters: ";
        for (char letter : notAcceptableLetters) {
            cout << letter << ' ';  // Виведення неприйнятних літер
        }
        cout << endl;
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;  // Обробка винятків, якщо файл не знайдено
    }
    return 0;
}

//  C:\Users\miy_PC\CLionProjects\untitled\testing.txt