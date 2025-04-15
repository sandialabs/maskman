#!/usr/bin/env python3

i = 0
org = {}
with open("supp.txt", "r") as f:
    for fline in f:
        if "{" in fline:
            i = i + 1
            org[i] = []
        org[i].append(fline)

for key in org:
    with open("supp-{}.txt".format(key), "w") as f:
        f.writelines(org[key])

