import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("contrib/ai/examples/nr-rl/positions.csv")

gnb = data[data["NodeType"]=="gNB"]
ue  = data[data["NodeType"]=="UE"]

# Plot gNBs in red
plt.scatter(gnb["X"], gnb["Y"], c="red", marker="x", s=200, label="gNBs")
for _, row in gnb.iterrows():
    plt.text(row["X"], row["Y"], str(row["CellID"]), fontsize=9,
             ha="center", va="bottom", color="red")

# Plot UEs colored by throughput
sc = plt.scatter(ue["X"], ue["Y"], c=ue["ThroughputMbps"], cmap="viridis",
                 marker="o", s=50, label="UEs")

for _, row in ue.iterrows():
    plt.text(row["X"], row["Y"], str(row["IPidentifier"]), fontsize=8,
             ha="center", va="bottom", color="blue")

# Add colorbar for throughput
cbar = plt.colorbar(sc)
cbar.set_label("Throughput (Mbps)")

plt.xlabel("X position (m)")
plt.ylabel("Y position (m)")
plt.title("gNB and UE positions with throughput")
plt.legend()
plt.grid(True)
plt.axis("equal")
plt.show()