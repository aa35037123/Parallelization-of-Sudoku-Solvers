from functools import cmp_to_key

def compare(a, b):
    if a < b:
        return -1
    elif a > b:
        return 1
    else:
        return 0


my_list = [5, 2, 4, 1, 3]
my_list.sort(key=cmp_to_key(compare))
print(my_list)
