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
const calendarGrid = document.getElementById('calendarGrid');
const prevMonthBtn = document.getElementById('prevMonthBtn');
const nextMonthBtn = document.getElementById('nextMonthBtn');
const calendarTitle = document.getElementById('calendarTitle');
const streakCount = document.getElementById('streakCount');
const totalDayTime = document.getElementById('totalDayTime');
const saveNotesBtn = document.getElementById('saveNotesBtn');
const dayNotesInput = document.getElementById('dayNotesInput');
const notesWidget = document.getElementById('notesWidget');
const toggleNotesBtn = document.getElementById('toggleNotesBtn');
const tabTasksBtn = document.querySelector('[data-tab="tasks"]');
const tabStatsBtn = document.querySelector('[data-tab="stats"]');
const sidebarTasksPanel = document.getElementById('sidebar-tasks-panel');
const sidebarStatsPanel = document.getElementById('sidebar-stats-panel');
const contentTasksPanel = document.getElementById('tasks-content-panel');
const contentStatsPanel = document.getElementById('stats-content-panel');
const viewFullStatsBtn = document.getElementById('viewFullStatsBtn');
const backToTasksBtn = document.getElementById('backToTasksBtn');

// Массив для хранения задач
let history={};

let displayYear = new Date().getFullYear();
let displayMonth = new Date().getMonth();

const saved=localStorage.getItem('history');
if(saved){
    history=JSON.parse(saved);
}

let currentDayKey="";

function getTodaykey(){
    const today= new Date();
    
    return today.toISOString().split('T')[0];
}

currentDayKey = getTodaykey();

let selectedDay=currentDayKey;

if(history[selectedDay]==undefined){
    history[selectedDay]={
        tasks : [],
        totalTime : 0,
        status : "red",
        dayNotes : ""
    }
}

let tasks = history[selectedDay].tasks;

let activeIntervals = {}; 

let currentFilter = 'All';


function saveTasks() {
    localStorage.setItem('history', JSON.stringify(history));
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
        completed: false,
        isEditing: false,

        seconds: 0,
        isTrunning: false,
        startTime: null
    };

    tasks.push(task);
    saveTasks();
    taskInput.value = '';
    renderTasks();
    updateDayStatus();
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

        if(!task.isEditing){
            li.innerHTML = `
                <div class="task-main">
                    <input type="checkbox" class="task-checkbox" ${task.completed ? 'checked' : ''}>
                    <span class="task-text">${task.text}</span>
                    <div class="task-actions">
                        <button class="Edit-btn">Редактировать</button>
                        <button class="delete-btn">Удалить</button>
                    </div>
                </div>
                <div class="task-timer">
                    <span class="task-seconds">${greatTime(task.seconds)}</span>
                    <button class="start-btn">${task.isTrunning ? '❚❚' : '▶'}</button>
                    <button class="stop-btn">◼</button>
                </div>
            `;
            const editBtn=li.querySelector('.Edit-btn');
            editBtn.addEventListener('click',()=>editTask(task.id));

            const startBtn=li.querySelector('.start-btn');
            startBtn.addEventListener('click', ()=>startTask(task.id));
            
            const stopBtn=li.querySelector('.stop-btn');
            stopBtn.addEventListener('click', ()=>stopTask(task.id));
        }
        else{
            li.innerHTML = `
            <input type="checkbox" class="task-checkbox" ${task.completed ? 'checked' : ''}>
                <input type="text" class="edit-input" value="${task.text}">
                <button class="Save-btn">Сохранить</button>
                <button class="delete-btn">Удалить</button>
            `;
            const editInput = li.querySelector('.edit-input');
            const saveBtn=li.querySelector('.Save-btn');
            saveBtn.addEventListener('click',()=>saver(task.id, editInput.value));
        }

        const checkbox = li.querySelector('.task-checkbox');
        checkbox.addEventListener('change',() => toggleTask(task.id));

        const deleteBtn = li.querySelector('.delete-btn');
        deleteBtn.addEventListener('click',() => deleteTask(task.id));

        tasksList.appendChild(li);})

    updateStats();
    
}

