#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct Task {
    string description;
    bool completed;
    int priority;
};

vector<Task> tasks;

// Function to load tasks from a file
extern "C" void loadTasks() {
    ifstream file("tasks.txt");
    if (!file) {
        return;
    }
    tasks.clear();
    bool completed;
    int priority;
    string description;
    
    while (file >> completed >> priority) {
        file.ignore();
        getline(file, description);
        Task newTask = {description, completed, priority};
        tasks.push_back(newTask);
    }
    file.close();
}

// Function to save tasks to a file
extern "C" void saveTasks() {
    ofstream file("tasks.txt");
    for (const auto& task : tasks) {
        file << task.completed << " " << task.priority << " " << task.description << endl;
    }
    file.close();
}

// Function to add a task (takes C-style string)
extern "C" void addTask(const char* task, int priority) {
    tasks.push_back({string(task), false, priority});
    saveTasks();
}

// Function to delete a task
extern "C" void deleteTask(int index) {
    if (index < 1 || index > tasks.size()) {
        return;
    }
    tasks.erase(tasks.begin() + (index - 1));
    saveTasks();
}

// Function to edit a task
extern "C" void editTask(int index, const char* newTask) {
    if (index < 1 || index > tasks.size()) {
        return;
    }
    tasks[index - 1].description = string(newTask);
    saveTasks();
}

// Function to mark a task as complete
extern "C" void completeTask(int index) {
    if (index < 1 || index > tasks.size()) {
        return;
    }
    tasks[index - 1].completed = true;
    saveTasks();
}

// Function to get task list as a string
extern "C" const char* showTasks() {
    static string output;
    output.clear();

    for (size_t i = 0; i < tasks.size(); i++) {
        output += to_string(i + 1) + ". [" + (tasks[i].completed ? "✓" : " ") + "] (Priority " 
            + to_string(tasks[i].priority) + ") " + tasks[i].description + "\n";
    }
    
    return output.c_str();
}
