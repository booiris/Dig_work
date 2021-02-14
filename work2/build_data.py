import json

ans = []
for i in range(100):
    filename = "out_file/" + str(i) + ".txt"
    with open(filename, "r", encoding="utf-8") as f:
        res = {"lanes": [], "run_time": 0}
        cnt = 0
        x = []
        for line in f:
            if cnt == 0:
                line = line.replace("e:/Dig_work/work2/", "")
                line = line.replace("\n", "")
                res["file_name"] = line
            elif line[0] == '#':
                res["lanes"].append(x)
                x = []
            else:
                x.append(int(line))
            cnt += 1
        ans.append(res)
f2 = open('res.json', 'w')
json.dump(ans, f2)
f2.close()
