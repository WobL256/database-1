This is a console/text-based "database" interaction tool written in C++. Do not let the word "database" fool you, it is not an 
actual database (like what MySQL uses for example), but it is just a .json file and the program lets you manipualte it. It was 
mostly built with the intention of keeping track of customers. You can add an element to the database (a customer), you can edit
it's attributes like name, age and so on, and you can then browse the entire database and search elements based on attribute 
criteria. All of this in a nicely built console/text-based interface. Also the whole interface is in romanian, not english, I
am planning to make an engligh version of the interface some time in the future.

Of course right now only the seraching and opening of elements works, and you only search by index number. As of writing this,
I am working on transitioning from text files to the json system mentioned earlier. Right now it works with a text file for
each element in the database, and a main database text file that keeps track of all of the elements. This, as you may realize,
is wildly inefficient and hard to manage, hence why I am transitioning to a json system that uses one single file for the
entire database.
