A program that takes in a log file of books (ISBN13, year published, author, title)
and provides an interface to sort, manage, and create an excerpt list of books.
The command line requires 1 argument, the input log file, and an optional help
command (-h or --help).

The commands for the interface are as follows:
/# - A comment line, ignored by the program
y XXXX | YYYY - A year search between XXXX and YYYY with XXXX < YYYY
k "  " - A keyword search for one or more words that match titles and/or authors, 
	(case insensitive)
g - View search results
r - Append search results to excerpt list
l - Clear excerpt list
a int - Append an entry int from the excerpt list to the excerpt list, making another copy
d int - Delete entry int from excerpt list
b int - Move entry int to the beginning of the excerpt list
e int - Move entry int to the end of the excerpt list
s - Sort the entry list by year (displays information of list before and after sort)
