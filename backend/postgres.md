## Document all the learning experience here (Once the Md file is substantially large put it into the Obsidian)

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
- Why it is not recommended to do select * 
  -  What is a Toast mechanism in postgres https://www.postgresql.org/docs/current/storage-toast.html
  -  It is better to be explicit on the name of the column getting retrieved
-  Visiblity rule of select alias (select name as name2) name2 is an alias.
   -  FROM, WHERE,GROUP BY, HAVING,JOIN : is executed first before SELECT. hence alias is not available here
   -  ORDER BY, LIMIT/FETCH: is executed after the select clause to order or limit the resultant rows.

- Different function and operators available in postgres
  - (TODO) later get a glimpse of date/time functions and operators
- I was thinking SELECT and FROM part of same clause, but now think that FROM is a different clause -> where -> groupby->having->join-> select -> orderby
- order by has nulls first or nulls last
- Postgres also provides distinct on clause to choose columns on which entries will be distinct.
- With out ORDER BY DISTINCT ON becomes unpredictable.
- (TODO) https://www.postgresql.org/docs/current/queries-table-expressions.html#QUERIES-FROM this is next read to understand join,from clause etc.
- NULL does not mean nothing , It means UNKNOWN, whatever comparison with UNKNOWN produces UNKNOWN
- I have to start reading from WHERE again at least to understand the the not exists , what is the distinction of where , how it works as a filter.