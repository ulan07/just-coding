-- Database Assignment - Week 7
-- Deadline: Week 7

-- ЗАДАЧА 1: Создание таблицы users
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    username VARCHAR(100) UNIQUE NOT NULL,
    age INT NOT NULL CHECK (age > 16),
    email VARCHAR(100),
    country VARCHAR(70)
);

-- ЗАДАЧА 2: Вставка 3 записей
INSERT INTO users (username, age, country) VALUES
('john_doe', 25, 'Kazakhstan'),
('jane_smith', 30, 'USA'),
('alex_brown', 22, 'UK');

-- ЗАДАЧА 3: Добавление колонки phone_number
-- Формат: + (1 символ) + 11 цифр = 12 символов
-- Дефолт: казахстанский номер +77001234567

ALTER TABLE users
ADD COLUMN phone_number VARCHAR(12) DEFAULT '+77001234567'
ADD CONSTRAINT ch CHECK(
    phone_number ~ '^\+[0-9]{11}$'
);

-- ЗАДАЧА 4: Добавление колонки password
-- Требования:
-- - Минимум 8 символов
-- - Хотя бы 1 строчная буква
-- - Хотя бы 1 заглавная буква
-- - Хотя бы 1 цифра
-- - Хотя бы 1 спецсимвол (! @ # $ % ^ & *)

ALTER TABLE users
ADD COLUMN password VARCHAR(100) NOT NULL,
ADD CONSTRAINT chk_password_strength CHECK (
    LENGTH(password) >= 8 
    AND password ~ '[a-z]' 
    AND password ~ '[A-Z]' 
    AND password ~ '[0-9]' 
    AND password ~ '[!@#$%^&*]'
);


-- ЗАДАЧА 5: Удаление NOT NULL с username

ALTER TABLE users
ALTER COLUMN username DROP NOT NULL;


-- ЗАДАЧА 6: Вставка 4 записей
-- - 1 запись: username заполнен, country NULL
-- - 2 записи: username NULL, валидные пароли
-- - UNIQUE позволяет несколько NULL значений

INSERT INTO users (username, age, country, password) VALUES
('user_one', 20, NULL, 'Pass123!'),
(NULL, 28, NULL, 'Secure456#'),
(NULL, 35, 'Canada', 'Strong789$'),
('user_two', 40, 'Germany', 'Valid012@');


-- ЗАДАЧА 7: CHECK constraint для lowercase username
-- Проверяет, что username только в нижнем регистре

ALTER TABLE users
ADD CONSTRAINT check_username_lowercase
CHECK (username = LOWER(username));


-- ЗАДАЧА 8: CHECK constraint для email или phone
-- Пользователь должен иметь хотя бы один способ связи

ALTER TABLE users
ADD CONSTRAINT check_contact_info
CHECK (email IS NOT NULL OR phone_number IS NOT NULL);


-- ЗАДАЧА 9: Удаление CHECK lowercase username

ALTER TABLE users
DROP CONSTRAINT check_username_lowercase;


-- ЗАДАЧА 10: UNIQUE constraint для email

ALTER TABLE users
ADD CONSTRAINT unique_email UNIQUE (email);


-- ЗАДАЧА 11: Добавление created_at
-- TIMESTAMP с автоматической текущей датой

ALTER TABLE users
ADD COLUMN created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL;


-- ЗАДАЧА 12: Таблица course_registrations
-- COMPOSITE PRIMARY KEY (student_id, course_code)
-- FOREIGN KEY к таблице users

CREATE TABLE course_registrations (
    student_id INT NOT NULL,
    course_code VARCHAR(20) NOT NULL,
    registration_date DATE DEFAULT CURRENT_DATE,
    PRIMARY KEY (student_id, course_code),
    FOREIGN KEY (student_id) REFERENCES users(user_id)
);

-- Вставка 3 записей
INSERT INTO course_registrations (student_id, course_code) VALUES
(1, 'CS101'),
(1, 'CS102'),
(2, 'CS101');


-- ЗАДАЧА 13: Три таблицы с FOREIGN KEY
-- departments -> managers -> employees
-- ON DELETE RESTRICT и ON DELETE SET NULL


-- 1. departments
CREATE TABLE departments (
    dept_id SERIAL PRIMARY KEY,
    dept_name VARCHAR(100) NOT NULL
);

-- 2. managers
CREATE TABLE managers (
    manager_id SERIAL PRIMARY KEY,
    manager_name VARCHAR(100) NOT NULL,
    dept_id INT NOT NULL,
    FOREIGN KEY (dept_id) REFERENCES departments(dept_id) ON DELETE RESTRICT
);

-- 3. employees
CREATE TABLE employees (
    emp_id SERIAL PRIMARY KEY,
    emp_name VARCHAR(100) NOT NULL,
    dept_id INT NOT NULL,
    manager_id INT,
    FOREIGN KEY (dept_id) REFERENCES departments(dept_id) ON DELETE RESTRICT,
    FOREIGN KEY (manager_id) REFERENCES managers(manager_id) ON DELETE SET NULL
);

-- Вставка данных
INSERT INTO departments (dept_name) VALUES ('IT'), ('HR'), ('Sales');

INSERT INTO managers (manager_name, dept_id) VALUES
('Alice', 1),
('Bob', 2);

INSERT INTO employees (emp_name, dept_id, manager_id) VALUES
('Charlie', 1, 1),
('David', 2, 2),
('Eve', 1, NULL);


-- ЗАДАЧА 14: Таблицы orders и order_items
-- ON DELETE CASCADE и ON UPDATE CASCADE
-- Сложная логика для discount


-- 1. orders
CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    order_date DATE DEFAULT CURRENT_DATE
);

