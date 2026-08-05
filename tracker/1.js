// Получаем элементы
const taskInput = document.getElementById("taskInput");
const addTaskBtn = document.getElementById("addTaskBtn");
const tasksList = document.getElementById("tasksList");
const currentDate = document.getElementById("currentDate");
const completedCount = document.getElementById("completedCount");
const totalCount = document.getElementById("totalCount");

let tasks = [];

function updateDate() {
  const today = new Date();
  const options = {
    weekday: "short",
    year: "numeric",
    month: "long",
    day: "numeric",
  };
  currentDate.textContent = today.toLocaleDateString("ru-RU", options);
}

function addTask() {
  const taskText = taskInput.value.trim();

  if (taskText == "") {
    alert("Введите задачу");
    return;
  }

  const task = {
    id: Date.now(),
    text: taskText,
    completed: false,
  };

  tasks.push(task);
  taskInput.value = "";
  renderTasks();
}

function renderTasks() {
  tasksList.innerHTML = "";
  tasks.forEach((task) => {
    const li = document.createElement("li");
    li.className = "task-item";
    if (task.completed) {
      li.classList.add("completed");
    }

    li.innerHTML = `
        <input type="checkbox" class="task-checkbox" ${task.completed ? "checked" : ""}>
            <span class="task-text">${task.text}</span>
            <button class="delete-btn">Удалить</button>
        `;

    const checkbox = li.querySelector(".task-checkbox");
    checkbox.addEventListener("change", () => toggleTask(task.id));

    const deleteBtn = li.querySelector(".delete-btn");
    deleteBtn.addEventListener("click", () => deleteTask(task.id));

    tasksList.appendChild(li);
  });
  updateStats();
}

function toggleTask(id) {
  const task = tasks.find((t) => t.id === id);
  if (task) {
    task.completed = !task.completed;
    renderTasks();
  }
}

function deleteTask(id) {
  tasks = tasks.filter((t) => t.id !== id);
  renderTasks();
}

function updateStats() {
  const completed = tasks.filter((t) => t.completed).length;
  const total = tasks.length;
  completedCount.textContent = completed;
  totalCount.textContent = total;
}

addTaskBtn.addEventListener("click", addTask);
taskInput.addEventListener("keypress", (e) => {
  if (e.key == "Enter") {
    addTask();
  }
});

updateDate();
