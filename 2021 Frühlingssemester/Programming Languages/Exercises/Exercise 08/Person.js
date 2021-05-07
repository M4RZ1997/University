// constructor for person
function Person(name,password) {
    this.name = name;
    const pwd = password;

    (!Person.prototype.counter) ? Person.prototype.counter = 1 : Person.prototype.counter++;

    // we need a getter function for the password, because it is defined as a private variable
    this.getPassword = function () {
        return pwd.toString();
    }

    this.increaseCounter = function () {
        Person.prototype.counter++;
    }
}

var person1 = new Person("Alice","0987");
var person2 = new Person("Bob","1234");

console.log("Person1's name: " + person1.name); // -> Alice
console.log("Alice's password (direct access): " + person1.pwd); // -> undefined
console.log("Alice's password (function access): " + person1.getPassword()); // -> 0987
console.log("Alice's counter: " + person1.counter); // -> 2
console.log("Person2's name: " + person2.name); // -> Bob
console.log("Bob's counter: " + person2.counter); // -> 2
console.log();

console.log("Increasing counter with inbuilt function");
person1.increaseCounter();
console.log("Alice's counter: " + person1.counter); // -> 3
console.log("Bob's counter: " + person2.counter); // -> 3
console.log();

console.log("Increasing Alice's counter twice");
person1.counter++;
person1.counter++;
console.log("Alice's counter: " + person1.counter); // -> 5
console.log("Bob's counter: " + person2.counter); // -> 3
console.log();

console.log("Increasing Bob's counter once");
person2.counter++;
console.log("Alice's counter: " + person1.counter); // -> 5
console.log("Bob's counter: " + person2.counter); // -> 4