-- 2. order_items
CREATE TABLE order_items (
    item_id SERIAL PRIMARY KEY,
    order_id INT NOT NULL,
    item_name VARCHAR(100) NOT NULL,
    quantity INT NOT NULL CHECK (quantity BETWEEN 1 AND 100),
    price NUMERIC(10, 2) NOT NULL,
    discount NUMERIC(5, 2) CHECK (
        discount IS NULL OR
        (discount = 40 AND price <= 100) OR
        (discount < 40)
    ),
    FOREIGN KEY (order_id) REFERENCES orders(order_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);


-- ЗАДАЧА 15: Проверка CASCADE
-- Вставка, UPDATE, DELETE для демонстрации CASCADE


-- Вставка 2 заказов
INSERT INTO orders (order_id) VALUES (1), (2);

-- Вставка items для order 1
INSERT INTO order_items (order_id, item_name, quantity, price, discount) VALUES
(1, 'Laptop', 1, 1200.00, 30.00);

-- Вставка items для order 2
INSERT INTO order_items (order_id, item_name, quantity, price, discount) VALUES
(2, 'Mouse', 5, 50.00, 40.00);

-- Проверка: показать все заказы и items
SELECT 'Before UPDATE/DELETE:' AS status;
SELECT * FROM orders;
SELECT * FROM order_items;

-- Изменение order_id с 1 на 100 (ON UPDATE CASCADE)
UPDATE orders SET order_id = 100 WHERE order_id = 1;

-- Проверка: item с order_id=1 автоматически стал order_id=100
SELECT 'After UPDATE order_id 1 -> 100:' AS status;
SELECT * FROM order_items WHERE order_id = 100;

-- Удаление order 2 (ON DELETE CASCADE)
DELETE FROM orders WHERE order_id = 2;

-- Проверка: все items с order_id=2 автоматически удалены
SELECT 'After DELETE order 2:' AS status;
SELECT * FROM order_items WHERE order_id = 2;  -- Пустой результат

-- Финальная проверка: остался только item от order 100
SELECT 'Final state (only order 100 item remains):' AS status;
SELECT * FROM orders;
SELECT * FROM order_items;


-- КОНЕЦ ФАЙЛА
-- Результат задачи 15: остался только 1 item
-- (Laptop) от заказа №100
