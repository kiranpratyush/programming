## Document all the learning experience here (Once the Md file is substantially large put it into the Obsidian)

1. Keep this document well documented iteratively so that you can come here and come upto the speed in no time
2. Don't jump between topics, even if you do come here and update what ever your learnt

### What are the concepts needs to be covered.

- Just cover what ART of Postgres does
- Give instruction what you want in a declarative way in contrast to imperative programming language.
  -After that, the RDBMS of your choice is going to figure out a plan then execute it, and hopefully return just the result set you wanted!

DML Commands
Insert into
Delete where
Update set
DDL Commands
Create table
alter table
drop table
TCL Commands
begin transaction
commit
rollback
savepoint
prepare commit
commit prepared
rollback prepared
DCL commands
grant
revoke
Postgres maintenance commands
vacuum
analyse
cluster
prepare
execute
explain
listen
notify
lock
set

### Select command (Projection operator)

- In Postgres "FROM" is optional, you can use like select 1; or select now(); etc
- Fetch first n rows only : Is an official standard of sql of limit N.
- Why it is not recommended to do select \*
  - What is a Toast mechanism in postgres https://www.postgresql.org/docs/current/storage-toast.html understand and write something about it , take x premium tomorrow sunday 30th august 2026
  - It is better to be explicit on the name of the column getting retrieved
- Visiblity rule of select alias (select name as name2) name2 is an alias.
  - FROM, WHERE,GROUP BY, HAVING,JOIN : is executed first before SELECT. hence alias is not available here
  - ORDER BY, LIMIT/FETCH: is executed after the select clause to order or limit the resultant rows.

- Different function and operators available in postgres
  - (TODO) later get a glimpse of date/time functions and operators
- I was thinking SELECT and FROM part of same clause, but now think that FROM is a different clause -> where -> groupby->having->join-> select -> orderby
- order by has nulls first or nulls last
- Postgres also provides distinct on clause to choose columns on which entries will be distinct.
- With out ORDER BY DISTINCT ON becomes unpredictable.
- (TODO) https://www.postgresql.org/docs/current/queries-table-expressions.html#QUERIES-FROM this is next read to understand join,from clause etc.
- NULL does not mean nothing , It means UNKNOWN, whatever comparison with UNKNOWN produces UNKNOWN
- I have to start reading from WHERE again at least to understand the the not exists , what is the distinction of where , how it works as a filter.
- That said we always try to keep the where clause simple to make sure the indexing is used properly (What does it mean).
- with Where we can add and clause so that when the first condition fails it get's short circuted.
- SQL uses three valued logic (true,false,unknown)
- not exists just filters if the row was selected or not. You can simply select anything may be select 1 from table where some condition , then not exists filters the row out
- You can use order by with custom conditions like custom comparitor in c++ using CASE when status ='Power failure' then 1 else 2
- Try to understand the explain command and try to read the query planner execution
- What is table inheritance hierarchy
- This is equivalent of inheritance of OOP . You can create a parent table and child table can inherit from the parent table
- You can put additional constrain on join also for example join on x.num=y.num and y.value = 3;

## TODOS

https://www.postgresql.org/docs/current/storage-toast.html (Why it is not recommended to do select \*)
Why not to use offset,and how to implement pagination
Lateral join query what is does
Indexing

## Postgres chapter 7 (september 1st to 4th)

The general syntax is
[With something][select list exptressions] from [table reference,[table reference]]
SELECT a.\* FROM (my_table AS a JOIN your_table AS b ON ...) AS c : a is not available after the parenthesis.

Status:
Table function :
I have understood that table function can be part of from clause
They can be combined like table alias unset(Array[1,2,3]) as table
CTE

Data-modifying WITH

1. CTE can contain:
   INSERT / UPDATE / DELETE / MERGE

2. RETURNING creates the CTE's output rows.

3. Other parts should consume RETURNING
   to observe those changes.

4. DML CTEs run exactly once and to completion.

5. Sibling statements share the same snapshot;
   don't assume execution ordering.

6. Never design sibling statements to modify
   the same row.

Data-modifying WITH

1. CTE can contain:
   INSERT / UPDATE / DELETE / MERGE

2. RETURNING creates the CTE's output rows.

3. Other parts should consume RETURNING
   to observe those changes.

4. DML CTEs run exactly once and to completion.

5. Sibling statements share the same snapshot;
   don't assume execution ordering.

6. Never design sibling statements to modify
   the same row.

SELECT
FROM
WHERE
column/table aliases
expressions
DISTINCT
ORDER BY
LIMIT
OFFSET
INNER JOIN
LEFT JOIN
RIGHT JOIN
FULL OUTER JOIN
CROSS JOIN
SELF JOIN
multiple joins
GROUP BY
HAVING
COUNT / SUM / AVG / MIN / MAX
subqueries
correlated subqueries
derived tables
CTEs
UNION
UNION ALL
INTERSECT
EXCEPT
window functions
PARTITION BY
ROW_NUMBER
RANK
DENSE_RANK
pagination basics