function startTask(id){
    const task = tasks.find(t=>t.id===id);
    if(!task.isTrunning){
        task.isTrunning=true;
        task.startTime = Date.now();
        activeIntervals[task.id]=setInterval(() => {
            task.seconds++;  // Прибавляем секунду каждую 1000 мс
            saveTasks();     // Сохраняем новое время в память
            renderTasks();   // Перерисовываем цифру на экране
        },1000);
    }
    else{
        task.isTrunning=false;
        clearInterval(activeIntervals[task.id]);
        saveTasks();
        renderTasks();
    }
}

function initTimers() {
    tasks.forEach(task => {
        if (task.isTrunning) {
            const passedsec=Math.floor((Date.now()-task.startTime)/1000);
            task.seconds=passedsec;
            activeIntervals[task.id]=setInterval(() => {
                task.seconds++;  // Прибавляем секунду каждую 1000 мс
                saveTasks();     // Сохраняем новое время в память
                renderTasks();   // Перерисовываем цифру на экране
            },1000);
        }
    });
}

function stopTask(id){
    const task = tasks.find(t=>t.id===id);
    task.isTrunning=false;
    clearInterval(activeIntervals[task.id]);
    task.seconds=0;
    saveTasks();
    renderTasks();
}

function greatTime(time){
    let tH=Math.floor(time / 3600);
    let tM=Math.floor((time % 3600)/60);
    let tS=time%60;
    let h=String(tH).padStart(2, '0');
    let m=String(tM).padStart(2, '0');
    let s=String(tS).padStart(2, '0');
    return h+":"+m+":"+s;
}

function editTask(id){
    const task = tasks.find(t=>t.id===id);
    task.isEditing=true;
    renderTasks();

}

function saver(id, oldtxt){
    const task = tasks.find(t=>t.id===id);
    if(oldtxt!=''){
        task.text=oldtxt.trim();
        task.isEditing=false;
        saveTasks();
        renderTasks();
    }
    else{
        task.isEditing=false;
        saveTasks();
        renderTasks();
    }
}


function toggleTask(id){
    const task = tasks.find(t =>t.id===id);
    if(task){
        task.completed = !task.completed;
        saveTasks();
        renderTasks();
        updateDayStatus();
    }

}

function deleteTask(id){
    tasks=tasks.filter(t=>t.id!==id);
    updateDayStatus();
    saveTasks();
    renderTasks();
}

function clear(){
    tasks.length = 0; // Очищаем массив, сохраняя ссылку на history!
    
    // Сбрасываем статус дня на красный, так как задач больше нет
    history[selectedDay].status = "red"; 
    saveTasks();
    updateDayStatus();
    renderTasks();
}

function updateDayStatus(){
    const total= tasks.length;
    const completed = tasks.filter(t=>t.completed).length;
    if(total==0){
        history[selectedDay].status="red";
    }
    else if(completed==total){
        history[selectedDay].status="green";
    }
    else if(completed>0){
        history[selectedDay].status="yellow";
    }
    else{
        history[selectedDay].status="red";
    }
    saveTasks();
    renderCalendar();
}

function updateStats() {
    const completed = tasks.filter(t => t.completed).length;
    const total = tasks.length;

    completedCount.textContent = completed;
    totalCount.textContent = total;
    streakCount.textContent=calculateStreak();
    updateTotalDayTime();
    totalDayTime.textContent=greatTime(history[selectedDay].totalTime);
}

function updateTotalDayTime() {
    let dayTime=0;
    tasks.forEach(task => {
        dayTime+=task.seconds;
    });
    history[selectedDay].totalTime = dayTime;
}

function fAll(){
    currentFilter = 'All';
    updateFilterButtons();
    renderTasks();
}

function fActive(){
    currentFilter = 'Active';
    updateFilterButtons();
    renderTasks();
}

function fCompleted(){
    currentFilter = 'Completed';
    updateFilterButtons();
    renderTasks();
}

function updateFilterButtons(){
    filterAll.classList.remove('active');
    filterActive.classList.remove('active');
    filterCompleted.classList.remove('active');

    if(currentFilter === 'All'){
        filterAll.classList.add('active');
    }
    else if(currentFilter === 'Active'){
        filterActive.classList.add('active');
    }
    else if(currentFilter === 'Completed'){
        filterCompleted.classList.add('active');
    }
}

