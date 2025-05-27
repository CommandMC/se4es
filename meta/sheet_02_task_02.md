# 1

init:

```cpp
int b = a >> 2;
```

loop:

```cpp
for(int i = 0; i < a; ++i)
```

loop body:

```cpp
a -= b;
```

return:

```cpp
return a + b;
```

# 2

init:

- a = [0,42]
- b = [0,10]

lower bound:

- a = 0 (direkter Abbruch)

upper bound:

Step | a | b
------------
     | 0 
