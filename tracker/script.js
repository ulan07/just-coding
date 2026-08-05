// Получаем элементы
const taskInput = document.getElementById('taskInput');
const addTaskBtn = document.getElementById('addTaskBtn');
const tasksList = document.getElementById('tasksList');
const currentDate = document.getElementById('currentDate');
const completedCount = document.getElementById('completedCount');
const totalCount = document.getElementById('totalCount');
const clearBtn = document.getElementById('clearBtn');
const filterAll = document.getElementById('filterAll');
const filterActive = document.getElementById('filterActive');
const filterCompleted = document.getElementById('filterCompleted');

// Массив для хранения задач
let tasks=[];

let currentFilter = 'All';

const saved=localStorage.getItem('tasks');
if(saved){
    tasks=JSON.parse(saved);
    renderTasks();
}

function saveTasks() {
    localStorage.setItem('tasks', JSON.stringify(tasks));
}

// Показываем текущую дату
function updateDate() {
    const today = new Date();
    const options = { weekday: 'long', year: 'numeric', month: 'numeric', day: 'numeric' };
    currentDate.textContent = today.toLocaleDateString("ru-RU", options);
}

// добавляем задачу
function addTask() {
    const taskText = taskInput.value.trim();

    if (taskText=='') {
        alert('Пожалуйста, введите задачу.');
        return;
    }

    const task ={
        id: Date.now(),
        text: taskText,
        completed: false
    };

    tasks.push(task);
    saveTasks();
    taskInput.value = '';
    renderTasks();
}

function renderTasks() {
    tasksList.innerHTML = '';

    let filteredTasks= [];

    if(currentFilter=='All'){
        filteredTasks=tasks;
    }
    else if(currentFilter=='Active'){
        filteredTasks=tasks.filter(t=>t.completed==false);
    }
    else{
        filteredTasks=tasks.filter(t=>t.completed==true);
    }

    filteredTasks.forEach(task => {
        const li = document.createElement('li');
        li.className='task-item';
        if(task.completed){
            li.classList.add('completed');
        }

        li.innerHTML = `
        <input type="checkbox" class="task-checkbox" ${task.completed ? 'checked' : ''}>
            <span class="task-text">${task.text}</span>
            <button class="delete-btn">Удалить</button>
        `;

        const checkbox = li.querySelector('.task-checkbox');
        checkbox.addEventListener('change',() => toggleTask(task.id));

        const deleteBtn = li.querySelector('.delete-btn');
        deleteBtn.addEventListener('click',() => deleteTask(task.id));

        tasksList.appendChild(li);})

    updateStats();
    
}

function toggleTask(id){
    const task = tasks.find(t =>t.id===id);
    if(task){
        task.completed = !task.completed;
        saveTasks();
        renderTasks();
    }

}

function deleteTask(id){
    tasks=tasks.filter(t=>t.id!==id);
    saveTasks();
    renderTasks();
}

function clear(){
    tasks=[];
    saveTasks();
    renderTasks();
}

function updateStats() {
    const completed = tasks.filter(t => t.completed).length;
    const total = tasks.length;

    completedCount.textContent = completed;
    totalCount.textContent = total;
}

function fAll(){
    currentFilter = 'All';
    renderTasks();
}

function fActive(){
    currentFilter = 'Active';
    renderTasks();
}

function fCompleted(){
    currentFilter = 'Completed';
    renderTasks();
}

// События
filterAll.addEventListener('click',fAll);
filterCompleted.addEventListener('click',fCompleted);
filterActive.addEventListener('click',fActive);
clearBtn.addEventListener('click', clear);
addTaskBtn.addEventListener('click', addTask);
taskInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') {
        addTask();
    }
});

// Инициализация
updateDate();
