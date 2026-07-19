import sys
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.pyplot as plt
import os
os.makedirs("output", exist_ok=True)

path = sys.argv[1] if len(sys.argv) > 1 else "output/balance.csv"
df = pd.read_csv(path)

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

for col in df.columns:
    if col in ("tick", "mult", "Food", "Flour"):
        continue
    ax1.plot(df["tick"], df[col], label=col)
ax1.set_ylabel("population")
ax1.set_title("Population by tier over time")
ax1.legend()
ax1.grid(alpha=0.3)

for col in ("Food", "Flour"):
    if col in df.columns:
        ax2.plot(df["tick"], df[col], label=col)
ax2.set_ylabel("stock")
ax2.set_xlabel("tick")
ax2.set_title("Resources over time")
ax2.legend()
ax2.grid(alpha=0.3)

plt.tight_layout()
plt.savefig("output/balance.png", dpi=120)
print("saved output/balance.png")