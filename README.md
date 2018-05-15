This is to play with some binary tree implementations cause I'm bored.

I'm going to use a vector to store nodes and ints to refer to children.
I'll probably use a set to keep track of unused indices as I delete nodes

# Dot stuff

cat tmp.2.dot | dot -Tpng | imgcat

https://stackoverflow.com/a/23430742/2958070

# NOTES

- I need to be very careful with my vector-based memory approach. Need to use
  emplace_back, and I might need to manually call destructors if I reassign an
  element.

  https://stackoverflow.com/q/50357280/2958070


