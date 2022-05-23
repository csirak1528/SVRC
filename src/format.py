import json

# Opening JSON file
f = open("data.json")

import matplotlib.pyplot as plt

# returns JSON object as
# a dictionary
data = json.load(f)

# Iterating through the json
# list
keys = [int(i) for i in data]

keys.sort()

get = lambda x: data[str(x)]

plt.plot(keys, [get(i)["gzip"]["maxScore"] for i in keys], label="gzip")
plt.plot(keys, [get(i)["lz4"]["maxScore"] for i in keys], label="lz4")
plt.plot(keys, [get(i)["snappy"]["maxScore"] for i in keys], label="snappy")
plt.plot(keys, [get(i)["zlib"]["maxScore"] for i in keys], label="zlib")
plt.plot(keys, [get(i)["zstd"]["maxScore"] for i in keys], label="zstd")

f.close()
plt.legend()
plt.show()
