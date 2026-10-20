
-- 2.
-- DDL (Data Definition Language) - commands for defining and modifying the database structure:
--   CREATE, ALTER, DROP, TRUNCATE
--   Used to create or modify tables, indexes, and schemas.
--
-- DML (Data Manipulation Language) - data manipulation commands:
--   SELECT, INSERT, UPDATE, DELETE
--   Used to read, add, modify, and delete data.

-- 3. Create a database called Faculty
CREATE DATABASE Faculty;


-- 4. Create Students table

CREATE TABLE Students (
    id SERIAL,
    firstname VARCHAR(70),
    lastname VARCHAR(70),
    isActive INT,
    email VARCHAR(70),
    age INT
);


-- 5. Add gender column

ALTER TABLE Students
ADD COLUMN gender INT;


-- 6. Set default value of gender to 0

ALTER TABLE Students
ALTER COLUMN gender SET DEFAULT 0;


-- 7. Remove default value from gender

ALTER TABLE Students
ALTER COLUMN gender DROP DEFAULT;


-- 8. Add PRIMARY KEY constraint to id

ALTER TABLE Students
ADD PRIMARY KEY (id);


-- 9. Set default value of email

ALTER TABLE Students
ALTER COLUMN email SET DEFAULT 'abc@gmail.com';


-- 10. Add birthdate column

ALTER TABLE Students
ADD COLUMN birthdate DATE;


-- 11. Change max length of firstname to 100

ALTER TABLE Students
ALTER COLUMN firstname TYPE VARCHAR(100);


-- 12. Rename lastname to surname, then back

ALTER TABLE Students
RENAME COLUMN lastname TO surname;

ALTER TABLE Students
RENAME COLUMN surname TO lastname;


-- 13. Insert 1 row with only firstname and lastname

INSERT INTO Students (firstname, lastname)
VALUES ('John', 'Doe');


-- 14. Insert row with age explicitly NULL

INSERT INTO Students (firstname, lastname, age)
VALUES ('Jane', 'Smith', NULL);


-- 15. Insert 3 rows in single statement

INSERT INTO Students (firstname, age, isActive)
VALUES
    ('Alice', 17, 1),
    ('Bob', 19, 0),
    ('Charlie', 25, 1);


-- 16. Set default age to 20, insert and verify

ALTER TABLE Students
ALTER COLUMN age SET DEFAULT 20;

INSERT INTO Students (firstname)
VALUES ('David');


-- 17. Update NULL values in gender and age

UPDATE Students
SET gender = 0, age = 20
WHERE gender IS NULL OR age IS NULL;


-- 18. Change isActive to BOOLEAN using USING clause

ALTER TABLE Students
ALTER COLUMN isActive TYPE BOOLEAN
USING isActive::BOOLEAN;


-- 19. Create Teachers table LIKE Students

CREATE TABLE Teachers (LIKE Students INCLUDING DEFAULTS INCLUDING CONSTRAINTS);


-- 20. Insert all rows from Students to Teachers

INSERT INTO Teachers
SELECT * FROM Students;


-- 21. Increase age by 2 and return firstname and updated age

UPDATE Students
SET age = age + 2
RETURNING firstname, age AS update_age;


-- 22. Update multiple students in single query

UPDATE Students
SET
    email = CASE WHEN id = 2 THEN 'newemail@example.com' ELSE email END,
    firstname = CASE WHEN id = 4 THEN 'UpdatedName' ELSE firstname END,
    isActive = CASE WHEN id = 4 THEN TRUE ELSE isActive END
WHERE id IN (2, 4);


-- 23. Change birthdate of student with id = 4

UPDATE Students
SET birthdate = '2000-01-01'
WHERE id = 4;


-- 24. Delete from Teachers where id exists in Students

DELETE FROM Teachers
WHERE id IN (SELECT id FROM Students)
RETURNING *;


-- 25. Remove students under age 20

DELETE FROM Students
WHERE age < 20;


-- 26. Remove all remaining rows from Students

DELETE FROM Students
RETURNING *;
