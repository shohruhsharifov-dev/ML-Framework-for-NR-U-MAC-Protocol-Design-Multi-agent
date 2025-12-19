import mitsuba as mi
import matplotlib.pyplot as plt
from sionna.rt import load_scene, scene

mi.set_variant("scalar_rgb")

scene = mi.load_file("scenarios/SionnaExampleScenario/scene.xml")


sensor = mi.load_dict({
    "type": "perspective",
    "to_world": mi.ScalarTransform4f.look_at(
        origin=[10, 10, 700],
        target=[0, 0, 0],
        up=[0, 0, 1]
    ),
    "fov": 45,
    "film": {
        "type": "hdrfilm",
        "width": 2000,
        "height": 1000,
        "rfilter": {"type": "gaussian"}
    }
})

image = mi.render(scene, sensor=sensor)

# Convert Mitsuba image to numpy array and display inline
plt.imshow(image)
plt.axis("off")
plt.show()
