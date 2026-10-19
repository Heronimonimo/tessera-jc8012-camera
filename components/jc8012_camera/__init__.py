"""JC8012 camera: the camera in the top bar while Home Assistant takes pictures of the screen's own camera.

The camera itself is esp_video_camera beside this folder (plugin.yaml); this component listens to it, as ESPHome's API
does, and says on the screen when pictures leave it. It also keeps what the camera said in the log while it started and
shows it in a text sensor (`status`): the camera starts before Home Assistant connects, so its log is gone by then.
register_plugin() takes the id, version and the screen's texts from the manifest and translations/ beside this folder.
"""
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import logger, smart_display, text_sensor
from esphome.const import CONF_ID

DEPENDENCIES = ["smart_display", "camera", "logger"]

CONF_CAMERA = "camera"
CONF_STATUS = "status"

jc8012_camera_ns = cg.esphome_ns.namespace("jc8012_camera")
# ESPHome's camera base (esphome/components/camera/camera.h), which declares no class of its own in Python.
Camera = cg.esphome_ns.namespace("camera").class_("Camera", cg.EntityBase, cg.Component)
JC8012Camera = jc8012_camera_ns.class_("JC8012Camera", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(JC8012Camera),
        cv.Required(CONF_CAMERA): cv.use_id(Camera),
        cv.Optional(CONF_STATUS): cv.use_id(text_sensor.TextSensor),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await smart_display.register_plugin(var, __file__)
    cg.add(var.set_camera(await cg.get_variable(config[CONF_CAMERA])))
    if CONF_STATUS in config:
        cg.add(var.set_status(await cg.get_variable(config[CONF_STATUS])))
        # The camera starts in setup(), before this component's own; listening from here hears it.
        logger.request_log_listener()
        cg.add(var.listen_to_log())