// События
tabTasksBtn.addEventListener('click',()=> switchTab('tasks'));
tabStatsBtn.addEventListener('click',()=>switchTab('stats'));
viewFullStatsBtn.addEventListener('click',()=>switchTab('stats'));
backToTasksBtn.addEventListener('click',()=> switchTab('tasks'));
toggleNotesBtn.addEventListener('click',()=> {
    notesWidget.classList.toggle('collapsed');
})
saveNotesBtn.addEventListener('click', ()=> saveNote());
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

function saveNote() {
    const saveN=dayNotesInput.value.trim();
    history[selectedDay].dayNotes=saveN;
    saveTasks();
}

function renderCalendar(){
    calendarGrid.innerHTML = '';
    let startDay = new Date(displayYear, displayMonth, 1).getDay();
    let dayInMonth= new Date(displayYear, displayMonth + 1, 0).getDate();

    const monthName = new Date(displayYear, displayMonth).toLocaleString('ru-RU', { month: 'long' });
    calendarTitle.textContent = monthName.charAt(0).toUpperCase() + monthName.slice(1) + ` ${displayYear}`;

    // Преобразуем: getDay() даёт 0=Вс,1=Пн,2=Вт...6=Сб
    // Нам нужно: Пн=1, Вт=2, Ср=3, Чт=4, Пт=5, Сб=6, Вс=7
    startDay = startDay === 0 ? 7 : startDay;

    // Создаем пустые карточки для дней предыдущего месяца
    // Если 1-е число = понедельник (startDay=1), то пустых 0
    // Если 1-е число = вторник (startDay=2), то 1 пустая и т.д.
    for (let i = 1; i < startDay; i++) {
        const emptySquare = document.createElement('div');
        emptySquare.className = 'calendar-day empty';
        calendarGrid.appendChild(emptySquare);
    }

    // Цикл бежит по всем дням месяца от 1 до dayInMonth
    for(let day=1;day<=dayInMonth;day++){
        const formattedMonth=String(displayMonth+1).padStart(2,'0');
        const formattedDay = String(day).padStart(2,'0');
        const daykey = `${displayYear}-${formattedMonth}-${formattedDay}`;
        const status = history[daykey] ? history[daykey].status : "empty";

        const daySquare = document.createElement('div');
        daySquare.className = `calendar-day ${status}`;
        daySquare.textContent = day;
        calendarGrid.appendChild(daySquare);
        daySquare.addEventListener('click', ()=> changeDay(daykey));
    }
}

function changeDay(thatday) {
    selectedDay=thatday;
    if(!history[selectedDay]){
        history[selectedDay]={
            tasks : [],
            totalTime : 0,
            status : "red",
            dayNotes : ""
        }
    }
    tasks = history[selectedDay].tasks;
    dayNotesInput.value = history[selectedDay].dayNotes;

    renderCalendar();
    renderTasks();
}

function calculateStreak() {
    let streak = 0;
    let daysago = 0;

    while(true) {
        const d = new Date();
        d.setDate(d.getDate()-daysago);
        const daykey=d.toISOString().split('T')[0];
        const status = history[daykey] ? history[daykey].status : 'empty';

        if(status=="green" || status=="yellow"){
            streak++;
            daysago++;
        }
        else{
            break;
        }
    }
    return streak;
}

function switchTab(tabName){
    tabTasksBtn.classList.toggle('active', tabName === 'tasks');
    tabStatsBtn.classList.toggle('active', tabName === 'stats');
    sidebarTasksPanel.classList.toggle('active', tabName === 'tasks');
    sidebarStatsPanel.classList.toggle('active', tabName === 'stats');
    contentTasksPanel.classList.toggle('active', tabName === 'tasks');
    contentStatsPanel.classList.toggle('active', tabName === 'stats');
}

// Инициализация
dayNotesInput.value = history[selectedDay].dayNotes;
updateDate();
initTimers();
renderTasks();
renderCalendar();

prevMonthBtn.addEventListener('click', () => {
    displayMonth--;
    if(displayMonth < 0){
        displayMonth = 11;
        displayYear--;
    }
    renderCalendar();
});

nextMonthBtn.addEventListener('click', () => {
    displayMonth++;
    if(displayMonth > 11){
        displayMonth = 0;
        displayYear++;
    }
    renderCalendar();
});