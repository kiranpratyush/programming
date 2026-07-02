# HOW Typescript thinks about modules

## Why we need a Module ? 
- Js files can be loaded into browser with the normal script tags, but all the variables and functions declared in each file can be overwritten by each 
other. so we have to be very careful,this is inefficient as we have so many js files and codes.
- We need a system , which will provide each file to their own scope with making available some part of the code to other files as well. (so this each file is called a module and the system is called as a module system)
```
import {x} from "./main.js"
export {y} from "./value.js"
```
commonJs module system 
```
module.exports = {

}
const file = require('fs');
```
TypeScript : Job of TypeScript is to catch certain kind of runtime errors by catching them at compile time, with or without modules involved.
```
function func(a)
{
    // what is a ? 

}
```
You can add a bit of type safety in Js files using checkJs : true , AllowJs : true

Module : Module output format determines for which host (where the output will be used) it will run.
Module Specifier : The string from which you import (file.js/file.ts)

The three component of typescripts jobs 
1. Compile files into a valid output module format
2. Ensure that imports in those outputs will resolves successfully
3. Know what type to assign to imported names.
for determining the type we can use .d.ts ( declaration file)